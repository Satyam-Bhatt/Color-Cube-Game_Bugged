// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementClass.h"
#include "RayTrace_Component.h"
#include "ColorBlocks.h"

// Sets default values for this component's properties
URayTrace_Component::URayTrace_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URayTrace_Component::BeginPlay()
{
	Super::BeginPlay();
	Class_Script = Cast<AMovementClass>(GetOwner());
}


// Called every frame
void URayTrace_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Other method is to check when the line trace hits and then turn it on and off
	//for loop experimental code
	//transfer to the scene component
	int numIncreaser = 0;
	UE_LOG(LogTemp, Warning, TEXT("numIncreaser: %i"), numIncreaser);
	UE_LOG(LogTemp, Warning, TEXT("Actors: %i"), MyTestActor.Num());

	for(AColorBlocks* ColorBlock : MyTestActor)
	{
		numIncreaser = numIncreaser + ColorBlock->Universal;
	}

	if(MyTestActor.Num() == numIncreaser)
	{
		UE_LOG(LogTemp, Error, TEXT("Funny"));
	}
}

