// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeOut.h"

// Sets default values for this component's properties
UFadeOut::UFadeOut()
{
	PrimaryComponentTick.bCanEverTick = true;
	TriggerVol->OnActorBeginOverlap.AddDynamic(this, &UFadeOut::OnOverlapEnter);
	TriggerVol->OnActorBeginOverlap.AddDynamic(this, &UFadeOut::OnOverlapEnd);
}


// Called when the game starts
void UFadeOut::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UFadeOut::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (b_IsFading)
		FadeTimer -= DeltaTime;

	if (FadeTimer <= 0)
	{
		//disable collision
		ReappearTimer = 3.0f;
	}

}

void UFadeOut::OnOverlapEnter(AActor* OverlappedActor, AActor* Other)
{
	if (Other && (Other != TriggerVol))
		if (!b_IsFading)
		{
			FadeTimer = 3.0f;
			b_IsFading = true;
		}
}

void UFadeOut::OnOverlapEnd(AActor* OverlappedActor, AActor* Other)
{

}
