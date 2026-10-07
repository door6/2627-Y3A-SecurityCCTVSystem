// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionBasedDetector.h"

// Sets default values
ACollisionBasedDetector::ACollisionBasedDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a cube mesh component
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	CubeMeshComponent->SetStaticMesh(CubeMesh);
	CubeMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//add a collider shape component
	ColliderBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	ColliderBoxComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetectorComponent>(TEXT("Detector Component"));
}

// Called every frame
void ACollisionBasedDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void ACollisionBasedDetector::BeginPlay()
{
	Super::BeginPlay();
	
}