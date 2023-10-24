// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h"

// Sets default values
AMovementClass::AMovementClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Static Mesh"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(CubeMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void AMovementClass::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, Value);
	SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void AMovementClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Up"), IE_Pressed, this, &AMovementClass::UpMovement);
}

// Called when the game starts or when spawned
void AMovementClass::BeginPlay()
{
	Super::BeginPlay();
	
	if(CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(true);

		StartLoc = EndLoc = GetActorLocation();
		EndLoc.Z += ZOffset;

		CurveTimeline.PlayFromStart();
	}
}

void AMovementClass::UpMovement()
{
	FVector ActorLocation = GetActorLocation();
	FVector ActorScale = GetActorScale();
	float MoveLocation = ActorLocation.X + 50;
	FVector MovePlace = FVector(MoveLocation, ActorLocation.Y, ActorLocation.Z);
	FVector LerpValues = FMath::Lerp(GetActorLocation(), MovePlace, 0.5);
	SetActorLocation(LerpValues);
}

// Called every frame
void AMovementClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);
}


