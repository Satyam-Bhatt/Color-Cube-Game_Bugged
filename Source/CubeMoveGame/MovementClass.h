// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "MovementClass.generated.h"

class UCurveFloat;

UCLASS()
class CUBEMOVEGAME_API AMovementClass : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMovementClass();

	UFUNCTION()
	void TimelineProgress(float Value);

	UPROPERTY(EditAnywhere)
	bool ScriptAttachmentCheck = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FVector StartLoc;

	UPROPERTY()
	FVector EndLoc;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float XOffset = 100.f;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float YOffset = 100.f;


private:
#pragma region //Components

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class URayTrace_Component* RayTracing_Boy;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_Red;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_Green;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_Blue;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_Orange;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_Yellow;

	UPROPERTY(EditAnywhere)
	class UMaterial* MaterialToAssign_White;

#pragma endregion

#pragma region //KeyBindings

	void LookUp(float AxisValue);
	void UpMovement();
	void RightMovement();
	void LeftMovement();
	void DownMovement();
	void RotateUp();
	void RotateRight();
	void RotateLeft();
	void RotateDown();

#pragma endregion

	FTimerHandle TimeHandleBar;
	FVector LastActorLocation;
	float HorizontalAxis = 0.f;
	float VerticalAxis = 0.f;
	float Angle = 0.f;
	float Step = 0.f;
	float Counter = 1.f;

	FVector MoveLocation;
	FVector PivotLocation(float HAxis, float VAxis);
	FVector AxisOfRotation(float HAxis, float VAxis);
	class AColorBlocks* ColorBlocks;

	void MoveCube();
	void ColorOtherBlocks(FVector Direction, FColor Line_Color, UMaterial* Material_Assign);
	void TimelineFunction();
	void DragTimerFunction();
	bool MovementConsentFunction();
	bool WallChecker(FVector Direction);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool FrontMove = false;
	bool BackMove = false;
	bool RightMove = false;
	bool LeftMove = false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
