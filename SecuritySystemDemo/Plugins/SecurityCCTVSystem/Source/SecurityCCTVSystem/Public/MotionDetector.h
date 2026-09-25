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
	// Sets default values for this actor's properties
	AMotionDetector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DetectIntruderArray(const TArray<AActor*>& DetectedActors);

	UFUNCTION()
	void DetectIntruder(AActor* Actor, FAIStimulus Stimulus);

#if WITH_GAMEPLAY_DEBUGGER_MENU
	virtual void DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const;
#endif // WITH_GAMEPLAY_DEBUGGER_MENU


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMeshComponent;

	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere, Category = "Security System", Instanced)
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor LoseSightRangeDebugColor;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DetectionDebugColor;

	UPROPERTY(VisibleAnywhere, Category = "Detector")
	UDetector* DetectorComponent;
};
