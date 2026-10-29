// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManager.h"
#include "Responder.h"
#include "SecuritySystemLog.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Detector.h"

void USecurityManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		UDetectorComponent* Detector = Actor->FindComponentByClass<UDetectorComponent>();
		if (!Detector)
			continue;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(Actor->GetWorld(), FName(Actor->GetActorLabel()), FoundActors);

		if (!FoundActors.IsEmpty())
		{
			Detector->BindManager();
			UE_LOG(LogSecuritySystem, Log, TEXT("Manager: BeginPlay: Bound to %s"), *Actor->GetActorLabel());
		}

		for (AActor* ResponderActor : FoundActors)
		{
			FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(Actor->GetActorLabel());
			ResponderActor->FindComponentByClass<UResponderComponent>()->BindToManager(Delegate);
		}
	}
}

void USecurityManagerSubsystem::TriggerResponders(const FString& DetectorName, ESecurityState SecurityState)
{
	UE_LOG(LogSecuritySystem, Log, TEXT("Manager: Trigger responders of %s"), *DetectorName);

	if (!DetectorDelegates.Find(DetectorName))
	{
		UE_LOG(LogSecuritySystem, Warning, TEXT("Manager: TriggerResponders: %s is NOT bound"), *DetectorName);
		return;
	}

	DetectorDelegates[DetectorName].Broadcast(SecurityState);
}
