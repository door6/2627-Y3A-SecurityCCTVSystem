// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;
class UDetectorComponent;
class UResponderComponent;
class USecurityManagerSubsystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayDebugDraw, FGameplayDebuggerCategory*);

struct SECURITYCCTVSYSTEM_API FDebugMessage
{
	FString Message;
	float Timer;
};

class SECURITYCCTVSYSTEM_API FGameplayDebuggerCategory_SecuritySystem : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_SecuritySystem();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	static void AddOnScreenDebugMessage(const FString& Message, float DelayTime = 5.0f);

	static void BindToGameplayDebugDraw();

	static inline FGameplayDebugDraw OnGameplayDebugDraw;

private:
	//TArray<AActor*> Detectors;
	//TArray<AActor*> Responders;
	//USecurityManagerSubsystem* SecurityManager = nullptr;
	TArray<TWeakObjectPtr<UDetectorComponent>> DetectorComponents;
	TArray<TWeakObjectPtr<UResponderComponent>> ResponderComponents;
	TWeakObjectPtr<USecurityManagerSubsystem> SecurityManager;

	static inline TArray<FDebugMessage> PendingMessages;
};

#endif // WITH_GAMEPLAY_DEBUGGER_MENU