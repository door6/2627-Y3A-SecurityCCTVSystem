// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;

class SECURITYCCTVSYSTEM_API FGameplayDebuggerCategory_SecuritySystem : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_SecuritySystem();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
};

#endif // WITH_GAMEPLAY_DEBUGGER_MENU