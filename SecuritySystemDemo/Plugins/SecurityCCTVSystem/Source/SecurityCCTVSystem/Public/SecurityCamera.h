// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Detector.h"
#include "SecurityCamera.generated.h"

UCLASS()
class SECURITYCCTVSYSTEM_API ASecurityCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* CubeMeshComponent;

	USceneCaptureComponent2D* SceneCaptureComponent2D;
	UDetector* DetectorComponent;
};
