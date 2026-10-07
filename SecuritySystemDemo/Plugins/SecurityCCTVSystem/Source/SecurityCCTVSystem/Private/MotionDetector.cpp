// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionDetector.h"
//#include "Perception/AISenseConfig_Sight.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerTypes.h"
#include "GameplayDebuggerCategory.h"

#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

// Sets default values
AMotionDetector::AMotionDetector()
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

	//add ai perception component and create sight configuration for it
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruders);

	//set sight sense properties
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	//set sight sense debug colors
	LoseSightRangeDebugColor = FColorList::NeonPink;
	DetectionDebugColor = FColor::Red;

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetectorComponent>(TEXT("Detector Component"));
}

// Called every frame
void AMotionDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AMotionDetector::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMotionDetector::DetectIntruders(const TArray<AActor*>& DetectedActors)
{
	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SightConfig->GetSenseImplementation(), PerceivedActors);

	ESecurityState NewSecurityState = ESecurityState::Neutral;
	switch (DetectorComponent->GetCurrentState())
	{
	case ESecurityState::Neutral:
		// return if current state is neutral and no actors were perceived
		if (PerceivedActors.IsEmpty())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("%s: State: Neutral, ActorArray: Empty"), *GetActorLabel());
			return;
		}
		NewSecurityState = ESecurityState::Alarm;
		break;
	case ESecurityState::Alarm:
		// return if current state is alarm and actors were perceived
		if (!PerceivedActors.IsEmpty())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("%s: State: Alarm, ActorArray: Not empty"), *GetActorLabel());
			FString Message = "Perceived actors: ";
			for (AActor* Actor : PerceivedActors)
			{
				Message += Actor->GetActorLabel() + ", ";
			}
			UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *Message);
			return;
		}
		NewSecurityState = ESecurityState::Neutral;
		break;
	}

	DetectorComponent->ChangeState(NewSecurityState);
}

/*void AMotionDetector::DetectIntruderOld(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.SensingSucceeded)
	{
		if (DetectorComponent->GetCurrentState() != ESecurityState::Alarm)
		{
#if WITH_GAMEPLAY_DEBUGGER_MENU
			FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage("Detected");
#endif // WITH_GAMEPLAY_DEBUGGER_MENU
		}
	}
	else
	{
		if (DetectorComponent->GetCurrentState() != ESecurityState::Neutral)
		{
#if WITH_GAMEPLAY_DEBUGGER_MENU
			FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage("Forgotten");
#endif // WITH_GAMEPLAY_DEBUGGER_MENU
		}

	}
}*/

#if WITH_GAMEPLAY_DEBUGGER_MENU
// implemented based on the DescribeSelfToGameplayDebugger from AIPerceptionComponent and AISenseConfig_Sight
void AMotionDetector::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const
{
	if (PerceptionComponent == nullptr || DebuggerCategory == nullptr)
	{
		return;
	}

	FColor SightRangeColor = SightConfig->GetDebugColor();
	FColor LoseSightRangeColor = LoseSightRangeDebugColor;

	if (DetectorComponent->GetCurrentState() == ESecurityState::Alarm)
	{
		SightRangeColor = DetectionDebugColor;
		LoseSightRangeColor = DetectionDebugColor;
	}

	FVector BodyLocation, BodyFacing;
	PerceptionComponent->GetLocationAndDirection(BodyLocation, BodyFacing);

	// sight vectors
	const float SightPieLength = FMath::Max(SightConfig->LoseSightRadius, SightConfig->SightRadius) + SightConfig->PointOfViewBackwardOffset;
	const float MinSightPieLength = SightPieLength - (SightConfig->LoseSightRadius - SightConfig->SightRadius);
	const FVector RootLocation = BodyLocation - (BodyFacing * SightConfig->PointOfViewBackwardOffset);
	const FVector LeftDirection = BodyFacing.RotateAngleAxis(SightConfig->PeripheralVisionAngleDegrees, FVector::UpVector);
	const FVector RightDirection = BodyFacing.RotateAngleAxis(-SightConfig->PeripheralVisionAngleDegrees, FVector::UpVector);
	const FVector DownDirection = BodyFacing.RotateAngleAxis(SightConfig->PeripheralVisionAngleDegrees, FVector::RightVector);
	const FVector UpDirection = BodyFacing.RotateAngleAxis(-SightConfig->PeripheralVisionAngleDegrees, FVector::RightVector);
		
	// forward / left / right / down / up debug lines
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * SightConfig->NearClippingRadius), RootLocation + (BodyFacing * SightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (LeftDirection * SightConfig->NearClippingRadius), RootLocation + (LeftDirection * SightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (RightDirection * SightConfig->NearClippingRadius), RootLocation + (RightDirection * SightPieLength), SightRangeColor));		
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (DownDirection * SightConfig->NearClippingRadius), RootLocation + (DownDirection * SightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (UpDirection * SightConfig->NearClippingRadius), RootLocation + (UpDirection * SightPieLength), SightRangeColor));

	// near clipping radius debug lines
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (LeftDirection * SightConfig->NearClippingRadius), RootLocation + (BodyFacing * SightConfig->NearClippingRadius), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * SightConfig->NearClippingRadius), RootLocation + (RightDirection * SightConfig->NearClippingRadius), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (DownDirection * SightConfig->NearClippingRadius), RootLocation + (BodyFacing * SightConfig->NearClippingRadius), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * SightConfig->NearClippingRadius), RootLocation + (UpDirection * SightConfig->NearClippingRadius), SightRangeColor));
	
	// sight radius, lose sight radius debug lines
	// left -> forward -> right
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (LeftDirection * MinSightPieLength), RootLocation + (BodyFacing * MinSightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * MinSightPieLength), RootLocation + (RightDirection * MinSightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (LeftDirection * SightPieLength), RootLocation + (BodyFacing * SightPieLength), LoseSightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * SightPieLength), RootLocation + (RightDirection * SightPieLength), LoseSightRangeColor));
	// down -> forward -> up
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (DownDirection * MinSightPieLength), RootLocation + (BodyFacing * MinSightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * MinSightPieLength), RootLocation + (UpDirection * MinSightPieLength), SightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (DownDirection * SightPieLength), RootLocation + (BodyFacing * SightPieLength), LoseSightRangeColor));
	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(RootLocation + (BodyFacing * SightPieLength), RootLocation + (UpDirection * SightPieLength), LoseSightRangeColor));
}
#endif // WITH_GAMEPLAY_DEBUGGER_MENU