// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SwordFlyGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8 {
	ENotConnected				UMETA(DisplayName = "Not Connected"),
    EWaitingForPlayers			UMETA(DisplayName = "Waiting For Players"),
    EStartingGame				UMETA(DisplayName = "Starting Game"),
    EGameInProgress				UMETA(DisplayName = "Game In Progress"),
    EGameComplete				UMETA(DisplayName = "Game Complete"),
    ERestartingGame				UMETA(DisplayName = "Restarting Game"),

};
/**
 * 
 */
UCLASS()
class SWORDFLY_API ASwordFlyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASwordFlyGameStateBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
	TSubclassOf<class UUserWidget> cWaitingForPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
	TSubclassOf<class UUserWidget> cGameInProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
	TSubclassOf<class UUserWidget> cChangingLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
	TSubclassOf<class UUserWidget> cGameComplete;

	UPROPERTY(Replicated, BlueprintReadOnly)
	APlayerState *Winner;

	UFUNCTION(NetMulticast, BlueprintCallable, Reliable, Category = "Match State")
        void ChangeState(EMatchState newState);

	//get current state
	UFUNCTION(BlueprintCallable, Category = "Match State")
        EMatchState GetMatchState();

	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable, WithValidation, Category = "Match State")
        void UpdateMatchTime(float NewTime);

	//Set the winner
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Match State")
        void SetWinner(APlayerState *NewWinner);

	private:
	//the currently displayed widget
	UUserWidget *CurrentWidget;

	//the current state, replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	EMatchState CurrentMatchState;

	UPROPERTY(Replicated, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float TotalMatchTime;

	//for changing state
	void EnterState(EMatchState newState);
	void LeaveState();
};
