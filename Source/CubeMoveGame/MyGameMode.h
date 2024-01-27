// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CUBEMOVEGAME_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();
	
protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	class AMovementClass* CubeMovement;
	class AMyPlayerController* PlayerController_My;

	UFUNCTION(BlueprintCallable)
	void StartGame(bool bState);

public:
	void WinCondition();

};
