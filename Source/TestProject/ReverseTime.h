// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/GameplayStatics.h"
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
	int ReverseActor();
	UFUNCTION(BlueprintCallable)
	int ReverseCharacter();

	//Toggles b_isReversing
	UFUNCTION(BlueprintCallable)
	void ToggleReverse();

private:
	//Updates the transform and velocity array
	int UpdateArrayActor(float DeltaTime);
	int UpdateArrayCharacter(float DeltaTime);

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
	TArray<TKeyframe> KeyframeArray;
	TKeyframe PreviousKeyframe;

	AActor* Actor = nullptr;
	ACharacter* Character = nullptr;
	UStaticMeshComponent* Mesh = nullptr;
	UCapsuleComponent* CapsuleComp = nullptr;

	float Timer = 0.0f;
	bool b_RecentChange = false;
};
