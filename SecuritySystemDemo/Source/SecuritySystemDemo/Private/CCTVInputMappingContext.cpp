// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVInputMappingContext.h"

UCCTVInputMappingContext::UCCTVInputMappingContext()
{
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
        DefaultKeyMappings.Mappings.Add(InteractMapping);

        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVNextCamInputAction, EKeys::Right));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVPrevCamInputAction, EKeys::Left));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVTriggerInputAction, EKeys::V));
        DefaultKeyMappings.Mappings.Add(FEnhancedActionKeyMapping(CCTVExitInputAction, EKeys::Escape));
    }
}