// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"

#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"

ABow::ABow()
{
    PrimaryActorTick.bCanEverTick = true;
    ASwordFlyBaseWeapon::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;
    SetWeaponType(EWeaponType::EBow);
}

void ABow::Attack()
{
    AttackServer();
}

void ABow::AttackServer()
{
    AttackNetMulticast();
}

void ABow::AttackNetMulticast()
{
    if (thisOwner == nullptr)return;
    
    UAnimInstance* PlayerAnimation = thisOwner->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
            //thisOwner->GetActorRotation();
            PlayerAnimation->Montage_Play(AttackAnimMontage);
            PlayerAnimation->Montage_JumpToSection("BowAttack",AttackAnimMontage);
            
           
        }
           
    }
}

void ABow::BeginPlay()
{
    Super::BeginPlay();
}
