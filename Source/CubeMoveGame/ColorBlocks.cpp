// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorBlocks.h"
#include "Components/StaticMeshComponent.h"
#include "MovementClass.h"

// Sets default values
AColorBlocks::AColorBlocks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Color Cube Mesh"));
	RootComponent = ColorMesh;

	MaterialCheck = CreateDefaultSubobject<UMaterial>(TEXT("Material That Should be"));
}

// Called when the game starts or when spawned
void AColorBlocks::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColorBlocks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceToMove(GetActorForwardVector(), FColor::Red);
	LineTraceToMove(-GetActorForwardVector(), FColor::Orange);
	LineTraceToMove(GetActorRightVector(), FColor::Green);
	LineTraceToMove(-GetActorRightVector(), FColor::Blue);
}

void AColorBlocks::LineTraceToMove(FVector Direction_Line, FColor Line_Color)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = Direction_Line * 55.f + GetActorLocation();

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, Line_Color, false, -1.f, 0, 1.f);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if(bHit)
	{

	}
}

int AColorBlocks::NumberSetter()
{
	UMaterialInterface* Material_Mine = ColorMesh->GetMaterial(0);

	if(Material_Mine == MaterialCheck)
	{
		Universal = 1;
	}
	else{
		Universal = 0;
	}
	return Universal;
}

