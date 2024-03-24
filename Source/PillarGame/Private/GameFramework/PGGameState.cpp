// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGGameState.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PGPlayerController.h"




bool APGGameState::IsPlayerTurn(class APGPlayerController* InPC) const
{
	return PlayerInTurn.IsValid() == false || InPC&& InPC->GetUniqueNetId() == PlayerInTurn;
}

void APGGameState::Multicast_StartPlacing_Implementation(FUniqueNetIdRepl InPlacingPlayer, FDateTime InExpiration)
{
	ExpirationTime = InExpiration;
	PlayerInTurn = InPlacingPlayer;
	State = EPGState::Placing;

	auto LocalPlayer = GetGameInstance()->FindLocalPlayerFromUniqueNetId(InPlacingPlayer.GetUniqueNetId());
	if (auto PGPC = LocalPlayer ? Cast<APGPlayerController>(LocalPlayer->GetPlayerController(GetWorld())) : nullptr)
	{
		PGPC->OnStartPlacing(this);
	}
}

void APGGameState::Multicast_StartJudging_Implementation(FUniqueNetIdRepl InJudgingPlayer, FDateTime InExpiration)
{
	ExpirationTime = InExpiration;
	PlayerInTurn = InJudgingPlayer;
	State = EPGState::Judging;

	auto LocalPlayer = GetGameInstance()->FindLocalPlayerFromUniqueNetId(InJudgingPlayer.GetUniqueNetId());
	if (auto PGPC = LocalPlayer ? Cast<APGPlayerController>(LocalPlayer->GetPlayerController(GetWorld())) : nullptr)
	{
		PGPC->OnStartJudging(this);
	}
}

void APGGameState::Multicast_EndGame_Implementation(FUniqueNetIdRepl InLosePlayer)
{
	PlayerInTurn = InLosePlayer;
	State = EPGState::End;

	auto LocalPlayer = GetGameInstance()->FindLocalPlayerFromUniqueNetId(InLosePlayer.GetUniqueNetId());
	if (auto PGPC = LocalPlayer ? Cast<APGPlayerController>(LocalPlayer->GetPlayerController(GetWorld())) : nullptr)
	{
		PGPC->OnEndGame(this);
	}
}