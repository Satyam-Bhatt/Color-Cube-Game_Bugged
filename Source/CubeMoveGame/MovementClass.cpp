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

	RotationDoer = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotatiing Component"));
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
	if(SecondMethod)
	{
		FVector WorldLocaion = GetActorLocation() + FVector(50.f, 0, -50.f);
		RotationDoer->PivotTranslation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), WorldLocaion);
		RotationDoer->RotationRate = FRotator(-90.f,0.f,0.f);

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMovementClass::TimerFunction, 0.01, true, 0.99);
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(TimeHandleBar)){
			return;
		}
		else{
			HorizontalAxis = 0.f;
			VerticalAxis = 1.f;
			Angle = 90.f;
			LastActorLocation = GetActorLocation();
			GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
		}
	}


}

void AMovementClass::RotateRight()
{
	if(SecondMethod){
		FVector WorldLocation = GetActorLocation() + FVector(0,50.f, -50.f);
		RotationDoer->PivotTranslation = UKismetMathLibrary::InverseTransformLocation(GetTransform(), WorldLocation);
		RotationDoer->RotationRate = FRotator(0.f, 0.f, 90.f);

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMovementClass::TimerFunction, 0.01, true, 0.99);
	}
	else{
		if(GetWorldTimerManager().IsTimerActive(TimeHandleBar)){
			return;
		}
		else{ 
			HorizontalAxis = 1.f;
			VerticalAxis = 0.f;
			Angle = -90.f;
			LastActorLocation = GetActorLocation();

			GetWorldTimerManager().SetTimer(TimeHandleBar, this, &AMovementClass::MoveCube, 0.01, true);
		}
	}
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

void AMovementClass::TimerFunction()
{	
	int64 RollValueNew = AActor::GetTransform().Rotator().Roll;
	int64 PitchValueNew = AActor::GetTransform().Rotator().Pitch;
	int64 YawValueNew = AActor::GetTransform().Rotator().Yaw;
	
	UE_LOG(LogTemp, Warning, TEXT("Roll Value: %i"), RollValueNew);
	UE_LOG(LogTemp, Warning, TEXT("Pitch Value %i"), PitchValueNew);
	UE_LOG(LogTemp, Warning, TEXT("Yaw Value %i"), YawValueNew);

	FRotator NewRotaion = FRotator(PitchValueNew, YawValueNew, RollValueNew);
	SetActorRotation(NewRotaion, ETeleportType::None);

	RotationDoer->PivotTranslation = GetActorLocation();
	RotationDoer->RotationRate = FRotator::ZeroRotator;
	GetWorldTimerManager().ClearTimer(TimerHandle);
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
	
	//FRotator CubeRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(AxisOfRotation(HorizontalAxis,VerticalAxis), Angle/Step);
	//FRotator CombinedRotation = UKismetMathLibrary::ComposeRotators(GetActorRotation(), CubeRotation);
	//SetActorRotation(CombinedRotation);

	FRotator CubeRotaion_World = FRotator(VerticalAxis * -Angle/Step ,0, HorizontalAxis * -Angle/Step);
	AddActorWorldRotation(CubeRotaion_World);

	SetActorLocation(PivotLocation(HorizontalAxis, VerticalAxis) + RotatedVector);
	

	if(Counter >= Step)
	{
		GetWorldTimerManager().ClearTimer(TimeHandleBar);
		MoveLocation = GetActorLocation();
		Counter = 1.f;
	}
	else{
		Counter = Counter + 1;
	}

}