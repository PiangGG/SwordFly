// Fill out your copyright notice in the Description page of Project Settings.

#include <basetyps.h>
#include <ThirdParty/CryptoPP/5.6.5/include/misc.h>

#include "Net/UnrealNetwork.h"
#include "SwordFlyBaseWeapon.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"
#include "Components/SphereComponent.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"

void ASwordFlyBaseWeapon::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    
    
}

ASwordFlyBaseWeapon::ASwordFlyBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    ASwordFlyBaseWeapon::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;

    Collision_Attack=CreateDefaultSubobject<USphereComponent>(FName("Collision_Attack"));
    Collision_Attack->SetupAttachment(RootComponent);
    Collision_Attack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Collision_Attack->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision_Attack->SetCollisionResponseToChannel(ECC_EngineTraceChannel1,ECR_Overlap);
    
    bReplicates = true;
    bReplayRewindable = true;
    bAlwaysRelevant = true;
    AActor::SetReplicateMovement(true);

    isAttack=false;
    ActorID=1;
}

void ASwordFlyBaseWeapon::SetItmeType(EItmeType Type)
{
    thisItmeType=Type;
   
}

void ASwordFlyBaseWeapon::Attack()
{
    AttackServer();
}

void ASwordFlyBaseWeapon::AttackServer_Implementation()
{
    AttackNetMulticast();
}
bool ASwordFlyBaseWeapon::AttackServer_Validate()
{

    return true;
}

void ASwordFlyBaseWeapon::AttackNetMulticast_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("attack"));
    //if (GetLocalRole()=ROLE_Authority)return;
    if (thisOwner == nullptr)return;
    
    UAnimInstance* PlayerAnimation = thisOwner->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false)
            {
                PlayerAnimation->Montage_Play(AttackAnimMontage);
           
            }
           
    }
   
}

void ASwordFlyBaseWeapon::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //if (GetLocalRole()!=ROLE_Authority)return;
    Super::Collision_Pack_BeginOverlap(Component, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    ASwordFlyCharacter* Player = Cast<ASwordFlyCharacter>(OtherActor);
    thisOwner=Player;
    Pack(Player);
}

EWeaponType ASwordFlyBaseWeapon::GetWeaponType()
{
    return  thisWeaponType;
}

void ASwordFlyBaseWeapon::SetWeaponType(EWeaponType newType)
{
    thisWeaponType=newType;
}

FRotator ASwordFlyBaseWeapon::SetOwerRotation()
{
    FRotator newRotator1=FRotator(thisOwner->GetActorRotation().Pitch,thisOwner->GetController()->GetControlRotation().Yaw,thisOwner->GetActorRotation().Roll);
    FRotator newRotator=FMath::RInterpTo(thisOwner->GetActorRotation(),newRotator1,0.3f,0.3f);
    return newRotator;
}

void ASwordFlyBaseWeapon::Equipment(ASwordFlyCharacter* Player)
{
    EquipmentServer(Player);
}

void ASwordFlyBaseWeapon::EquipmentServer_Implementation(ASwordFlyCharacter* Player)
{
    EquipmentNetMulticast(Player);
}

bool ASwordFlyBaseWeapon::EquipmentServer_Validate(ASwordFlyCharacter* Player)
{
    return true;

}

void ASwordFlyBaseWeapon::EquipmentNetMulticast_Implementation(ASwordFlyCharacter* Player)
{
    if (!thisOwner)return;
    ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(thisOwner->GetPlayerState());
    USwordFlyInformationrComponent *info=Cast<USwordFlyInformationrComponent>(PS->InformationCompoent);
    //if (!thisOwner->GetController()->IsLocalController())return;

    if (!PS||!info)return;
    if (!info->CurrentWeaponArray.IsValidIndex(0))
    {
        PS->Equipment(this);
        
        Mesh->SetSimulatePhysics(false);
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        this->AttachToComponent(thisOwner->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,AttachLocation);
        //thisOwner->SetCurrentWeapon(this);
        thisOwner->Equipment(this);
    }else if (info->CurrentWeaponArray.IsValidIndex(0)&&info->CurrentWeaponArray[0]->GetWeaponType()!=this->GetWeaponType())
    {
        PS->Equipment(this);

        Mesh->SetSimulatePhysics(false);
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        this->AttachToComponent(thisOwner->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,AttachBackLocation);
    }else
    {
        /*this->SetActorHiddenInGame(true);
        for(auto&thisItmeArray:info->PackItmeArray)
        {
            if(thisItmeArray.thisItem->ItemName==this->ItemName)
            {
                thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+1;
                return;
            }
        }
        FPackItme newItme;
        newItme.thisItem=this;
        newItme.thisItemnumber=1;
        info->PackItmeArray.Add(newItme);
        return;*/
    }
  
    
}

void ASwordFlyBaseWeapon::UnEquipment(class ASwordFlyCharacter* Player)
{
    UnEquipmentServer(Player);
}

void ASwordFlyBaseWeapon::Pack(ASwordFlyCharacter* theOwner)
{
    if (!theOwner)return;
  
    ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(theOwner->GetPlayerState());
    USwordFlyInformationrComponent *info=Cast<USwordFlyInformationrComponent>(PS->InformationCompoent);
    if (!PS||!info)return;
    if (!info->CurrentWeaponArray.IsValidIndex(0))
    {
        Equipment(thisOwner);
        return;
    }
    if (!info->CurrentWeaponArray.IsValidIndex(1))
    {
        Equipment(thisOwner);
        return;
    }
    /*this->SetActorHiddenInGame(true);
    for(auto&thisItmeArray:info->PackItmeArray)
    {
        if(thisItmeArray.thisItem->ItemName==this->ItemName)
        {
            thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+1;
            return;
        }
    }
    FPackItme newItme;
    newItme.thisItem=this;
    newItme.thisItemnumber=1;
    info->PackItmeArray.Add(newItme);
    return;*/
}

void ASwordFlyBaseWeapon::UnEquipmentNetMulticast_Implementation(ASwordFlyCharacter* Player)
{
    if (thisOwner)
    {
        //thisOwner->UnEquipment();
        this->AfterThroud(thisOwner);
    }
}

void ASwordFlyBaseWeapon::UnEquipmentServer_Implementation(ASwordFlyCharacter* Player)
{
    UnEquipmentNetMulticast(Player);
}

bool ASwordFlyBaseWeapon::UnEquipmentServer_Validate(ASwordFlyCharacter* Player)
{
    return true;
}
