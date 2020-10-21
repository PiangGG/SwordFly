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

    void Attack() override;

    virtual void AttackServer_Implementation()override;
	virtual bool AttackServer_Validate() override;
	virtual void AttackNetMulticast_Implementation() override;
	virtual EWeaponType GetWeaponType() override;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SwordAttackBegin();

	UFUNCTION(BlueprintCallable)
	void Attack_2();
	UFUNCTION(Server,WithValidation,Reliable)
	void AttackServer_2();
	UFUNCTION(NetMulticast,Reliable)
	void AttackNetMulticast_2();
	//void SwordFly();
	//攻击
	int32 AttackNumber;

	FTimerHandle TimerHandle_AttackNumberCtrl;

	void AttackNumberCtrl();
};
