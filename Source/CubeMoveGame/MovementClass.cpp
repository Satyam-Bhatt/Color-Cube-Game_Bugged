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
#include "ColorBlocks.h"
#include "MyGameMode.h"

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

	MaterialToAssign_Red = CreateDefaultSubobject<UMaterial>(TEXT("Material Red"));
	MaterialToAssign_Green = CreateDefaultSubobject<UMaterial>(TEXT("Material Green"));
	MaterialToAssign_Blue = CreateDefaultSubobject<UMaterial>(TEXT("Material Blue"));
	MaterialToAssign_Orange = CreateDefaultSubobject<UMaterial>(TEXT("Material Orange"));
	MaterialToAssign_Yellow = CreateDefaultSubobject<UMaterial>(TEXT("Material Yellow"));
	MaterialToAssign_White = CreateDefaultSubobject<UMaterial>(TEXT("Material White"));
}

void AMovementClass::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, Value);
	SetActorLocation(NewLocation);

	if(Value >= 1.f)
	{
		ColorOtherBlocks(GetActorForwardVector(), FColor::Red, MaterialToAssign_Red);
		ColorOtherBlocks(-GetActorForwardVector(), FColor::Orange, MaterialToAssign_Orange);
		ColorOtherBlocks(GetActorUpVector(), FColor::White, MaterialToAssign_White);
		ColorOtherBlocks(-GetActorUpVector(), FColor::Yellow, MaterialToAssign_Yellow);
		ColorOtherBlocks(GetActorRightVector(), FColor::Green, MaterialToAssign_Green);
		ColorOtherBlocks(-GetActorRightVector(), FColor::Blue, MaterialToAssign_Blue);
		DragTimerFunction();
	}
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
	MoveLocation = GetActorLocation();
	Step = 30.f;

	GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
}

void AMovementClass::UpMovement()
{
	if(CurveFloat && !WallChecker(FVector::ForwardVector))
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
	if(CurveFloat && !WallChecker(FVector::RightVector))
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
	if(CurveFloat && !WallChecker(-FVector::RightVector))
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
	if(CurveFloat && !WallChecker(-FVector::ForwardVector))
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
		
		ColorBlocks = nullptr; //Didn't give much thought. If problem with rolling and dragging arises think about this code

		ColorOtherBlocks(GetActorForwardVector(), FColor::Red, MaterialToAssign_Red);
		ColorOtherBlocks(-GetActorForwardVector(), FColor::Orange, MaterialToAssign_Orange);
		ColorOtherBlocks(GetActorUpVector(), FColor::White, MaterialToAssign_White);
		ColorOtherBlocks(-GetActorUpVector(), FColor::Yellow, MaterialToAssign_Yellow);
		ColorOtherBlocks(GetActorRightVector(), FColor::Green, MaterialToAssign_Green);
		ColorOtherBlocks(-GetActorRightVector(), FColor::Blue, MaterialToAssign_Blue);
	}
	else{
		Counter = Counter + 1;
	}
}

void AMovementClass::DragTimerFunction()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = -FVector::UpVector * 200.f + GetActorLocation();

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 4.f);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if(bHit)
	{
		ColorBlocks = Cast<AColorBlocks>(Hit.GetActor());
	}

	else if(!bHit && ColorBlocks != nullptr)
	{
		FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, ColorBlocks->GetActorLocation().Z);
		ColorBlocks->SetActorLocation(NewLocation);
	}
}

bool AMovementClass::WallChecker(FVector Direction)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = Direction * 100.f + GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_GameTraceChannel1, CollisionParams);
	return bHit;
}

void AMovementClass::ColorOtherBlocks(FVector Direction_Line, FColor Line_Color, UMaterial* Material_Assign)
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = Direction_Line * 200.f + GetActorLocation();

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, Line_Color, false, -1.f, 0, 1.f);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if(bHit)
	{
		UStaticMeshComponent* CubeMesh_Other = Cast<UStaticMeshComponent>(Hit.GetComponent());;
		ColorBlocks = Cast<AColorBlocks>(Hit.GetActor());

		if(CubeMesh_Other != nullptr)
		{
			CubeMesh_Other->SetMaterial(0, Material_Assign);

			//Call Box Trigger Funtion and then then verify the solution
			if(RayTracing_Boy->Counter_Mine() && GameMode != nullptr)
			{
				GameMode->WinCondition();
			}
			else if(!RayTracing_Boy->Counter_Mine() && GameMode != nullptr)
			{
				//check if failed
			}
		}
	}
}