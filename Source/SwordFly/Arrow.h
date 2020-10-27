// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Itme/Weapons/SwordFlyBaseWeapon.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API AArrow : public ASwordFlyBaseWeapon
{
	GENERATED_BODY()

	public:
	AArrow();

	virtual void Collision_Pack_BeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
    void OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float DamageVar;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimeLive;

	bool bIsShoot;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Itme")
	class UProjectileMovementComponent *MovementComponent; 
};
