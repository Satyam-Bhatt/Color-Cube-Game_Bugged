// Fill out your copyright notice in the Description page of Project Settings.


#include "Box_Trigger.h"

UBox_Trigger::UBox_Trigger()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBox_Trigger::BeginPlay()
{
    Super::BeginPlay();
}

void UBox_Trigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    if(OverlappingActors.Num() > 0)
    {
        for(AActor* Actor : OverlappingActors)
        {
            if(Actor->ActorHasTag("Needed"))
            {
                Overlapped_OR_Not = 1;
            }
        }
    }
    else{
        Overlapped_OR_Not = 0;
    }
}

