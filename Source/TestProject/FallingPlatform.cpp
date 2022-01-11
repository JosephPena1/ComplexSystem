// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingPlatform.h"

// Sets default values
AFallingPlatform::AFallingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	BoxComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AFallingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlatform::OnOverlapBegin);
}

// Called every frame
void AFallingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		FVector FallLocation = GetActorLocation();
		SetActorLocation({ FallLocation.X, FallLocation.Y, FallLocation.Z - 1 });
	}

	if (MaxFallTime <= 0)
		Reset();
}

void AFallingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && (OtherActor->ActorHasTag("Player")))
		if (!b_IsFalling && !b_IsEntered)
		{
			FallTimerDelay = 2.0f;
			b_IsEntered = true;
		}
}

void AFallingPlatform::Reset()
{
	FallTimerDelay = 2.0f;
	MaxFallTime = 2.0f;
	b_IsFalling = false;
	b_IsEntered = false;
}

