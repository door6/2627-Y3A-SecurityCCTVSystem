// Fill out your copyright notice in the Description page of Project Settings.


#include "Responder.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

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
	
	//UE_LOG(LogSecuritySystem, Log, TEXT("Responder: BeginPlay"));
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
	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, GetOwner()->GetName());

#if WITH_GAMEPLAY_DEBUGGER_MENU
	FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(DebugMessage);
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, Message);

	UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *LogMessage);
}

