// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyGameInstance.h"


#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Online.h"

USwordFlyGameInstance::USwordFlyGameInstance(const FObjectInitializer& ObjectInitializer)
{

    CurrentState = EGameState::ENone;

    CurrentWidget = nullptr;


    //Create
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnCreateSessionComplete);

    //Start
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnStartOnlineGameComplete);

    //Find
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnFindSessionsComplete);

    //Join
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnJoinSessionComplete);

    //Update
    OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnUpdateSessionComplete);

    //Destroy
    OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USwordFlyGameInstance::OnDestroySessionComplete);
}

void USwordFlyGameInstance::Init()
{
    Super::Init();

    if (GEngine) {
        GEngine->OnNetworkFailure().AddUObject(this, &USwordFlyGameInstance::HandleNetworkError);
    }
}

void USwordFlyGameInstance::ChangeState(EGameState newState)
{
    //if (CurrentState==nullptr)return;
    
    if (CurrentState != newState) {
        LeaveState();
        EnterState(newState);
    }
}

EGameState USwordFlyGameInstance::GetGameState()
{
    return CurrentState;
}

void USwordFlyGameInstance::SetInputMode(EInputMode newInputMode, bool bShowMouseCursor)
{
    if (!GetWorld()->GetFirstPlayerController())return;
   
    switch (newInputMode) {
    case EInputMode::EUIOnly: {
            GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
            break;
    }
    case EInputMode::EUIAndGame: {
            GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
            break;
    }
    case EInputMode::EGameOnly: {
            GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
            break;
    }
    }

    //show or hide mouse cursor
    //GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bShowMouseCursor;
    GetFirstLocalPlayerController()->bShowMouseCursor=bShowMouseCursor;
    //retain the values for further use
    CurrentInputMode = newInputMode;
 
    bIsShowingMouseCursor = bShowMouseCursor;
}

void USwordFlyGameInstance::HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {

        TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

        TMap<FString, FOnlineSessionSetting> SpecialSettings = TMap<FString, FOnlineSessionSetting>();

        for (auto &setting : sessionSettings) {
        
            FOnlineSessionSetting newSetting;

            newSetting.Data = setting.value;
            
            newSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

            SpecialSettings.Add(setting.key, newSetting);
        }

        ChangeState(EGameState::ELoadingScreen);

        HostSession(pid, GameSessionName, bIsLAN, MaxNumPlayers, SpecialSettings);
    }
}

bool USwordFlyGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN,
    int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid()) {
          
            SessionSettings = MakeShareable(new FOnlineSessionSettings());

            SessionSettings->bIsLANMatch = bIsLAN;
            SessionSettings->bUsesPresence = true; //we will always be using presence; though this can be changed.
            SessionSettings->NumPublicConnections = MaxNumPlayers;
            SessionSettings->NumPrivateConnections = 0;
            SessionSettings->bAllowInvites = true;
            SessionSettings->bAllowJoinInProgress = true;
            SessionSettings->bShouldAdvertise = true;
            SessionSettings->bAllowJoinViaPresence = true;
            SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

            SessionSettings->Set(SETTING_MAPNAME, FString("TextMap"), EOnlineDataAdvertisementType::ViaOnlineService);

            for (auto &setting : SettingsMap) {
                SessionSettings->Settings.Add(FName(*setting.Key), setting.Value);
            }

            OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

            return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
        }
    }
    return false;
}

void USwordFlyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
 
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
      
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            
            Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

           
            if (bWasSuccessful) {
               
                OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

              
                Sessions->StartSession(SessionName);
            }
        }
    }
}

void USwordFlyGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
    //Get the Online subsystem as we need to clear the delegate handle
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        //get the sessions interface
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            //clear the delegate
            Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        }
    }

    //if the start was successful, we open our map as a listen server
    if (bWasSuccessful) {
        UGameplayStatics::OpenLevel(GetWorld(), "TextMap", true, "listen");
        ChangeState(EGameState::ETravelling);
    }
}

void USwordFlyGameInstance::FindGames(bool bIsLAN)
{
    //Get the Online Subsystem
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    bHasFinishedSearchingForGames = false;
    bSearchingForGames = false;
    searchResults.Empty();

    if (OnlineSub) {
        //Get unique net ID
        TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

        FindSessions(pid, GameSessionName, bIsLAN);
    }
}

void USwordFlyGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN)
{
    
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid()) {
            
            SessionSearch = MakeShareable(new FOnlineSessionSearch());

           
            SessionSearch->bIsLanQuery = bIsLAN;
            SessionSearch->MaxSearchResults = 100000000;
            SessionSearch->PingBucketSize = 50;
            SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

            TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

            OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

            bSearchingForGames = true;

            Sessions->FindSessions(*UserId, SearchSettingsRef);
        }
    }
    else {
        //if there's no online subsystem; immediately call the failure of OnFindSessionsComplete
        OnFindSessionsComplete(false);
    }
}

void USwordFlyGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    //first we need to clear the delegate handle

    //get Online subsystem
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        //get sessions interface
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            //clear the delegate handle
            Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
        }
    }

    //then we deal with any search results

    //if we were successful, parse search results; else just end the search
    if (bWasSuccessful) {
        //loop through search results and add them to our blueprint accessible array
        for (auto &result : SessionSearch->SearchResults) {
            FBlueprintSearchResult newresult = FBlueprintSearchResult(result);
            searchResults.Add(newresult);
        }
    }

    //mark searching as over
    bHasFinishedSearchingForGames = true;
    bSearchingForGames = false;
}

void USwordFlyGameInstance::JoinGame(FBlueprintSearchResult result)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        
        TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

        JoinSession(pid, GameSessionName, result.result);
    }
}

bool USwordFlyGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
    const FOnlineSessionSearchResult& SearchResult)
{
    //to hold the result
    bool bSuccessful = false;

    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid()) {
           
            OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

            bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
        }
    }
    return bSuccessful;
}

void USwordFlyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    //Get Online Subsystem so we can clear the delegate
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            //clear the delegate handle
            Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

            //get the first player controller so we can call client travel
            APlayerController *const PlayerController = GetFirstLocalPlayerController();

            //we need an FString to use ClientTravel
            //we can let the session interface construct this string for us
            //by giving it the session name and an empty string
            FString TravelURL;

            if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
                //finally call ClientTravel. If you wanted to you could print the TravelURL
                //to see what it looks like
                PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
                //change state to travelling
                ChangeState(EGameState::ETravelling);
            }
        }
    }
}

FString USwordFlyGameInstance::GetSessionSpecialSettingString(FString key)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
           
            FOnlineSessionSettings *settings = Sessions->GetSessionSettings(GameSessionName);

           
            if (settings) {
                
                if (settings->Settings.Contains(FName(*key))) {
                    
                    FString value;
                    settings->Settings[FName(*key)].Data.GetValue(value);

                    return value;
                }
                else {
                    
                    return FString("INVALID KEY");
                }
            }
        }
        else {
            return FString("NO SESSION!");
        }
    }
    return FString("NO ONLINE SUBSYSTEM");
}

void USwordFlyGameInstance::SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            
            FOnlineSessionSettings *settings = Sessions->GetSessionSettings(GameSessionName);

            if (settings) {
                
                if (settings->Settings.Contains(FName(*newSetting.key))) {
             
                    settings->Settings[FName(*newSetting.key)].Data = newSetting.value;
                }
                else { 
                    FOnlineSessionSetting setting;

                    setting.Data = newSetting.value;
                    setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

                    settings->Settings.Add(FName(*newSetting.key), setting);
                }

                OnUpdateSessionCompleteDelegateHandle = Sessions->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

                Sessions->UpdateSession(GameSessionName, *settings, true);
            }
        }
    }
}

void USwordFlyGameInstance::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
           
            Sessions->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
        }
    }
}

void USwordFlyGameInstance::LeaveGame()
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
       
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
           
            OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

            Sessions->DestroySession(GameSessionName);
        }
    }
}

void USwordFlyGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
           
            Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

        }
    }

    if (bWasSuccessful) {
        UGameplayStatics::OpenLevel(GetWorld(), "Main", true);
        ChangeState(EGameState::ETravelling);
    }
}

void USwordFlyGameInstance::HandleNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
    const FString& ErrorString)
{
    LeaveGame();
}

void USwordFlyGameInstance::EnterState(EGameState newState)
{
  
	CurrentState = newState;
    
	switch (CurrentState) {
	case EGameState::ELoadingScreen: {
		
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cLoadingScreen);

	    if (!CurrentWidget)return;

	    CurrentWidget->AddToViewport();

		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMainMenu: {
		
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMainMenu);

        if (!CurrentWidget)return;
       
        CurrentWidget->AddToViewport();
           
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerHome: {
	
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHome);
	    if (!CurrentWidget)return;
		CurrentWidget->AddToViewport();

		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerJoin: {

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPJoin);
	    if (!CurrentWidget)return;
		CurrentWidget->AddToViewport();
	        
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerHost: {
		
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHost);
	    if (!CurrentWidget)return;
		CurrentWidget->AddToViewport();

		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerInGame: {
		
		SetInputMode(EInputMode::EGameOnly, false);
		break;
	}
	case EGameState::ETravelling: {

		SetInputMode(EInputMode::EUIOnly, false);
		break;
	}
	default: {
		break;
	}
	}
}

void USwordFlyGameInstance::LeaveState()
{
    //if(CurrentState==nullptr)return;
    switch (CurrentState) {
    case EGameState::ELoadingScreen: {
    }
    case EGameState::EMainMenu: {
    }
    case EGameState::EMultiplayerHome: {
    }
    case EGameState::EMultiplayerJoin: {
    }
    case EGameState::EMultiplayerHost: {
           
            if (CurrentWidget) {
                CurrentWidget->RemoveFromViewport();
                CurrentWidget = nullptr;
            }
            break;
    }
    case EGameState::EMultiplayerInGame: {
            break;
    }
    case EGameState::ETravelling: {
            break;
    }
    default: {
            break;
    }
    }

    EnterState(EGameState::ENone);
}


