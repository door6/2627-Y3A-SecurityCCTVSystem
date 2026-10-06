// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class USecurityManagerSubsystem;

class SECURITYCCTVSYSTEM_API FSecurityManagerInterface : public IDetailCustomization
{
public:

    static TSharedRef<IDetailCustomization> MakeInstance();
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    FReply OnDeleteResponderClicked(FString DetectorName, AActor* ResponderActor, IDetailLayoutBuilder* DetailBuilder);
    TSharedRef<SWidget> BuildRespondersDropdown(AActor* DetectorActor, IDetailLayoutBuilder* DetailBuilder);
    TArray<AActor*> GetResponderCandidates() const;
    void OnResponderSelected(AActor* DetectorActor, AActor* ResponderActor, IDetailLayoutBuilder* DetailBuilder);

    TWeakObjectPtr<USecurityManagerSubsystem> SecurityManager;
};
