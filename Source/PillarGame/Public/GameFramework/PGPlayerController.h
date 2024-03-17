// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PILLARGAME_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	virtual void SpawnActor_Server(int32 InClassIndex, const FTransform& InTransform);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnActor_Blueprint(TSubclassOf<AActor> InClass, const FTransform& InTransform);

protected:
	template<typename T = AActor>
	T* SpawnActor_Internal(TSubclassOf<AActor> InClass, const FTransform& InTransform)
	{
		static_assert(TIsDerivedFrom<T, AActor>::Value, "T is not AActor");
		T* SpawnedActor = GetWorld()->SpawnActor<T>(InClass, InTransform);
		SpawnedActor->SetReplicates(true);
		SpawnedActor->SetReplicatingMovement(true);
		return SpawnedActor;
	}


	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> SpawnClasses;
};
