// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SecurityCCTVSystem/Public/PrintScreen.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePrintScreen() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
SECURITYCCTVSYSTEM_API UClass* Z_Construct_UClass_UPrintScreen();
SECURITYCCTVSYSTEM_API UClass* Z_Construct_UClass_UPrintScreen_NoRegister();
UPackage* Z_Construct_UPackage__Script_SecurityCCTVSystem();
// End Cross Module References

// Begin Class UPrintScreen
void UPrintScreen::StaticRegisterNativesUPrintScreen()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPrintScreen);
UClass* Z_Construct_UClass_UPrintScreen_NoRegister()
{
	return UPrintScreen::StaticClass();
}
struct Z_Construct_UClass_UPrintScreen_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "PrintScreen.h" },
		{ "ModuleRelativePath", "Public/PrintScreen.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPrintScreen>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UPrintScreen_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_SecurityCCTVSystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPrintScreen_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UPrintScreen_Statics::ClassParams = {
	&UPrintScreen::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UPrintScreen_Statics::Class_MetaDataParams), Z_Construct_UClass_UPrintScreen_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UPrintScreen()
{
	if (!Z_Registration_Info_UClass_UPrintScreen.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPrintScreen.OuterSingleton, Z_Construct_UClass_UPrintScreen_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UPrintScreen.OuterSingleton;
}
template<> SECURITYCCTVSYSTEM_API UClass* StaticClass<UPrintScreen>()
{
	return UPrintScreen::StaticClass();
}
UPrintScreen::UPrintScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UPrintScreen);
UPrintScreen::~UPrintScreen() {}
// End Class UPrintScreen

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_doras_Documents_GitHub_Year3_BlockA_2627_Y3A_SecurityCCTVSystem_SecuritySystemDemo_Plugins_SecurityCCTVSystem_Source_SecurityCCTVSystem_Public_PrintScreen_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UPrintScreen, UPrintScreen::StaticClass, TEXT("UPrintScreen"), &Z_Registration_Info_UClass_UPrintScreen, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPrintScreen), 281679379U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_doras_Documents_GitHub_Year3_BlockA_2627_Y3A_SecurityCCTVSystem_SecuritySystemDemo_Plugins_SecurityCCTVSystem_Source_SecurityCCTVSystem_Public_PrintScreen_h_2189122586(TEXT("/Script/SecurityCCTVSystem"),
	Z_CompiledInDeferFile_FID_Users_doras_Documents_GitHub_Year3_BlockA_2627_Y3A_SecurityCCTVSystem_SecuritySystemDemo_Plugins_SecurityCCTVSystem_Source_SecurityCCTVSystem_Public_PrintScreen_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_doras_Documents_GitHub_Year3_BlockA_2627_Y3A_SecurityCCTVSystem_SecuritySystemDemo_Plugins_SecurityCCTVSystem_Source_SecurityCCTVSystem_Public_PrintScreen_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
