// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordFlyBaseWeapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API ABow : public ASwordFlyBaseWeapon
{
	GENERATED_BODY()

	public:
	ABow();

	virtual void Attack(/*class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/) override;
	
	virtual void AttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;
	
	virtual void AttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void Shoot();

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class AArrow> ArrowClass;
};
