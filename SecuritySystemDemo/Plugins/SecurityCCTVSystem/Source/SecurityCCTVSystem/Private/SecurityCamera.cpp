// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCamera.h"

// Sets default values
ASecurityCamera::ASecurityCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a cube mesh component
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	CubeMeshComponent->SetStaticMesh(CubeMesh);
	CubeMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//add scene capture component 2d
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component 2D"));
	SceneCaptureComponent2D->SetupAttachment(Root);
	//SceneCaptureComponent2D->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetector>(TEXT("Detector Component"));

}

// Called when the game starts or when spawned
void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

