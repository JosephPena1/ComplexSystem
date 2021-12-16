// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CppCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

#include "ReverseTime.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTPROJECT_API UReverseTime : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UReverseTime();


	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Toggles b_isReversing
	UFUNCTION(BlueprintCallable)
	void ToggleReverse();

private:
	int ReverseActor();
	int ReverseCharacter();

public:
	UPROPERTY(BlueprintReadWrite)
	bool b_isReversing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//The Time between saving positions
	float Delay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//Max array size, set to 0 for no limit
	int MaxPositions = 0;

private:
	TArray<FTransform> TransformArray;
	TArray<FTransform> VelocityArray;

	AActor* Actor = nullptr;
	ACppCharacter* Character = nullptr;
	UStaticMeshComponent* Mesh = nullptr;
	UCapsuleComponent* CapsuleComp = nullptr;

	bool b_isCapsuleComp = false;
	float Timer = 0.0f;

	bool b_RecentChange = false;
	FTransform PhysicsVelocity;
};
