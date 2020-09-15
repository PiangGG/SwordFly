// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerState.h"

#include <stdbool.h>

#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/GamePlay/PlayerController/SwordFlyPlayerController.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"

ASwordFlyPlayerState::ASwordFlyPlayerState()
{
    MaxHealth=100.f;
    CurrentHealth=MaxHealth;
    MaxVitality=100.f;
    CurrentVitality=MaxVitality;
    PlayerSpeed = 300.f; 
}

void ASwordFlyPlayerState::SetCurrentWeapon(ASwordFlyBaseWeapon* NewWeapon)
{
    
}

void ASwordFlyPlayerState::SweapWeapon(ASwordFlyBaseWeapon* newWeapon)
{
    
}

void ASwordFlyPlayerState::PackUp(ABaseItem* Itme)
{
   
}

void ASwordFlyPlayerState::UnEquipmentNetMulticast_Implementation()
{
    ASwordFlyCharacter *Player=Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    ASwordFlyPlayerController *PC=Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (Player&&PC&&PC->IsLocalController())
    {
        //if (GetLocalRole()!=ROLE_Authority)return;
       
        if (CurrentWeaponArray.IsValidIndex(0))
        {
            if (CurrentWeaponArray[0])
            {
                CurrentWeaponArray[0]->UnEquipment(Player);
            }
            
            CurrentWeaponArray.RemoveAt(0);
           
            //CurrentWeaponArray[0]==nullptr;
            //return;
        }
       
        if (CurrentWeaponArray.IsValidIndex(0))
        {
           
            CurrentWeaponArray[0]-> Mesh->SetSimulatePhysics(false);
            CurrentWeaponArray[0]->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            CurrentWeaponArray[0]->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            CurrentWeaponArray[0]->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,CurrentWeaponArray[0]->AttachLocation);
            Player->SetCurrentWeapon(CurrentWeaponArray[0]);
          
        }else
        {
            Player->SetCharacterState(ECharacterState::ENone);
        }
      
    }
}

void ASwordFlyPlayerState::CollectHeart(float var)
{
    CurrentHealth += var;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void ASwordFlyPlayerState::ReceiveDamage(float var)
{
    //UE_LOG(LogTemp, Warning, TEXT("ReceiveDamagePlayerState"));
    CurrentHealth -= var;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
    if (CurrentHealth<=0)
    {
        ASwordFlyCharacter *Player=Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
        if (Player)
        {
            Player->Death();
            if (GetWorld()->GetFirstPlayerController())
            {
                GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->UnPossess();
            }
        }
    }
}

void ASwordFlyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASwordFlyPlayerState, CurrentHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentVitality);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxVitality);
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentWeaponArray);
   
}
void ASwordFlyPlayerState::Equipment(ASwordFlyCharacter* Player,ASwordFlyBaseWeapon* Itme)
{
    if (GetLocalRole()!=ROLE_Authority)return;
   
    if (CurrentWeaponArray.IsValidIndex(0)==false&&Itme)
    {
        CurrentWeaponArray.Insert(Itme,0);
        Itme-> Mesh->SetSimulatePhysics(false);
        Itme->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Itme->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Itme->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Itme->AttachLocation);
        Player->SetCurrentWeapon(Itme);
        return;
    }
    if (CurrentWeaponArray.IsValidIndex(0)&&CurrentWeaponArray.IsValidIndex(1)==false)
    {
        CurrentWeaponArray.Insert(Itme,1);
        Itme-> Mesh->SetSimulatePhysics(false);
        Itme-> Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Itme-> Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Itme->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Itme->AttachBackLocation);
        return;
    }else
    {
        return;
    }
}

void ASwordFlyPlayerState::UnEquipment()
{
    UnEquipmentServer();
    /*UE_LOG(LogTemp, Warning, TEXT("UnEquipment2"));
    ASwordFlyCharacter *Player=Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    ASwordFlyPlayerController *PC=Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (Player&&PC&&PC->IsLocalController())
    {
        CurrentWeaponArray[0]->UnEquipment(Player);
        CurrentWeaponArray.RemoveAt(0);
        if (CurrentWeaponArray.IsValidIndex(0))
        {
           
            CurrentWeaponArray[0]-> Mesh->SetSimulatePhysics(false);
            CurrentWeaponArray[0]->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            CurrentWeaponArray[0]->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            CurrentWeaponArray[0]->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,CurrentWeaponArray[0]->AttachLocation);
            Player->SetCurrentWeapon(CurrentWeaponArray[0]);
          
        }else
        {
            Player->SetCharacterState(ECharacterState::ENone);
        }
      
    }*/
}

void ASwordFlyPlayerState::UnEquipmentServer_Implementation()
{
   UnEquipmentNetMulticast();
}

bool ASwordFlyPlayerState::UnEquipmentServer_Validate()
{
    return true;
}
