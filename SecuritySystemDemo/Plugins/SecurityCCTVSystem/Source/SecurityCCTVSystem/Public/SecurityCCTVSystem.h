// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"

class FSecurityCCTVSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static TSharedRef<SDockTab> SpawnSecuritySystemTab(const FSpawnTabArgs& Args);
};
