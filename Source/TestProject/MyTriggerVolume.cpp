// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTriggerVolume.h"

// Sets default values for this component's properties
AMyTriggerVolume::AMyTriggerVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &AMyTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AMyTriggerVolume::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMyTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	Player = (ACharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AMyTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = Player->GetActorLocation();

	//Starting: 2500; Ending: 4200
	if (CurrentLocation.X <= 2500)
		b_IsAcross = false;

	if (CurrentLocation.X >= 4200)
		b_IsAcross = true;

}

void AMyTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* Other)
{
	if (Other && (Other != this) && !(Other->ActorHasTag("Platform")))
	{
		if (!b_IsAcross)
			Other->SetActorLocation(TeleportPosition);
		if (b_IsAcross)
			Other->SetActorLocation(TeleportPosition2);
	}
}

void AMyTriggerVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* Other)
{

}
