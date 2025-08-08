[Setup]
AppName=HumanAimbot
AppVersion=1.0
DefaultDirName={commonpf}\HumanAimbot
DefaultGroupName=HumanAimbot
OutputDir=dist
OutputBaseFilename=HumanAimbotInstaller
Compression=lzma
SolidCompression=yes
SetupIconFile="assets\images\human_aimbot_profile_pic.ico"

[Files]
Source: "dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "assets\images\human_aimbot_profile_pic.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Pixel Engine"; Filename: "{app}\HumanAimbot.exe"; IconFilename: "{app}\human_aimbot_profile_pic.ico"
Name: "{group}\Uninstall Pixel Engine"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\HumanAimbot.exe"; Description: "Launch Pixel Engine"; Flags: nowait postinstall skipifsilent