// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Components/CapsuleComponent.h"
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


void ASword::AttackNetMulticast_Implementation()
{
    if (!GetOwner())return;
    ASwordFlyCharacter* Player=Cast<ASwordFlyCharacter>(GetOwner());
    if (!Player)return;
    UAnimInstance* PlayerAnimation = Player->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
          
            PlayerAnimation->Montage_Play(AttackAnimMontage);
            PlayerAnimation->Montage_JumpToSection("SwordAttack",AttackAnimMontage);
        }
           
    }
}

void ASword::SwordAttack()
{
    SwordAttackServer();
}

void ASword::SwordAttackServer_Implementation()
{
    SwordAttackNetMulticast();
}

bool ASword::SwordAttackServer_Validate()
{
    return true;
}

void ASword::SwordAttackNetMulticast_Implementation()
{
    if (!GetOwner())return;
    ASwordFlyCharacter* Player=Cast<ASwordFlyCharacter>(GetOwner());
    if (!Player)return;
    UAnimInstance* PlayerAnimation = Player->GetMesh()->GetAnimInstance();
    if (PlayerAnimation)
    {
        if (AttackAnimMontage&&PlayerAnimation->IsAnyMontagePlaying()==false) {
          
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
