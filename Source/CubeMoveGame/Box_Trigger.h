// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Box_Trigger.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUBEMOVEGAME_API UBox_Trigger : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UBox_Trigger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Tirgger Variable")
	int32 Overlapped_OR_Not = 0;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int Overlapped_Count();

};
