// Fill out your copyright notice in the Description page of Project Settings.


#include "Responder.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

// Sets default values for this component's properties
UResponderComponent::UResponderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UResponderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UResponderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//uper::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResponderComponent::BindToManager(FDetectorDelegate& ManagerDelegate)
{
	ManagerDelegate.AddUniqueDynamic(this, &UResponderComponent::Respond);
}

void UResponderComponent::Respond(ESecurityState SecurityState)
{
	// debug messages
	FString DebugMessage = "{yellow}" + GetOwner()->GetActorLabel() + ": {white} Responed: switch to ";
	FString LogMessage = GetOwner()->GetActorLabel() + ": Responed: switch to ";
	if (SecurityState == ESecurityState::Alarm)
	{
		LogMessage += "Alarm";
		DebugMessage += "{red} Alarm";
	}
	else 
	{ 
		LogMessage += "Neutral";
		DebugMessage += "{green} Neutral";
	}

#if WITH_GAMEPLAY_DEBUGGER_MENU
	FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(DebugMessage);
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

	UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *LogMessage);

	// call custom On Respond Triggered event implemented in blueprints
	OnRespondTriggered.Broadcast(SecurityState);
}

