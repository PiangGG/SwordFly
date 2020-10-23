// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyPlayerController.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "SwordFly/Core/SwordFlyGameInstance.h"


class USwordFlyGameInstance;

ASwordFlyPlayerController::ASwordFlyPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

    bPauseMenuDisplayed = false;
    bPlayerListDisplayed = false;
    bisInventoryOpen=false;

    UUserWidget_PauseMenu=nullptr;
}

void ASwordFlyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent)return;
    InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::TogglePauseMenu);
    InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::ShowPlayerList);
    InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Released, this, &ASwordFlyPlayerController::HidePlayerList);
    InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::Inventory);
    //InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ASwordFlyPlayerController::Run);
    //InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ASwordFlyPlayerController::Run);
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
            if (IsValid(UUserWidget_PauseMenu))
            {
                UUserWidget_PauseMenu->AddToViewport();
            }else
            {
                UUserWidget_PauseMenu = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseMenu);
                UUserWidget_PauseMenu->AddToViewport();
            }
            GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
        }
        else {
            bShowCursorBeforePause = GameInstance->bIsShowingMouseCursor;
            if (IsValid(UUserWidget_PauseMenu))
            {
                UUserWidget_PauseMenu->RemoveFromParent();
                UUserWidget_PauseMenu=nullptr;
            }
            GameInstance->SetInputMode(ModeBeforePause, bShowCursorBeforePause);
            GameInstance->SetInputMode(EInputMode::EGameOnly, false);
        }
    }
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

void ASwordFlyPlayerController::UpDateInfoWBPList_Implementation()
{
    if (!InfoWBPList)return;
    
}

void ASwordFlyPlayerController::HideAllMenus()
{
    HidePlayerList();

    if (bPauseMenuDisplayed) {
        TogglePauseMenu();
    }
}

void ASwordFlyPlayerController::CharacterDeath()
{
    HideAllMenus();
    UUserWidget_Death = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), Death);
    if (UUserWidget_Death)
    {
        UUserWidget_Death->AddToViewport();
    }
}

void ASwordFlyPlayerController::Inventory()
{
    if (!bisInventoryOpen)
    {
        bisInventoryOpen=true;
        USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

        if (GameInstance) {
            GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
        }

        if (InfoWBP)
        {
            InfoWBPList=CreateWidget<UUserWidget>(this,InfoWBP);//CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(),InfoWBP);
            InfoWBPList->AddToViewport();
        }
   
    }else
    {
        bisInventoryOpen=false;
        USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

        if (GameInstance) {
            GameInstance->SetInputMode(EInputMode::EGameOnly, false);
        }
        if(InfoWBPList)
        {
            InfoWBPList->RemoveFromViewport();
        }
    }
}
