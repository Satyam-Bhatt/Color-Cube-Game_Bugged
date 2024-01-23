// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "MovementClass.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	RootComponent = ArrowMesh;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MovementClass != nullptr)
	{
		FVector LocationOfCube = MovementClass->GetActorLocation();
		SetActorLocation(FVector(150,0,0) + LocationOfCube);
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Save Crash"));
	}

}

