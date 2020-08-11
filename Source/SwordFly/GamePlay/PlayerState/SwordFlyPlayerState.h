// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFlyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API ASwordFlyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	ASwordFlyPlayerState();
	
	/*UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	class USwordFlyInformationrComponent* InformationCompoent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int CurrentHealth;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int MaxHealth;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int CurrentVitality;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int MaxVitality;*/

public:
	UFUNCTION(BlueprintCallable, Category = "Player State")
    void CollectHeart();
};
