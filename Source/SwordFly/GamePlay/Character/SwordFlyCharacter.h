// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SwordFlyCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState:uint8
{
	ENone        	UMETA(DisplayName="正常状态"),
	ESword 			UMETA(DisplayName="剑"),
	EBow 			UMETA(DisplayName="弓"),
    EOther         	UMETA(DisplayName="其他")
};
UCLASS()
class SWORDFLY_API ASwordFlyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwordFlyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 * Component
	 */
	//相机手臂
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	//相机第三人称
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	class UCameraComponent* TiredCamera;

	//设置角色基础移动
	void MoveForward(float amount);

	void MoveRight(float amount);

	void RotateCamera(float amount);

	void ChangeCameraHeight(float amount);
	//控制角色状态
	UFUNCTION(BlueprintCallable)
	ECharacterState GetCharacterState();
	UFUNCTION(BlueprintCallable)
	void SetCharacterState(ECharacterState newState);
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	ECharacterState CurrentCharacterState;

	//角色武器
	class ABaseItem * CurrentWeapon;
	ABaseItem * GetCurrentWeapon();
	void SetCurrentWeapon(class ABaseItem* Weapon);
	
	class BaseItem * LastWeapon;
	
	//拾取物品
	void PackUp(class ABaseItem* Itme);
};
