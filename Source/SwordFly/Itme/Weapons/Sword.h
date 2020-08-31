// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordFlyBaseWeapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API ASword : public ASwordFlyBaseWeapon
{
	GENERATED_BODY()
	
	public:
	ASword();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UCapsuleComponent* Collision_Capsule;
	
    virtual void Attack(/*class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/) override;
	
    virtual void AttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;
	
    virtual void AttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SwordAttackBegin();
	
};