// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordFlyInformationrComponent.generated.h"


class ABaseItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFLY_API USwordFlyInformationrComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwordFlyInformationrComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<class ABaseItem>* ItmeArray;
		
};
