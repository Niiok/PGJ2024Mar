// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGPlayerController.h"

#include "GameFramework/PGGameMode.h"
#include "GameFramework/PGGameState.h"

#if UE_EDITOR
#include "Misc/MessageDialog.h"
#endif 

void APGPlayerController::SpawnActor_Server_Implementation(int32 InClassIndex, const FTransform& InTransform)
{
	UClass* SpawnClass = SpawnClasses.IsValidIndex(InClassIndex) ? SpawnClasses[InClassIndex].LoadSynchronous() : nullptr;

	if (SpawnClass != nullptr)
	{
		SpawnActor_Internal(SpawnClass, InTransform);
	}
}

AActor* APGPlayerController::SpawnActor_Blueprint(const TSubclassOf<AActor> InClass, const FTransform& InTransform)
{
	APGGameState* GS = GetWorld()->GetGameState<APGGameState>();
	if (GS == nullptr || GS->IsPlayerTurn(this) == false)
	{
		return nullptr;
	}

	if (HasAuthority())
	{
		if (InClass != nullptr)
		{
			return SpawnActor_Internal(InClass, InTransform);
		}
		else
		{
#if UE_EDITOR
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This Actor doesn't exist in PGPlayerController!")));
#endif
			return nullptr;
		}
	}
	else
	{
		auto ClassIndex = GetClassIndex(InClass);

		if (ClassIndex > INDEX_NONE)
		{
			SpawnActor_Server(ClassIndex, InTransform);
			return nullptr;
		}
		else
		{
#if UE_EDITOR
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This Actor doesn't exist in PGPlayerController!")));
#endif
			return nullptr;
		}
	}
}

FUniqueNetIdRepl APGPlayerController::GetUniqueNetId() const
{
	if (NetConnection)
	{
		return NetConnection->PlayerId;
	}
	else
	{
		check(GetLocalPlayer() != nullptr);
		return GetLocalPlayer()->GetPreferredUniqueNetId();
	}
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		Server_ImReady();
	}
}

AActor* APGPlayerController::SpawnActor_Internal(TSubclassOf<AActor> InClass, const FTransform& InTransform)
{
	check(HasAuthority());

	APGGameMode* GM = GetWorld()->GetAuthGameMode<APGGameMode>();
	if (GM && GM->TryPlacing(this))
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(InClass, &InTransform);
		SpawnedActor->SetReplicates(true);
		SpawnedActor->SetReplicatingMovement(true);
		return SpawnedActor;
	}
	return nullptr;
}

int32 APGPlayerController::GetClassIndex(const TSubclassOf<AActor> InClass) const
{
	// init class map
	if (ClassToIndex.Num() != SpawnClasses.Num())
	{
		ClassToIndex.Empty(SpawnClasses.Num());
		for (auto it = SpawnClasses.CreateConstIterator(); it; ++it)
		{
			ClassToIndex.Emplace(it->GetUniqueID(), it.GetIndex());
		}
	}

	auto Index = ClassToIndex.Find(InClass->GetPathName());
	return Index ? *Index : INDEX_NONE;
}

void APGPlayerController::Server_ImReady_Implementation()
{
	bIsClientReady = true;
}
