// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManager.h"

// Sets default values
ASecurityManager::ASecurityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASecurityManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecurityManager::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void ASecurityManager::BindResponderToDetector(const FString& DetectorName, UResponder& Responder)
{
	DetectorDelegates[DetectorName].AddDynamic(&Responder, &UResponder::Respond);
}

void ASecurityManager::RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder)
{
	DetectorDelegates[DetectorName].RemoveDynamic(&Responder, &UResponder::Respond);
}

void ASecurityManager::TriggerResponders(const FString& DetectorName, ESecurityState SecurityState)
{
	DetectorDelegates[DetectorName].Broadcast(SecurityState);
}

