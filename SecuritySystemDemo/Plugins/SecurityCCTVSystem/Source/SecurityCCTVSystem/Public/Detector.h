// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"

//#if WITH_GAMEPLAY_DEBUGGER_MENU
//#include "GameplayDebugger_SecuritySystem.h"
//#endif // WITH_GAMEPLAY_DEBUGGER_MENU

#include "Detector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyManagerDelegate, const FString&, DetectorName, ESecurityState, SecurityState);

//#if WITH_GAMEPLAY_DEBUGGER_MENU
//DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayDebugDraw, FGameplayDebuggerCategory*);
//#endif // WITH_GAMEPLAY_DEBUGGER_MENU

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECURITYCCTVSYSTEM_API UDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetectorComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindManager();
	inline void SetAlertTime(float NewAlertTime);

	UFUNCTION(BlueprintCallable, Category = "Security System")
	inline ESecurityState GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "Security System")
	void ChangeState(ESecurityState NewState);

//#if WITH_GAMEPLAY_DEBUGGER_MENU
	//FGameplayDebugDraw OnGameplayDebugDraw;
//#endif //WITH_GAMEPLAY_DEBUGGER_MENU

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void TriggerResponders(ESecurityState SecurityState);		// broadcast new state ( new functio name ? )

	UPROPERTY(EditAnywhere, Category = "Security System")
	float AlertTime = 2.0f;

	float CooldownTimer = 0.0f;
	ESecurityState CurrentState = ESecurityState::Neutral;
	UPROPERTY()
	FNotifyManagerDelegate NotifyManagerDelegate;
};
