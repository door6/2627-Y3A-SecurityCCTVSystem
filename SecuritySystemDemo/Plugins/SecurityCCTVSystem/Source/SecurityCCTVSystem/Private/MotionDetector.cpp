// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionDetector.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SecuritySystemLog.h"

// Sets default values
AMotionDetector::AMotionDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a cube mesh
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	CubeMeshComponent->SetStaticMesh(CubeMesh);
	CubeMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	//create sight configuration for ai perception component
	UAIPerceptionComponent* PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruder);
	//PerceptionComponent->OnTargetPerceptionUpdated.AddUnique(this, &AMotionDetector::DetectIntruder);

	//set sight sense properties
	SightConfig->SightRadius = 20.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 20.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), this);

}

// Called when the game starts or when spawned
void AMotionDetector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMotionDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMotionDetector::DetectIntruder(const TArray<AActor*>& DetectedActors)
{
	for (AActor* Actor : DetectedActors)
	{
		FString Message = Actor->GetActorLabel() + ": Detected intruder";
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Message);

		UE_LOG(LogSecuritySystem, Display, TEXT("%s"), *Message);
	}
}

