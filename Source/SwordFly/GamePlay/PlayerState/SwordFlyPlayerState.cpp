// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"

ASwordFlyPlayerState::ASwordFlyPlayerState()
{
    InformationCompoent=CreateDefaultSubobject<USwordFlyInformationrComponent>(FName("InformationCompoent"));

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

void ASwordFlyPlayerState::CollectHeart(float var)
{
    CurrentHealth += var;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void ASwordFlyPlayerState::ReceiveDamage(float var)
{
    UE_LOG(LogTemp, Warning, TEXT("ReceiveDamagePlayerState"));
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

    DOREPLIFETIME(ASwordFlyPlayerState, InformationCompoent);
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxHealth);
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentVitality);
    DOREPLIFETIME(ASwordFlyPlayerState, MaxVitality);
   
}
void ASwordFlyPlayerState::Equipment(ASwordFlyBaseWeapon* Itme)
{
   
    if(!GetWorld()->GetFirstPlayerController()->IsLocalController())return;
    //if (GetLocalRole()!=ROLE_Authority)return;
 
    if (!InformationCompoent->CurrentWeaponArray.IsValidIndex(0))
    {
        InformationCompoent->CurrentWeaponArray.Insert(Itme,0);
        InformationCompoent->CurrentWeapon=Itme;
        
    }else if (InformationCompoent->CurrentWeaponArray.IsValidIndex(0)&&InformationCompoent->CurrentWeaponArray[0]->GetWeaponType()!=Itme->GetWeaponType())
    {
        InformationCompoent->CurrentWeaponArray.Insert(Itme,1);

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

void ASwordFlyPlayerState::UnEquipment()
{
  
    
}
