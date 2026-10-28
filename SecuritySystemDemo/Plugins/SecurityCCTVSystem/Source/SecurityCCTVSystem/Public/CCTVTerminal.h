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
	ACCTVTerminal();

protected:
	virtual void BeginPlay() override;

public:	
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
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> RenderMaterial;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCCTVTerminalWidget> TerminalWidget;

	UPROPERTY(EditAnywhere, Category = "Security System")
	float InteractRadius;

private:
	TArray<TWeakObjectPtr<ASecurityCamera>> SecurityCameras;
	int CurrentCameraIndex = 0;
	bool InUse = false;
};
