// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "MyTriggerVolume.generated.h"

UCLASS()
class TESTPROJECT_API AMyTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
	
public:
	AMyTriggerVolume();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	//Overlap Begin function
	void OnOverlapBegin(AActor* OverlappedActor, AActor* Other);
	UFUNCTION()
	//Overlap End Function
	void OnOverlapEnd(AActor* OverlappedActor, AActor* Other);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TeleportPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TeleportPosition2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TestActor = nullptr;

private:
	ACharacter* Player = nullptr;

	bool b_IsAcross = false;
};
