// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class CUBEMOVEGAME_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere)
	class AMovementClass* MovementClass;

	UPROPERTY(EditAnywhere)
	float xValue = 0.f;

	UPROPERTY(EditAnywhere)
	float yValue = 0.f;

};
