// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerController.h"
#include "Components/InputComponent.h"
#include "SwordFly/SwordFlyGameInstance.h"


ASwordFlyPlayerController::ASwordFlyPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

    bPauseMenuDisplayed = false;
    bPlayerListDisplayed = false;}

void ASwordFlyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent)return;
    InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::TogglePauseMenu);
    InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::ShowPlayerList);
    InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Released, this, &ASwordFlyPlayerController::HidePlayerList);
}

void ASwordFlyPlayerController::Tick(float DeltaTime)
{
    if (IsLocalController()) {
        
        USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

        if (GameInstance) {
            
            if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) {
                
                if (HUD == nullptr) {
                   
                    if (cHUD) {
                        HUD = CreateWidget<UUserWidget>(this, cHUD);
                        HUD->AddToViewport();
                    }
                }
            }
            else if (HUD != nullptr) {
                
                HUD->RemoveFromViewport();
                HUD = nullptr;
            }
        }
    }
}

void ASwordFlyPlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    if (HUD) {
        HUD->RemoveFromViewport();
        HUD = nullptr;
    }
}

void ASwordFlyPlayerController::TogglePauseMenu()
{
    bPauseMenuDisplayed = !bPauseMenuDisplayed;
		
    USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

    if (GameInstance) {
        if (bPauseMenuDisplayed) {

            if (bPlayerListDisplayed) {
                bPlayerListDisplayed = false;
            }
            
            ModeBeforePause = GameInstance->CurrentInputMode;
            bShowCursorBeforePause = GameInstance->bIsShowingMouseCursor;

           
            GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
        }
        else {
            
            GameInstance->SetInputMode(ModeBeforePause, bShowCursorBeforePause);
            GameInstance->SetInputMode(EInputMode::EGameOnly, false);
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("TogglePauseMenu"));
}

void ASwordFlyPlayerController::ShowPlayerList()
{
   
    if (!bPauseMenuDisplayed) {
        
        bPlayerListDisplayed = true;
        
        USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

        if (GameInstance) {
            GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
        }
    }
}

void ASwordFlyPlayerController::HidePlayerList()
{
    if (bPlayerListDisplayed) {
        bPlayerListDisplayed = false;
        
        USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

        if (GameInstance) {
            GameInstance->SetInputMode(EInputMode::EGameOnly, false);
        }
    }
}

void ASwordFlyPlayerController::HideAllMenus()
{
    HidePlayerList();

    if (bPauseMenuDisplayed) {
        TogglePauseMenu();
    }
}