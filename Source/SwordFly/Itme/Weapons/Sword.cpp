// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASword::ASword()
{
    PrimaryActorTick.bCanEverTick = true;

    this->thisOwner=nullptr;
    ASwordFlyBaseWeapon::SetWeaponType(EWeaponType::ESword);
    AttachLocation="Socket_Right_FString";
    AttachBackLocation="AttachBackLocation1";
    
    Collision_Capsule=CreateDefaultSubobject<UCapsuleComponent>(FName("Collision_Capsule"));
    Collision_Capsule->SetupAttachment(RootComponent);
    ActorID=2;
    
}

void ASword::Attack()
{
    AttackServer();     
}

void ASword::AttackServer_Implementation()
{
    AttackNetMulticast();
}

bool ASword::AttackServer_Validate()
{
    return true;
}

void ASword::AttackNetMulticast_Implementation()
{
    if (!GetOwner())return;
    ASwordFlyCharacter* Player=Cast<ASwordFlyCharacter>(GetOwner());
    if (!Player)return;
    UAnimInstance* PlayerAnimation = Player->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
      
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
            UE_LOG(LogTemp,Warning,TEXT("Attack %s"),*Player->GetName());
            PlayerAnimation->Montage_Play(AttackAnimMontage);
            PlayerAnimation->Montage_JumpToSection("SwordAttack",AttackAnimMontage);
        }
           
    } 
   
}


EWeaponType ASword::GetWeaponType()
{
    return EWeaponType::ESword;
}

void ASword::BeginPlay()
{
    Super::BeginPlay();
}

void ASword::Attack_2()
{
    if (GetLocalRole()<ROLE_Authority)
    {
        AttackServer_2();
    }

    //获取拥有者角色
    ASwordFlyCharacter *Player=Cast<ASwordFlyCharacter>(GetOwner());
    if (!Player)return;
    if (Player->bIsFly)
    {
        Player->bIsFly=false;
        Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        this->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,this->AttachLocation);
        Player->SetCharacterState(ECharacterState::ESword);
        this->SetActorScale3D(FVector(1));
        if (Player->PlayerWeaponArray.IsValidIndex(1))
        {
            Player->PlayerWeaponArray[1]->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, Player->PlayerWeaponArray[1]->AttachBackLocation);
        }
    }else if (!Player->bIsFly)
    {
        Player->bIsFly=true;
        Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
        Player->SetCharacterState(ECharacterState::ESwordFly);
        if (Player->PlayerWeaponArray.IsValidIndex(1))
        {
            Player->PlayerWeaponArray[1]->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, Player->PlayerWeaponArray[1]->AttachLocation);
        }
        this->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName());
        this->SetActorScale3D(FVector(1)*3);
    }
}

void ASword::AttackNetMulticast_2_Implementation()
{
}

void ASword::AttackServer_2_Implementation()
{
    Attack_2();
}

bool ASword::AttackServer_2_Validate()
{
    return true;
}
