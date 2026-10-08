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
	// Sets default values for this actor's properties
	ACollisionBasedDetector();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_GAMEPLAY_DEBUGGER_MENU
	virtual void DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const;
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnColliderBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnColliderBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Security System")
	UBoxComponent* ColliderBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	UDetectorComponent* DetectorComponent;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DebugColor;

	UPROPERTY(EditAnywhere, Category = "Security System")
	FColor DetectionDebugColor;

	TArray<AActor*> OverlappedActors;
};
