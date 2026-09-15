// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManagerInterface.h"

//#include "SecurityManager.h"
#include "DetailLayoutBuilder.h"
#include "Detector.h"
#include "IDetailGroup.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> FSecurityManagerInterface::MakeInstance()
{
    return MakeShareable(new FSecurityManagerInterface);
}

void FSecurityManagerInterface::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    //ShareRecipeKeys();
    // Create a category so this is displayed early in the properties
    //IDetailCategoryBuilder& CraftingCategory = DetailBuilder.EditCategory("Crafting", FText::FromString(TEXT("Crafting")), ECategoryPriority::Important);

    //You can get properties using the detailbuilder
    //MyProperty= DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(MyClass, MyClassPropertyName));
    //TSharedPtr<IPropertyHandle> RecipesProperty = DetailBuilder.GetProperty("Recipes");
    //TSharedRef<FDetailArrayBuilder> RecipesArrayPropertyBuilder = MakeShareable(new FDetailArrayBuilder(RecipesProperty.ToSharedRef()));

    //RecipesArrayPropertyBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FResourceGeneratorDetails::GenerateRecipeArrayElementWidget, &DetailBuilder));
    //CraftingCategory.AddCustomBuilder(RecipesArrayPropertyBuilder, false);





    //DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(ASecurityManager, CooldownTimer));

    CachedDetailBuilder = &DetailBuilder;

    IDetailCategoryBuilder& Connections = DetailBuilder.EditCategory(TEXT("Security System"));

    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

    ASecurityManager* CurrentSecurityManager = Cast<ASecurityManager>(SelectedObjects[0].Get());
    if (!CurrentSecurityManager) return;
    SecurityManager = CurrentSecurityManager;

    // Find every actor in the world with a DetectorComponent
    for (TObjectIterator<AActor> It; It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor->FindComponentByClass<UDetector>())
            continue;

        IDetailGroup& Group = Connections.AddGroup(*Actor->GetName(), FText::FromString(Actor->GetActorLabel()));

        FDetectorDelegate* DetectorDelegate = SecurityManager->GetDetectorDelegate(Actor->GetName());
        //TArray<FWeakObjectPtr*> Responders = DetectorDelegate->GetAllObjectRefsEvenIfUnreachable();
        TArray<FWeakObjectPtr*> Responders = DetectorDelegate ? DetectorDelegate->GetAllObjectRefsEvenIfUnreachable() : TArray<FWeakObjectPtr*>();
        //int32 NumResponders = DetectorDelegate ? Responders.Num() : 0;

        for (FWeakObjectPtr* Responder : Responders)
        //for (int32 i = 0; i < NumResponders; ++i)
        {
            Group.AddWidgetRow()
                .NameContent()
                [
                    //SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("Responder %d"), i)))                  
                    SNew(STextBlock).Text(FText::FromString(Cast<UActorComponent>(Responder->Get())->GetOwner()->GetActorLabel()))
                ]
                .ExtensionContent()
                [
                    //BuildResponderDropdown(Actor, 0) //i
                    SNew(SButton)
                        //.Text(FText::FromString("Delete"))
                        .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnDeleteResponderClicked, Actor->GetName(), Responder))
                        .ContentPadding(FMargin(1.f))
                        [
                            SNew(SImage)
                                .Image(FAppStyle::GetBrush("Icons.Delete"))
                        ]
                    
                ];
        }

        // "Add Responder" button per detector
        //UResponder* SelectedResponder = nullptr;

        Group.AddWidgetRow()
            .NameContent()
            [
                //SNew(SButton)
                   // .Text(FText::FromString("Add Responder"))
                   // .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnAddResponderClicked, Actor->GetName(), CurrentSelectedResponder))
                SNew(STextBlock).Text(FText::FromString("Add Responder"))
                
            ]
            .ValueContent()
            [
                BuildResponderDropdown(Actor, CurrentSelectedResponder) //SelectedResponder
            ];
    }
    
}

FReply FSecurityManagerInterface::OnDeleteResponderClicked(FString DetectorName, FWeakObjectPtr* Responder)
{
    UResponder* ResponderComponent = Cast<UResponder>(Responder->Get());
    SecurityManager->RemoveDetectorResponder(DetectorName, *ResponderComponent);

    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();

    return FReply::Handled();
}

TSharedRef<SWidget> FSecurityManagerInterface::BuildResponderDropdown(AActor* DetectorActor, UResponder* SelectedResponder)
{
    //return FMenuBuilder(true, nullptr).MakeWidget();


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
                            //this, &FSecurityManagerInterface::OnResponderSelected,
                            //Candidate, CurrentSelectedResponder))
                            this, &FSecurityManagerInterface::OnResponderSelectedTest,
                            DetectorActor->GetName(), Candidate))
                    );
                }
                return MenuBuilder.MakeWidget();
            })
        .ButtonContent()
        [
            SNew(STextBlock)
                .Text_Lambda([this, SelectedResponder]()
                    {
                        //return GetResponderDisplayText(DetectorActor, ResponderIndex);
                        return CurrentSelectedResponder ? FText::FromString(CurrentSelectedResponder->GetOwner()->GetName()) : FText::FromString("None");
                    })
        ];
}

FReply FSecurityManagerInterface::OnAddResponderClicked(FString DetectorName, UResponder* Responder)
{
    // not sure if this check is needed
    //ASecurityManager* SecurityManager = Cast<ASecurityManager>(electedObjects[0].Get());
    //if (!SecurityManager) return FReply::Handled();

    if (!Responder)
    {
        UE_LOG(LogTemp, Warning, TEXT("Responder NULLPTR"));
        return FReply::Handled();
    }

    SecurityManager->BindResponderToDetector(DetectorName, *Responder);

    UE_LOG(LogTemp, Warning, TEXT("Change Responder NULLPTR"));
    CurrentSelectedResponder = nullptr;

    // Adding a row changes the layout structure -> full rebuild required
    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();

    return FReply::Handled();
}

TArray<AActor*> FSecurityManagerInterface::GetResponderCandidates() const
{
    //return TArray<AActor*>();

    //dont think i need this
    //TArray<AActor*> Result;
    //if (!SelectedObjects.IsValidIndex(0)) return Result;

    TArray<AActor*> Responders;
    //UWorld* World = SecurityManager ? SecurityManager->GetWorld() : nullptr;
    //if (!World) return Result;

    for (TObjectIterator<AActor> It; It; ++It)
    {
        if (It->FindComponentByClass<UResponder>())
            Responders.Add(*It);
    }
    return Responders;
}

void FSecurityManagerInterface::OnResponderSelected(AActor* NewResponder, UResponder* SelectedResponder)
{
    UE_LOG(LogTemp, Warning, TEXT("Responder Selected"));

    SelectedResponder = NewResponder->GetComponentByClass<UResponder>();

    if (NewResponder)
        UE_LOG(LogTemp, Warning, TEXT("Responder NULLPTR"));


}

void FSecurityManagerInterface::OnResponderSelectedTest(FString DetectorName, AActor* Responder)
{
    if (!Responder)
    {
        UE_LOG(LogTemp, Warning, TEXT("Responder NULLPTR"));
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("%s"), *DetectorName);

    SecurityManager->BindResponderToDetector(DetectorName, *Responder->GetComponentByClass<UResponder>());

    UE_LOG(LogTemp, Warning, TEXT("Change Responder NULLPTR"));
    CurrentSelectedResponder = nullptr;

    // Adding a row changes the layout structure -> full rebuild required
    if (CachedDetailBuilder)
        CachedDetailBuilder->ForceRefreshDetails();
}
