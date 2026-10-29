// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "SecurityManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);

USTRUCT()
struct FResponders
{
	GENERATED_BODY()

	FDetectorDelegate Delegate;

	UPROPERTY(VisibleAnywhere, Config, Category = "Security System")
	TArray<FString> NameArray = TArray<FString>();
};

UCLASS()
class SECURITYCCTVSYSTEM_API USecurityManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION()
	void TriggerResponders(const FString& DetectorName, ESecurityState SecurityState);

	TMap<FString, FDetectorDelegate> DetectorDelegates;
};
