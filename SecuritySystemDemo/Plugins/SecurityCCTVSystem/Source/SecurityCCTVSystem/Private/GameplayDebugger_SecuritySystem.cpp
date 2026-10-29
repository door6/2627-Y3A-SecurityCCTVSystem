// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayDebugger_SecuritySystem.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU

#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "SecurityManager.h"
#include "Detector.h"
#include "Responder.h"
#include "EngineUtils.h"
#include "MotionDetector.h"
#include "CollisionBasedDetector.h"

FGameplayDebuggerCategory_SecuritySystem::FGameplayDebuggerCategory_SecuritySystem()
{
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_SecuritySystem::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_SecuritySystem());
}

void FGameplayDebuggerCategory_SecuritySystem::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	UWorld* World = OwnerPC->GetWorld();

	// check to avoid crashes if the world cannot be acceessed
	if (!World)
		return;

	// display all pending debug messages
	for (FDebugMessage& DebugMessage : PendingMessages)
	{
		this->AddTextLine(DebugMessage.Message);
		DebugMessage.Timer -= World->GetDeltaSeconds();
	}
	// remove all messages whose timer reached 0
	PendingMessages.RemoveAll([](const FDebugMessage& DebugMessage)
	{
		return DebugMessage.Timer <= 0.0f;
	});

	// call detectors' debug drawing functions
	OnGameplayDebugDraw.Broadcast(this);
}

void FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(const FString& Message, float DelayTime)
{
	PendingMessages.Add(FDebugMessage(Message, DelayTime));
}

#endif // WITH_GAMEPLAY_DEBUGGER_MENU