// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UTextureRenderTarget2D* TextureRenderTarget;
	UMaterial* RenderMaterial;
	UMaterialInstanceDynamic* DynamicMaterialInstance;

};
