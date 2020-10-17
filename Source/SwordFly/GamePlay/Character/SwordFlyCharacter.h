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
	ESwordFly		UMETA(DisplayName="御剑飞行"),
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

	//Mesh 上面的接口
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName Socket_Right="Socket_Right_FString";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName Socket_Left="Socket_Left_FString";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName Socket_Back="Socket_Back_FString";
	
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
	
	UPROPERTY(Replicated,BlueprintReadOnly,EditDefaultsOnly)
	ECharacterState CurrentCharacterState;

	UPROPERTY(Replicated, BlueprintReadOnly, EditDefaultsOnly)
	float CharacterMaxSpeed;
	//角色武器
	UFUNCTION(BlueprintCallable)
	class ASwordFlyBaseWeapon* GetCurrentWeapon();
	void SetCurrentWeapon(class ASwordFlyBaseWeapon* Weapon);
	
	
	//拾取物品
	UFUNCTION(BlueprintCallable)
	void PackUp(class ABaseItem* Itme);
	UPROPERTY(Replicated,BlueprintReadOnly,EditDefaultsOnly)
	TArray<ASwordFlyBaseWeapon*> PlayerWeaponArray;
	UPROPERTY(Replicated,BlueprintReadOnly,EditDefaultsOnly)
	TArray<ABaseItem* >PlayerItemArray;
	//装备武器
	UFUNCTION(BlueprintCallable)
	void Equipment(class ASwordFlyBaseWeapon* Itme);
	UFUNCTION(Server,WithValidation,Reliable)
    void EquipmentServer(class ASwordFlyBaseWeapon* Itme);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipment();
	UFUNCTION(Server,WithValidation,Reliable)
    void UnEquipmentServer();
	
	

	UFUNCTION(BlueprintCallable)
    void SweapWeapon(class ASwordFlyBaseWeapon* newWeapon);
	
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(Server,WithValidation,Reliable)
    void AttackServer();

	UFUNCTION(BlueprintCallable)
    void Attack_2();
	UFUNCTION(Server,WithValidation,Reliable)
    void AttackServer_2();
	
	//被伤害
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float var);

	//virtual void recae
	UFUNCTION(BlueprintCallable)
	void Death();
	UFUNCTION(BlueprintCallable,Server,WithValidation,Reliable)
    void DeathServer();
	
	//角色跑
	bool isRuning;
	UFUNCTION()
    void Run();
	UFUNCTION(Server, WithValidation, Reliable)
    void RunServer();

	//角色御剑
	bool bIsFly;
};

