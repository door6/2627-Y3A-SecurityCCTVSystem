// Fill out your copyright notice in the Description page of Project Settings.

#include "SecurityManagerInterface.h"
#include "DetailLayoutBuilder.h"
#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SecurityManager.h"
#include "Detector.h"
#include "Responder.h"
#include "SecuritySystemLog.h"

TSharedRef<IDetailCustomization> FSecurityManagerInterface::MakeInstance()
{
    return MakeShareable(new FSecurityManagerInterface);
}

void FSecurityManagerInterface::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // get security manager
    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);
    SecurityManager = Cast<USecurityManagerSubsystem>(SelectedObjects[0].Get());
    if (!SecurityManager.IsValid()) 
        return;

    UWorld* World = SecurityManager->GetWorld();
    if (!World) 
        return;

    IDetailCategoryBuilder& Connections = DetailBuilder.EditCategory(TEXT("Security System"));

    // find every actor in the world with a detector component
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        // get detector
        AActor* DetectorActor = *It;
        if (!DetectorActor->FindComponentByClass<UDetectorComponent>())
            continue;

        // create detector group
        FString DetectorName = DetectorActor->GetActorLabel();
        IDetailGroup& Group = Connections.AddGroup(*DetectorName, FText::FromString(DetectorName));

        // get all responders with that detector tag
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(World, FName(DetectorName), FoundActors);
        for (AActor* ResponderActor : FoundActors)
        {
            Group.AddWidgetRow()
                .NameContent()
                [
                    SNew(STextBlock).Text(FText::FromString(ResponderActor->GetActorLabel()))
                ]
                .ExtensionContent()
                [
                    SNew(SButton)
                        .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnDeleteResponderClicked, DetectorName, ResponderActor, &DetailBuilder))
                        .ContentPadding(FMargin(1.f))
                        [
                            SNew(SImage)
                                .Image(FAppStyle::GetBrush("Icons.Delete"))
                        ]
                ];
        }

        Group.AddWidgetRow()
            .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString("Add Responder"))
                
            ]
            .ValueContent()
            [
                BuildRespondersDropdown(DetectorActor, World, &DetailBuilder)
            ];
    }  
}

FReply FSecurityManagerInterface::OnDeleteResponderClicked(FString DetectorName, AActor* ResponderActor, IDetailLayoutBuilder* DetailBuilder)
{
    ResponderActor->Modify();
    ResponderActor->Tags.Remove(FName(DetectorName));

    UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *ResponderActor->GetActorLabel(), *DetectorName);

    // update panel to match removing of a responder
    if (DetailBuilder)
        DetailBuilder->ForceRefreshDetails();

    return FReply::Handled();
}

TSharedRef<SWidget> FSecurityManagerInterface::BuildRespondersDropdown(AActor* DetectorActor, UWorld* World, IDetailLayoutBuilder* DetailBuilder)
{
    return SNew(SComboButton)
        .OnGetMenuContent_Lambda([this, DetectorActor, World, DetailBuilder]() -> TSharedRef<SWidget>
            {
                FMenuBuilder MenuBuilder(true, nullptr);
                TArray<AActor*> ResponderCandidates = GetResponderCandidates(World);
                // fill dropdown with all actors with responder component
                for (AActor* Candidate : ResponderCandidates)
                {
                    MenuBuilder.AddMenuEntry(
                        FText::FromString(Candidate->GetActorLabel()),
                        FText::GetEmpty(),
                        FSlateIcon(),
                        FUIAction(FExecuteAction::CreateSP(this, &FSecurityManagerInterface::OnResponderSelected, DetectorActor, Candidate, DetailBuilder))
                    );
                }
                return MenuBuilder.MakeWidget();
            })
        .ButtonContent()
        [
            SNew(STextBlock)
                .Text_Lambda([this]()
                    {
                        return  FText::FromString("None");
                    })
        ];
}

TArray<AActor*> FSecurityManagerInterface::GetResponderCandidates(UWorld* World) const
{
    TArray<AActor*> Responders;

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        if (It->FindComponentByClass<UResponderComponent>())
            Responders.Add(*It);
    }
    return Responders;
}

void FSecurityManagerInterface::OnResponderSelected(AActor* DetectorActor, AActor* ResponderActor, IDetailLayoutBuilder* DetailBuilder)
{
    if (!ResponderActor)
    {
        UE_LOG(LogSecuritySystem, Error, TEXT("Responder is NULLPTR"));
        return;
    }

    ResponderActor->Modify();
    ResponderActor->Tags.AddUnique(FName(DetectorActor->GetActorLabel()));

    UE_LOG(LogSecuritySystem, Log, TEXT("Bind %s to %s"), *ResponderActor->GetActorLabel(), *DetectorActor->GetActorLabel());

    // rebuild panel so the bound responder gets added
    if (DetailBuilder)
        DetailBuilder->ForceRefreshDetails();
}
