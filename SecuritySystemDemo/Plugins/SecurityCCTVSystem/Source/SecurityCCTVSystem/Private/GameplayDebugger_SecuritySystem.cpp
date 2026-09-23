// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayDebugger_SecuritySystem.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU

#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

FGameplayDebuggerCategory_SecuritySystem::FGameplayDebuggerCategory_SecuritySystem()
{
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_SecuritySystem::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_SecuritySystem());
}

void FGameplayDebuggerCategory_SecuritySystem::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{


	UAIPerceptionComponent* PerceptionComponent = nullptr;
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
	}
}

#endif // WITH_GAMEPLAY_DEBUGGER_MENU