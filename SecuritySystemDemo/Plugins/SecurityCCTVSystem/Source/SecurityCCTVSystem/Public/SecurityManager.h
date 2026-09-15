// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Responder.h"
#include "SecurityManager.generated.h"

/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);

USTRUCT()
struct SECURITYCCTVSYSTEM_API FRespondersDelegate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UResponder>> Responders;
	FDetectorDelegate Delegate;
};*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);

UCLASS()
class SECURITYCCTVSYSTEM_API ASecurityManager : public AActor
{
	


	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//not sure should this exist or not
	FDetectorDelegate* GetDetectorDelegate(const FString& DetectorName);

	void BindResponderToDetector(const FString& DetectorName, UResponder& Responder);
	void RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder);
	void TriggerResponders(const FString& DetectorName, ESecurityState SecurityState);

private:
	TMap<FString, FDetectorDelegate> DetectorDelegates;
	//TMap<FString, FRespondersDelegate> DetectorResponders;
	
};
