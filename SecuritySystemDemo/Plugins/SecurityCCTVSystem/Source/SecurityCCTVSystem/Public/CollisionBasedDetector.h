// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Detector.h"
#include "CollisionBasedDetector.generated.h"

class FGameplayDebuggerCategory;

UCLASS()
class SECURITYCCTVSYSTEM_API ACollisionBasedDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollisionBasedDetector();
	virtual void Tick(float DeltaTime) override;

#if WITH_GAMEPLAY_DEBUGGER_MENU
	virtual void DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const;
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnColliderBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnColliderBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// components
	UPROPERTY(VisibleAnywhere, Category = "Security System")
	TObjectPtr<UBoxComponent> ColliderBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	TObjectPtr<UDetectorComponent> DetectorComponent;

	// variables
	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DebugColor;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DetectionDebugColor;

	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
};
