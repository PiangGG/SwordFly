// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow2.generated.h"

UCLASS()
class SWORDFLY_API AArrow2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Itme")
	class UStaticMeshComponent* Mesh1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	class USphereComponent* AttackComp;

	UFUNCTION(Server,Reliable,WithValidation)
	void OnHitOther(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit);

	float TimeLiva;
};
