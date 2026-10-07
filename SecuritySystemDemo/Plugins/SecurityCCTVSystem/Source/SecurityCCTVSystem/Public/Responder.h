// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ESecurityState.h"
#include "SecurityManager.h"
#include "Responder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRespondTriggered, ESecurityState, SecurityState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECURITYCCTVSYSTEM_API UResponderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResponderComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindToManager(FDetectorDelegate& ManagerDelegate);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Security System")
	FOnRespondTriggered OnRespondTriggered;

private:
	UFUNCTION(Category = "Security System")
	void Respond(ESecurityState SecurityState);

	ESecurityState CurrentState = ESecurityState::Neutral;
};
