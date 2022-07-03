# Get the location of the desired Unreal version by querying the registry.
function Get-UnrealEngineLocation {
    param ($Version)

    if (!(Test-Path "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$Version"))
    {
        Write-Error "Unreal Engine $Version not found!"
        return $null
    }

    return Get-ItemPropertyValue -Path "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$Version" -Name InstalledDirectory
}

# Build the plugin for a specific version of Unreal
function Build-Plugin {
    param ($Version)

    $EngineRoot = Get-UnrealEngineLocation $Version
    if ($null -eq $EngineRoot) { exit 1 }
    
    $PLUGIN="$pwd\SiriusUtilityNodes.uplugin"
    $TARGETPLATFORMS="Win32+Win64+Linux+LinuxAArch64+Android"
    
    & "$EngineRoot\Engine\Build\BatchFiles\RunUAT.bat" `
    "BuildPlugin" `
    "-Plugin=$PLUGIN" `
    "-Package=D:\PluginBuilds\SiriusUtilityNodes-$Version" `
    "-TargetPlatforms=$TARGETPLATFORMS" `
    "-StrictIncludes"
}

# Build the plugin for the last three supported engine versions.
# Make sure to use the appropriate linux toolchain for each version.
$env:LINUX_MULTIARCH_ROOT = 'C:\UnrealToolchains\v19_clang-11.0.1-centos7\'
Build-Plugin 4.27
$env:LINUX_MULTIARCH_ROOT = 'C:\UnrealToolchains\v17_clang-10.0.1-centos7\'
Build-Plugin 4.26