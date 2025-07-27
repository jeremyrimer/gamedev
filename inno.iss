[Setup]
AppName=PixelEngine
AppVersion=1.0
DefaultDirName={commonpf}\PixelEngine
DefaultGroupName=PixelEngine
OutputDir=dist
OutputBaseFilename=PixelEngineInstaller
Compression=lzma
SolidCompression=yes
SetupIconFile="assets\human_aimbot_profile_pic.ico"

[Files]
Source: "dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "assets\human_aimbot_profile_pic.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Pixel Engine"; Filename: "{app}\PixelEngine.exe"; IconFilename: "{app}\human_aimbot_profile_pic.ico"
Name: "{group}\Uninstall Pixel Engine"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\PixelEngine.exe"; Description: "Launch Pixel Engine"; Flags: nowait postinstall skipifsilent