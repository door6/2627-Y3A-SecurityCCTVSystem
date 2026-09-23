// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayDebugger_SecuritySystem.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU

#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "SecurityManager.h"
#include "Detector.h"
#include "EngineUtils.h"
#include "MotionDetector.h"

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

	if (!SecurityManager)
	{			
		if (!World)
			return;

		SecurityManager = World->GetSubsystem<USecurityManagerSubsystem>();
		if (!SecurityManager)
		{
			this->AddTextLine("{red} No Security Manager!!");
			return;
		}

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->FindComponentByClass<UDetector>())
			{
				Detectors.AddUnique(Actor);
			}
			else if (Actor->FindComponentByClass<UResponder>())
			{
				Responders.AddUnique(Actor);
			}
			else
				continue;
		}
	}
	
	//FString Message = FString::Printf(TEXT("Detectors = %d"), Detectors.Num());
	//this->AddTextLine(Message);

	for (FDebugMessage& DebugMessage : PendingMessages)
	{
		this->AddTextLine(DebugMessage.Message);
		DebugMessage.Timer -= World->GetDeltaSeconds();
	}
	PendingMessages.RemoveAll([](const FDebugMessage& DebugMessage)
	{
		return DebugMessage.Timer <= 0.0f;
	});

	for (AActor* Detector : Detectors)
	{
		if (!Detector)
			continue;

		AMotionDetector* MotionDetector = Cast<AMotionDetector>(Detector);
		if (MotionDetector)
		{
			MotionDetector->DescribeSelfToGameplayDebugger(this);
		}
	}

	/*UAIPerceptionComponent* PerceptionComponent = nullptr;
	APawn* MyPawn = Cast<APawn>(DebugActor);
	if (MyPawn)
	{
		AController* Controller = MyPawn->GetController();
		if (AAIController* AIC = Cast<AAIController>(Controller))
		{
			PerceptionComponent = AIC->GetPerceptionComponent();
		}
		else
		{
			PerceptionComponent = MyPawn->FindComponentByClass<UAIPerceptionComponent>();
			// try the controller if the Pawn doesn't have it
			if (PerceptionComponent == nullptr && Controller)
			{
				PerceptionComponent = Controller->FindComponentByClass<UAIPerceptionComponent>();
			}
		}
	}

	if (PerceptionComponent == nullptr && DebugActor != nullptr)
	{
		PerceptionComponent = DebugActor->FindComponentByClass<UAIPerceptionComponent>();
	}

	if (PerceptionComponent)
	{
		PerceptionComponent->DescribeSelfToGameplayDebugger(this);
	}*/
}

void FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(const FString& Message, float DelayTime)
{
	PendingMessages.Add(FDebugMessage(Message, DelayTime));
}

#endif // WITH_GAMEPLAY_DEBUGGER_MENU