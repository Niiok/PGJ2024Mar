// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGGameState.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PGPlayerController.h"




bool APGGameState::IsPlayerTurn(class APGPlayerController* InPC) const
{
	if (PlayerInTurn.IsValid() == false && HasAuthority())
	{
		return true;
	}

	return InPC && InPC->GetUniqueNetId() == PlayerInTurn;
}

void APGGameState::Multicast_StartPlacing_Implementation(FUniqueNetIdRepl InPlacingPlayer, FDateTime InExpiration)
{
	ExpirationTime = InExpiration;
	PlayerInTurn = InPlacingPlayer;
	State = EPGState::Placing;

	if (auto PGPC = Cast<APGPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		PGPC->OnStartPlacing(this);
	}
}

void APGGameState::Multicast_StartJudging_Implementation(FUniqueNetIdRepl InJudgingPlayer, FDateTime InExpiration)
{
	ExpirationTime = InExpiration;
	PlayerInTurn = InJudgingPlayer;
	State = EPGState::Judging;

	if (auto PGPC = Cast<APGPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		PGPC->OnStartJudging(this);
	}
}

void APGGameState::Multicast_EndGame_Implementation(FUniqueNetIdRepl InLosePlayer)
{
	PlayerInTurn = InLosePlayer;
	State = EPGState::End;

	if (auto PGPC = Cast<APGPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		PGPC->OnEndGame(this);
	}
}