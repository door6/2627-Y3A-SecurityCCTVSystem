// Copyright Epic Games, Inc. All Rights Reserved.

#include "SecurityCCTVSystem.h"

#include "PropertyEditorModule.h"
#include "SecurityManager.h"
#include "SecurityManagerInterface.h"
#include "Modules/ModuleManager.h"
#include "SecuritySystemLog.h"

#define LOCTEXT_NAMESPACE "FSecurityCCTVSystemModule"

void FSecurityCCTVSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		ASecurityManager::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSecurityManagerInterface::MakeInstance)
	);
}

void FSecurityCCTVSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule =
			FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(ASecurityManager::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSecurityCCTVSystemModule, SecurityCCTVSystem)

DEFINE_LOG_CATEGORY(SecuritySystem);