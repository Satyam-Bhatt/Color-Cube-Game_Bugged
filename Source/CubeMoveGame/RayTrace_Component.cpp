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

	MaterialToAssign_1 = CreateDefaultSubobject<UMaterial>(TEXT("Material 1"));
	MaterialToAssign_2 = CreateDefaultSubobject<UMaterial>(TEXT("Material 2"));
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

	//if(SetMateria_Or_Not)
	//{
		AActor* Owner = GetOwner();

		FVector StartLocation = Owner->GetActorLocation();
		FVector EndLocation = GetForwardVector() * 1000.f + Owner->GetActorLocation();

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

		if(bHit)
		{
			UStaticMeshComponent* CubeMesh_Other = Cast<UStaticMeshComponent>(Hit.GetComponent());

			if(CubeMesh_Other != nullptr)
			{
				CubeMesh_Other->SetMaterial(0, MaterialToAssign_1);
			}
		}	
	//}	

	//Other method is to check when the line trace hits and then turn it on and off
	//for loop experimental code
	//transfer to the scene component
		

	int numIncreaser = 0;
	for(AColorBlocks* ColorBlock : MyTestActor)
	{
		if(ColorBlock->Universal)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Chla"));
		}
		else{
			//UE_LOG(LogTemp, Warning, TEXT("Nahi"));
		}
	}

	if(veryTrue)
	{
		UE_LOG(LogTemp, Warning, TEXT("Yesssssssss"));
	}

}

void URayTrace_Component::SetMaterial_Mine()
{
	AActor* Owner = GetOwner();

	FVector StartLocation = Owner->GetActorLocation();
	FVector EndLocation = GetForwardVector() * 1000.f + Owner->GetActorLocation();

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if(bHit)
	{
		UStaticMeshComponent* CubeMesh_Other = Cast<UStaticMeshComponent>(Hit.GetComponent());

		if(CubeMesh_Other != nullptr)
		{
			CubeMesh_Other->SetMaterial(0, MaterialToAssign_1);
		}
	}
}

void URayTrace_Component::CheckOne()
{
	UE_LOG(LogTemp, Warning, TEXT("Wow"));
}

