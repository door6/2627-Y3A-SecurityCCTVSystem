// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"
#include "SecurityManager.h"
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

    FReply OnDeleteResponderClicked(FString DetectorName, FWeakObjectPtr* Responder);

    TSharedRef<SWidget> BuildResponderDropdown(AActor* DetectorActor, UResponder* SeelectedResponder);
    FReply OnAddResponderClicked(FString DetectorName, UResponder* Responder);
    TArray<AActor*> GetResponderCandidates() const;
    //FText GetResponderDisplayText(AActor* DetectorActor, int32 Index) const;
    void OnResponderSelected(AActor* NewResponder, UResponder* SelectedResponder);
    void OnResponderSelectedTest(FString DetectorName, AActor* Responder);

private:

    //void ShareRecipeKeys();

    //TArray<TSharedPtr<FName>> RecipeKeys;

    //TSharedPtr<IPropertyHandle> CurrentHandle;

    UResponder* CurrentSelectedResponder = nullptr;

    IDetailLayoutBuilder* CachedDetailBuilder = nullptr;
    ASecurityManager* SecurityManager = nullptr;


    //TMap<FString, TArray<TWeakObjectPtr<UResponder>>> DetectorResponders;
};
