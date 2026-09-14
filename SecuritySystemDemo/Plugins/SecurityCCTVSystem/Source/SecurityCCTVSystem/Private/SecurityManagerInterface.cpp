// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityManagerInterface.h"

#include "SecurityManager.h"
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

    IDetailCategoryBuilder& Connections = DetailBuilder.EditCategory(TEXT("Security System"));

    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

    ASecurityManager* SecurityManager = Cast<ASecurityManager>(SelectedObjects[0].Get());
    if (!SecurityManager) return;

    // Find every actor in the world with a DetectorComponent
    for (TObjectIterator<AActor> It; It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor->FindComponentByClass<UDetector>())
            continue;

        IDetailGroup& Group = Connections.AddGroup(*Actor->GetName(), FText::FromString(Actor->GetActorLabel()));

        FDetectorDelegate* DetectorDelegate = SecurityManager->DetectorDelegates.Find(Actor->GetName());
        int32 NumResponders = DetectorDelegate ? DetectorDelegate->GetAllObjectRefsEvenIfUnreachable().Num() : 0;

        for (int32 i = 0; i < NumResponders; ++i)
        {
            Group.AddWidgetRow()
                .NameContent()
                [
                    SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("Responder %d"), i)))
                ]
                .ValueContent()
                [
                    BuildResponderDropdown(Actor, i)
                ];
        }

        // "Add Responder" button per detector
        Group.AddWidgetRow()
            .WholeRowContent()
            [
                SNew(SButton)
                    .Text(FText::FromString("Add Responder"))
                    .OnClicked(FOnClicked::CreateSP(this, &FSecurityManagerInterface::OnAddResponderClicked, Actor))
            ];
    }
    
}

TSharedRef<SWidget> FSecurityManagerInterface::BuildResponderDropdown(AActor* DetectorActor, int32 ResponderIndex)
{
    return FMenuBuilder(true, nullptr).MakeWidget();
}

FReply FSecurityManagerInterface::OnAddResponderClicked(AActor* DetectorActor)
{
    return FReply::Handled();
}
