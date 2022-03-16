// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

using UnrealBuildTool;

public class SiriusUtilityNodesEditor : ModuleRules
{
	public SiriusUtilityNodesEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"BlueprintGraph",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"EditorStyle",
				"PropertyEditor",
				"SlateCore",
				"Slate",
				"GraphEditor",
				"UnrealEd",
				"KismetCompiler", 
				"SiriusUtilityNodes"
			}
		);
	}
}