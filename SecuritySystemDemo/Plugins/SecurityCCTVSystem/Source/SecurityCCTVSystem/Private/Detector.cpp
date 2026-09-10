// Fill out your copyright notice in the Description page of Project Settings.


#include "Detector.h"

// Sets default values for this component's properties
UDetector::UDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDetector::TriggerResponders(ESecurityState SecurityState)
{
	NotifyManagerDelegate.Broadcast(GetOwner()->GetFullName(), SecurityState);
}

