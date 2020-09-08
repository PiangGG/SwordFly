// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordFlyInformationrComponent.generated.h"
class ABaseItem;

USTRUCT(BlueprintType)
struct FPackItme {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	ABaseItem* thisItem;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	int32 thisItemnumber;
};

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FPackItme> PackItmeArray;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<class ASwordFlyBaseWeapon*> CurrentWeaponArray;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	ASwordFlyBaseWeapon* CurrentWeapon;
	//穿戴装备
	UFUNCTION(BlueprintCallable)
	void Equipment(ABaseItem* Item);
	UFUNCTION(Reliable,WithValidation,Server)
	void EquipmentServer(ABaseItem* Item);
	UFUNCTION(NetMulticast,Reliable)
	void EquipmentServerNetMulticast(ABaseItem* Item);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipmen(ABaseItem* Item,int32 var);
	UFUNCTION(Reliable,WithValidation,Server)
	void UnEquipmenServer(ABaseItem* Item,int32 var);
	UFUNCTION(NetMulticast,Reliable)
	void UnEquipmenNetMulticast(ABaseItem* Item,int32 var);

	UFUNCTION(BlueprintCallable)
	void Putbackpack(ABaseItem* Item,int32 var);
	UFUNCTION(Reliable,WithValidation,Server)
	void PutbackpackServer(ABaseItem* Item,int32 var);
	UFUNCTION(NetMulticast,Reliable)
	void PutbackpackNetMulticast(ABaseItem* Item,int32 var);

	UFUNCTION(BlueprintCallable)
    void OutPutbackpack(ABaseItem* Item,int32 var);
	UFUNCTION(Reliable,WithValidation,Server)
    void OutPutbackpackServer(ABaseItem* Item,int32 var);
	UFUNCTION(NetMulticast,Reliable)
    void OutPutbackpackNetMulticast(ABaseItem* Item,int32 var);
};
