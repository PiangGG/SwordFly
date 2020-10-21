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
	
	//UFUNCTION(BlueprintCallable)
	virtual void Attack(/*class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;
	virtual void AttackServer_Implementation(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;
	virtual void AttackNetMulticast_Implementation() override;
	virtual void BeginPlay() override;

	virtual EWeaponType GetWeaponType() override;
	UFUNCTION(BlueprintCallable)
	void Shoot();
	UFUNCTION(BlueprintCallable,Server,Reliable,WithValidation)
	void ShootServer();
	UFUNCTION(Reliable,NetMulticast)
	void ShootNetMulticast();

	UFUNCTION(BlueprintCallable)
    void Attack_2();
	UFUNCTION(Server,WithValidation,Reliable)
    void AttackServer_2();
	UFUNCTION(NetMulticast,Reliable)
    void AttackNetMulticast_2();

	bool bIsArming;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class AArrow2> ArrowClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float ArrowForce;
};
