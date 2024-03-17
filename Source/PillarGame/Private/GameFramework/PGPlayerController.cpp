// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PGPlayerController.h"

#if UE_EDITOR
#include "Misc/MessageDialog.h"
#endif 

void APGPlayerController::SpawnActor_Server_Implementation(int32 InClassIndex, const FTransform& InTransform)
{
	UClass* SpawnClass = SpawnClasses.IsValidIndex(InClassIndex) ? SpawnClasses[InClassIndex] : nullptr;

	if (SpawnClass != nullptr)
	{
		SpawnActor_Internal(SpawnClass, InTransform);
	}
}

AActor* APGPlayerController::SpawnActor_Blueprint(TSubclassOf<AActor> InClass, const FTransform& InTransform)
{
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
		auto ClassIndex = SpawnClasses.IndexOfByKey(InClass);

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