// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionDetector.h"
#include "SecuritySystemLog.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerTypes.h"
#include "GameplayDebuggerCategory.h"

#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

AMotionDetector::AMotionDetector()
{
	PrimaryActorTick.bCanEverTick = false;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

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

void AMotionDetector::BeginPlay()
{
	Super::BeginPlay();

	FGameplayDebuggerCategory_SecuritySystem::OnGameplayDebugDraw.AddUObject(this, &AMotionDetector::DescribeSelfToGameplayDebugger);
}

void AMotionDetector::DetectIntruders(const TArray<AActor*>& DetectedActors)
{
	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SightConfig->GetSenseImplementation(), PerceivedActors);

	switch (PerceivedActors.IsEmpty())
	{
	case true:
		DetectorComponent->CheckCanStateBeChanged(ESecurityState::Neutral);
		return;
	case false:
		// log all perceived actors
		FString Message = GetActorLabel() + ": Perceived actors = ";
		for (AActor* Actor : PerceivedActors)
		{
			Message += Actor->GetActorLabel() + ", ";
		}
		UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *Message);

		DetectorComponent->CheckCanStateBeChanged(ESecurityState::Alarm);
		return;
	}
}

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