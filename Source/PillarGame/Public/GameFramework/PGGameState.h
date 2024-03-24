// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGGameState.generated.h"


UENUM(BlueprintType)
enum class EPGState : uint8
{
	None,
	Ready,
	Placing,
	Judging,
	End
};

/**
 * 
 */
UCLASS()
class PILLARGAME_API APGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FDateTime GetExpirationTime() const { return ExpirationTime; }
	UFUNCTION(BlueprintPure)
	bool IsPlayerTurn(class APGPlayerController* InPC) const;
	UFUNCTION(BlueprintPure)
	EPGState GetState() const { return State; }

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartPlacing(FUniqueNetIdRepl InPlacingPlayer, FDateTime InExpiration);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartJudging(FUniqueNetIdRepl InJudgingPlayer, FDateTime InExpiration);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndGame(FUniqueNetIdRepl InLosePlayer);

private:
	FDateTime ExpirationTime;
	FUniqueNetIdRepl PlayerInTurn;
	EPGState State = EPGState::Ready;
};
