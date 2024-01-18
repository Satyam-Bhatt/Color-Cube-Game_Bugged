// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SceneComponent.h"
#include "RayTrace_Component.h"

// Sets default values
AMovementClass::AMovementClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Static Mesh"));
	RootComponent = CubeMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(CubeMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	RayTracing_Boy = CreateDefaultSubobject<URayTrace_Component>(TEXT("Ray Trace"));
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
	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &AMovementClass::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &AMovementClass::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("RotateDown"), IE_Pressed, this, &AMovementClass::RotateDown);
}

// Called when the game starts or when spawned
void AMovementClass::BeginPlay()
{
	Super::BeginPlay();

	LastActorLocation = GetActorLocation();
	Step = 30.f;
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
	if(GetWorldTimerManager().IsTimerActive(TimeHandleBar)) return;
	else
	{
		HorizontalAxis = 0.f;
		VerticalAxis = 1.f;
		Angle = 90.f;
		LastActorLocation = GetActorLocation();
		GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
	}
}

void AMovementClass::RotateRight()
{
	if(GetWorldTimerManager().IsTimerActive(TimeHandleBar)) return;
	else
	{ 
		HorizontalAxis = 1.f;
		VerticalAxis = 0.f;
		Angle = -90.f;
		LastActorLocation = GetActorLocation();
		GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
	}
}

void AMovementClass::RotateLeft()
{
	if(GetWorldTimerManager().IsTimerActive(TimeHandleBar))	return;
	else
	{
		HorizontalAxis = -1.f; 
		VerticalAxis = 0;
		Angle = -90.f;
		LastActorLocation = GetActorLocation();
		GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
	}
}

void AMovementClass::RotateDown()
{
	if(GetWorldTimerManager().IsTimerActive(TimeHandleBar)) return;
	else
	{
		HorizontalAxis = 0.f;
		VerticalAxis = -1.f;
		Angle = 90.f;
		LastActorLocation = GetActorLocation();
		GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
	}
}

void AMovementClass::TimelineFunction()
{
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);	

	CurveTimeline.PlayFromStart();
}

void AMovementClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

}

FVector AMovementClass::PivotLocation(float HAxis, float VAxis)
{
	FVector PivotPoint = LastActorLocation + FVector(VAxis * 50, HAxis * 50, -50);
	return PivotPoint;
}

FVector AMovementClass::AxisOfRotation(float HAxis, float VAxis)
{
	FVector AxisToRotate = FVector(HAxis * 1, VAxis * 1, 0);
	return AxisToRotate;
}

void AMovementClass::MoveCube()
{
	FVector InVector = GetActorLocation() - PivotLocation(HorizontalAxis, VerticalAxis);
	FVector RotatedVector = UKismetMathLibrary::RotateAngleAxis(InVector, Angle/Step, AxisOfRotation(HorizontalAxis, VerticalAxis));
	
	FRotator CubeRotaion_World = FRotator(VerticalAxis * -Angle/Step ,0, HorizontalAxis * -Angle/Step);
	AddActorWorldRotation(CubeRotaion_World);

	SetActorLocation(PivotLocation(HorizontalAxis, VerticalAxis) + RotatedVector);

	if(Counter >= Step)
	{
		GetWorldTimerManager().ClearTimer(TimeHandleBar);
		MoveLocation = GetActorLocation();
		Counter = 1.f;

		RayTracing_Boy->CheckOne();
	}
	else{
		Counter = Counter + 1;
	}

}