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

    for (AActor* Actors : OverlappingActors)
    {
        
        if(Actors->ActorHasTag("Needed"))
        {
            UE_LOG(LogTemp, Error, TEXT("Overlapped"));
            Overlapped_OR_Not = 1;
        }
        else
        {
            Overlapped_OR_Not = 0;
        }
    }

}

int UBox_Trigger::Overlapped_Count()
{
    return Overlapped_OR_Not;
}
