// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, bWasSuccessfull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoinDelegate, bool, bWasSuccessfull);

/**
 * 
 */
UCLASS()
class COOPAVENTURE_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	UMultiplayerSessionsSubsystem();

	IOnlineSessionPtr SessionInterface;

	FString SubsystemName;

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	UFUNCTION(BlueprintCallable)
	void FindServer(FString ServerName);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	bool CreateServerAfterDestroy;
	FString DestroyServerName;
	FString ServerNameToFind;
	FName MySessionName;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate ServerCreateDel;

	UPROPERTY(BlueprintAssignable)
	FServerJoinDelegate ServerJoinDel;

};
