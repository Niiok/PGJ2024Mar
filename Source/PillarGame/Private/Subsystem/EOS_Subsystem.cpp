// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/EOS_Subsystem.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"



void UEOS_Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEOS_Subsystem::Deinitialize()
{
	Super::Deinitialize();
}

FOnlineSessionSettings& UEOS_Subsystem::SetCreateSessionKeyword(FOnlineSessionSettings& InSetting, const FString& InString) const
{
	InSetting.Set(SEARCH_KEYWORDS, InString, EOnlineDataAdvertisementType::ViaOnlineService);
	return InSetting;
}

FOnlineSearchSettings& UEOS_Subsystem::SetSearchSessionKeyword(FOnlineSearchSettings& InSetting, const FString& InString) const
{
	InSetting.Set(SEARCH_KEYWORDS, InString, EOnlineComparisonOp::Equals);
	return InSetting;
}

FOnlineSearchSettings& UEOS_Subsystem::SetSearchSessionKeyword_Neg(FOnlineSearchSettings& InSetting, const FString& InString) const
{
	InSetting.Set(SEARCH_KEYWORDS, InString, EOnlineComparisonOp::NotEquals);
	return InSetting;
}

//void UEOS_Subsystem::CreateSession()
//{
//	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
//	IOnlineSessionPtr SessionInterface = OSS ? OSS->GetSessionInterface() : nullptr;
//	
//	if (SessionInterface.IsValid())
//	{
//		FOnlineSessionSettings SessionSettings;
//		SessionSettings.bIsDedicated = false;
//		SessionSettings.bShouldAdvertise = true;
//		SessionSettings.bIsLANMatch = false;
//		SessionSettings.NumPrivateConnections = 16;
//		SessionSettings.bAllowJoinInProgress = true;;
//		SessionSettings.bAllowJoinViaPresence = true;
//		SessionSettings.bUsesPresence = true;
//
//		//SessionInterface->OnCreateSessionCompleteDelegates.AddDynamic
//		SessionInterface->CreateSession(0, TEXT("SessionName"), SessionSettings);
//	}
//}
//
//void UEOS_Subsystem::OnCreateSession_Complete(FName SessionName, bool bWasSuccessful)
//{
//	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
//	IOnlineSessionPtr SessionInterface = OSS ? OSS->GetSessionInterface() : nullptr;
//
//	if (SessionInterface.IsValid())
//	{
//		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
//	}
//
//	UE_LOG(LogTemp, Warning, TEXT("CreateSession Successful! : %d"), bWasSuccessful);
//}

