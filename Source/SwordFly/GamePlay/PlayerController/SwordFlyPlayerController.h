// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "SwordFlyPlayerController.generated.h"

enum class EInputMode : unsigned char;
/**
 * 
 */
UCLASS()
class SWORDFLY_API ASwordFlyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASwordFlyPlayerController();

	//To hold a reference to our UMG HUD widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<class UUserWidget> cHUD;

	//HUD instance
	UUserWidget *HUD;

	//to hide or show pause menu
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
	bool bPauseMenuDisplayed;

	EInputMode ModeBeforePause;

	bool bShowCursorBeforePause;

	//to hide or show the player list
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
	bool bPlayerListDisplayed;


	//To setup the input component
	virtual void SetupInputComponent() override;

	//tick
	virtual void Tick(float DeltaTime) override;

	//endplay
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	void TogglePauseMenu();

	void ShowPlayerList();
	
	void HidePlayerList();
	//装备信息面板
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<class UUserWidget> InfoWBP;
	//库存
	void Inventory();
	bool bisInventoryOpen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UUserWidget *InfoWBPList;

	UFUNCTION(BlueprintNativeEvent)
	void UpDateInfoWBPList();
	UFUNCTION(BlueprintCallable, Category = "Platformer Player Controller")
    void HideAllMenus();

	

	bool isRuning;
	UFUNCTION()
	void Run();

	UFUNCTION(Server, WithValidation, Reliable)
	void RunServer();

	UFUNCTION(NetMulticast, Reliable)
	void RunNetMulticast();
};
