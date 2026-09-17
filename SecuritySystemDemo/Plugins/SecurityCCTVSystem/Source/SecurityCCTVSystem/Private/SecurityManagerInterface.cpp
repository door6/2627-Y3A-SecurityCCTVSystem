// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManagerInterface.h"

//#include "SecurityManager.h"
#include "DetailLayoutBuilder.h"
#include "Detector.h"
#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "SecuritySystemLog.h"

#include "EngineUtils.h"
//#include "Kismet/GameplayStatics.h" //temp

TSharedRef<IDetailCustomization> FSecurityManagerInterface::MakeInstance()
{
    return MakeShareable(new FSecurityManagerInterface);
}

void FSecurityManagerInterface::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    CachedDetailBuilder = &DetailBuilder;

    IDetailCategoryBuilder& Connections = DetailBuilder.EditCategory(TEXT("Security System"));

    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

    /*ASecurityManager* CurrentSecurityManager = Cast<ASecurityManager>(SelectedObjects[0].Get());
    if (!CurrentSecurityManager) return;
    SecurityManager = CurrentSecurityManager;*/

    USecurityManagerSubsystem* Subsystem = Cast<USecurityManagerSubsystem>(SelectedObjects[0].Get());
    if (!Subsystem) return;

    //not sure if this is needed
    UWorld* World = Subsystem->GetWorld();
    if (!World) return;

    SecurityManager = Subsystem;

    //temp check
    /*TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(SecurityManager->GetWorld(), SecurityManager->GetClass(), FoundActors);
    UE_LOG(LogSecuritySystem, Warning, TEXT("Number of SecurityManagers = %d"), FoundActors.Num());*/

    UE_LOG(LogSecuritySystem, Log, TEXT("Number of detectors in manager map = %d"), SecurityManager->DetectorDelegates.Num());

    // Find every actor in the world with a DetectorComponent
    for (TActorIterator<AActor> It(SecurityManager->GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor->FindComponentByClass<UDetector>())
            continue;

        //temp for testing
        /*if (!Actor->FindComponentByClass<UDetector>()->IsBound)
        {
            UE_LOG(LogSecuritySystem, Error, TEXT("Editor: Manager is NOT bound to %s"), *Actor->GetActorLabel());
        }
        else
            UE_LOG(LogSecuritySystem, Log, TEXT("Editor: Manager is bound to %s"), *Actor->GetActorLabel());*/


        IDetailGroup& Group = Connections.AddGroup(*Actor->GetName(), FText::FromString(Actor->GetActorLabel()));

        FDetectorDelegate* DetectorDelegate = SecurityManager->DetectorDelegates.Find(Actor->GetName());

        TArray<FWeakObjectPtr*> Responders = DetectorDelegate ? DetectorDelegate->GetAllObjectRefsEvenIfUnreachable() : TArray<FWeakObjectPtr*>();

        for (FWeakObjectPtr* Responder : Responders)
        {
            UResponder* ResponderComponent = Cast<UResponder>(Responder->Get());

            Group.AddWidgetRow()
                .NameContent()
                [
                    SNew(STextBlock).Text(FText::FromString(ResponderComponent->GetOwner()->GetActorLabel()))
                ]
                .ExtensionContent()
                [
                    SNew(SButton)
                        .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnDeleteResponderClicked, Actor->GetName(), ResponderComponent))
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
                BuildResponderDropdown(Actor, CurrentSelectedResponder)
            ];
    }  
}

FReply FSecurityManagerInterface::OnDeleteResponderClicked(FString DetectorName, UResponder* Responder)
{
    SecurityManager->Modify();

    SecurityManager->RemoveDetectorResponder(DetectorName, *Responder);

    Responder->Modify();
    Responder->ConnectedDetector = "";

    UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *Responder->GetOwner()->GetActorLabel(), *DetectorName);

    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();

    return FReply::Handled();
}

TSharedRef<SWidget> FSecurityManagerInterface::BuildResponderDropdown(AActor* DetectorActor, UResponder* SelectedResponder)
{
    return SNew(SComboButton)
        .OnGetMenuContent_Lambda([this, DetectorActor, SelectedResponder]() -> TSharedRef<SWidget>
            {
                FMenuBuilder MenuBuilder(true, nullptr);
                for (AActor* Candidate : GetResponderCandidates())
                {
                    MenuBuilder.AddMenuEntry(
                        FText::FromString(Candidate->GetActorLabel()),
                        FText::GetEmpty(),
                        FSlateIcon(),
                        FUIAction(FExecuteAction::CreateSP(
                            this, &FSecurityManagerInterface::OnResponderSelectedTest,
                            DetectorActor, Candidate))
                    );
                }
                return MenuBuilder.MakeWidget();
            })
        .ButtonContent()
        [
            SNew(STextBlock)
                .Text_Lambda([this, SelectedResponder]()
                    {
                        return CurrentSelectedResponder ? FText::FromString(CurrentSelectedResponder->GetOwner()->GetName()) : FText::FromString("None");
                    })
        ];
}

FReply FSecurityManagerInterface::OnAddResponderClicked(FString DetectorName, UResponder* Responder)
{
    if (!Responder)
    {
        UE_LOG(LogSecuritySystem, Error, TEXT("Responder NULLPTR"));
        return FReply::Handled();
    }

    SecurityManager->BindResponderToDetector(DetectorName, *Responder);

    UE_LOG(LogSecuritySystem, Log, TEXT("Bind %s to %s"), *DetectorName, *Responder->GetOwner()->GetActorLabel());
    CurrentSelectedResponder = nullptr;
    UE_LOG(LogSecuritySystem, Log, TEXT("Change Responder to NULLPTR"));

    // Adding a row changes the layout structure -> full rebuild required
    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();

    return FReply::Handled();
}

TArray<AActor*> FSecurityManagerInterface::GetResponderCandidates() const
{
    TArray<AActor*> Responders;

    for (TObjectIterator<AActor> It; It; ++It)
    {
        if (It->FindComponentByClass<UResponder>())
            Responders.Add(*It);
    }
    return Responders;
}

void FSecurityManagerInterface::OnResponderSelected(AActor* NewResponder, UResponder* SelectedResponder)
{
    UE_LOG(LogSecuritySystem, Log, TEXT("Responder Selected"));

    SelectedResponder = NewResponder->GetComponentByClass<UResponder>();

}

void FSecurityManagerInterface::OnResponderSelectedTest(AActor* DetectorActor, AActor* Responder)
{
    if (!Responder)
    {
        UE_LOG(LogSecuritySystem, Error, TEXT("Responder NULLPTR"));
        return;
    }

    SecurityManager->Modify();

    SecurityManager->BindResponderToDetector(DetectorActor->GetName(), *Responder->GetComponentByClass<UResponder>());

    UResponder* ResponderComponent = Responder->FindComponentByClass<UResponder>();
    ResponderComponent->Modify();
    ResponderComponent->ConnectedDetector = DetectorActor->GetName();


    UDetector* Detector = DetectorActor->FindComponentByClass<UDetector>();

    Detector->Modify();
    Detector->IsBound = true;

    UE_LOG(LogSecuritySystem, Log, TEXT("Bind %s to %s"), *Responder->GetActorLabel(), *DetectorActor->GetActorLabel());

    CurrentSelectedResponder = nullptr;
    UE_LOG(LogSecuritySystem, Log, TEXT("Change CurrentSelectedResponder to NULLPTR"));

    // Adding a row changes the layout structure -> full rebuild required
    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();
}
