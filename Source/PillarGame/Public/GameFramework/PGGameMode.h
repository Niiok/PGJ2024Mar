// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PILLARGAME_API APGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController* NewPlayer) override;

	class APGGameState* GetGS() const;

	UFUNCTION(BlueprintCallable)
	void StartNewTurn();

	bool TryPlacing(class APGPlayerController* InPC);

	UFUNCTION(BlueprintCallable)
	void GameOver();

private:
	class APGPlayerController* GetNextPlayer(bool bGetFirstPlayer = false) const;

	UPROPERTY(EditDefaultsOnly)
	FTimespan PlacingTime = 20.0f * ETimespan::TicksPerSecond;
	UPROPERTY(EditDefaultsOnly)
	FTimespan JudgingTime = 5.0f * ETimespan::TicksPerSecond;

	TWeakObjectPtr<class APGPlayerController> PlayerInTurn;
	FTimerHandle TimerHandle_TurnEnd;
	mutable TWeakObjectPtr<class APGGameState> CachedGS;
};
