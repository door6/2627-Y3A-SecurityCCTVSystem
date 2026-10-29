// Fill out your copyright notice in the Description page of Project Settings.


#include "InputManager.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "CCTVTerminal.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTerminalInput, Log, All);

AInputManager::AInputManager()
{
}

void AInputManager::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
	{
		UE_LOG(LogTerminalInput, Log, TEXT("Local player found"));
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			UE_LOG(LogTerminalInput, Log, TEXT("Input system found"));
			InputMapping = NewObject<UCCTVInputMappingContext>();
			if (InputMapping)
			{
				UE_LOG(LogTerminalInput, Log, TEXT("Input mapping found"));
				InputSystem->AddMappingContext(InputMapping, 0);

				FEnhancedActionKeyMapping CCTVMapping = InputMapping->InteractMapping;
				if (!CCTVMapping.Action)
					UE_LOG(LogTerminalInput, Error, TEXT("CCTVMapping.Action is NULL"));

				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

				Input->BindAction(CCTVMapping.Action, ETriggerEvent::Triggered, this, &AInputManager::AccessCCTVTerminal);
			}
		}
	}
}

void AInputManager::AccessCCTVTerminal()
{
	UE_LOG(LogTerminalInput, Log, TEXT("AccessCCTVTerminal Triggered"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCTVTerminal::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		TObjectPtr<ACCTVTerminal> CCTVTerminal = Cast<ACCTVTerminal>(Actor);
		CCTVTerminal->TurnOn();
	}
}

DEFINE_LOG_CATEGORY(LogTerminalInput);