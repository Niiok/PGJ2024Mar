// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"

#include "EOS_Subsystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UEOS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem
	bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	// USubsystem

	UFUNCTION(BlueprintPure)
	FName GetOnlinePlatformName() const { return GetGameInstance()->GetOnlinePlatformName(); }
	
	UFUNCTION(BlueprintPure)
	FOnlineSessionSettings& SetCreateSessionKeyword(UPARAM(ref) FOnlineSessionSettings& InSetting, const FString& InString) const;
	
	UFUNCTION(BlueprintPure)
	FOnlineSearchSettings& SetSearchSessionKeyword(UPARAM(ref) FOnlineSearchSettings& InSetting, const FString& InString) const;
	
	UFUNCTION(BlueprintPure)
	FOnlineSearchSettings& SetSearchSessionKeyword_Neg(UPARAM(ref) FOnlineSearchSettings& InSetting, const FString& InString) const;

	/*
	UFUNCTION(BlueprintCallable)
	void CreateSession();*/

protected:
	//void OnCreateSession_Complete(FName SessionName, bool bWasSuccessful);
	//void OnCreateSession_(FName SessionName, bool bWasSuccessful);
};
