// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementClass.h"
#include "RayTrace_Component.h"
#include "ColorBlocks.h"
#include "Box_Trigger.h"

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

	BoxTriggers.Empty();

	for(AActor* Actor : StaticMeshes)
	{
		UBox_Trigger* BoxTriggerComponent = Actor->FindComponentByClass<UBox_Trigger>();
		if(BoxTriggerComponent != nullptr)
		{
			BoxTriggers.Add(BoxTriggerComponent);
		}
	}
}


// Called every frame
void URayTrace_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URayTrace_Component::Counter_Mine()
{
	int numIncreaser = 0; 
	int box_numMultiplier = 1;

	for(AColorBlocks* ColorBlock : MyTestActor)
	{
		numIncreaser = numIncreaser + ColorBlock->NumberSetter();
	}

	if(BoxTriggers.Num() != 0)
	{
		for(UBox_Trigger* BoxTrigger : BoxTriggers)
		{
			box_numMultiplier = box_numMultiplier * BoxTrigger->Overlapped_OR_Not;
		}
	}

	if(MyTestActor.Num() == numIncreaser && box_numMultiplier == 1)
	{
		return true;
	}
	else return false;
}

