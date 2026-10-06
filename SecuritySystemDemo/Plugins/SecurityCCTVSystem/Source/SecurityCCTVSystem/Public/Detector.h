// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"
#include "Detector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyManagerDelegate, const FString&, DetectorName, ESecurityState, SecurityState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SECURITYCCTVSYSTEM_API UDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetectorComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindManager();
	inline const ESecurityState GetCurrentState();
	inline const void SetCurrentState(ESecurityState NewState);

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void TriggerResponders(ESecurityState SecurityState);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Security System")
	float CooldownTimer = 2.0f;

	ESecurityState CurrentState = ESecurityState::Neutral;

	UPROPERTY()
	FNotifyManagerDelegate NotifyManagerDelegate;
};
