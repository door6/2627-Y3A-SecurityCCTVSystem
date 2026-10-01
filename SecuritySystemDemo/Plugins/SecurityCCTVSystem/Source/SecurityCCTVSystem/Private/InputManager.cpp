// Fill out your copyright notice in the Description page of Project Settings.


#include "InputManager.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "SecuritySystemLog.h"
#include "CCTVTerminal.h"
#include "Kismet/GameplayStatics.h"

AInputManager::AInputManager()
{
	//InputMapping = NewObject<UCCTVInputMappingContext>();
}

void AInputManager::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	//InputMapping = NewObject<UCCTVInputMappingContext>();
	if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("Local player found"));
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("Input system found"));
			InputMapping = NewObject<UCCTVInputMappingContext>();
			if (InputMapping)
			{
				UE_LOG(LogSecuritySystem, Log, TEXT("Input mapping found"));
				InputSystem->AddMappingContext(InputMapping, 0);
				UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

				FEnhancedActionKeyMapping CCTVMapping = InputMapping->InteractMapping;
				//FEnhancedActionKeyMapping CCTVMapping = InputMapping->GetMapping(InputMapping->InteractKeyIndex);
				//FEnhancedActionKeyMapping& CCTVMapping = InputMapping->GetInteractKeyMapping();
				FString Name = CCTVMapping.GetDisplayName().ToString();
				UE_LOG(LogSecuritySystem, Warning, TEXT("CCTVMapping DisplayName: %s"), *Name);		//display name returns nothing
				if (!CCTVMapping.Action)
					UE_LOG(LogSecuritySystem, Error, TEXT("CCTVMapping.Action is NULL"));

				Input->BindAction(CCTVMapping.Action, ETriggerEvent::Triggered, this, &AInputManager::AccessCCTVTerminal);
			}
		}
	}
}

void AInputManager::AccessCCTVTerminal()
{
	UE_LOG(LogSecuritySystem, Warning, TEXT("AccessCCTVTerminal Triggered"));

	/*AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACCTVTerminal::StaticClass());
	if (FoundActor)
	{
		TObjectPtr<ACCTVTerminal> CCTVTerminal = Cast<ACCTVTerminal>(FoundActor);
		CCTVTerminal->TurnOn(FInputActionValue());
	}*/

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCTVTerminal::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		TObjectPtr<ACCTVTerminal> CCTVTerminal = Cast<ACCTVTerminal>(Actor);
		CCTVTerminal->TurnOn(FInputActionValue());
	}

	//fix widget being bound only to the first terminal
	//maybe turn it into uobject and have the manager call the binding action sfunction from its begin play
}
