// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTriggerVolume.h"

// Sets default values for this component's properties
AMyTriggerVolume::AMyTriggerVolume()
{
	OnActorBeginOverlap.AddDynamic(this, &AMyTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AMyTriggerVolume::OnOverlapEnd);
}

void AMyTriggerVolume::BeginPlay()
{

}

void AMyTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* Other)
{
	if (Other && (Other != this))
	{
		Other->SetActorLocation({2340,1550,-130});
	}
}

void AMyTriggerVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* Other)
{

}
