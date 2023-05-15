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
    $TARGETPLATFORMS="Win64+Linux+LinuxArm64+Android"
    
    & "$EngineRoot\Engine\Build\BatchFiles\RunUAT.bat" `
    "BuildPlugin" `
    "-Plugin=$PLUGIN" `
    "-Package=D:\PluginBuilds\SiriusUtilityNodes-$Version" `
    "-TargetPlatforms=$TARGETPLATFORMS" `
    "-StrictIncludes"
}

# Build the plugin for the last three supported engine versions.
# Make sure to use the appropriate linux toolchain for each version.
$env:LINUX_MULTIARCH_ROOT = 'C:\UnrealToolchains\v20_clang-13.0.1-centos7\'
Build-Plugin '5.0'
Build-Plugin '5.1'

$env:LINUX_MULTIARCH_ROOT = 'C:\UnrealToolchains\v21_clang-15.0.1-centos7\'
Build-Plugin '5.2'