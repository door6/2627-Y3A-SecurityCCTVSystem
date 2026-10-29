// Fill out your copyright notice in the Description page of Project Settings.


#include "Responder.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

UResponderComponent::UResponderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResponderComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UResponderComponent::BindToManager(FDetectorDelegate& ManagerDelegate)
{
	ManagerDelegate.AddUniqueDynamic(this, &UResponderComponent::Respond);
}

inline ESecurityState UResponderComponent::GetCurrentState()
{
	return CurrentState;
}

void UResponderComponent::Respond(ESecurityState SecurityState)
{
	// debug messages
	FString DebugMessage = "{yellow}" + GetOwner()->GetActorLabel() + ": {white} Responed: switch to ";
	FString LogMessage = GetOwner()->GetActorLabel() + ": Responed: switch to ";
	switch (SecurityState)
	{
	case ESecurityState::Alarm:
		LogMessage += "Alarm";
		DebugMessage += "{red} Alarm";

		// increase the count of bound detectors in the alarm state
		AlarmCounter++;
		break;
	case ESecurityState::Neutral:
		LogMessage += "Neutral";
		DebugMessage += "{green} Neutral";

		// if there are still bound detectors in the alarm state, return
		if (--AlarmCounter > 0)
			return;
		break;
	default:
		return;
	}

	// return if the new security state is the same as the current state
	if (CurrentState == SecurityState)
		return;

	CurrentState = SecurityState;

#if WITH_GAMEPLAY_DEBUGGER_MENU
	FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(DebugMessage);
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

	UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *LogMessage);

	// call custom On Respond Triggered event implemented in blueprints
	OnRespondTriggered.Broadcast(SecurityState);
}

