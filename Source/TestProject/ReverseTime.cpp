// Fill out your copyright notice in the Description page of Project Settings.

#include "ReverseTime.h"

// Sets default values for this component's properties
UReverseTime::UReverseTime()
{
	// turn off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
}

// Called when the game starts
void UReverseTime::BeginPlay()
{
	Super::BeginPlay();

	Actor = GetOwner();
	
	Mesh = Actor->FindComponentByClass<UStaticMeshComponent>();
	CapsuleComp = Actor->FindComponentByClass<UCapsuleComponent>();

	if (!Mesh && !CapsuleComp)
		UE_LOG(LogTemp, Warning, TEXT("No StaticMeshComponent or CapsuleComponent found"));

	if (Mesh)
		b_OriginalPhysicsSim = Mesh->IsSimulatingPhysics();

	if (b_UsingDistance && Mesh)
	{
		SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &UReverseTime::OnOverlapBegin);
		SphereCollider->OnComponentEndOverlap.AddDynamic(this, &UReverseTime::OnOverlapEnd);
		SphereCollider->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SphereCollider->SetSphereRadius(MaxRadius);
	}
}

// Called every frame
void UReverseTime::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Timer -= DeltaTime;

	//Check if owner has a Mesh and is not being reversed
	if (Mesh && !b_isReversing)
		UpdateArrayActor();

	//Check if owner has a CapsuleComp and is not being reversed
	if (CapsuleComp && !b_isReversing)
		UpdateArrayActor();

	//Set IsReversing back to false if a
	//reverse function is not being called
	if (b_isReversing)
		b_isReversing = false;
}

int UReverseTime::ReverseActor()
{
	if (!Mesh)
		return 1;

	//Check if MinDistance is 0 or if distance is less than MinDistance
	if (!b_UsingDistance || b_IsNear)
	{
		b_isReversing = true;

		Mesh->SetSimulatePhysics(false);
		b_IsPhysicsActive = false;

		//Get the last index in the Keyframe array
		int KeyframeIndex = KeyframeArray.Num() - 1;
		//Check if the index is 0
		if (KeyframeIndex <= 0)
		{
			TKeyframe Keyframe = KeyframeArray[KeyframeIndex];
			Mesh->SetWorldLocation(Keyframe.Position);
			Mesh->SetWorldRotation(Keyframe.Rotation);
			//Reduces jittering
			Mesh->SetPhysicsLinearVelocity({ 0, 0, 0 });
			Mesh->SetPhysicsAngularVelocity({ 0, 0, 0 });
			return 0;
		}

		TKeyframe Keyframe = KeyframeArray[KeyframeIndex];

		//Update Mesh position, rotation and velocity
		Keyframe.Position = FMath::Lerp(PreviousKeyframe.Position, Keyframe.Position, 0.5f);
		Mesh->SetWorldLocation(Keyframe.Position);
		Mesh->SetWorldRotation(Keyframe.Rotation);

		Mesh->SetPhysicsLinearVelocity(Keyframe.Velocity.GetLocation());
		Mesh->SetPhysicsAngularVelocity(Keyframe.Velocity.GetScale3D());
		b_RecentChange = true;

		PreviousKeyframe = Keyframe;
		//If the index is greater than 0, Remove the last keyframe in the array
		if (KeyframeIndex > 0)
			KeyframeArray.RemoveAt(KeyframeIndex);
	}
	
	return 0;
}

