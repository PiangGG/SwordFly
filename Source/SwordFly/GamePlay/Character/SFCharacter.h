// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SFCharacter.generated.h"

UENUM(BlueprintType)
enum class ESFPlayerState:uint8
{
	ENone        	UMETA(DisplayName="正常状态"),
    ESword 			UMETA(DisplayName="剑"),
    EBow 			UMETA(DisplayName="弓"),
    EOther         	UMETA(DisplayName="其他")
};
UCLASS()
class SWORDFLY_API ASFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASFCharacter();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	UFUNCTION()
	void RunStart();
	UFUNCTION(Server,Unreliable)
	void RunStartServer();
	UFUNCTION()
    void RunEnd();
	UFUNCTION(Server,Unreliable)
    void RunEndServer();
	UPROPERTY(ReplicatedUsing = OnRep_bIsRun,EditDefaultsOnly,BlueprintReadOnly)
	bool bIsRun;
	UFUNCTION()
	void OnRep_bIsRun();
	
	void Jump();
};