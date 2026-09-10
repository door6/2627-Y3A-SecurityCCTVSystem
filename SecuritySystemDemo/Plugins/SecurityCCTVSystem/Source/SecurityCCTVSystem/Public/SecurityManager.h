// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Responder.h"
#include "SecurityManager.generated.h"

UCLASS()
class SECURITYCCTVSYSTEM_API ASecurityManager : public AActor
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BindResponderToDetector(const FString& DetectorName, UResponder& Responder);
	void RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder);
	void TriggerResponders(const FString& DetectorName, ESecurityState SecurityState);

	TMap<FString, FDetectorDelegate> DetectorDelegates;
	
};