int UReverseTime::ReverseCharacter()
{
	if (!CapsuleComp)
		return 1;
	 
	b_isReversing = true;

	//Gets the last index in the Keyframe array
	int KeyframeIndex = KeyframeArray.Num() - 1;
	//Check if the index is 0
	if (KeyframeIndex <= 0)
	{
		TKeyframe Keyframe = KeyframeArray[KeyframeIndex];
		CapsuleComp->SetWorldLocation(Keyframe.Position);
		CapsuleComp->SetWorldRotation(Keyframe.Rotation);
		//Reduces jittering
		CapsuleComp->SetPhysicsLinearVelocity({ 0, 0, 0 });
		CapsuleComp->SetPhysicsAngularVelocity({ 0, 0, 0 });
		return 0;
	}

	TKeyframe Keyframe = KeyframeArray[KeyframeIndex];

	//Update Mesh position, rotation and velocity
	Keyframe.Position = FMath::Lerp(PreviousKeyframe.Position, Keyframe.Position, 0.5f);
	CapsuleComp->SetWorldLocation(Keyframe.Position);
	CapsuleComp->SetWorldRotation(Keyframe.Rotation);

	CapsuleComp->SetPhysicsLinearVelocity(Keyframe.Velocity.GetLocation());
	CapsuleComp->SetPhysicsAngularVelocity(Keyframe.Velocity.GetScale3D());

	PreviousKeyframe = Keyframe;
	//If the index is greater than 0, Remove the last keyframe in the array
	if (KeyframeIndex > 0)
		KeyframeArray.RemoveAt(KeyframeIndex);

	return 0;
}

void UReverseTime::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != Actor) && (OtherActor->ActorHasTag("Player")))
		b_IsNear = true;
}

void UReverseTime::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != Actor) && (OtherActor->ActorHasTag("Player")))
		b_IsNear = false;
}

int UReverseTime::UpdateArrayActor()
{	
	if (!Mesh)
		return 1;

	if (!b_IsPhysicsActive)
	{
		Mesh->SetSimulatePhysics(b_OriginalPhysicsSim);
		b_IsPhysicsActive = b_OriginalPhysicsSim;
	}

	if (Timer <= 0)
	{
		//Continues velocity after reversing is stopped
		if (b_RecentChange)
		{
			Mesh->SetPhysicsLinearVelocity(PreviousKeyframe.Velocity.GetLocation());
			Mesh->SetPhysicsAngularVelocity(PreviousKeyframe.Velocity.GetScale3D());
			b_RecentChange = false;
		}

		//Removes the first index if the max amount is reached
		if (KeyframeArray.Num() == MaxPositions && MaxPositions > 0)
			KeyframeArray.RemoveAt(0, 1, true);

		//Add current Actor info to Keyframe array
		TKeyframe keyframe;
		keyframe.Velocity = { {0.0f, 0.0f, 0.0f, 0.0f}, Mesh->GetPhysicsAngularVelocity(), {0,0,0} };
		keyframe.Velocity.SetLocation(Mesh->GetPhysicsLinearVelocity());
		keyframe.Position = Actor->GetActorLocation();
		keyframe.Rotation = Actor->GetTransform().GetRotation();
		PreviousKeyframe = keyframe;

		KeyframeArray.Add(keyframe);
		Timer = Delay;
	}
	return 0;
}

int UReverseTime::UpdateArrayCharacter()
{
	if (!CapsuleComp)
		return 1;

	if (Timer <= 0)
	{
		//Continues velocity after reversing is stopped
		if (b_RecentChange)
		{
			Mesh->SetPhysicsLinearVelocity(PreviousKeyframe.Velocity.GetLocation());
			Mesh->SetPhysicsAngularVelocity(PreviousKeyframe.Velocity.GetScale3D());
			b_RecentChange = false;
		}

		//Removes the first index if the max amount is reached
		if (KeyframeArray.Num() == MaxPositions && MaxPositions > 0)
			KeyframeArray.RemoveAt(0, 1, true);

		//Add current Character info to Keyframe array
		TKeyframe keyframe;
		keyframe.Velocity = { {0.0f, 0.0f, 0.0f, 0.0f}, CapsuleComp->GetPhysicsAngularVelocity(), {0,0,0} };
		keyframe.Velocity.SetLocation(CapsuleComp->GetPhysicsLinearVelocity());
		keyframe.Position = Actor->GetActorLocation();
		keyframe.Rotation = Actor->GetTransform().GetRotation();
		PreviousKeyframe = keyframe;

		KeyframeArray.Add(keyframe);
		Timer = Delay;
	}
	return 0;
}
