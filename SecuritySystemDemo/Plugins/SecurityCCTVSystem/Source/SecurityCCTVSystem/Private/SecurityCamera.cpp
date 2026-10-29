// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCamera.h"

ASecurityCamera::ASecurityCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a cube mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	StaticMeshComponent->SetStaticMesh(CubeMesh);
	StaticMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//add scene capture component 2d
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component 2D"));
	SceneCaptureComponent->SetupAttachment(Root);
	SceneCaptureComponent->HiddenActors.AddUnique(this);

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetectorComponent>(TEXT("Detector Component"));
	DetectorComponent->SetAlertTime(0.0f);	// so the switch between security states is instant
}

void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();	
}