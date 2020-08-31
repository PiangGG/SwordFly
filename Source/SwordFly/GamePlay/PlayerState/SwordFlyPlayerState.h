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
	
public:

	ASwordFlyPlayerState();
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	class USwordFlyInformationrComponent* InformationCompoent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float CurrentHealth;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float MaxHealth;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float CurrentVitality;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float MaxVitality;

public:
	UFUNCTION(BlueprintCallable, Category = "Player State")
    void CollectHeart(float var);

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float PlayerSpeed;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void ReceiveDamage(float var);
};
