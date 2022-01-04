// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CppCharacter.generated.h"

UCLASS()
class TESTPROJECT_API ACppCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACppCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Forward(float AxisValue);
	void Right(float AxisValue);

	void ToggleTime();

private:
	void VerticalRot(float Value);

public:
	USkeletalMeshComponent* skele;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsReversing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sensitivity = 1.0f;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	AActor* actor = nullptr;

	bool bIsMovingX = false;
	bool bIsMovingY = false;
};
