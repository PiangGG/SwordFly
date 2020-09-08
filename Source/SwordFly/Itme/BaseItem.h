// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "BaseItem.generated.h"

UENUM(BlueprintType)
enum class EItmeType:uint8
{
	EWeapon        UMETA(DisplayName="武器"),
	EOther         UMETA(DisplayName="其他")
};
UCLASS()
class SWORDFLY_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual EItmeType GetItemType();

	//物品类型
	UFUNCTION(BlueprintCallable)
	virtual void SetItmeType(EItmeType Type);
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EItmeType thisItmeType;

	//物品所有者
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class ASwordFlyCharacter* thisOwner;
	
	/*Component*///物品组件与信息
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	class USphereComponent* Collision_Pack;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	USlateBrushAsset* ItemIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	FString ItemName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Itme")
	int32 ActorID;
	
	UFUNCTION(Server, WithValidation, Reliable)
	virtual void Collision_Pack_BeginOverlapServer(class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(NetMulticast, Reliable)
	virtual void Collision_Pack_BeginOverlapNetMulticast(class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	virtual void Collision_Pack_BeginOverlap(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	virtual void AfterThroud(class ASwordFlyCharacter* theOwner);
	UFUNCTION(Server, WithValidation, Reliable)
	virtual void AfterThroudServer(class ASwordFlyCharacter* theOwner);
	UFUNCTION(NetMulticast, Reliable)
	virtual void AfterThroudNetMulticast(class ASwordFlyCharacter* theOwner);
	
	//拾取定时器
	FTimerHandle MemberTimerHandle;
	//重置定时器
	void ReSetPackupFTimerHndle();
	
	private:
	
	
};
