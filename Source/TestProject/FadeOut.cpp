// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeOut.h"

// Sets default values for this component's properties
UFadeOut::UFadeOut()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFadeOut::BeginPlay()
{
	Super::BeginPlay();

	Actor = GetOwner();
}


// Called every frame
void UFadeOut::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (b_IsEntered)
		FallTimerDelay -= DeltaTime;

	if (b_IsFalling)
		MaxFallTime -= DeltaTime;

	if (FallTimerDelay <= 0 && !b_IsFalling)
	{
		b_IsFalling = true;
		b_IsEntered = false;
		MaxFallTime = 2.0f;
	}

	if (MaxFallTime > 0 && b_IsFalling)
	{
		FVector FallLocation = Actor->GetActorLocation();
		Actor->SetActorLocation({ FallLocation.X, FallLocation.Y, FallLocation.Z - 1 });
	}

	if (MaxFallTime <= 0)
		Reset();

}

void UFadeOut::OnOverlapEnter(AActor* OverlappedActor, AActor* Other)
{
	if (Other && (Other != Actor))
		if (!b_IsFalling && !b_IsEntered)
		{
			FallTimerDelay = 2.0f;
			b_IsEntered = true;
		}
}

void UFadeOut::OnOverlapEnd(AActor* OverlappedActor, AActor* Other)
{

}

void UFadeOut::Reset()
{
	FallTimerDelay = 2.0f;
	MaxFallTime = 2.0f;
	b_IsFalling = false;
	b_IsEntered = false;
}
