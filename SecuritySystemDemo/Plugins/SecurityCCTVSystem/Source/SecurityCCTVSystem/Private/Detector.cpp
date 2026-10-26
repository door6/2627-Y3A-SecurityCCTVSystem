// Fill out your copyright notice in the Description page of Project Settings.

#include "Detector.h"
#include "SecurityManager.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

UDetectorComponent::UDetectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// if detector is not in cooldown, return
	if (CooldownTimer == 0.0f)
		return;

	// if cooldown timer exceeds the alert time, change state to neutral and reset timer
	if (CooldownTimer >= AlertTime)
	{
		ChangeStateAndNotifyManager(ESecurityState::Neutral);
		CooldownTimer = 0.0f;
		return;
	}

	// otherwise increase cooldown timer
	CooldownTimer += DeltaTime;
}

void UDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDetectorComponent::BindManager()
{
	USecurityManagerSubsystem* SecurityManager = GetWorld()->GetSubsystem<USecurityManagerSubsystem>();
	NotifyManagerDelegate.AddUniqueDynamic(SecurityManager, &USecurityManagerSubsystem::TriggerResponders);
}

inline void UDetectorComponent::SetAlertTime(float NewAlertTime)
{
	AlertTime = NewAlertTime;
}

inline ESecurityState UDetectorComponent::GetCurrentState()
{
	return CurrentState;
}

void UDetectorComponent::CheckCanStateBeChanged(ESecurityState NewState)
{
	// if state changes to alarm while in cooldown, stop cooldown
	if (NewState == ESecurityState::Alarm && CooldownTimer > 0.0f)
	{
		CooldownTimer = 0.0f;
		return;
	}

	// return if the new security state is the same as the current state
	if (CurrentState == NewState)
		return;

	// if it needs to switch to neutral, start the cooldown timer
	if (NewState == ESecurityState::Neutral)
	{
		CooldownTimer += UE_SMALL_NUMBER;
		return;
	}

	// otherwise change state and notify manager
	ChangeStateAndNotifyManager(NewState);
}

void UDetectorComponent::ChangeStateAndNotifyManager(ESecurityState SecurityState)
{
	CurrentState = SecurityState;

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
	default:
		return;
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