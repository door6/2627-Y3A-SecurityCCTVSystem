// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Detector.h"
#include "MotionDetector.generated.h"

class FGameplayDebuggerCategory;

UCLASS()
class SECURITYCCTVSYSTEM_API AMotionDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	AMotionDetector();

#if WITH_GAMEPLAY_DEBUGGER_MENU
	virtual void DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const;
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void DetectIntruders(const TArray<AActor*>& DetectedActors);

	// components
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(EditAnywhere, Category = "Security System", Instanced)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	TObjectPtr<UDetectorComponent> DetectorComponent;

	// variables
	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor LoseSightRangeDebugColor;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DetectionDebugColor;
};
