// Fill out your copyright notice in the Description page of Project Settings.


#include "CppCharacter.h"

// Sets default values
ACppCharacter::ACppCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeLocationAndRotation(FVector( 0.0f, 0.0f, 90.0f), FRotator( -10.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACppCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	actor = GetOwner();
}

// Called every frame
void ACppCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACppCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Reads player movement
	InputComponent->BindAxis("Forward", this, &ACppCharacter::Forward);
	InputComponent->BindAxis("Right", this, &ACppCharacter::Right);

	//Reads camera movement
	InputComponent->BindAxis("LookUp", this, &ACppCharacter::VerticalRot);
	InputComponent->BindAxis("LookRight", this, &ACppCharacter::AddControllerYawInput);

	InputComponent->BindAction("ToggleTimeStop", IE_Pressed, this, &ACppCharacter::ToggleTime);
}

void ACppCharacter::Forward(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector forward = rotator.Vector();
	AddMovementInput(forward, AxisValue);

	bIsMovingX = AxisValue != 0;
	//bUseControllerRotationYaw = bIsMovingX || bIsMovingY;
}

void ACppCharacter::Right(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector right = FVector::CrossProduct({0,0,1}, rotator.Vector());
	AddMovementInput(right, AxisValue);

	bIsMovingY = AxisValue != 0;
	//bUseControllerRotationYaw = bIsMovingX || bIsMovingY;
}

void ACppCharacter::ToggleTime()
{
	bIsReversing = !bIsReversing;
}

void ACppCharacter::VerticalRot(float Value)
{
	if (Value)
	{
		float temp = SpringArm->GetRelativeRotation().Pitch + (Value * Sensitivity);
		if (temp < 40 && temp > -65)
			SpringArm->AddLocalRotation(FRotator(Value, 0, 0));
	}
}
