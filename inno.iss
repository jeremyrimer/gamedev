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
Name: "{group}\Human Aimbot"; Filename: "{app}\HumanAimbot.exe"; IconFilename: "{app}\human_aimbot_profile_pic.ico"
Name: "{group}\Uninstall Human Aimbot"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\HumanAimbot.exe"; Description: "Launch Human Aimbot"; Flags: nowait postinstall skipifsilent