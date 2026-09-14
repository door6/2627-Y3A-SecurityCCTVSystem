// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Modules/ModuleInterface.h"

class SECURITYCCTVSYSTEM_API FSecuritySystemEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};