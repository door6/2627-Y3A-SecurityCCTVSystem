// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"
#include "Detector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECURITYCCTVSYSTEM_API UDetector : public UActorComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyManagerDelegate, FString, DetectorName, ESecurityState, SecurityState);

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TriggerResponders(ESecurityState SecurityState);

	ESecurityState CurrentState = ESecurityState::Neutral;
	float CooldownTimer = 2.0f;
	FNotifyManagerDelegate NotifyManagerDelegate;

};
