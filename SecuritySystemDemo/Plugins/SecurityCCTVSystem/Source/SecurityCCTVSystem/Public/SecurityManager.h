// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "Responder.h"
#include "SecurityManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);

#if 0
USTRUCT()
struct SECURITYCCTVSYSTEM_API FRespondersDelegate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UResponder>> Responders;
	FDetectorDelegate Delegate;
};

UCLASS()
class SECURITYCCTVSYSTEM_API ASecurityManager : public AActor
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityManager();
	~ASecurityManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//FResponders* GetDetectorResponders(const FString& DetectorName);

	void BindResponderToDetector(const FString& DetectorName, UResponder& Responder);
	void RemoveDetectorResponder(const FString& DetectorName, UResponder& Responder);

	/*UFUNCTION()
	void TriggerResponders(const FString& DetectorName, ESecurityState SecurityState);*/


	
	//TMap<FString, FResponders> DetectorRespondersMap;

	UPROPERTY()
	TMap<FString, FDetectorDelegate> DetectorDelegates;
	//TMap<FString, FRespondersDelegate> DetectorResponders;

	UPROPERTY()
	FString DetectorNameTemp; //temp for testing
	
};
#endif


USTRUCT()
struct FResponders
{
	GENERATED_BODY()

	FDetectorDelegate Delegate;

	UPROPERTY(VisibleAnywhere, Config, Category = "Security System")	//BlueprintReadOnly
	TArray<FString> NameArray = TArray<FString>();
};



//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetectorDelegate, ESecurityState, SecurityState);


UCLASS(/*Config = Game*/)
class SECURITYCCTVSYSTEM_API USecurityManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void BindResponderToDetector(const FString& DetectorName, const FString& ResponderName);
	void RemoveDetectorResponder(const FString& DetectorName, const FString& ResponderName);

	UFUNCTION()
	void TriggerResponders(const FString& DetectorName, ESecurityState SecurityState);

	//UPROPERTY(VisibleAnywhere, Config, Category = "Security System")
	//TMap<FString, FResponders> DetectorResponders;

	TMap<FString, FDetectorDelegate> DetectorDelegates;

};
