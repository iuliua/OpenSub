; The name of the installer
Name "OpenSubtitles Agent"

; The file to write
OutFile "OSubAgent.exe"

; The default installation directory
InstallDir $PROGRAMFILES\OSubAgent

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

;Page components
;Page directory
;Page instfiles

;UninstPage uninstConfirm
;UninstPage instfiles

;--------------------------------
; The stuff to install
Section "Install"
  
 SetRegView 64
  ; Remove registry keys
  DeleteRegKey HKCR '*\shell\OpenSubtitles.org'
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent"
  ; Remove files and uninstaller
  Delete $INSTDIR\OpenSub.exe
  Delete $INSTDIR\uninstall.exe
  
  RMDir "$INSTDIR"

  SetOutPath $INSTDIR
  File "OpenSub.exe"
  SetRegView 64
  WriteRegStr HKCR '*\shell\OpenSubtitles.org\command' "" '"$INSTDIR\OpenSub.exe" %1'
  WriteRegStr HKCR '*\shell\OpenSubtitles.org' "Icon" "$INSTDIR\OpenSub.exe"
  
  
  ; uninstall
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent" "DisplayName" "OSubAgent"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent" "NoModify" 1
  WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent" "NoRepair" 1
  
  WriteUninstaller "uninstall.exe"
  
SectionEnd
;--------------------------------

; Uninstaller

Section "Uninstall"
  SetRegView 64
  ; Remove registry keys
  DeleteRegKey HKCR '*\shell\OpenSubtitles.org'
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OSubAgent"
  ; Remove files and uninstaller
  Delete $INSTDIR\OpenSub.exe
  Delete $INSTDIR\uninstall.exe
  
  RMDir "$INSTDIR"

SectionEnd
