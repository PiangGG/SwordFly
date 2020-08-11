// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Itme/BaseItem.h"
#include "SwordFl_BaseSword.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API ASwordFl_BaseSword : public ABaseItem
{
	GENERATED_BODY()
	
public:
	
	ASwordFl_BaseSword();

	//UFUNCTION(BlueprintCallable)
    virtual void SetItmeType(EItmeType Type) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USphereComponent* Collision_Attack;
	
	UFUNCTION()
    void Attack(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //UFUNCTION()
	virtual void Collision_Pack_BeginOverlap(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
