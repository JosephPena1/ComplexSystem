// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

#include "TimeControlComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTPROJECT_API UTimeControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeControlComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	//Toggles b_IsPaused
	void TogglePause();

	UFUNCTION(BlueprintCallable)
	int ForwardActor();
	int ForwardCharacter();

	UFUNCTION(BlueprintCallable)
	int ReverseActor();
	int ReverseCharacter();

private:
	int PauseActor();
	int PauseCharacter();

	void NormalTimeActor();
	void NormalTimeCharacter();

public:
	UPROPERTY(BlueprintReadWrite)
		bool b_IsPaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//The Time between saving positions
		float Delay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//Max array size, set to 0 for no limit
		int MaxPositions = 0;

private:
	TArray<FTransform> TransformArray;
	TArray<FTransform> VelocityArray;
	int IteratorIndex = 0;
	bool b_IterSet = false;
	bool b_IsDirty = false;

	AActor* Actor = nullptr;
	ACharacter* Character = nullptr;
	UStaticMeshComponent* Mesh = nullptr;
	UCapsuleComponent* CapsuleComp = nullptr;

	bool b_isCapsuleComp = false;
	float Timer = 0.0f;

	bool b_RecentChange = false;
	FTransform PhysicsVelocity;

	float FunctionDelay = 0.0f;
};
