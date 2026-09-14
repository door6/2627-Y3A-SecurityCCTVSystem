// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h" 
class IPropertyHandle;
class IDetailChildrenBuilder;

/**
 * 
 */
class SECURITYCCTVSYSTEM_API FSecurityManagerInterface : public IDetailCustomization
{
public:

    static TSharedRef<IDetailCustomization> MakeInstance();

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
protected:

    //void GenerateRecipeArrayElementWidget(TSharedRef<IPropertyHandle> ChildHandle, int32 ArrayIndex, IDetailChildrenBuilder& ChildrenBuilder, IDetailLayoutBuilder* DetailLayout);

    TSharedRef<SWidget> BuildResponderDropdown(AActor* DetectorActor, int32 ResponderIndex);
    FReply OnAddResponderClicked(AActor* DetectorActor);
    //TArray<AActor*> GetResponderCandidates() const;
    //FText GetResponderDisplayText(AActor* DetectorActor, int32 Index) const;
    //void OnResponderSelected(AActor* NewResponder, AActor* DetectorActor, int32 ResponderIndex);

private:

    //void ShareRecipeKeys();

    //TArray<TSharedPtr<FName>> RecipeKeys;

    //TSharedPtr<IPropertyHandle> CurrentHandle;


    //TMap<FString, TArray<TWeakObjectPtr<UResponder>>> DetectorResponders;
};
