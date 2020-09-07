// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerState.h"
#include "Net/UnrealNetwork.h"

ASwordFlyPlayerState::ASwordFlyPlayerState()
{
    InformationCompoent=CreateDefaultSubobject<USwordFlyInformationrComponent>(FName("InformationCompoent"));

    MaxHealth=100.f;
    CurrentHealth=MaxHealth;
    MaxVitality=100.f;
    CurrentVitality=MaxVitality;
    PlayerSpeed = 300.f;
    CurrentWeapon=nullptr;
}

ASwordFlyBaseWeapon* ASwordFlyPlayerState::GetCurrentWeapon()
{
    return CurrentWeapon;
}

void ASwordFlyPlayerState::SetCurrentWeapon(ASwordFlyBaseWeapon* NewWeapon)
{
    CurrentWeapon=NewWeapon;
}

void ASwordFlyPlayerState::SweapWeapon(ABaseItem* newWeapon)
{
    if (CurrentWeapon)
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
    }
}

void ASwordFlyPlayerState::PackUp(ABaseItem* Itme)
{
    ABaseItem* thisItem = Cast<ABaseItem>(Itme);
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
            for (auto& thisItmeArray:PS->InformationCompoent->ItmeArray)
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
            PS->InformationCompoent->ItmeArray.Add(newItme);
        }	
        break;
    case EItmeType::EOther:
        thisItem->Destroy();
        break;
    default:
        thisItem->Destroy();
        break;
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
    DOREPLIFETIME(ASwordFlyPlayerState, CurrentWeapon);
}
void ASwordFlyPlayerState::Equipment(ABaseItem* Itme)
{
    ASwordFlyBaseWeapon* NewWeapon = Cast<ASwordFlyBaseWeapon>(Itme);
    if (NewWeapon)
    {
        NewWeapon->Equipment(this);
        UE_LOG(LogTemp, Warning, TEXT("Equipment"));
    } 
}

void ASwordFlyPlayerState::UnEquipment()
{
    if (CurrentWeapon)

        {ASwordFlyBaseWeapon* thisCurrentWeapon = Cast<ASwordFlyBaseWeapon>(CurrentWeapon);
        thisCurrentWeapon->UnEquipment(this);
        UE_LOG(LogTemp, Warning, TEXT("UnEquipment"));
    }
    
}
