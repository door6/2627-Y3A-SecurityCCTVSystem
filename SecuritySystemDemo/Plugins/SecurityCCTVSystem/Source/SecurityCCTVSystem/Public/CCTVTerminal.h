// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
#include "CCTVTerminalWidget.h"
#include "SecurityCamera.h"
#include "CCTVTerminal.generated.h"

UCLASS()
class SECURITYCCTVSYSTEM_API ACCTVTerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCTVTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TurnOn(const FInputActionValue& Value);

	UFUNCTION()
	void TurnOff();

	UFUNCTION()
	void SwitchToNextCamera();
	UFUNCTION()
	void SwitchToPreviousCamera();
	UFUNCTION()
	void TriggerResponders();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget;
	//UTextureRenderTarget2D* TextureRenderTarget;

	UPROPERTY(VisibleAnywhere)
	UMaterialInterface* RenderMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY()
	UCCTVTerminalWidget* TerminalWidget;


	UPROPERTY(EditAnywhere, Category = "Security System")
	float InteractRadius = 500.f;

	TArray<ASecurityCamera*> SecurityCameras;
	int CurrentCameraIndex = 0;
	bool InUse = false;
};
