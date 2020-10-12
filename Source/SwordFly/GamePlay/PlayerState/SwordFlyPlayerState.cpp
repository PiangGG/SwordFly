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
    PlayerItemArray=TArray<ABaseItem*>();
    PlayerWeaponArray=TArray<ASwordFlyBaseWeapon*>();
}

void ASwordFlyPlayerState::SetCurrentWeapon(ASwordFlyBaseWeapon* NewWeapon)
{
    
}

void ASwordFlyPlayerState::SweapWeapon(ASwordFlyBaseWeapon* newWeapon)
{
    
}

void ASwordFlyPlayerState::PackUp(ABaseItem* Itme)
{
    PlayerItemArray.Add(Itme); 
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
            
        }
    }
}

int ASwordFlyPlayerState::GetCurrentHealth()
{
    return CurrentHealth;
}

void ASwordFlyPlayerState::PlayerRespawnedAfterDeath_Implementation()
{
    if (GetLocalRole() == ROLE_Authority) {
        CurrentHealth = MaxHealth;
    }
}

bool ASwordFlyPlayerState::PlayerRespawnedAfterDeath_Validate()
{
    return true;
}

void ASwordFlyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASwordFlyPlayerState, CurrentHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentVitality);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxVitality);
    DOREPLIFETIME(ASwordFlyPlayerState, PlayerItemArray);
    DOREPLIFETIME(ASwordFlyPlayerState, PlayerWeaponArray);
   
}
void ASwordFlyPlayerState::Equipment(ASwordFlyBaseWeapon* Itme)
{
        PlayerWeaponArray.Add(Itme);   
}

void ASwordFlyPlayerState::UnEquipment()
{
    if (PlayerWeaponArray.IsValidIndex(0))
    {
        PlayerWeaponArray.RemoveAt(0);
    }
    
}
