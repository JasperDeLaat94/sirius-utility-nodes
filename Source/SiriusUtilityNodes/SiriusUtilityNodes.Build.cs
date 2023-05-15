// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

using UnrealBuildTool;

public class SiriusUtilityNodes : ModuleRules
{
	public SiriusUtilityNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
#if UE_5_2_OR_LATER
		IWYUSupport = IWYUSupport.Full;
#else
		bEnforceIWYU = true;
#endif
		
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