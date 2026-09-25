// Copyright Epic Games, Inc. All Rights Reserved.

#include "SecurityCCTVSystem.h"

#include "PropertyEditorModule.h"
#include "SecurityManager.h"
#include "SecurityManagerInterface.h"
#include "SecuritySystemLog.h"

#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

#if WITH_GAMEPLAY_DEBUGGER_CORE
#include "GameplayDebugger.h"
#endif // WITH_GAMEPLAY_DEBUGGER_CORE

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger_SecuritySystem.h"
#endif // WITH_GAMEPLAY_DEBUGGER

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
		.SetTooltipText(FText::FromString("Security System"))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
		.SetMenuType(ETabSpawnerMenuType::Enabled);

	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("MotionDetector", "Security System", NSLOCTEXT("FSecurityCCTVSystemModule", "SecuritySection", "Security System"));
	Section->AddCategory("Security System");


#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("SecuritySystem", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_SecuritySystem::MakeInstance));
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif // WITH_GAMEPLAY_DEBUGGER
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

		PropertyModule.RemoveSection("MotionDetector", "Security System");
	}

#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("SecuritySystem");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif // WITH_GAMEPLAY_DEBUGGER
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