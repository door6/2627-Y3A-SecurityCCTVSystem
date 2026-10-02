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
	return InteractMapping;
}

void UCCTVInputMappingContext::PostInitProperties()
{
    Super::PostInitProperties();

    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UCCTVInteractInputAction* CCTVInteractInputAction = NewObject<UCCTVInteractInputAction>(this);
        UCCTVNextCamInputAction* CCTVNextCamInputAction = NewObject<UCCTVNextCamInputAction>(this);
        UCCTVPrevCamInputAction* CCTVPrevCamInputAction = NewObject<UCCTVPrevCamInputAction>(this);
        UCCTVTriggerInputAction* CCTVTriggerInputAction = NewObject<UCCTVTriggerInputAction>(this);
        UCCTVExitInputAction* CCTVExitInputAction = NewObject<UCCTVExitInputAction>(this);

        InteractMapping = FEnhancedActionKeyMapping(CCTVInteractInputAction, EKeys::F);
        /*InteractKeyIndex =*/ DefaultKeyMappings.Mappings.Add(InteractMapping);

        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVNextCamInputAction, EKeys::Right));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVPrevCamInputAction, EKeys::Left));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVTriggerInputAction, EKeys::V));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVExitInputAction, EKeys::Escape));

        UE_LOG(LogSecuritySystem, Log, TEXT("1 Mappings size: %d"), Mappings.Num());
        UE_LOG(LogSecuritySystem, Log, TEXT("2 Mappings size: %d"), GetMappings().Num());

        UE_LOG(LogSecuritySystem, Log, TEXT("this=%p Mappings.Num()=%d GetMappings().Num()=%d"),
            this, Mappings.Num(), GetMappings().Num());
    }
}