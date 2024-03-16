// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGPlayerController.h"

#if UE_EDITOR
#include "Misc/MessageDialog.h"
#endif 

void APGPlayerController::SpawnPillar_Server_Implementation(FTransform InTransform, int32 InClassIndex)
{
	UClass* SpawnClass = SpawnClasses.IsValidIndex(InClassIndex) ? SpawnClasses[InClassIndex] : nullptr;

	if (SpawnClass != nullptr)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(SpawnClass);
		SpawnedActor->SetReplicates(true);
		SpawnedActor->SetReplicatingMovement(true);
	}
}

bool APGPlayerController::SpawnPillar_Blueprint(FTransform InTransform, TSubclassOf<AActor> InClass)
{
	if (HasAuthority())
	{
		if (InClass != nullptr)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(InClass);
			SpawnedActor->SetReplicates(true);
			SpawnedActor->SetReplicatingMovement(true);
			return true;
		}
		else
		{
#if UE_EDITOR
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This Actor doesn't exist in PGPlayerController!")));
#endif
			return false;
		}
	}
	else
	{
		auto ClassIndex = SpawnClasses.IndexOfByKey(InClass);

		if (ClassIndex > INDEX_NONE)
		{
			SpawnPillar_Server(InTransform, ClassIndex);
			return true;
		}
		else
		{
#if UE_EDITOR
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This Actor doesn't exist in PGPlayerController!")));
#endif
			return false;
		}
	}
}
