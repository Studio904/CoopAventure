// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"



void PrintString(const FString& PrintValue, const FColor& PrintColor = FColor::Red) {
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, PrintColor, PrintValue);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PrintValue);
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//PrintString("Multiplayer SubSystem Constructor", FColor::Cyan);
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Co-op Adventure Session Name");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//PrintString("Multiplayer SubSystem Initialize", FColor::Cyan);
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
	 	SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName, FColor::Blue);

		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		}
	}

}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	//PrintString("Multiplayer SubSystem Deinitialize", FColor::Blue);
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	PrintString("Create Server");

	if (ServerName.IsEmpty()) {
		PrintString("Server cannot be empty!");
		ServerCreateDel.Broadcast(false);
		return;

	}

	FNamedOnlineSession *ExistingSession =  SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{
		FString msg = FString::Printf(TEXT("Session with name %s already exists, destroying it"), *MySessionName.ToString());
		PrintString(msg, FColor::Emerald);
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}


	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true; // To find the session
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true; //Region Systems to find session is necessary
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = SubsystemName == "NULL" ? true : false;
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	PrintString("Find Server");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		ServerJoinDel.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = SubsystemName == "NULL" ? true : false;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	ServerNameToFind = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessioncomplete: %d"), bWasSuccessful), FColor::Cyan);

	ServerCreateDel.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/Island/Island?listen");

	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	FString Msg = FString::Printf(TEXT("OnDestroySessionComplete, SessionName: %s, Success: %d"),
		*SessionName.ToString(), bWasSuccessful);
	PrintString(Msg);
	if (CreateServerAfterDestroy)
	{
		CreateServer(DestroyServerName);
		CreateServerAfterDestroy = false;
	}
	
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	

	if (!bWasSuccessful) return;
	if (ServerNameToFind.IsEmpty()) return;
	TArray <FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = 0;

	
	if (Results.Num() > 0)
	{

		FString Msg = FString::Printf(TEXT("OnFindSessionComplete, Success: %d"), Results.Num());
		PrintString(Msg);

		for (FOnlineSessionSearchResult Result : Results)
		{
			if (Result.IsValid()) 
			{
				FString	ServerName = "No-name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &Result;
					FString Msg2 = FString::Printf(TEXT("Found server with Name: %s"),
						*ServerName);
					PrintString(Msg2);
					break;

				}
			}
		}

		

		if (CorrectResult)
		{
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}
		else
		{
			
			PrintString(FString::Printf(TEXT("couldn't find server: %s"),
				*ServerNameToFind));
			ServerNameToFind = "";
			ServerJoinDel.Broadcast(false);
		}
	}
	else
	{
		PrintString("Zero sessions found");
		ServerJoinDel.Broadcast(false);

	}

}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	ServerJoinDel.Broadcast(JoinResult == EOnJoinSessionCompleteResult::Success);

	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		FString Msg = FString::Printf(TEXT("Successfully joined session %s"),
			*SessionName.ToString());
		PrintString(Msg);

		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if (Success)
		{
			PrintString(FString::Printf(TEXT("Address: %s"),
				*Address));

			APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("GetResolvedConnectString  returned failed");
		}
	}
	else
	{
		PrintString("OnJoinSession Complete failed");
	}
}

