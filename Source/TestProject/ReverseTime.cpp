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
	Character = (ACppCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Mesh = Actor->FindComponentByClass<UStaticMeshComponent>();
	CapsuleComp = Actor->FindComponentByClass<UCapsuleComponent>();

	//Checks if a skeleton mesh is being used
	if (CapsuleComp)
		b_isCapsuleComp = true;

	Timer = Delay;
}

// Called every frame
void UReverseTime::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Timer -= DeltaTime;

	if (b_isCapsuleComp)
		ReverseCharacter();
	else
		ReverseActor();
}

int UReverseTime::ReverseActor()
{
	if (!Mesh)
		return 1;

	if (b_isReversing)
	{
		//Gets the last index in the Transform array
		int TransformIndex = TransformArray.Num() - 1;
		//Checks if the index is 0
		if (TransformIndex <= 0)
		{
			FTransform Transform = TransformArray[TransformIndex];
			Mesh->SetWorldTransform(Transform);
			//Reduces jittering
			Mesh->SetPhysicsLinearVelocity({0, 0, 0});
			Mesh->SetPhysicsAngularVelocity({0, 0, 0});
			return 0;
		}

		//Set up transforms to interpolate
		FTransform Transform1 = Mesh->GetComponentTransform();
		FTransform Transform2 = TransformArray[TransformIndex];
		float alpha = 0.5f;

		//Interpolate
		FTransform Transform;
		Transform.SetTranslation(FMath::Lerp(Transform1.GetTranslation(), Transform2.GetTranslation(), alpha));
		Transform.SetRotation(Transform2.GetRotation());
		Transform.SetScale3D(FMath::Lerp(Transform1.GetScale3D(), Transform2.GetScale3D(), alpha));

		//Set the Meshs [Transform] and [Linear Velocity]
		Mesh->SetWorldTransform(Transform);
		Mesh->SetPhysicsLinearVelocity(Transform.GetLocation());

		//If the index is not at 0, Remove the last index in the array
		if (TransformIndex > 0)
			TransformArray.RemoveAt(TransformIndex);

		//Gets the last index in the Velocity array
		int VelocityIndex = VelocityArray.Num() - 1;
		//Sets a [Transform] to the [Velocity] at the index
		FTransform Velocity = VelocityArray[VelocityIndex];
		PhysicsVelocity = Velocity;
		b_RecentChange = true;

		Mesh->SetPhysicsLinearVelocity(Velocity.GetLocation());
		Mesh->SetPhysicsAngularVelocity(Velocity.GetScale3D());

		//If the index is not at 0, Remove the last index in the array
		if (VelocityIndex > 0)
			VelocityArray.RemoveAt(VelocityIndex);
	}
	//If not reversing add current [Transform] and [Velocity] into array
	else if (Timer <= 0)
	{
		//Continues velocity after exiting reverse mode
		if (b_RecentChange)
		{
			Mesh->SetPhysicsLinearVelocity(PhysicsVelocity.GetLocation());
			Mesh->SetPhysicsAngularVelocity(PhysicsVelocity.GetScale3D());
			b_RecentChange = false;
		}
		//Removes the first index if the max amount is reached
		if (TransformArray.Num() == MaxPositions && MaxPositions > 0)
			TransformArray.RemoveAt(0, 1, true);
		if (VelocityArray.Num() == MaxPositions && MaxPositions > 0)
			VelocityArray.RemoveAt(0, 1, true);

		//Adds the current transform into the [Transform Array]
		TransformArray.Add(Mesh->GetComponentTransform());

		//Adds the current velocity into the [Velocity Array]
		FTransform NewVelocity;
		NewVelocity.SetLocation(Mesh->GetPhysicsLinearVelocity());
		NewVelocity.SetScale3D(Mesh->GetPhysicsAngularVelocity());
		NewVelocity.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));

		VelocityArray.Add(NewVelocity);
		Timer = Delay;
	}

	return 0;
}

int UReverseTime::ReverseCharacter()
{
	if (!CapsuleComp)
		return 1;

	if (b_isReversing)
	{
		//Gets the last index in the Transform array 
		int TransformIndex = TransformArray.Num() - 1;

		FTransform Transform = TransformArray[TransformIndex];

		//Set the Meshs [Transform] and [Linear Velocity]
		CapsuleComp->SetWorldTransform(Transform);
		CapsuleComp->SetPhysicsLinearVelocity(Transform.GetLocation());

		PhysicsVelocity = Transform;
		b_RecentChange = true;

		//If the index is not at 0, Remove the last index in the array
		if (TransformIndex > 0)
			TransformArray.RemoveAt(TransformIndex);

		//Gets the last index in the Velocity array
		int VelocityIndex = VelocityArray.Num() - 1;
		//Sets a [Transform] to the [Velocity] at the index
		FTransform Velocity = VelocityArray[VelocityIndex];

		CapsuleComp->SetPhysicsLinearVelocity(Velocity.GetLocation());
		CapsuleComp->SetPhysicsAngularVelocity(Velocity.GetScale3D());

		//If the index is not at 0, Remove the last index in the array
		if (VelocityIndex > 0)
			VelocityArray.RemoveAt(VelocityIndex);
	}
	//If not reversing add current [Transform] and [Velocity] into array
	else if (Timer <= 0)
	{
		if (b_RecentChange)
		{
			CapsuleComp->SetWorldRotation(PhysicsVelocity.GetRotation());
			b_RecentChange = false;
		}

		//Adds the current transform into the [Transform Array]
		TransformArray.Add(CapsuleComp->GetComponentTransform());
		
		//Adds the current velocity into the [Velocity Array]
		FTransform NewVelocity;
		NewVelocity.SetLocation(CapsuleComp->GetPhysicsLinearVelocity());
		NewVelocity.SetScale3D(CapsuleComp->GetPhysicsAngularVelocity());
		NewVelocity.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));

		VelocityArray.Add(NewVelocity);
		Timer = Delay;
	}

	return 0;
}

void UReverseTime::ToggleReverse()
{
	b_isReversing = !b_isReversing;
}
