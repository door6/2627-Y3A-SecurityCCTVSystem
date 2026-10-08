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

	// if pointer to the security manager doesn't exist, get a security manager and create arrays of detectors and responders
	if (!SecurityManager.IsValid())	//!SecurityManager
	{			
		if (!World)
			return;

		SecurityManager = World->GetSubsystem<USecurityManagerSubsystem>();
		if (!SecurityManager.IsValid())	//!SecurityManager
		{
			this->AddTextLine("{red} No Security Manager!!");
			return;
		}

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->FindComponentByClass<UDetectorComponent>())
			{
				Detectors.AddUnique(Actor);
			}
			else if (Actor->FindComponentByClass<UResponderComponent>())
			{
				Responders.AddUnique(Actor);
			}
			else
				continue;
		}
	}

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
	for (TWeakObjectPtr<AActor> Detector : Detectors)		//for (AActor* Detector : Detectors)
	{
		if (!Detector.IsValid())	//!Detector
			continue;

		AMotionDetector* MotionDetector = Cast<AMotionDetector>(Detector);
		if (MotionDetector)
		{
			MotionDetector->DescribeSelfToGameplayDebugger(this);
			continue;
		}
		ACollisionBasedDetector* CollisionBasedDetector = Cast<ACollisionBasedDetector>(Detector);
		if (CollisionBasedDetector)
		{
			CollisionBasedDetector->DescribeSelfToGameplayDebugger(this);
			continue;
		}
	}
}

void FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(const FString& Message, float DelayTime)
{
	PendingMessages.Add(FDebugMessage(Message, DelayTime));
}

#endif // WITH_GAMEPLAY_DEBUGGER_MENU