// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

using UnrealBuildTool;

public class SiriusUtilityNodes : ModuleRules
{
	public SiriusUtilityNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}