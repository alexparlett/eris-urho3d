@ECHO OFF
cd %1

Tools\PackageTool.exe "Assets\CoreData" "Game\00.pak" -c
xcopy "Assets\Data" "Game\Data" /Y /I /E /D
xcopy "Assets\Schema" "Game\Data" /Y /I /E /D
xcopy "Assets\Mods" "Game\Mods" /Y /I /E /D
xcopy "Assets\icon.ico" "Game" /Y /D
xcopy "Assets\settings.xml" "Game" /Y /D
xcopy "Assets\bindings.xml" "Game" /Y /D