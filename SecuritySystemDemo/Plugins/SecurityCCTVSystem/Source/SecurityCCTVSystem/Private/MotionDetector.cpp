// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionDetector.h"
#include "Perception/AISenseConfig_Sight.h"
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

	//add ai perception component and create sight configuration for it
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruderArray);
	//PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruder);

	//set sight sense properties
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetector>(TEXT("Detector Component"));


}

// Called when the game starts or when spawned
void AMotionDetector::BeginPlay()
{
	Super::BeginPlay();
	
	//PerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruderArray);
	//PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMotionDetector::DetectIntruder);
}

// Called every frame
void AMotionDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMotionDetector::DetectIntruderArray(const TArray<AActor*>& DetectedActors)
{
	TArray<AActor*> PerceivedActors;	
	PerceptionComponent->GetCurrentlyPerceivedActors(PerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>()->GetSenseImplementation(), PerceivedActors);

	FString Message = this->GetActorLabel() + ": Switch to ";

	switch (DetectorComponent->CurrentState)
	{
	case ESecurityState::Neutral:
		if (PerceivedActors.IsEmpty())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("State: Neutral, ActorArray: Empty"));
			return;
		}
		DetectorComponent->CurrentState = ESecurityState::Alarm;
		Message += "Alarm";
		break;
	case ESecurityState::Alarm:
		if (!PerceivedActors.IsEmpty())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("State: Alarm, ActorArray: Not empty"));
			Message = "";
			for (AActor* Actor : PerceivedActors)
			{
				Message += Actor->GetActorLabel() + ", ";
			}
			UE_LOG(LogSecuritySystem, Log, TEXT("%s"), *Message);
			return;
		}
		DetectorComponent->CurrentState = ESecurityState::Neutral;
		Message += "Neutral";
		break;
	}

#if WITH_GAMEPLAY_DEBUGGER_MENU
	FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage(Message);
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Message);
	UE_LOG(LogSecuritySystem, Display, TEXT("%s"), *Message);

	DetectorComponent->TriggerResponders(DetectorComponent->CurrentState);


	/*for (AActor* Actor : DetectedActors)
	{
		DetectorComponent->TriggerResponders(ESecurityState::Alarm);

		FString Message = Actor->GetActorLabel() + ": Detected intruder";
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Message);

		UE_LOG(LogSecuritySystem, Display, TEXT("%s"), *Message);
	}*/
}

void AMotionDetector::DetectIntruder(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.SensingSucceeded)
	{
		if (DetectorComponent->CurrentState != ESecurityState::Alarm)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Detected");

#if WITH_GAMEPLAY_DEBUGGER_MENU
			FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage("Detected");
#endif // WITH_GAMEPLAY_DEBUGGER_MENU
		}
	}
	else
	{
		if (DetectorComponent->CurrentState != ESecurityState::Neutral)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Forgotten");

#if WITH_GAMEPLAY_DEBUGGER_MENU
			FGameplayDebuggerCategory_SecuritySystem::AddOnScreenDebugMessage("Forgotten");
#endif // WITH_GAMEPLAY_DEBUGGER_MENU
		}

	}
}

#if WITH_GAMEPLAY_DEBUGGER_MENU
void AMotionDetector::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const
{
	if (DebuggerCategory == nullptr)
	{
		return;
	}

	for (UAIPerceptionComponent::FActorPerceptionContainer::TConstIterator It(PerceptionComponent->GetPerceptualDataConstIterator()); It; ++It)
	{
		const FActorPerceptionInfo& ActorPerceptionInfo = It->Value;
		const AActor* Target = ActorPerceptionInfo.Target.Get();
		if (Target != nullptr)
		{
			const FVector TargetLocation = Target->GetActorLocation();
			for (const FAIStimulus& Stimulus : ActorPerceptionInfo.LastSensedStimuli)
			{
				const UAISenseConfig* SenseConfig = PerceptionComponent->GetSenseConfig(Stimulus.Type);
				if (Stimulus.IsValid() && (Stimulus.IsExpired() == false) && SenseConfig)
				{
					const FString Description = FString::Printf(TEXT("%s: %.2f age:%.2f"), *SenseConfig->GetSenseName(), Stimulus.Strength, Stimulus.GetAge());
					const FColor DebugColor = SenseConfig->GetDebugColor();

					DebuggerCategory->AddShape(FGameplayDebuggerShape::MakePoint(Stimulus.StimulusLocation + FVector(0, 0, 30), 30.0f, DebugColor, Description));
					DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(Stimulus.ReceiverLocation, Stimulus.StimulusLocation, DebugColor));
					DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeSegment(TargetLocation, Stimulus.StimulusLocation, FColor::Black));
				}
			}
		}
	}

	PerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>()->DescribeSelfToGameplayDebugger(PerceptionComponent, DebuggerCategory);
}
#endif // WITH_GAMEPLAY_DEBUGGER_MENU