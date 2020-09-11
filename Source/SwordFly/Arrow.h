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
        void OnHitActor(UPrimitiveComponent * HitComponent, AActor *OtherActor, UPrimitiveComponent* OtherComp
            , FVector NormalImpulse, const FHitResult &Hit);
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float DamageVar;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimeLive;
	virtual void Tick(float DeltaSeconds) override;
};
