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
	virtual void AttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;
	virtual void AttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/)override;

	UFUNCTION(BlueprintCallable)
	void BowAttack(/*class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	UFUNCTION(Server,WithValidation,Reliable)
	void BowAttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/) ;
	//bool AttackServer_Validate(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	UFUNCTION(NetMulticast, Reliable)
	void BowAttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/) ;

	virtual void BeginPlay() override;

	virtual EWeaponType GetWeaponType() override;
	UFUNCTION(BlueprintCallable)
	void Shoot();
	UFUNCTION(BlueprintCallable,Server,Reliable,WithValidation)
	void ShootServer();
	UFUNCTION(Reliable,NetMulticast)
	void ShootNetMulticast();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class AArrow> ArrowClass;
};
