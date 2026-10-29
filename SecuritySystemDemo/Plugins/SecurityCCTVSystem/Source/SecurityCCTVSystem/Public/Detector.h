// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"
#include "Detector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyManagerDelegate, const FString&, DetectorName, ESecurityState, SecurityState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECURITYCCTVSYSTEM_API UDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDetectorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindManager();
	inline void SetAlertTime(float NewAlertTime);

	UFUNCTION(BlueprintCallable, Category = "Security System")
	inline ESecurityState GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void CheckCanStateBeChanged(ESecurityState NewState);

protected:
	virtual void BeginPlay() override;

private:
	void ChangeStateAndNotifyManager(ESecurityState SecurityState);

	UPROPERTY(EditAnywhere, Category = "Security System")
	float AlertTime = 2.0f;

	float CooldownTimer = 0.0f;
	ESecurityState CurrentState = ESecurityState::Neutral;
	UPROPERTY()
	FNotifyManagerDelegate NotifyManagerDelegate;
};
