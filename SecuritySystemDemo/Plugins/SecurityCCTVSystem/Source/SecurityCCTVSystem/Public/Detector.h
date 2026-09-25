// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"
#include "Detector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyManagerDelegate, const FString&, DetectorName, ESecurityState, SecurityState);

//class FGameplayDebuggerCategory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName = "Detector"))
class SECURITYCCTVSYSTEM_API UDetector : public UActorComponent
{
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

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void TriggerResponders(ESecurityState SecurityState);

	UPROPERTY(EditAnywhere, Category = "Security System")
	float CooldownTimer = 2.0f;

	ESecurityState CurrentState = ESecurityState::Neutral;


	UPROPERTY()
	FNotifyManagerDelegate NotifyManagerDelegate;

	/*UPROPERTY(VisibleAnywhere, Category = "Security System")
	TArray<FString> ConnectedResponders = TArray<FString>();*/

};
