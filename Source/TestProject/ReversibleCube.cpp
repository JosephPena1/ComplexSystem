// Fill out your copyright notice in the Description page of Project Settings.


#include "ReversibleCube.h"

// Sets default values
AReversibleCube::AReversibleCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AReversibleCube::BeginPlay()
{
	Super::BeginPlay();
	
	//Gets the players character
	Character = (ACppCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AReversibleCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Reverse();
}

void AReversibleCube::Reverse()
{
	if (Character->bIsReversing)
	{
		//Gets the last index in the Transform array 
		int TransformIndex = TransformArray.Num() - 1;
		//Sets a [Transform] to the [Transform] at the index
		FTransform Transform = TransformArray[TransformIndex];

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

		Mesh->SetPhysicsLinearVelocity(Velocity.GetLocation());

		Mesh->SetPhysicsAngularVelocity(Velocity.GetScale3D());

		//If the index is not at 0, Remove the last index in the array
		if (VelocityIndex > 0)
			VelocityArray.RemoveAt(VelocityIndex);
	}
	//If not reversing add current [Transform] and [Velocity] into array
	else
	{
		//Adds the current transform into the [Transform Array]
		TransformArray.Add(Mesh->GetComponentTransform());

		//Adds the current velocity into the [Velocity Array]
		FTransform NewVelocity;
		NewVelocity.SetLocation(Mesh->GetPhysicsLinearVelocity());
		NewVelocity.SetScale3D(Mesh->GetPhysicsAngularVelocity());
		NewVelocity.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));

		VelocityArray.Add(NewVelocity);

	}
}
