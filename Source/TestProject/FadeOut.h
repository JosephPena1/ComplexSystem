// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "FadeOut.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UFadeOut : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFadeOut();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void OnOverlapEnter(AActor* OverlappedActor, AActor* Other);
	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* Other);

private:
	ATriggerVolume* TriggerVol;

	float ReappearTimer = 0.0f;
	float FadeTimer = 0.0f;
	bool b_IsFading = false;
};
