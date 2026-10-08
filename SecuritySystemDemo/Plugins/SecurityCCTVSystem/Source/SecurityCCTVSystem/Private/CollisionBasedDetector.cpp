// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionBasedDetector.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerTypes.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER_MENU

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
	//ColliderBoxComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	ColliderBoxComponent->SetupAttachment(Root);
	ColliderBoxComponent->SetGenerateOverlapEvents(true);
	ColliderBoxComponent->SetBoxExtent(FVector(300.f, 300.f, 300.f), true);	//false
	//ColliderBoxComponent->SetCollisionProfileName(TEXT("Trigger"), false);

	//set debug colors
	DebugColor = FColorList::Green;
	DetectionDebugColor = FColor::Red;

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

	// bind collider overlap delegates
	ColliderBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollisionBasedDetector::OnColliderBoxBeginOverlap);
	ColliderBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACollisionBasedDetector::OnColliderBoxEndOverlap);
	
}

void ACollisionBasedDetector::OnColliderBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedActors.AddUnique(OtherActor);
	DetectorComponent->ChangeState(ESecurityState::Alarm);
}

void ACollisionBasedDetector::OnColliderBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.RemoveSingle(OtherActor);

	// return to neutral state only if there are no overlapped actors
	if (OverlappedActors.IsEmpty())
	{
		DetectorComponent->ChangeState(ESecurityState::Neutral);
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