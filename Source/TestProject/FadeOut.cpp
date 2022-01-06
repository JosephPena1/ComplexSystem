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

}


// Called every frame
void UFadeOut::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

