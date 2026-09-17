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

    FReply OnDeleteResponderClicked(FString DetectorName, UResponder* Responder);

    TSharedRef<SWidget> BuildResponderDropdown(AActor* DetectorActor, UResponder* SeelectedResponder);
    FReply OnAddResponderClicked(FString DetectorName, UResponder* Responder);
    TArray<AActor*> GetResponderCandidates() const;
    void OnResponderSelected(AActor* NewResponder, UResponder* SelectedResponder);
    void OnResponderSelectedTest(AActor* DetectorActor, AActor* Responder);

private:

    UResponder* CurrentSelectedResponder = nullptr;

    IDetailLayoutBuilder* CachedDetailBuilder = nullptr;
    //ASecurityManager* SecurityManager = nullptr;
    USecurityManagerSubsystem* SecurityManager = nullptr;

};
