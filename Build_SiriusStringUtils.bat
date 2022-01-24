@echo off

set PLUGIN="%~dp0Plugins\SiriusStringUtils\SiriusStringUtils.uplugin"
set TARGETPLATFORMS="Win32+Win64+Linux+LinuxAArch64+Android"

call "D:\EpicLibrary\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin ^
-Plugin=%PLUGIN% ^
-Package="D:\PluginBuilds\SiriusStringUtils-4.27" ^
-NoHostPlatform -TargetPlatforms=%TARGETPLATFORMS% ^
-StrictIncludes

call "D:\EpicLibrary\UE_4.26\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin ^
-Plugin=%PLUGIN% ^
-Package="D:\PluginBuilds\SiriusStringUtils-4.26" ^
-NoHostPlatform -TargetPlatforms=%TARGETPLATFORMS% ^
-StrictIncludes

call "D:\EpicLibrary\UE_4.25\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin ^
-Plugin=%PLUGIN% ^
-Package="D:\PluginBuilds\SiriusStringUtils-4.25" ^
-NoHostPlatform -TargetPlatforms=%TARGETPLATFORMS% ^
-StrictIncludes