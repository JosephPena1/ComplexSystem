// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseTime.h"

// Sets default values for this component's properties
UReverseTime::UReverseTime()
{
	// turn off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UReverseTime::BeginPlay()
{
	Super::BeginPlay();

	Actor = GetOwner();
	Character = (ACharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Mesh = Actor->FindComponentByClass<UStaticMeshComponent>();
	CapsuleComp = Actor->FindComponentByClass<UCapsuleComponent>();

	Timer = Delay;
}

// Called every frame
void UReverseTime::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Timer -= DeltaTime;

	if (!b_isReversing)
		UpdateArrayActor(DeltaTime);
	else
		b_isReversing = false;

}

int UReverseTime::ReverseActor()
{
	if (!Mesh)
		return 1;

	b_isReversing = true;

	//Gets the last index in the Transform array
	int KeyframeIndex = KeyframeArray.Num() - 1;
	//Checks if the index is 0
	if (KeyframeIndex <= 0)
	{
		TKeyframe Keyframe = KeyframeArray[KeyframeIndex];
		Mesh->SetWorldLocation(Keyframe.Position);
		Mesh->SetWorldRotation(Keyframe.Rotation);
		//Reduces jittering
		Mesh->SetPhysicsLinearVelocity({0, 0, 0});
		Mesh->SetPhysicsAngularVelocity({0, 0, 0});
		return 0;
	}

	TKeyframe Keyframe = KeyframeArray[KeyframeIndex];

	//Update Mesh position and velocity
	//Keyframe.Position = FMath::Lerp(PreviousKeyframe.Position, Keyframe.Position, 0.5f);
	Mesh->SetWorldLocation(Keyframe.Position);
	Mesh->SetWorldRotation(Keyframe.Rotation);

	Mesh->SetPhysicsLinearVelocity(Keyframe.Position);
	b_RecentChange = true;

	Mesh->SetPhysicsLinearVelocity(Keyframe.Velocity.GetLocation());
	Mesh->SetPhysicsAngularVelocity(Keyframe.Velocity.GetScale3D());

	PreviousKeyframe = Keyframe;
	//If the index is not at 0, Remove the last index in the array
	if (KeyframeIndex > 0)
		KeyframeArray.RemoveAt(KeyframeIndex);

	return 0;
}

int UReverseTime::ReverseCharacter()
{
	if (!CapsuleComp)
		return 1;
	 
	b_isReversing = true;

	//Gets the last index in the Transform array
	int KeyframeIndex = KeyframeArray.Num() - 1;
	//Checks if the index is 0
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

	//Update Mesh position and velocity
	//Keyframe.Position = FMath::Lerp(PreviousKeyframe.Position, Keyframe.Position, 0.5f);
	CapsuleComp->SetWorldLocation(Keyframe.Position);
	CapsuleComp->SetWorldRotation(Keyframe.Rotation);

	CapsuleComp->SetPhysicsLinearVelocity(Keyframe.Position);
	b_RecentChange = true;

	CapsuleComp->SetPhysicsLinearVelocity(Keyframe.Velocity.GetLocation());
	CapsuleComp->SetPhysicsAngularVelocity(Keyframe.Velocity.GetScale3D());

	PreviousKeyframe = Keyframe;
	//If the index is not at 0, Remove the last index in the array
	if (KeyframeIndex > 0)
		KeyframeArray.RemoveAt(KeyframeIndex);

	return 0;
}

//Toggles b_isReversing
void UReverseTime::ToggleReverse()
{
	b_isReversing = !b_isReversing;
}

int UReverseTime::UpdateArrayActor(float DeltaTime)
{	
	if (!Mesh)
		return 1;

	else if (Timer <= 0)
	{
		//Continues velocity after exiting reverse mode
		if (b_RecentChange)
		{
			Mesh->SetPhysicsLinearVelocity(PreviousKeyframe.Velocity.GetLocation());
			Mesh->SetPhysicsAngularVelocity(PreviousKeyframe.Velocity.GetScale3D());
			b_RecentChange = false;
		}

		//Removes the first index if the max amount is reached
		if (KeyframeArray.Num() == MaxPositions && MaxPositions > 0)
			KeyframeArray.RemoveAt(0, 1, true);

		//Add info to Keyframe array
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

int UReverseTime::UpdateArrayCharacter(float DeltaTime)
{
	if (!CapsuleComp)
		return 1;

	//If not reversing add current [Transform] and [Velocity] into array
	else if (Timer <= 0)
	{
		//Continues velocity after exiting reverse mode
		if (b_RecentChange)
		{
			Mesh->SetPhysicsLinearVelocity(PreviousKeyframe.Velocity.GetLocation());
			Mesh->SetPhysicsAngularVelocity(PreviousKeyframe.Velocity.GetScale3D());
			b_RecentChange = false;
		}

		//Removes the first index if the max amount is reached
		if (KeyframeArray.Num() == MaxPositions && MaxPositions > 0)
			KeyframeArray.RemoveAt(0, 1, true);

		//Add info to Keyframe array
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
