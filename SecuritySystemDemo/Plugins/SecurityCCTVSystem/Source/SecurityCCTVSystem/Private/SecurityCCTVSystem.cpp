// Copyright Epic Games, Inc. All Rights Reserved.

#include "SecurityCCTVSystem.h"

#include "PropertyEditorModule.h"
#include "SecurityManager.h"
#include "SecurityManagerInterface.h"
#include "SecuritySystemLog.h"

#define LOCTEXT_NAMESPACE "FSecurityCCTVSystemModule"

void FSecurityCCTVSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		USecurityManagerSubsystem::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSecurityManagerInterface::MakeInstance)
	);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		"SecuritySystemPanel",
		FOnSpawnTab::CreateStatic(&FSecurityCCTVSystemModule::SpawnSecuritySystemTab))
		.SetDisplayName(FText::FromString("Security Manager"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
}

void FSecurityCCTVSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("SecuritySystemPanel");

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(USecurityManagerSubsystem::StaticClass()->GetFName());
	}
}

TSharedRef<SDockTab> FSecurityCCTVSystemModule::SpawnSecuritySystemTab(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	TSharedRef<IDetailsView> DetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);

	// Pull the subsystem from the currently active editor world
	UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	USecurityManagerSubsystem* Subsystem = EditorWorld ? EditorWorld->GetSubsystem<USecurityManagerSubsystem>() : nullptr;

	if (Subsystem)
	{
		DetailsView->SetObject(Subsystem);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			DetailsView
		];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSecurityCCTVSystemModule, SecurityCCTVSystem)

DEFINE_LOG_CATEGORY(LogSecuritySystem);