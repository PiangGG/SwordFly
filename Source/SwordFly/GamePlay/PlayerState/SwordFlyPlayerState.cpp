// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerState.h"
#include "Net/UnrealNetwork.h"
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

void ASwordFlyPlayerState::SweapWeapon(ABaseItem* newWeapon)
{
    /*if (CurrentWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("SweapWeapon"));
        ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>( this);
	
        CurrentWeapon->SetActorHiddenInGame(true);
        for (auto& thisItmeArray:PS->InformationCompoent->ItmeArray)
        {
            if (thisItmeArray.thisItem->ItemName==CurrentWeapon->ItemName)
            {
                thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+1;
                return;
            }
		
        }
        CurrentWeapon=nullptr;
        Equipment(newWeapon);
    }*/
}

void ASwordFlyPlayerState::PackUp(ABaseItem* Itme)
{
    /*ABaseItem* thisItem = Cast<ABaseItem>(Itme);
    UE_LOG(LogTemp, Warning, TEXT("PackUp"));
    switch (thisItem->GetItemType()) {
    case EItmeType::EWeapon:
        if (GetCurrentWeapon()==nullptr)
        {
            Equipment(thisItem);
            //return;
        }else
        {
            ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>( this);
	
            thisItem->SetActorHiddenInGame(true);
            for (auto& thisItmeArray:PS->InformationCompoent->PackItmeArray)
            {
                if (thisItmeArray.thisItem->ItemName==Itme->ItemName)
                {
                    thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+1;
                    return;
                }
		
            }
            FPackItme newItme;
            newItme.thisItem=Itme;
            newItme.thisItemnumber=1;
            PS->InformationCompoent->PackItmeArray.Add(newItme);
        }	
        break;
    case EItmeType::EOther:
        thisItem->Destroy();
        break;
    default:
        thisItem->Destroy();
        break;
    }*/

    if (InformationCompoent)
    {
        InformationCompoent->Putbackpack(Itme,1);
    }
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
void ASwordFlyPlayerState::Equipment(ABaseItem* Itme)
{
    if (InformationCompoent)
    {
        InformationCompoent->Equipment(Itme);
    }
}

void ASwordFlyPlayerState::UnEquipment()
{
    if (InformationCompoent)
    {
        //InformationCompoent->UnEquipmen(InformationCompoent->CurrentWeaponArray);
    }
    
}
