// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "SwordFly/Core/SwordFlyGameInstance.h"
#include "Engine/World.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"

ASwordFlyGameStateBase::ASwordFlyGameStateBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CurrentWidget = nullptr;
    CurrentMatchState = EMatchState::ENotConnected;
}

EMatchState ASwordFlyGameStateBase::GetMatchState()
{
    return CurrentMatchState;
}

void ASwordFlyGameStateBase::Tick(float DeltaTime)
{
	if (!GetWorld()->GetFirstPlayerController())return;
	
    ASwordFlyPlayerState *PS = Cast<ASwordFlyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

    if (PS) {
        //if we're out of sync
        //ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(PlayerState);
        
        if (PS->CurrentLocalMatchState != CurrentMatchState) {
            LeaveState();
            EnterState(CurrentMatchState);
        }
    }
}

void ASwordFlyGameStateBase::SetWinner_Implementation(APlayerState* NewWinner)
{
    if (GetLocalRole() == ROLE_Authority) {
        Winner = NewWinner;
    }
}

bool ASwordFlyGameStateBase::SetWinner_Validate(APlayerState* NewWinner)
{
    return true;
}

void ASwordFlyGameStateBase::EnterState(EMatchState newState)
{
    CurrentMatchState = newState;

	//Change state on player
    if (!GetWorld()->GetFirstPlayerController())return;
   
	ASwordFlyPlayerState *CurrentPlayerState = Cast<ASwordFlyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

	if (CurrentPlayerState) {
		CurrentPlayerState->CurrentLocalMatchState = CurrentMatchState;
	}

	//switch on new state
	switch (CurrentMatchState) {
	case EMatchState::ENotConnected: {
		break;
	}
	case EMatchState::EWaitingForPlayers: {
		//Show waiting for players UI
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cWaitingForPlayers);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
		//change input mode
		USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(this->GetGameInstance());

		if (GameInstance) {
			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}

		if (GetLocalRole() == ROLE_Authority) {
			//set in progress session setting to true
			FBlueprintSessionSetting NewSetting;
			NewSetting.key = FString("InProgress");
			NewSetting.value = FString("false");

			GameInstance->SetOrUpdateSessionSpecialSettingString(NewSetting);
		}

		break;
	}
	case EMatchState::EStartingGame: {

		//Change to Game In Progress
		if (GetLocalRole() == ROLE_Authority) {
			//respawn all connected players if we're the server
			for (auto &player : PlayerArray) {
				//destroy the current pawn
				player->GetNetOwningPlayer()->PlayerController->GetPawn()->Destroy();
				//spawn the new pawn
				player->GetNetOwningPlayer()->PlayerController->ServerRestartPlayer();
			}

			ChangeState(EMatchState::EGameInProgress);
		}
		break;
	}
	case EMatchState::EGameInProgress: {

		USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cGameInProgress);
			CurrentWidget->AddToViewport();

			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}

		if (GetLocalRole() == ROLE_Authority) {
			//set in progress session setting to true
			FBlueprintSessionSetting NewSetting;
			NewSetting.key = FString("InProgress");
			NewSetting.value = FString("true");

			GameInstance->SetOrUpdateSessionSpecialSettingString(NewSetting);
		}
		break;
	}
	case EMatchState::EGameComplete: {

		USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

		if (GetLocalRole() == ROLE_Authority) {
			//respawn all players as spectator
			for (auto &player : PlayerArray) {
				APlayerController *PC = player->GetNetOwningPlayer()->PlayerController;

				PC->GetPawn()->Destroy();
				PC->ServerRestartPlayer();
			}
		}

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cGameComplete);
		CurrentWidget->AddToViewport();

		GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EMatchState::ERestartingGame: {

		//get the game isntance
		USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			//display the change level widget
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cChangingLevel);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
			

			GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		}

		//respawn the players as spectators
		if (GetLocalRole() == ROLE_Authority) {

			Winner = nullptr;

			//respawn all connected players if we're the server
			for (auto &player : PlayerArray) {
				//destroy the current pawn
				player->GetNetOwningPlayer()->PlayerController->GetPawn()->Destroy();
				//spawn the new pawn
				player->GetNetOwningPlayer()->PlayerController->ServerRestartPlayer();
			}

			//Travel to a new instance of our map by getting the session setting
			//"MAPNAME" and passing it to ServerTravel
			FString mapname = GameInstance->GetSessionSpecialSettingString("MAPNAME");
			GetWorld()->ServerTravel(mapname);
		}
		break;
	}
	}
}

void ASwordFlyGameStateBase::LeaveState()
{
	//switch on new state
	switch (CurrentMatchState) {
	case EMatchState::ENotConnected: {
			//		break;
	}
	case EMatchState::EWaitingForPlayers: {
			//		break;
	}
	case EMatchState::EStartingGame: {
			//		break;
	}
	case EMatchState::EGameInProgress: {
			//		break;
	}
	case EMatchState::EGameComplete: {
			//		break;
	}
	case EMatchState::ERestartingGame: {
			//hide any widget
			if (CurrentWidget) {
				CurrentWidget->RemoveFromViewport();
				CurrentWidget = nullptr;
			}
			break;
	}
	}
}

void ASwordFlyGameStateBase::UpdateMatchTime_Implementation(float NewTime)
{
    TotalMatchTime = NewTime;
}

bool ASwordFlyGameStateBase::UpdateMatchTime_Validate(float NewTime)
{
    return true;
}

void ASwordFlyGameStateBase::ChangeState_Implementation(EMatchState newState)
{
    if (CurrentMatchState != newState) {
        LeaveState();
        EnterState(newState);
    }
}

void ASwordFlyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
    DOREPLIFETIME(ASwordFlyGameStateBase, CurrentMatchState);
    DOREPLIFETIME(ASwordFlyGameStateBase, TotalMatchTime);
    DOREPLIFETIME(ASwordFlyGameStateBase, Winner);
}
