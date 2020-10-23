// Copyright Epic Games, Inc. All Rights Reserved.


#include "SwordFlyGameModeBase.h"


#include "SwordFly/Core/SwordFlyGameInstance.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "SwordFly/GamePlay/GameState/SwordFlyGameStateBase.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"

ASwordFlyGameModeBase::ASwordFlyGameModeBase()
{
   // DefaultPawnClass=ASwordFlyCharacter().GetClass();
   PrimaryActorTick.bStartWithTickEnabled = true;
   PrimaryActorTick.bCanEverTick = true;
}

void ASwordFlyGameModeBase::Logout(AController* Exiting)
{
   //get game state
   ASwordFlyGameStateBase *GS = Cast<ASwordFlyGameStateBase>(GameState);

   if (GS) {
      //remove the player state from the list of players
      GS->RemovePlayerState(Exiting->PlayerState);
   }
}

void ASwordFlyGameModeBase::Tick(float DeltaSeconds)
{
   //ensure we're on server
   if (GetLocalRole() == ROLE_Authority) {
      //get current game state
      ASwordFlyGameStateBase *PlatGameState = Cast<ASwordFlyGameStateBase>(GetWorld()->GetGameState());

      //get game instance
      USwordFlyGameInstance *GameInstance = Cast<USwordFlyGameInstance>(GetGameInstance());

      if (GameInstance && PlatGameState) {
         //check we're in game
         if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) {
            //check match state
            switch (PlatGameState->GetMatchState()) {
            case EMatchState::ENotConnected: {
                  PlatGameState->ChangeState(EMatchState::EWaitingForPlayers);
                  break;
            }
            case EMatchState::EGameInProgress: {
                  //TimeSinceStart += DeltaTime;

                  //check for player deaths and respawn them as appropriate
                  for (auto &player : PlatGameState->PlayerArray) {
                     //cast the state to PlatformerPlayerState
                     ASwordFlyPlayerState *PlatPlayer = Cast<ASwordFlyPlayerState>(player);

                     //if the player is valid and is not a spectator
                     if (PlatPlayer/* && !PlatPlayer->bSpectator*/) {
                        //check if the player is dead
                        if (PlatPlayer->GetCurrentHealth() <= 0) {
                           //get the player controller
                           APlayerController *PlayerController = PlatPlayer->GetNetOwningPlayer()->PlayerController;
                           //triger stat reset
                           PlatPlayer->PlayerRespawnedAfterDeath();

                           //respawn the player
                           PlayerController->GetPawn()->Destroy();
                           PlayerController->ServerRestartPlayer();
                        }
                     }
                  }

                  //PlatGameState->UpdateMatchTime(TimeSinceStart);

                  break;
            }
            case EMatchState::EGameComplete:{
                  break;
            }
            default: {
                  //any other match state will reset timesince start to 0
                  /*if (TimeSinceStart != 0.f) {
                     TimeSinceStart = 0.f;
                     PlatGameState->UpdateMatchTime(TimeSinceStart);
                  }*/
            }
            }
         }
      }
   }
}
