// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVInputMappingContext.h"
#include "SecuritySystemLog.h"

UCCTVInputMappingContext::UCCTVInputMappingContext()
{
	/*CCTVInteractInputAction = NewObject<UCCTVInteractInputAction>();

	InteractKey = FEnhancedActionKeyMapping(CCTVInteractInputAction, EKeys::E);
	InteractKeyIndex = Mappings.Add(InteractKey);

	UE_LOG(LogSecuritySystem, Log, TEXT("Mappings size: %d"), GetMappings().Num());*/
}

FEnhancedActionKeyMapping& UCCTVInputMappingContext::GetInteractKeyMapping()
{
	return InteractKey;
}

void UCCTVInputMappingContext::PostInitProperties()
{
    Super::PostInitProperties();

    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        CCTVInteractInputAction = NewObject<UCCTVInteractInputAction>(this);
        InteractKey = FEnhancedActionKeyMapping(CCTVInteractInputAction, EKeys::E);
        InteractKeyIndex = DefaultKeyMappings.Mappings.Add(InteractKey);

        UE_LOG(LogSecuritySystem, Log, TEXT("1 Mappings size: %d"), Mappings.Num());
        UE_LOG(LogSecuritySystem, Log, TEXT("2 Mappings size: %d"), GetMappings().Num());

        UE_LOG(LogSecuritySystem, Log, TEXT("this=%p Mappings.Num()=%d GetMappings().Num()=%d"),
            this, Mappings.Num(), GetMappings().Num());
    }
}