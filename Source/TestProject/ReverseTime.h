// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"

#include "ReverseTime.generated.h"

struct TKeyframe
{
	FTransform Velocity;
	FVector Position;
	FQuat Rotation;
};

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

	UFUNCTION(BlueprintCallable)
	//Reverses an Actor with this component (If using MinDistance give CapsuleComponent)
	int ReverseActor(UCapsuleComponent* CapsuleComponent = nullptr);
	UFUNCTION(BlueprintCallable)
	int ReverseCharacter();

private:
	//Updates the transform and velocity array
	int UpdateArrayActor();
	int UpdateArrayCharacter();

public:
	UPROPERTY(BlueprintReadOnly)
	bool b_isReversing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//The Time between saving positions
	float Delay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//Maximum Keyframes stored in array, set to 0 for no limit
	int MaxPositions = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//Minimum distance to reverse the object, set to 0 to ignore distance
	float MinDistance = 0;

private:
	TArray<TKeyframe> KeyframeArray;
	TKeyframe PreviousKeyframe;

	AActor* Actor = nullptr;
	UStaticMeshComponent* Mesh = nullptr;
	UCapsuleComponent* CapsuleComp = nullptr;

	float Timer = 0.0f;
	bool b_RecentChange = false;
	bool b_IsPhysicsActive = true;
	bool b_OriginalPhysicsSim = false;
};
