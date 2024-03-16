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
	virtual void SpawnPillar_Server(FTransform InTransform, int32 InClassIndex);

	UFUNCTION(BlueprintCallable)
	bool SpawnPillar_Blueprint(FTransform InTransform, TSubclassOf<AActor> InClass);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> SpawnClasses;
};
