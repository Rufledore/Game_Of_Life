; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

#define MyAppName="Simple Epidemic Simulation"
#define MyAppExeName="Simple-Epidemic-Simulator.exe"
#define BuildDir="..\..\builds\release\release"                      ;Change BuildDir to point to the directory where the release is on the current PC
#define CurrentVersion GetFileVersion(BuildDir + "\" + MyAppExeName) ;The version is get from the built EXE file

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppName} {#CurrentVersion}
AppPublisher=Ivan Petrov
AppPublisherURL=http://www.ivanpetrov.me
AppSupportURL=http://www.ivanpetrov.me
AppUpdatesURL=http://www.ivanpetrov.me
AppVersion={#CurrentVersion}
VersionInfoVersion={#CurrentVersion}
PrivilegesRequired=admin
DefaultDirName={pf}\Simple-Epidemic-Simulator
DisableDirPage=yes
DefaultGroupName=
DisableProgramGroupPage=yes
;LicenseFile=Installer_Resources\license.rtf
;WizardImageFile=Installer_Resources\inno_welcome.bmp
;WizardSmallImageFile=Installer_Resources\inno_banner.bmp
OutputDir=..\
OutputBaseFilename=Simple-Epidemic-Simulator
;UninstallDisplayIcon=MLX.ico

[Files]
Source: "{#BuildDir}\{#MyAppExeName}"; DestDir: "{app}"
Source: "{#BuildDir}\*.dll"; DestDir: "{app}"
Source: "{#BuildDir}\platforms\*.dll"; DestDir: "{app}\platforms"
Source: "{#BuildDir}\styles\*.dll"; DestDir: "{app}\styles"
;Source: "{#BuildDir}\settings.ini"; DestDir: "{app}";
;Source: "{#BuildDir}\multitool.cfg"; DestDir: "{app}";

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]
Name: "{group}\Simple Epidemic Simulator"; Filename: "{app}\Simple-Epidemic-Simulator"; WorkingDir: "{app}"
Name: "{group}\Melexis TPMS\Uninstall Melexis TPMS"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Simple-Epidemic-Simulator.EXE"; Filename: "{app}\Simple-Epidemic-Simulator"; Tasks: desktopicon


