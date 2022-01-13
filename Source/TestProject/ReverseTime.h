// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

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
	//Reverses the component owner if they have a StaticMeshComponent
	int ReverseActor();
	UFUNCTION(BlueprintCallable)
	//Reverses the component owner if they have a CapsuleComponent
	int ReverseCharacter();

	UFUNCTION()
	//Called when the component is colliding with an actor
	void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	//Called when the component is no longer colliding with an actor
	void OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	//Updates the Keyframe array with the owner's current info
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
	//Wether you want the object to reverse when you get close to it or not
	bool b_UsingDistance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//The radius of the sphere collider, no affect if UsingDistance is false 
	float MaxRadius = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool b_IsNear = false;

private:
	TArray<TKeyframe> KeyframeArray;
	TKeyframe PreviousKeyframe;

	AActor* Actor = nullptr;
	UStaticMeshComponent* Mesh = nullptr;
	UCapsuleComponent* CapsuleComp = nullptr;
	USphereComponent* SphereCollider;
	
	float Timer = 0.0f;
	bool b_RecentChange = false;
	bool b_IsPhysicsActive = true;
	bool b_OriginalPhysicsSim = false;

};
