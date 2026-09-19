// Fill out your copyright notice in the Description page of Project Settings.


#include "Detector.h"
#include "Kismet/GameplayStatics.h"
#include "SecuritySystemLog.h"

#include "SecurityManager.h"

// Sets default values for this component's properties
UDetector::UDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogSecuritySystem, Display, TEXT("Detector: BeginPlay"));

	//if (!ConnectedResponders.IsEmpty())
	{
		/*TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecurityManager::StaticClass(), FoundActors);
		ASecurityManager* SecurityManager = Cast<ASecurityManager>(FoundActors[0]);
		NotifyManagerDelegate.AddUniqueDynamic(SecurityManager, &ASecurityManager::TriggerResponders);*/

		USecurityManagerSubsystem* SecurityManager= GetWorld()->GetSubsystem<USecurityManagerSubsystem>();
		NotifyManagerDelegate.AddUniqueDynamic(SecurityManager, &USecurityManagerSubsystem::TriggerResponders);
	}


	if (!NotifyManagerDelegate.IsBound())
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("BeginPlay: %s: Manager is NOT bound"), *GetOwner()->GetActorLabel());
	}
	else
	 UE_LOG(LogSecuritySystem, Display, TEXT("BeginPlay: %s: Manager is bound"), *GetOwner()->GetActorLabel());

}


// Called every frame
void UDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDetector::TriggerResponders(ESecurityState SecurityState)
{
	//temp testing
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, GetOwner()->GetName());	

	if (!NotifyManagerDelegate.IsBound())
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("%s: Manager is NOT bound"), *GetOwner()->GetActorLabel());
	}

	UE_LOG(LogSecuritySystem, Display, TEXT("%s: TriggerResponders"), *GetOwner()->GetActorLabel());

	NotifyManagerDelegate.Broadcast(GetOwner()->GetName(), SecurityState);
}