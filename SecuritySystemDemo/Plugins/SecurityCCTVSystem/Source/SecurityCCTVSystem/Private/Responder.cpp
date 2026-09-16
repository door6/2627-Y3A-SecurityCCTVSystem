// Fill out your copyright notice in the Description page of Project Settings.


#include "Responder.h"
#include "SecuritySystemLog.h"

// Sets default values for this component's properties
UResponder::UResponder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UResponder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	UE_LOG(LogSecuritySystem, Display, TEXT("Responder: BeginPlay"));
}


// Called every frame
void UResponder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//uper::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResponder::Respond(ESecurityState SecurityState)
{
	//temp testing
	FString message = "Neutral";
	if (SecurityState == ESecurityState::Alarm) message = "Alarm";
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, GetOwner()->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, message);

	UE_LOG(LogSecuritySystem, Display, TEXT("%s: Responed: switch to %s"), *GetOwner()->GetActorLabel(), *message);
}

