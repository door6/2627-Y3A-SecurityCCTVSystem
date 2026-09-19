// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManager.h"
#include "SecuritySystemLog.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Detector.h"


/*ASecurityManager::ASecurityManager()
{
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogSecuritySystem, Display, TEXT("SecurityManager: Initialisation"));
}

ASecurityManager::~ASecurityManager()
{
	UE_LOG(LogSecuritySystem, Log, TEXT("SecurityManager destroyed"));
}

void ASecurityManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogSecuritySystem, Warning, TEXT("Number of bound detectors during BeginPlay = %d"), DetectorDelegates.Num());	

	FString Check;

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->FindComponentByClass<UResponder>())
			continue;

		UResponder* Responder = Actor->FindComponentByClass<UResponder>();
		FDetectorDelegate* Delegate = DetectorDelegates.Find(Responder->ConnectedDetector);
		if (Delegate)
		{
			Delegate->AddUniqueDynamic(Responder, &UResponder::Respond);

			Check = Actor->GetName();
			UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Check);
		}
	}
}

void ASecurityManager::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void ASecurityManager::BindResponderToDetector(const FString& DetectorName, UResponder& Responder)
{
	FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(DetectorName);
	Delegate.AddUniqueDynamic(&Responder, &UResponder::Respond);		//leave in for now, need to remove when dropdown gets refactored
}

void ASecurityManager::RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder)
{
	DetectorDelegates[DetectorName].RemoveDynamic(&Responder, &UResponder::Respond);
}

void ASecurityManager::TriggerResponders(const FString& DetectorName, ESecurityState SecurityState)
{
	UE_LOG(LogSecuritySystem, Log, TEXT("Manager: Trigger responders of %s"), *DetectorName);

	DetectorDelegates[DetectorName].Broadcast(SecurityState);
}*/





void USecurityManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	//LoadConfig();
}

void USecurityManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	//UE_LOG(LogSecuritySystem, Log, TEXT("Number of bound detectors during BeginPlay = %d"), DetectorDelegates.Num());
	/*UE_LOG(LogSecuritySystem, Log, TEXT("Number of bound detectors during BeginPlay = %d"), DetectorResponders.Num());

	if (TestChanges)
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("TestChanges = true"));
	}
	else
		UE_LOG(LogSecuritySystem, Warning, TEXT("TestChanges = false"));*/

	//FString Check;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		/*AActor* Actor = *It;
		UResponder* Responder = Actor->FindComponentByClass<UResponder>();
		if (!Responder)
		//if (!Actor->FindComponentByClass<UResponder>())
			continue;

		for (FString DetectorName : Responder->ConnectedDetectors)
		{
			FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(DetectorName);
			Delegate.AddUniqueDynamic(Responder, &UResponder::Respond);

			UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Actor->GetName());
		}*/

		AActor* Actor = *It;
		UDetector* Detector = Actor->FindComponentByClass<UDetector>();
		if (!Detector)
			continue;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(Actor->GetWorld(), FName(Actor->GetActorLabel()), FoundActors);

		for (AActor* ResponderActor : FoundActors)
		{
			FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(Actor->GetName());
			Delegate.AddUniqueDynamic(ResponderActor->FindComponentByClass<UResponder>(), &UResponder::Respond);

			UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Actor->GetName());
		}

		/*for (FString ResponderName : Detector->ConnectedResponders)
		{
			FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(Actor->GetName());
			Delegate.AddUniqueDynamic(Responder, &UResponder::Respond);

			UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Actor->GetName());
		}*/

		/*for (TTuple<FString, FResponders> DetectorRespondersPair : DetectorResponders)
		{
			if (DetectorRespondersPair.Value.NameArray.Find(Actor->GetName()))
			{
				UResponder* Responder = Actor->FindComponentByClass<UResponder>();
				DetectorRespondersPair.Value.Delegate.AddUniqueDynamic(Responder, &UResponder::Respond);

				UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Actor->GetName());
			}
		}*/

		/*UResponder* Responder = Actor->FindComponentByClass<UResponder>();
		//FDetectorDelegate* Delegate = DetectorDelegates.Find(Responder->ConnectedDetector);
		FResponders* Responders = DetectorResponders.Find(Responder->ConnectedDetector);
		//if (Delegate)
		if (Responders)
		{
			//Delegate->AddUniqueDynamic(Responder, &UResponder::Respond);
			Responders->Delegate.AddUniqueDynamic(Responder, &UResponder::Respond);

			Check = Actor->GetName();
			UE_LOG(LogSecuritySystem, Log, TEXT("BeginPlay:  Manager: Bound to %s"), *Check);
		}*/
	}
}

/*
ASecurityManager::ASecurityManager()
{
}

ASecurityManager::~ASecurityManager()
{
}

void ASecurityManager::BeginPlay()
{
}

void ASecurityManager::Tick(float DeltaTime)
{
}*/

void USecurityManagerSubsystem::BindResponderToDetector(const FString& DetectorName, const FString& ResponderName)
{
	//FResponders& Responders = DetectorResponders.FindOrAdd(DetectorName);
	//Responders.NameArray.AddUnique(*ResponderName);

	//FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(DetectorName);
	//Delegate.AddUniqueDynamic(&Responder, &UResponder::Respond);		//leave in for now, need to remove when dropdown gets refactored
}

void USecurityManagerSubsystem::RemoveDetectorResponder(const FString& DetectorName, const FString& ResponderName)
{
	//DetectorResponders[DetectorName].NameArray.RemoveSingle(ResponderName);

	//DetectorDelegates[DetectorName].RemoveDynamic(&Responder, &UResponder::Respond);
}

void USecurityManagerSubsystem::TriggerResponders(const FString& DetectorName, ESecurityState SecurityState)
{
	UE_LOG(LogSecuritySystem, Log, TEXT("Manager: Trigger responders of %s"), *DetectorName);

	if (!DetectorDelegates.Find(DetectorName))
	//if (!DetectorResponders.Find(DetectorName))
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("Manager: TriggerResponders: %s is NOT bound"), *DetectorName);
		return;
	}

	//DetectorResponders[DetectorName].Delegate.Broadcast(SecurityState);

	DetectorDelegates[DetectorName].Broadcast(SecurityState);
}
