set project_name=Sqlite
set uproject_path="%~dp0%project_name%.uproject"
set tag=BP_Release_V3132_338
rem Please input the tag of pak release
set /p tag=Please input a tag: [%tag%]

rem Get the current EngineAssociation value
for /f "usebackq tokens=1*" %%A in (%uproject_path%) do (
    echo %%A | findstr "EngineAssociation" >nul && (
        rem echo %%A %%B
        set engine_association=%%B
    )
)
set engine_association=%engine_association:~1,-2%
::echo engine association: %engine_association%

rem Get the engine path
set reg_path=HKEY_USERS
for /f "usebackq tokens=1,3" %%A in (`REG QUERY %reg_path% /t REG_SZ /s /v %engine_association%`) do (
    echo %%A | findstr %engine_association% >nul && (
        rem echo %%A engine association %%B
        set engine_path=%%B
    )
)
echo engine path: %engine_path%

rem Get the output path
::for %%d in (%~dp0..) do set output_path=%%~fd\output
set output_path=%~dp0PAKs\%tag%
set log_path=%~dp0log
echo output path: %output_path%

rem Build Source
%engine_path%\Engine\Binaries\DotNET\UnrealBuildTool.exe UnrealPak Win64 Development -Project=%uproject_path%  %uproject_path% -NoUBTMakefiles  -Manifest=%engine_path%\Engine\Intermediate\Build\Manifest.xml -NoHotReload -log=%log_path%\UnrealPak-Win64-%tag%.log
%engine_path%\Engine\Binaries\DotNET\UnrealBuildTool.exe %project_name% Android Development -Project=%uproject_path%  %uproject_path% -NoUBTMakefiles  -remoteini=%uproject_path% -skipdeploy -Manifest=%engine_path%\Engine\Intermediate\Build\Manifest.xml -NoHotReload -log=%log_path%\%project_name%-Android-%tag%.log
%engine_path%\Engine\Binaries\DotNET\UnrealBuildTool.exe %project_name% Win64 Development -Project=%uproject_path%  %uproject_path% -NoUBTMakefiles  -remoteini=%uproject_path% -skipdeploy -Manifest=%engine_path%\Engine\Intermediate\Build\Manifest.xml -NoHotReload -log=%log_path%\%project_name%-Win64--%tag%.log
%engine_path%\Engine\Binaries\DotNET\UnrealBuildTool.exe %project_name%Server Linux Development -Project=%uproject_path%  %uproject_path% -NoUBTMakefiles  -remoteini=%uproject_path% -skipdeploy -Manifest=%engine_path%\Engine\Intermediate\Build\Manifest.xml -NoHotReload -log=%log_path%\%project_name%-Linux-%tag%.log


rem Cook  Android+Linux+Win
%engine_path%\Engine\Binaries\Win64\UE4Editor-Cmd.exe %uproject_path% -run=Cook -TargetPlatform=Android_ETC1+LinuxServer+WindowsNoEditor -fileopenlog -unversioned -abslog=%log_path%\Cook-Android-Linux-WindowsNoEditor-%tag%.log -stdout -CrashForUAT -unattended -NoLogTimes  -UTF8Output
rem Build cloudia comps pak
rem Blueprint
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\Blueprint\Blueprint_And_%tag%.pak -Create=%~dp0Saved\Cooked\Android_ETC1\Sqlite\Content\Blueprint\ -compress
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\Blueprint\Blueprint_Lin_%tag%.pak -Create=%~dp0Saved\Cooked\LinuxServer\Sqlite\Content\Blueprint\ -compress
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\Blueprint\Blueprint_Win_%tag%.pak -Create=%~dp0Saved\Cooked\WindowsNoEditor\Sqlite\Content\Blueprint\ -compress

rem TestMapPak
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\TestMapPak\TestMapPak_And_%tag%.pak -Create=%~dp0Saved\Cooked\Android_ETC1\Sqlite\Content\TestMapPak\ -compress
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\TestMapPak\TestMapPak_Lin_%tag%.pak -Create=%~dp0Saved\Cooked\LinuxServer\Sqlite\Content\TestMapPak\ -compress
%engine_path%\Engine\Binaries\Win64\UnrealPak.exe %output_path%\TestMapPak\TestMapPak_Win_%tag%.pak -Create=%~dp0Saved\Cooked\WindowsNoEditor\Sqlite\Content\TestMapPak\ -compress