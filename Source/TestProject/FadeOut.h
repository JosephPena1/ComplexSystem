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
	ATriggerVolume trigger;
	float Timer = 0.0f;
};
