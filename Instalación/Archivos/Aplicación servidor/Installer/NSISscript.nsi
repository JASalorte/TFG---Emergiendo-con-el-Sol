# define installer name
OutFile "installer.exe"
 
# set desktop as install directory
InstallDir $PROGRAMFILES\EmergiendoConElSol
 
# default section start
Section
 
;SetShellVarContext current
 
# define output path
SetOutPath $INSTDIR
 
# specify file to go in output path
File /r /x Setup.conf Temp\*

;create desktop shortcut
  CreateShortCut "$DESKTOP\Cliente Emergiendo Con El Sol.lnk" "$INSTDIR\ClientUser.exe" ""

 
;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmergiendoConElSol" "DisplayName" "Emergiendo Con El Sol (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmergiendoConElSol" "UninstallString" "$INSTDIR\uninstaller.exe"
 
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "EmergiendoConElSol" "$INSTDIR\ClientUser.exe"

 
# define uninstaller name
WriteUninstaller $INSTDIR\uninstaller.exe

SetOutPath $LOCALAPPDATA\EmergiendoConElSol

File /r Temp\Setup.conf

ExecShell "" '"$INSTDIR\ClientUser.exe"'
 
 
#-------
# default section end
SectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"

;SetShellVarContext all
Delete "$INSTDIR\uninstaller.exe"
RMDir /r "$PROGRAMFILES\EmergiendoConElSol"
RMDir /r "$LOCALAPPDATA\EmergiendoConElSol"
Delete "$DESKTOP\Cliente Emergiendo Con El Sol.lnk"


;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\EmergiendoConElSol"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\EmergiendoConElSol"  
  DeleteRegValue HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "EmergiendoConElSol"
	
# Always delete uninstaller first
# Delete $INSTDIR\uninstaller.exe
 
# now delete installed file
# Delete $INSTDIR\*
 
SectionEnd