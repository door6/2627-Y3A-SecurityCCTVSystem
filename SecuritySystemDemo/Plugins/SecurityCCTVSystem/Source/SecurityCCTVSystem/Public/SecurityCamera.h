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
	ASecurityCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, Category = "Security System")
	TObjectPtr<UDetectorComponent> DetectorComponent;
};
