// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManager.h"
#include "SecuritySystemLog.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASecurityManager::ASecurityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogSecuritySystem, Display, TEXT("SecurityManager: Initialisation"));
}

ASecurityManager::~ASecurityManager()
{
	//UE_LOG(LogSecuritySystem, Warning, TEXT("DetectorRespondersMap.Num() = %d"), DetectorRespondersMap.Num());

	UE_LOG(LogSecuritySystem, Log, TEXT("SecurityManager destroyed"));
}

// Called when the game starts or when spawned
void ASecurityManager::BeginPlay()
{
	Super::BeginPlay();

	/*UE_LOG(LogSecuritySystem, Warning, TEXT("DetectorRespondersMap.Num() = %d"), DetectorRespondersMap.Num());

	for (const TTuple<FString, FResponders> DetectorRespondersPair : DetectorRespondersMap)
	{
		FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(DetectorRespondersPair.Key);
		for (UResponder* Responder : DetectorRespondersPair.Value.Array)
		{
			if (!Responder)
			{
				UE_LOG(LogSecuritySystem, Error, TEXT("Responder NULLPTR"));
				continue;
			}

			Delegate.AddUniqueDynamic(Responder, &UResponder::Respond);
		}

		
	}
	UE_LOG(LogSecuritySystem, Log, TEXT("Filled DetectorDelegates map"));*/

	UE_LOG(LogSecuritySystem, Warning, TEXT("Number of bound detectors during BeginPlay = %d"), DetectorDelegates.Num());	



	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), UResponder::StaticClass(), FoundActors);

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
	


	if (DetectorDelegates.Find(DetectorNameTemp))
	{
		if (!DetectorDelegates.Find(DetectorNameTemp)->IsBound())
		{
			UE_LOG(LogSecuritySystem, Error, TEXT("BeginPlay: Manager: DetectorDelegate is NOT bound"));
		}

		UE_LOG(LogSecuritySystem, Display, TEXT("BeginPlay:  Manager: DetectorDelegate is bound"));
	}
	else
		UE_LOG(LogSecuritySystem, Error, TEXT("BeginPlay:  Manager: DetectorDelegate doesn't exist"));
}

// Called every frame
void ASecurityManager::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void ASecurityManager::BindResponderToDetector(const FString& DetectorName, UResponder& Responder)
{
	/*FResponders& Responders = DetectorRespondersMap.FindOrAdd(DetectorName);
	Responders.Array.AddUnique(&Responder);*/

	DetectorNameTemp = DetectorName;

	FDetectorDelegate& Delegate = DetectorDelegates.FindOrAdd(DetectorName);
	Delegate.AddUniqueDynamic(&Responder, &UResponder::Respond);		//leave in for now, need to remove when dropdown gets refactored
}

void ASecurityManager::RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder)
{
	//DetectorRespondersMap[DetectorName].Array.Remove(&Responder);

	DetectorDelegates[DetectorName].RemoveDynamic(&Responder, &UResponder::Respond);
}

void ASecurityManager::TriggerResponders(const FString& DetectorName, ESecurityState SecurityState)
{
	UE_LOG(LogSecuritySystem, Log, TEXT("Manager: Trigger responders of %s"), *DetectorName);

	DetectorDelegates[DetectorName].Broadcast(SecurityState);
}

