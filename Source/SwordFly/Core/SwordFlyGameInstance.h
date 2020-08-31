// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SwordFlyGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState : uint8 {
	ENone				UMETA(DisplayName = "None"),
    ELoadingScreen		UMETA(DisplayName = "Loading Screen"),
    EStartup			UMETA(DisplayName = "Startup"),
    EMainMenu			UMETA(DisplayName = "Main Menu"),
    EMultiplayerHome	UMETA(DisplayName = "Multiplayer Home"),
    EMultiplayerJoin	UMETA(DisplayName = "Multiplayer Join"),
    EMultiplayerHost	UMETA(DisplayName = "Multiplayer Host"),
    EMultiplayerInGame	UMETA(DisplayName = "Multiplayer In Game"),
    ETravelling			UMETA(DisplayName = "Travelling"),
};

UENUM(BlueprintType)
enum class EInputMode : uint8 {
	EUIOnly			UMETA(DisplayName = "UI Only"),
    EUIAndGame		UMETA(DisplayName = "UI And Game"),
    EGameOnly		UMETA(DisplayName = "Game Only"),
};

USTRUCT(BlueprintType)
struct FBlueprintSessionSetting {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
	FString key;

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
	FString value;
};

USTRUCT(BlueprintType)
struct FBlueprintSearchResult {
	GENERATED_BODY()

	//Our search result. this type is not blueprint accessible
	FOnlineSessionSearchResult result;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bIsInProgress;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString MapName;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int PingInMs;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int CurrentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int MaxPlayers;

	//Constructor for empty Search Result
	FBlueprintSearchResult() {
		ServerName = FString("No Server Info");
		PingInMs = -1;
		MapName = FString("No Map Info");
		CurrentPlayers = 0;
		MaxPlayers = 0;
	}

	//Constructor when provided a search result
	FBlueprintSearchResult(FOnlineSessionSearchResult theResult) 
	{
		//keep the result
		result = theResult;

		//retrieve special settings from the search result
		ServerName = GetSpecialSettingString(FString("ServerName"));
		MapName = GetSpecialSettingString(FString("MAPNAME"));
		FString InProgressString = GetSpecialSettingString(FString("InProgress"));

		if (InProgressString == FString("true")) {
			bIsInProgress = true;
		}
		else {
			bIsInProgress = false;
		}

		//get some built in setting data
		MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
		CurrentPlayers = MaxPlayers - result.Session.NumOpenPublicConnections;
		PingInMs = result.PingInMs;
	}

	//function for getting special setting data from our result
	FString GetSpecialSettingString(FString key) {
		//retrieve the session settings
		FOnlineSessionSettings settings = result.Session.SessionSettings;

		//check to see if the key exists
		if (settings.Settings.Contains(FName(*key))) {
			FString value;

			//retrieve the value from the settings
			settings.Settings[FName(*key)].Data.GetValue(value);
			return value;
		}

		//if it doesnt contain that setting
		return FString("NO DATA AT THAT KEY");
	}
};

UCLASS()
class SWORDFLY_API USwordFlyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	USwordFlyGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	/* Widget references, these will be what is displayed on screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SwordFlyGameInstance")
	TSubclassOf<class UUserWidget> cMainMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SwordFlyGameInstance")
	TSubclassOf<class UUserWidget> cMPHome;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SwordFlyGameInstance")
	TSubclassOf<class UUserWidget> cMPJoin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SwordFlyGameInstance")
	TSubclassOf<class UUserWidget> cMPHost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SwordFlyGameInstance")
	TSubclassOf<class UUserWidget> cLoadingScreen;

	UFUNCTION(BlueprintCallable, Category = "SwordFlyGameInstance")
    void ChangeState(EGameState newState);

	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
    EGameState GetGameState();

	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
    void SetInputMode(EInputMode newInputMode, bool bShowMouseCursor);

	EInputMode CurrentInputMode;
	
	bool bIsShowingMouseCursor;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	UFUNCTION(BlueprintCallable, Category = "Session Management")
    void HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings);

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
	TArray<FBlueprintSearchResult> searchResults;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
	bool bHasFinishedSearchingForGames;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
	bool bSearchingForGames;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	UFUNCTION(BlueprintCallable, Category = "Session Management")
	void FindGames(bool bIsLAN);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN);

	void OnFindSessionsComplete(bool bWasSuccessful);
	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	UFUNCTION(BlueprintCallable, Category = "Session Management")
    void JoinGame(FBlueprintSearchResult result);

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	UFUNCTION(BlueprintCallable, Category = "Session Management")
    FString GetSessionSpecialSettingString(FString key);

	UFUNCTION(BlueprintCallable, Category = "Session Management")
    void SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting);

	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	UFUNCTION(BlueprintCallable, Category = "Session Management")
    void LeaveGame();

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	void HandleNetworkError(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);

private:
	UUserWidget *CurrentWidget;

	//current game state
	EGameState CurrentState;

	//function for entering a state
	void EnterState(EGameState newState);
	//function for leaving current state
	void LeaveState();
};
