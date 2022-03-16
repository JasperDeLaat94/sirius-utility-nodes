// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#include "SiriusUtilityNodesEditor.h"

#include "K2Node_SiriusFormatString.h"
#include "PropertyEditorModule.h"
#include "Details/FormatStringDetails.h"

IMPLEMENT_MODULE(FSiriusUtilityNodesEditorModule, SiriusUtilityNodesEditor)

void FSiriusUtilityNodesEditorModule::StartupModule()
{
	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(UK2Node_SiriusFormatString::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FFormatStringDetails::MakeInstance));
}

void FSiriusUtilityNodesEditorModule::ShutdownModule()
{
	// Unregister the details customization
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(UK2Node_SiriusFormatString::StaticClass()->GetFName());
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}
