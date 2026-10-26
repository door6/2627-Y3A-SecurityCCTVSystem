// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionBasedDetector.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerTypes.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

ACollisionBasedDetector::ACollisionBasedDetector()
{
	PrimaryActorTick.bCanEverTick = false;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a collider shape component
	ColliderBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	ColliderBoxComponent->SetupAttachment(Root);
	ColliderBoxComponent->SetGenerateOverlapEvents(true);
	ColliderBoxComponent->SetBoxExtent(FVector(300.f, 300.f, 300.f), true);

	//set debug colors
	DebugColor = FColorList::Green;
	DetectionDebugColor = FColor::Red;

	//add detector component
	DetectorComponent = CreateDefaultSubobject<UDetectorComponent>(TEXT("Detector Component"));
}

void ACollisionBasedDetector::Tick(float DeltaTime)
{
}

void ACollisionBasedDetector::BeginPlay()
{
	Super::BeginPlay();

	// bind collider overlap delegates
	ColliderBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollisionBasedDetector::OnColliderBoxBeginOverlap);
	ColliderBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACollisionBasedDetector::OnColliderBoxEndOverlap);

	FGameplayDebuggerCategory_SecuritySystem::OnGameplayDebugDraw.AddUObject(this, &ACollisionBasedDetector::DescribeSelfToGameplayDebugger);
	//DetectorComponent->OnGameplayDebugDraw.AddUObject(this, &ACollisionBasedDetector::DescribeSelfToGameplayDebugger);
	
}

void ACollisionBasedDetector::OnColliderBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedActors.Add(OtherActor);
	DetectorComponent->CheckCanStateBeChanged(ESecurityState::Alarm);
}

void ACollisionBasedDetector::OnColliderBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.Remove(OtherActor);

	// return to neutral state only if there are no overlapped actors
	if (OverlappedActors.IsEmpty())
	{
		DetectorComponent->CheckCanStateBeChanged(ESecurityState::Neutral);
	}
}

#if WITH_GAMEPLAY_DEBUGGER_MENU
void ACollisionBasedDetector::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory* DebuggerCategory) const
{
	if (ColliderBoxComponent == nullptr || DebuggerCategory == nullptr)
	{
		return;
	}

	FColor CurrentDebugColor = DebugColor;
	if (DetectorComponent->GetCurrentState() == ESecurityState::Alarm)
	{
		CurrentDebugColor = DetectionDebugColor;
	}

	DebuggerCategory->AddShape(FGameplayDebuggerShape::MakeBox(ColliderBoxComponent->GetComponentLocation(), ColliderBoxComponent->GetScaledBoxExtent(), CurrentDebugColor));
}
#endif // WITH_GAMEPLAY_DEBUGGER_MENU