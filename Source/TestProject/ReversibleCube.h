// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "ReversibleCube.generated.h"

UCLASS()
class TESTPROJECT_API AReversibleCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReversibleCube();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Reverse();

private:
	TArray<FTransform> TransformArray;
	TArray<FTransform> VelocityArray;

	ACppCharacter* Character;
};
