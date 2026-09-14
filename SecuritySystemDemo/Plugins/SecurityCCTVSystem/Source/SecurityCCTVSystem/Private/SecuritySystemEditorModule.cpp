// Fill out your copyright notice in the Description page of Project Settings.


#include "SecuritySystemEditorModule.h"
#include "PropertyEditorModule.h"
#include "SecurityManager.h"
#include "SecurityManagerInterface.h"
#include "Modules/ModuleManager.h"

void FSecuritySystemEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.RegisterCustomClassLayout(
        ASecurityManager::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FSecurityManagerInterface::MakeInstance)
    );
}

void FSecuritySystemEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule =
            FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(ASecurityManager::StaticClass()->GetFName());
    }
}

//IMPLEMENT_MODULE(FSecuritySystemEditorModule, SecurityCCTVSystem)
