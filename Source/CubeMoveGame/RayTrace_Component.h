// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RayTrace_Component.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBEMOVEGAME_API URayTrace_Component : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URayTrace_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AMovementClass* Class_Script;

	UPROPERTY(EditAnywhere)
	TArray<class AColorBlocks*> MyTestActor;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_1;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_2;

	bool veryTrue = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMaterial_Mine();

	void CheckOne();

	UPROPERTY(EditAnywhere, Category = "Kuch Ho")
	bool SetMateria_Or_Not = false;

		
};
