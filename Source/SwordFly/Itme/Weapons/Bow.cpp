// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Camera/CameraComponent.h"
#include "SwordFly/Arrow.h"
#include "SwordFly/Arrow2.h"
#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "DrawDebugHelpers.h"
#include "SwordFly/Arrow.h"
#include "SwordFly/GamePlay/PlayerController/SwordFlyPlayerController.h"
#include "Components/SphereComponent.h"

ABow::ABow()
{
    PrimaryActorTick.bCanEverTick = true;
    //this->thisOwner=nullptr;
    
   
    AttachLocation="Socket_Left_FString";
    AttachBackLocation="AttachBackLocation2";
    ActorID=3;
    this->SetReplicates(true);
    this->SetReplicateMovement(true);
    bAlwaysRelevant = true;
    bReplayRewindable=true;
    SetReplicates(true);
    SetReplicateMovement(true);
    bIsArming=false;
    ArrowForce=1000.f;
}

void ABow::Attack()
{
    AttackServer();  
}

void ABow::AttackServer_Implementation()
{
    AttackNetMulticast();
}

void ABow::AttackNetMulticast_Implementation()
{
    ASwordFlyCharacter *player=Cast<ASwordFlyCharacter>(GetOwner());
    if (player)
    {
        UAnimInstance* PlayerAnimation = player->GetMesh()->GetAnimInstance();
       
        if (PlayerAnimation)
        {
          
            if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
               
                //thisOwner->GetActorRotation();
                PlayerAnimation->Montage_Play(AttackAnimMontage);
                PlayerAnimation->Montage_JumpToSection("BowAttack1",AttackAnimMontage);
                    
                //Shoot();
            }
           
        }
    }
}

void ABow::BeginPlay()
{
    Super::BeginPlay();
    ASwordFlyBaseWeapon::SetWeaponType(EWeaponType::EBow);
    
}

EWeaponType ABow::GetWeaponType()
{
    return EWeaponType::EBow;
}

void ABow::Shoot()
{
   
    if (GetLocalRole()==ROLE_Authority)
    {
        UWorld* World=GetWorld();
        if (World&&GetNetOwner())
        {
            ASwordFlyCharacter *thisOwnert1=Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
            if (thisOwnert1)
            {
                if (thisOwnert1->GetController())
                {
                    FActorSpawnParameters ActorSpawnParameters;
                    ActorSpawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                    FVector eyelocation;
                    FRotator eyeRotator;
                    thisOwnert1->GetActorEyesViewPoint(eyelocation,eyeRotator);
                    
                    AArrow* thisAArrow=World->SpawnActor<AArrow>(ArrowClass,thisOwnert1->GetMesh()->GetSocketLocation("ShootSocket"),eyeRotator,ActorSpawnParameters);
                    
                    if (thisAArrow)
                    {
                        thisAArrow->SetOwner(thisOwnert1);
                       
                        //thisAArrow->MovementComponent->AddForce(thisOwnert1->TiredCamera->GetForwardVector()*ArrowForce);
                        //thisAArrow->AttackComp->AddForce(thisOwnert1->TiredCamera->GetForwardVector()*ArrowForce);
                        //DrawDebugLine(GetWorld(),GetNetOwner()->GetActorLocation(),GetNetOwner()->GetActorForwardVector()+200.f,FColor::Blue,true,10.f);
                    }
                }
            }
        }
    }
}

void ABow::Attack_2()
{
    
    AttackNetMulticast_2();

   
    
}

void ABow::AttackNetMulticast_2_Implementation()
{
    ASwordFlyCharacter *player=Cast<ASwordFlyCharacter>(GetOwner());
    if (player)
    {
        UAnimInstance* PlayerAnimation = player->GetMesh()->GetAnimInstance();
       
        if (PlayerAnimation)
        {
          
            if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false&&bIsArming==false)
            {  
                //thisOwner->GetActorRotation();
                bIsArming=true;
                PlayerAnimation->Montage_Play(AttackAnimMontage);
                PlayerAnimation->Montage_JumpToSection("BowAttack2",AttackAnimMontage);
                    
                //Shoot();
            }else if (AttackAnimMontage&&bIsArming==true)
            {
                bIsArming=false;
                PlayerAnimation->Montage_Play(AttackAnimMontage);
                PlayerAnimation->Montage_JumpToSection("BowAttack1",AttackAnimMontage);
            }
           
        }
    }
}

void ABow::AttackServer_2_Implementation()
{
    Attack_2();
}

bool ABow::AttackServer_2_Validate()
{
    return true;
}

void ABow::ShootNetMulticast_Implementation()
{
   
    
}

bool ABow::ShootServer_Validate()
{
    return true;
}

void ABow::ShootServer_Implementation()
{
    ShootNetMulticast();
   
}
