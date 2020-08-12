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
}

void ASwordFlyPlayerState::CollectHeart(float var)
{
    CurrentHealth += 1;
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