// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"

ASword::ASword()
{
    PrimaryActorTick.bCanEverTick = true;
    ASwordFlyBaseWeapon::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;
    SetWeaponType(EWeaponType::ESword);
}

void ASword::Attack()
{
    AttackServer();
}

void ASword::AttackServer()
{
    AttackNetMulticast();
}

void ASword::AttackNetMulticast()
{
    if (thisOwner == nullptr)return;
    
    UAnimInstance* PlayerAnimation = thisOwner->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
            //thisOwner->GetActorRotation();
            PlayerAnimation->Montage_Play(AttackAnimMontage);
            PlayerAnimation->Montage_JumpToSection("SwordAttack",AttackAnimMontage);
            
           
        }
           
    }
}

void ASword::BeginPlay()
{
    Super::BeginPlay();
}
