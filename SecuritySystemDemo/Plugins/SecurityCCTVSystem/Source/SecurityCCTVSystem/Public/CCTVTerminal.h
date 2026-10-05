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

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void TurnOn();

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void TurnOff();

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void SwitchToNextCamera();
	UFUNCTION(BlueprintCallable, Category = "Security System")
	void SwitchToPreviousCamera();
	UFUNCTION(BlueprintCallable, Category = "Security System")
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

	UPROPERTY(EditAnywhere)
	UCCTVTerminalWidget* TerminalWidget = nullptr;


	UPROPERTY(EditAnywhere, Category = "Security System")
	float InteractRadius = 500.f;

private:
	TArray<ASecurityCamera*> SecurityCameras;
	int CurrentCameraIndex = 0;
	bool InUse = false;
};
