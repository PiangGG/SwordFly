// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"



#include "Camera/CameraComponent.h"
#include "SwordFly/Arrow.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "DrawDebugHelpers.h"
#include "SwordFly/Arrow.h"
#include "Components/SphereComponent.h"

ABow::ABow()
{
    PrimaryActorTick.bCanEverTick = true;
    this->thisOwner=nullptr;
    ASwordFlyBaseWeapon::SetWeaponType(EWeaponType::EBow);
    AttachLocation="Socket_Left_FString";
    AttachBackLocation="AttachBackLocation2";
    ActorID=3;
}

void ABow::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("attack2"));
    AttackServer();
}

void ABow::AttackServer()
{
    AttackNetMulticast();
}

void ABow::AttackNetMulticast()
{
    if (thisOwner == nullptr||thisOwner->GetController()->IsLocalController())return;
    UAnimInstance* PlayerAnimation = thisOwner->GetMesh()->GetAnimInstance();
   
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

void ABow::BeginPlay()
{
    Super::BeginPlay();
}

void ABow::Shoot()
{
    //if (GetLocalRole()!=ROLE_Authority)return;
    UE_LOG(LogTemp, Warning, TEXT("attack"));
    UWorld* World=GetWorld();
    if (World)
    {
        AArrow* thisAArrow=GetWorld()->SpawnActor<AArrow>(ArrowClass,Mesh->GetSocketLocation("ShootSocket"),thisOwner->TiredCamera->GetComponentRotation());
        thisAArrow->Collision_Attack->AddImpulse(thisOwner->GetActorLocation()+thisOwner->TiredCamera->GetComponentRotation().Vector()*200000);
       
    }
  
}
