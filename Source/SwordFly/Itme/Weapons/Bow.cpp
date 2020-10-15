// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Camera/CameraComponent.h"
#include "SwordFly/Arrow.h"
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
                PlayerAnimation->Montage_JumpToSection("BowAttack",AttackAnimMontage);
                    
                Shoot();
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
            ASwordFlyCharacter *thisOwnert1=Cast<ASwordFlyCharacter>(GetOwner());
            if (thisOwnert1)
            {
                if (thisOwnert1->GetController())
                {
                    AArrow* thisAArrow=World->SpawnActor<AArrow>(ArrowClass,Mesh->GetSocketLocation("ShootSocket"),thisOwnert1->TiredCamera->GetForwardVector().Rotation());
                    if (thisAArrow)
                    { 
                        thisAArrow->Collision_Attack->AddImpulse(thisOwnert1->GetActorLocation()+thisOwnert1->TiredCamera->GetForwardVector()*200000);
           
                    }
                }
            }
        }
    }
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
