// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGGameMode.h"

#include "GameFramework/PGGameState.h"
#include "GameFramework/PGPlayerController.h"




void APGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto PGPC = Cast<APGPlayerController>(NewPlayer);
	if (PGPC && PlayerInTurn.IsValid() == false && NewPlayer->IsLocalController())
	{
		PlayerInTurn = PGPC;
	}
}

class APGGameState* APGGameMode::GetGS() const
{
	if (CachedGS.IsValid() == false)
	{
		CachedGS = GetGameState<APGGameState>();
		check(CachedGS.IsValid());
	}

	return CachedGS.Get();
}

void APGGameMode::StartNewTurn()
{
	PlayerInTurn = GetNextPlayer();
	GetGS()->Multicast_StartPlacing(PlayerInTurn->GetUniqueNetId(), FDateTime::UtcNow() + PlacingTime);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TurnEnd, this, &APGGameMode::GameOver, PlacingTime.GetTotalSeconds(), false);
}

bool APGGameMode::TryPlacing(class APGPlayerController* InPC)
{
	check(InPC);

	if (GetGS()->IsPlayerTurn(InPC) && GetGS()->GetState() != EPGState::Judging)
	{
		GetGS()->Multicast_StartJudging(InPC->GetUniqueNetId(), FDateTime::UtcNow() + JudgingTime);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_TurnEnd, this, &APGGameMode::StartNewTurn, JudgingTime.GetTotalSeconds(), false);
		return true;
	}
	else
	{
		return false;
	}
}

void APGGameMode::GameOver()
{
	check(PlayerInTurn.IsValid());
	GetGS()->Multicast_EndGame(PlayerInTurn->GetUniqueNetId());

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TurnEnd);
}

class APGPlayerController* APGGameMode::GetNextPlayer(bool bGetFirstPlayer /*= false*/) const
{
	bGetFirstPlayer = bGetFirstPlayer || PlayerInTurn.IsValid() == false;

	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (bGetFirstPlayer)
		{
			auto PGPC = Cast<APGPlayerController>(it->Get());
			if (PGPC && PGPC->IsClientReady())
			{
				return PGPC;
			}
		}
		else if (*it == PlayerInTurn)
		{
			if (it + 1)
			{
				auto PGPC = Cast<APGPlayerController>((it + 1)->Get());
				if (PGPC && PGPC->IsClientReady())
				{
					return PGPC;
				}
			}
			else
			{
				return GetNextPlayer(true);
			}
		}
	}

	return nullptr;
}
