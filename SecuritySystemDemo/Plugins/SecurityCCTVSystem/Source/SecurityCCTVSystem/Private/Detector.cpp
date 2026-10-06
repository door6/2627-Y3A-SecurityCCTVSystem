// Fill out your copyright notice in the Description page of Project Settings.

#include "Detector.h"
#include "SecurityManager.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

// Sets default values for this component's properties
UDetectorComponent::UDetectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called every frame
void UDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Called when the game starts
void UDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UDetectorComponent::BindManager()
{
	USecurityManagerSubsystem* SecurityManager = GetWorld()->GetSubsystem<USecurityManagerSubsystem>();
	NotifyManagerDelegate.AddUniqueDynamic(SecurityManager, &USecurityManagerSubsystem::TriggerResponders);
}

inline const ESecurityState UDetectorComponent::GetCurrentState()
{
	return CurrentState;
}

inline const void UDetectorComponent::SetCurrentState(ESecurityState NewState)
{
	CurrentState = NewState;
}

void UDetectorComponent::TriggerResponders(ESecurityState SecurityState)
{
	//debug messages
	FString DetectorName = GetOwner()->GetActorLabel();
	FString DebugMessage = "{yellow}" + DetectorName + ": {white} Switch to ";
	FString LogMessage = DetectorName + ": Switch to ";
	switch (SecurityState)
	{
	case ESecurityState::Neutral:
		DebugMessage += "{green} Neutral";
		LogMessage += "Neutral";
		break;
	case ESecurityState::Alarm:
		DebugMessage += "{red} Alarm";
		LogMessage += " Alarm";
		break;
	}
#if WITH_GAMEPLAY_DEBUGGER_MENU
	FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(DebugMessage);
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

	UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *LogMessage);

	// check if the delegate is bound to the manager
	if (!NotifyManagerDelegate.IsBound())
	{
		UE_LOG(LogSecuritySystem, Warning, TEXT("%s: Manager is NOT bound"), *DetectorName);
		return;
	}

	UE_LOG(LogSecuritySystem, Log, TEXT("%s: TriggerResponders"), *DetectorName);

	// notify the manager to trigger responders
	NotifyManagerDelegate.Broadcast(DetectorName, SecurityState);
}