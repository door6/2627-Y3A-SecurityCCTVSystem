// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
#include "Blueprint/UserWidget.h"
#include "SecurityCamera.h"
#include "CCTVTerminal.generated.h"

UCLASS()
class SECURITYCCTVSYSTEM_API ACCTVTerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCTVTerminal();

	const bool inline IsInUse();

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
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> RenderMaterial;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Security System")
	float InteractRadius;

	UPROPERTY(EditAnywhere, Category = "Security System") 
	TSubclassOf<UUserWidget> TerminalWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> TerminalWidget;

	TArray<TWeakObjectPtr<ASecurityCamera>> SecurityCameras;
	int CurrentCameraIndex = 0;
	bool InUse = false;
};
