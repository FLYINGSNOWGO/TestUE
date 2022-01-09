@echo off
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

rem Build Apk
set CookLevel = 0
echo 0:skipcook(default) 1:iterativecooking 2:cook
set /p CookLevel=Please input a CookLevel:[%CookLevel%]
set CookParam=skipcook
echo CookLevel:%CookLevel%
if %CookLevel% == 1 (
    CookParam=iterativecooking
)^
else if %CookLevel% == 2 (
    CookParam=cook
)
%engine_path%/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -nocompileeditor -nop4 -project=%uproject_path% -%CookParam% -stage -archive -archivedirectory=%output_path% -package -clientconfig=Development -ue4exe=%engine_path%/Engine/Binaries/Win64/UE4Editor-Cmd.exe -compressed -pak -prereqs -targetplatform=Android -cookflavor=ASTC -build -utf8output -abslog=%log_path%\Build-Apk-%tag%.log
pause