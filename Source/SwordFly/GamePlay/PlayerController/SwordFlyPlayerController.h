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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Player Controller")
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

	UFUNCTION(BlueprintCallable, Category = "Platformer Player Controller")
    void HideAllMenus();
};