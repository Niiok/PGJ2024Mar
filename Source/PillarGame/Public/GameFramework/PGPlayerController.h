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
	void SpawnActor_Server(int32 InClassIndex, const FTransform& InTransform);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnActor_Blueprint(const TSubclassOf<AActor> InClass, const FTransform& InTransform);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartPlacing(class APGGameState* InGameState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartJudging(class APGGameState* InGameState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndGame(class APGGameState* InGameState);

	bool IsClientReady() const { check(HasAuthority()); return bIsClientReady; }
	FUniqueNetIdRepl GetUniqueNetId() const;

protected:
	void BeginPlay() override;

	AActor* SpawnActor_Internal(TSubclassOf<AActor> InClass, const FTransform& InTransform);

	int32 GetClassIndex(const TSubclassOf<AActor> InClass) const;

	UFUNCTION(Server, Reliable)
	void Server_ImReady();

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<AActor>> SpawnClasses;
	mutable TMap<FSoftObjectPath, int32> ClassToIndex;

	bool bIsClientReady = false;
};
