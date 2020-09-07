// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"

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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	ASwordFlyBaseWeapon *CurrentWeapon;
	
	UFUNCTION(BlueprintCallable)
	ASwordFlyBaseWeapon* GetCurrentWeapon();
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeapon(ASwordFlyBaseWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	void SweapWeapon(ABaseItem* newWeapon);
	
	UFUNCTION(BlueprintCallable)
	void PackUp(ABaseItem* Itme);
	
	UFUNCTION(BlueprintCallable)
	void Equipment(ABaseItem* Itme);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipment();
public:
	UFUNCTION(BlueprintCallable, Category = "Player State")
    void CollectHeart(float var);

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	float PlayerSpeed;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void ReceiveDamage(float var);
};
