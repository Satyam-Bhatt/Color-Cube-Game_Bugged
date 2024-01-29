// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorBlocks.generated.h"

UCLASS()
class CUBEMOVEGAME_API AColorBlocks : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColorBlocks();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ColorMesh;

	UPROPERTY(VisibleAnywhere)
	int Universal = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "My Personal Mesh")
	class UMaterial* MaterialCheck;

	UPROPERTY(EditAnywhere)
	class AMovementClass* MovementClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int NumberSetter();

	UPROPERTY(EditAnywhere)
	bool MaterialMatch = true;


};
