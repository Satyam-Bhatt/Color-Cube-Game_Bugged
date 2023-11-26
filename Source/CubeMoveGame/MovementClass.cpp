// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

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
	PlayerInputComponent->BindAction(TEXT("Right"), IE_Pressed, this, &AMovementClass::RightMovement);
	PlayerInputComponent->BindAction(TEXT("Left"), IE_Pressed, this, &AMovementClass::LeftMovement);
	PlayerInputComponent->BindAction(TEXT("Down"), IE_Pressed, this, &AMovementClass::DownMovement);

	PlayerInputComponent->BindAction(TEXT("RotateUp"), IE_Pressed, this, &AMovementClass::RotateUp);
}

// Called when the game starts or when spawned
void AMovementClass::BeginPlay()
{
	Super::BeginPlay();

	MoveLocation = GetActorLocation();

}

void AMovementClass::UpMovement()
{
	if(CurveFloat)
	{
		if(GetActorLocation() == MoveLocation)
		{
			StartLoc = EndLoc = GetActorLocation();
			EndLoc.X += XOffset;

			MoveLocation = GetActorLocation() + FVector(XOffset,0,0); 

			TimelineFunction();
		}
	}
}

void AMovementClass::RightMovement()
{
	if(CurveFloat)
	{
		if(GetActorLocation() == MoveLocation)
		{
			StartLoc = EndLoc = GetActorLocation();
			EndLoc.Y += YOffset;

			TimelineFunction();

			MoveLocation = GetActorLocation() + FVector(0, YOffset, 0);
		}
	}
}

void AMovementClass::LeftMovement()
{
	if(CurveFloat)
	{
		if(GetActorLocation() == MoveLocation)
		{
			StartLoc = EndLoc = GetActorLocation();
			EndLoc.Y -= YOffset;

			TimelineFunction();

			MoveLocation = GetActorLocation() - FVector(0, YOffset, 0);
		}
	}
}

void AMovementClass::DownMovement()
{
	if(CurveFloat)
	{
		if(GetActorLocation() == MoveLocation)
		{
			StartLoc = EndLoc = GetActorLocation();
			EndLoc.X -= XOffset;

			TimelineFunction();

			MoveLocation = GetActorLocation() - FVector(XOffset,0,0); 			
		}
	}	
}

void AMovementClass::RotateUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Up hua Na"));

	FVector Anchor = GetActorLocation() + FVector(0.5, 0 , -0.5);
	FVector Axis = FVector::CrossProduct(FVector::UpVector, FVector::RightVector);

	FVector RotationOfAxis = UKismetMathLibrary::RotateAngleAxis(Anchor, 90, Axis);

	SetActorRotation(RotationOfAxis.Rotation(), ETeleportType::None);
}

void AMovementClass::TimelineFunction()
{
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);	

	CurveTimeline.PlayFromStart();
}

// Called every frame
void AMovementClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);
}


