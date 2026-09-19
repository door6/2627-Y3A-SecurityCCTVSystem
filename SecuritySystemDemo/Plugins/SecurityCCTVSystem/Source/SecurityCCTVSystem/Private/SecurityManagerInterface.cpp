// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManagerInterface.h"

//#include "SecurityManager.h"
#include "DetailLayoutBuilder.h"
#include "Detector.h"
#include "IDetailGroup.h"
#include "DetailWidgetRow.h"
#include "SecuritySystemLog.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h" //temp

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

    //UE_LOG(LogSecuritySystem, Log, TEXT("Number of detectors in manager map = %d"), SecurityManager->DetectorResponders.Num());
    //UE_LOG(LogSecuritySystem, Log, TEXT("Number of detectors in manager map = %d"), SecurityManager->DetectorDelegates.Num());

    // Find every actor in the world with a DetectorComponent
    for (TActorIterator<AActor> It(SecurityManager->GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        UDetector* Detector = Actor->FindComponentByClass<UDetector>();
        //if (!Actor->FindComponentByClass<UDetector>())
        if (!Detector)
            continue;

        //temp for testing
        /*if (!Actor->FindComponentByClass<UDetector>()->IsBound)
        {
            UE_LOG(LogSecuritySystem, Error, TEXT("Editor: Manager is NOT bound to %s"), *Actor->GetActorLabel());
        }
        else
            UE_LOG(LogSecuritySystem, Log, TEXT("Editor: Manager is bound to %s"), *Actor->GetActorLabel());*/


        IDetailGroup& Group = Connections.AddGroup(*Actor->GetName(), FText::FromString(Actor->GetActorLabel()));

        //FResponders* Responders = SecurityManager->DetectorResponders.Find(Actor->GetName());
        //FDetectorDelegate* DetectorDelegate = SecurityManager->DetectorDelegates.Find(Actor->GetName());

        //TArray<FString> RespondersArray = Responders ? Responders->NameArray : TArray<FString>();
        //TArray<FWeakObjectPtr*> Responders = DetectorDelegate ? DetectorDelegate->GetAllObjectRefsEvenIfUnreachable() : TArray<FWeakObjectPtr*>();

        /*TArray<UResponder*>* Responders = DropdownMap.Find(Actor->GetActorLabel());
        if (!Responders)
            UE_LOG(LogSecuritySystem, Warning, TEXT("No responders"));
        TArray<UResponder*> RespondersArray = Responders ? *Responders : TArray<UResponder*>();

        UE_LOG(LogSecuritySystem, Warning, TEXT("num: %d"), DropdownMap.Num());*/

        //UE_LOG(LogSecuritySystem, Log, TEXT("%s: ResponderNum = %d"), Actor->GetActorLabel(), Detector->ConnectedResponders.Num());

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(Actor->GetWorld(), FName(Actor->GetActorLabel()), FoundActors);


        //for (FWeakObjectPtr* Responder : Responders)
        //for (FString ResponderName : RespondersArray)
        //for (UResponder* Responder : RespondersArray)
        //for (FString ResponderName : Detector->ConnectedResponders)
        for (AActor* ResponderActor : FoundActors)
        {
            //UResponder* ResponderComponent = Cast<UResponder>(Responder->Get());

            Group.AddWidgetRow()
                .NameContent()
                [
                    //SNew(STextBlock).Text(FText::FromString(ResponderComponent->GetOwner()->GetActorLabel()))
                    //SNew(STextBlock).Text(FText::FromString(ResponderName))
                    SNew(STextBlock).Text(FText::FromString(ResponderActor->GetActorLabel()))
                ]
                .ExtensionContent()
                [
                    SNew(SButton)
                        .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnDeleteResponderClicked, Actor->GetActorLabel(), ResponderActor)) //ResponderComponent)
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

FReply FSecurityManagerInterface::OnDeleteResponderClicked(FString DetectorName, AActor* ResponderActor)
{
    //SecurityManager->Modify();

    /*SecurityManager->RemoveDetectorResponder(DetectorName, ResponderName); //*Responder

    SecurityManager->SaveConfig();*/

    /*DropdownMap[DetectorName].Remove(Responder);

    Responder->Modify();
    Responder->ConnectedDetectors.Remove(DetectorName);*/

    /*Detector->Modify();
    Detector->ConnectedResponders.Remove(ResponderName);*/

    ResponderActor->Modify();
    ResponderActor->Tags.Remove(FName(DetectorName));

    UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *ResponderActor->GetActorLabel(), *DetectorName);

    //UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *ResponderName, *Detector->GetOwner()->GetActorLabel());

    //Responder->ConnectedDetector = "";

    //UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *Responder->GetOwner()->GetActorLabel(), *DetectorName);

    //UE_LOG(LogSecuritySystem, Log, TEXT("%s removed from %s"), *ResponderName, *DetectorName);

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

    SecurityManager->BindResponderToDetector(DetectorName, Responder->GetOwner()->GetActorLabel());

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

    //SecurityManager->Modify();

    //SecurityManager->BindResponderToDetector(DetectorActor->GetName(), *Responder->GetComponentByClass<UResponder>());
    /*SecurityManager->BindResponderToDetector(DetectorActor->GetName(), Responder->GetName());

    SecurityManager->TestChanges = true;

    SecurityManager->SaveConfig();*/

    /*TArray<UResponder*>& Responders = DropdownMap.FindOrAdd(DetectorActor->GetActorLabel());
    Responders.AddUnique(Responder->FindComponentByClass<UResponder>());*/


    /*UResponder* ResponderComponent = Responder->FindComponentByClass<UResponder>();
    ResponderComponent->Modify();
    ResponderComponent->ConnectedDetectors.AddUnique(DetectorActor->GetName());*/

    Responder->Modify();
    Responder->Tags.AddUnique(FName(DetectorActor->GetActorLabel()));


    /*UDetector* Detector = DetectorActor->FindComponentByClass<UDetector>();

    Detector->Modify();
    Detector->ConnectedResponders.AddUnique(Responder->GetActorLabel());*/
    //Detector->IsBound = true;

    UE_LOG(LogSecuritySystem, Log, TEXT("Bind %s to %s"), *Responder->GetActorLabel(), *DetectorActor->GetActorLabel());

    CurrentSelectedResponder = nullptr;
    UE_LOG(LogSecuritySystem, Log, TEXT("Change CurrentSelectedResponder to NULLPTR"));

    // Adding a row changes the layout structure -> full rebuild required
    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();
}
