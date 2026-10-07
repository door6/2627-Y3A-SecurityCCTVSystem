// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Detector.h"
#include "CollisionBasedDetector.generated.h"

UCLASS()
class SECURITYCCTVSYSTEM_API ACollisionBasedDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollisionBasedDetector();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Security System")
	UBoxComponent* ColliderBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	UDetectorComponent* DetectorComponent;

};
