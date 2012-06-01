;--------------------------------
;Include Modern UI

!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\openvpncl.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

!define PRODUCT_NAME "Securepoint SSL VPN"
!define PRODUCT_PUBLISHER "Securepoint GmbH"
!define PRODUCT_WEB_SITE "http://www.securepoint.de"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Spvpncl.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

!define TAP "tap0901"
!define TAPDRV "${TAP}.sys"

;--------------------------------
;General

OutFile "SecurepointSSLVPN_RC2.exe"

;--------------------------------
;Pages

Var finishParm
Var isManage


  !define MUI_PAGE_CUSTOMFUNCTION_PRE WelcomePageSetupLinkPre
  !define MUI_PAGE_CUSTOMFUNCTION_SHOW WelcomePageSetupLinkShow
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\eula.txt"
  Page Custom MyCustomPage MyCustomLeave
  Page Custom MyAppType MyAppTypeLeave
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  ;!define MUI_FINISHPAGE_RUN "$INSTDIR\SPvpncl.exe"
  ;!define MUI_FINISHPAGE_RUN_PARAMETERS "$(finishParm)"
  !define MUI_FINISHPAGE_LINK "www.securepoint.de"  
  !define MUI_FINISHPAGE_LINK_LOCATION "http://www.securepoint.de"
  !insertmacro MUI_PAGE_FINISH
 
 
 !insertmacro MUI_LANGUAGE "German"
 !insertmacro MUI_LANGUAGE "English"

LangString StopSecServMes ${LANG_GERMAN} "Stoppe Securepoint VPN Dienst"
LangString SecAppName ${LANG_GERMAN} "Sp SSL VPN Client"
LangString SecServName ${LANG_GERMAN} "Sp SSL VPN Dienst"
LangString SecTapName ${LANG_GERMAN} "TAP-Treiber"
LangString SecOldTapName ${LANG_GERMAN} "Alte TAP-Devices entfernen"
LangString SecAllTapName ${LANG_GERMAN} "Alle TAP-Devices entfernen"
LangString Sec1Mes ${LANG_GERMAN} "Installiert den Securepoint SSL VPN Client."
LangString Sec2Mes ${LANG_GERMAN} "Installiert den Securepoint SSL VPN Dienst. Dieser ist nötig um Verbindungen ohne Administratorrechte herzustellen."
LangString Sec3Mes ${LANG_GERMAN} "Installiert den OpenVPN TAP-Treiber V9. Dieses TAP-Device benötigt OpenVPN um eine Verbindung aufbauen zu können."
LangString Sec4Mes ${LANG_GERMAN} "Alle alten vorhanden Devices (<tap0901) werden entfernen."
LangString Sec5Mes ${LANG_GERMAN} "Alle vorhanden TAP-Devices werden entfernen."
LangString InstallMesHeadShort ${LANG_GERMAN} "Installationsart"
LangString InstallMesHeadLong ${LANG_GERMAN} "Bitte wählen Sie die Installationsart aus"
LangString StartMesHeadShort ${LANG_GERMAN} "Startmodus"
LangString StartMesHeadLong ${LANG_GERMAN} "Bitte wählen Sie die Startmodus aus"
LangString RemoveApp1 ${LANG_GERMAN} "Möchten Sie "
LangString RemoveApp2 ${LANG_GERMAN} " und alle seinen Komponenten deinstallieren?"
LangString StartApp ${LANG_GERMAN} "Möchten Sie den Securepoint SSL VPN Client nun starten?"
LangString RemoveAppSuccess ${LANG_GERMAN} " wurde erfolgreich deinstalliert."
LangString RemoveConfigs ${LANG_GERMAN} "Möchten Sie die Konfigurationen mit löschen?"

;Eng
;----------------------------------
LangString RemoveApp1 ${LANG_ENGLISH} "Do you want to remove " 
LangString RemoveApp2 ${LANG_ENGLISH} " with all components?"  
LangString StopSecServMes ${LANG_ENGLISH} "Stopping Securepoint VPN Service"  
LangString SecAppName ${LANG_ENGLISH} "Sp SSL VPN Client"  
LangString SecServName ${LANG_ENGLISH} "Sp SSL VPN Service"
LangString SecTapName ${LANG_ENGLISH} "TAP-Device" 
LangString SecOldTapName ${LANG_ENGLISH} "Remove old TAP-Devices"  
LangString SecAllTapName ${LANG_ENGLISH} "Remove all TAP-Devices"  
LangString Sec1Mes ${LANG_ENGLISH} "Install the Securepoint SSL VPN Client."
LangString Sec2Mes ${LANG_ENGLISH} "Install the Securepoint SSL VPN Service. This is nessary if you want to make a connection without administrator rights."
LangString Sec3Mes ${LANG_ENGLISH} "Install the OpenVPN TAP-Device. This is needed to make a connection with OpenVPN"
LangString Sec4Mes ${LANG_ENGLISH} "Removes all old TAP-Devices (<tap0901)."
LangString Sec5Mes ${LANG_ENGLISH} "Remove all TAP-Devices."
LangString InstallMesHeadShort ${LANG_ENGLISH} "Installations context"
LangString InstallMesHeadLong ${LANG_ENGLISH} "Please select the context of the installation"
LangString StartMesHeadShort ${LANG_ENGLISH} "Starting context"
LangString StartMesHeadLong ${LANG_ENGLISH} "Please select the context of starting the client"
LangString StartApp ${LANG_ENGLISH} "Do you want to start the Securepoint SSL VPN client now?"
LangString RemoveAppSuccess ${LANG_ENGLISH} " was successfully removed."
LangString RemoveConfigs ${LANG_ENGLISH} "Do you want to delete the configurations in local appdata?"


  Name "${PRODUCT_NAME}"
InstallDir "$PROGRAMFILES\Securepoint SSL VPN"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
BrandingText "${PRODUCT_NAME}"

Section !$(SecAppName) s1
  ; Treiber Dateien
  SetOverwrite on
  SetOutPath "$INSTDIR\app\bin\"
  File "..\app\bin\7za.exe"
  File "..\app\bin\tapinstall.exe"
  File "..\app\bin\tapinstall64.exe"
  File "..\app\bin\openvpn.exe"
  File "..\app\bin\openssl.exe"
  File "..\app\bin\libssl32.dll"
  File "..\app\bin\libpkcs11-helper-1.dll"
  File "..\app\bin\libeay32.dll"

  SetOutPath "$INSTDIR\app\bin\driver"
  File "..\app\bin\driver\OemWin2k.inf"
  File "..\app\bin\driver\tap0901.cat"
  File "..\app\bin\driver\tap0901.sys"
  ;64bit Treiber immer mit kopieren
  SetOutPath "$INSTDIR\app\bin\driver\64bit"
  File "..\app\bin\driver\64bit\OemWin2k.inf"
  File "..\app\bin\driver\64bit\tap0901.cat"
  File "..\app\bin\driver\64bit\tap0901.sys"
  ; DLLs
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR"
  File "..\libgcc_s_dw2-1.dll"
  File "..\mingwm10.dll"
  File "..\QtCore4.dll"
  File "..\QtGui4.dll"
  ;File "..\QtSvg4.dll"
  File "..\QtXml4.dll"
  File "..\QtNetwork4.dll"
  File "..\setup.txt"
  File "..\SpSSLVpn_ger.qm"
  ;Dienst
  File "..\sc.exe"
  File "..\SPOpenVPNService.exe"
  ;Programm Dateien
  File "..\SPvpncl.exe"
  File "..\proxy.ini"
  
  
  ;Verknüpfungen
  CreateDirectory "$SMPROGRAMS\Securepoint SSL VPN"
  StrCmp $LANGUAGE 1031 gerini
  ; eng
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "-useEnglish $isManage"
  CreateShortCut "$DESKTOP\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "-useEnglish $isManage"
  StrCpy $finishParm "-useEnglish"
  goto lang-end

gerini:
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "$isManage"
  CreateShortCut "$DESKTOP\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "$isManage"
  StrCpy $finishParm ""
lang-end:
  
  
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Securepoint.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\openvpncl.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\SPvpncl.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Section !$(SecServName) s2
  SectionGetFlags ${s1} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

  goto installservice

 copyf:
  SetOutPath "$INSTDIR"
  File "..\libgcc_s_dw2-1.dll"
  File "..\mingwm10.dll"
  File "..\QtCore4.dll"
  File "..\QtGui4.dll"
  ;File "..\QtSvg4.dll"
  File "..\QtXml4.dll"
  File "..\QtNetwork4.dll"

  File "..\sc.exe"
  File "..\SPOpenVPNService.exe"

 installservice:  
  DetailPrint "$(StopSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" stop "Securepoint VPN"'
  DetailPrint "Remove Securepoint VPN Service"
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -u'
  DetailPrint "Install Securepoint VPN Service"
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -install'
  DetailPrint "Set Securepoint VPN Service to automatic"
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\sc.exe" config "Securepoint VPN" start= auto'
  DetailPrint "Starting Securepoint VPN Service"
  nsExec::Exec '"$INSTDIR\sc.exe" start "Securepoint VPN"'
SectionEnd

Section $(SecTapName) s4
  ;
  ; install
  ;
  SectionGetFlags ${s4} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" notap notap
  
  SectionGetFlags ${s1} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

    goto installtap

 copyf:
  SetOverwrite on
  SetOutPath "$INSTDIR\app\bin\"
  File "..\app\bin\tapinstall.exe"
  File "..\app\bin\tapinstall64.exe"

  SetOutPath "$INSTDIR\app\bin\driver"
  File "..\app\bin\driver\OemWin2k.inf"
  File "..\app\bin\driver\tap0901.cat"
  File "..\app\bin\driver\tap0901.sys"
  
  SetOutPath "$INSTDIR\app\bin\driver\64bit"
  File "..\app\bin\driver\64bit\OemWin2k.inf"
  File "..\app\bin\driver\64bit\tap0901.cat"
  File "..\app\bin\driver\64bit\tap0901.sys"

 installtap:
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  
  IntCmp $0 0 tap-32bit
  
  ; Hier kommt die 64 bit Implementierung
  DetailPrint "Installation des TAP-Treibers 64bit."
  DetailPrint "TAP-Win32 INSTALL (${TAP})"
  
  nsExec::ExecToLog '"$INSTDIR\app\bin\tapinstall64.exe" install "$INSTDIR\app\bin\driver\64bit\OemWin2k.inf" ${TAP}'
  Pop $R0 # return value/error/timeout
  IntOp $5 $5 | $R0
  DetailPrint "tapinstall64 install returned: $R0"

  goto tap-ende
  
  tap-32bit:
  DetailPrint "Installation des TAP-Treibers 32bit."
  DetailPrint "TAP-Win32 INSTALL (${TAP})"
  
  nsExec::ExecToLog '"$INSTDIR\app\bin\tapinstall.exe" install "$INSTDIR\app\bin\driver\OemWin2k.inf" ${TAP}'
  Pop $R0 # return value/error/timeout
  IntOp $5 $5 | $R0
  DetailPrint "tapinstall install returned: $R0"
  
 tapinstall_check_error:
    DetailPrint "Tap install status: $5"
    IntCmp $5 0 notap
    MessageBox MB_OK "An error occurred installing the TAP-Win32 device driver."
 notap:

tap-ende:
 
SectionEnd

Section $(SecOldTapName) s3
  SectionGetFlags ${s1} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

  goto deltap

 copyf:
  SetOverwrite on
  SetOutPath "$INSTDIR\app\bin\"
  File "..\app\bin\tapinstall.exe"

  SetOutPath "$INSTDIR\app\bin\driver"
  File "..\app\bin\driver\OemWin2k.inf"
  File "..\app\bin\driver\tap0901.cat"
  File "..\app\bin\driver\tap0901.sys"
      
 deltap:
    DetailPrint "TAP-Win32 REMOVE OLD TAP"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAP'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAP returned: $R0"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAPDEV'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAPDEV returned: $R0"
SectionEnd

Section /o $(SecAllTapName) s5 
  SectionGetFlags ${s1} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

  goto deltap

 copyf:
  SetOverwrite on
  SetOutPath "$INSTDIR\app\bin\"
  File "..\app\bin\tapinstall.exe"
  File "..\app\bin\tapinstall64.exe"

  SetOutPath "$INSTDIR\app\bin\driver"
  File "..\app\bin\driver\OemWin2k.inf"
  File "..\app\bin\driver\tap0901.cat"
  File "..\app\bin\driver\tap0901.sys"
  
  ;64
  SetOutPath "$INSTDIR\app\bin\driver\64bit"
  File "..\app\bin\driver\64bit\OemWin2k.inf"
  File "..\app\bin\driver\64bit\tap0901.cat"
  File "..\app\bin\driver\64bit\tap0901.sys"
  
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  
  IntCmp $0 0 deltap
  
 deltap64:
    DetailPrint "64 bit - TAP-Win32 REMOVE OLD TAP"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall64.exe" remove TAP'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAP returned: $R0"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall64.exe" remove TAPDEV'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAPDEV returned: $R0"
	nsExec::ExecToLog '"$INSTDIR\bin\tapinstall64.exe" remove tap0901'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAPDEV returned: $R0"
 
 goto deltap-ende
 
 deltap:
    DetailPrint "TAP-Win32 REMOVE OLD TAP"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAP'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAP returned: $R0"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAPDEV'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAPDEV returned: $R0"
	nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove tap0901'
    Pop $R0 # return value/error/timeout
    DetailPrint "tapinstall remove TAPDEV returned: $R0"

 deltap-ende:
	
SectionEnd!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${s1} "$(Sec1Mes)"
  !insertmacro MUI_DESCRIPTION_TEXT ${s2} "$(Sec2Mes)"
  !insertmacro MUI_DESCRIPTION_TEXT ${s4} "$(Sec3Mes)"
  !insertmacro MUI_DESCRIPTION_TEXT ${s3} "$(Sec4Mes)"  
  !insertmacro MUI_DESCRIPTION_TEXT ${s5} "$(Sec5Mes)"
!insertmacro MUI_FUNCTION_DESCRIPTION_END




;---- FUNCTIONS ----------------------------
Function MyCustomPage
  ; Wenn Deutsch eingestellt ist deutsche Datei öffnen, sont eng
  !insertmacro MUI_HEADER_TEXT "$(InstallMesHeadShort)" "$(InstallMesHeadLong)"
  StrCmp $LANGUAGE 1031 gerini
  
  ; eng
  ReserveFile "eng_user.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "eng_user.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "eng_user.ini"

  goto lang-end

gerini:

  ReserveFile "user.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "user.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "user.ini"
  
lang-end:
  
FunctionEnd
 
Function MyCustomLeave
  # Form validation here. Call Abort to go back to the page.
  # Use !insertmacro MUI_INSTALLOPTIONS_READ $Var "InstallOptionsFile.ini" ...
  # to get values.
  StrCmp $LANGUAGE 1031 gerini  
  ; eng
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "eng_user.ini" "Field 2" "State"
  
  goto lang-end

gerini:
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "user.ini" "Field 2" "State"
lang-end:
	StrCmp $R1 0 inCur
	StrCmp $R1 1 inAll
	Abort                 
	goto ende
	
	inCur:
		SetShellVarContext current
		StrCpy $INSTDIR "$LOCALAPPDATA\${PRODUCT_NAME}"
		goto ende
	inAll:
		SetShellVarContext all
		StrCpy $INSTDIR "$PROGRAMFILES\${PRODUCT_NAME}"
	
	ende:
FunctionEnd


Function MyAppType
  ; Wenn Deutsch eingestellt ist deutsche Datei öffnen, sont eng
  !insertmacro MUI_HEADER_TEXT "$(StartMesHeadShort)" "$(StartMesHeadLong)"
  StrCmp $LANGUAGE 1031 gerini
  
  ; eng
  ReserveFile "eng_type.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "eng_type.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "eng_type.ini"

  goto lang-end

gerini:

  ReserveFile "type.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "type.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "type.ini"
  
lang-end:
  
FunctionEnd
 
Function MyAppTypeLeave
  # Form validation here. Call Abort to go back to the page.
  # Use !insertmacro MUI_INSTALLOPTIONS_READ $Var "InstallOptionsFile.ini" ...
  # to get values.
  StrCmp $LANGUAGE 1031 gerini  
  ; eng
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "eng_type.ini" "Field 2" "State"
  
  goto lang-end

gerini:
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "type.ini" "Field 2" "State"
lang-end:
	StrCmp $R1 0 manage
	StrCmp $R1 1 normal
	Abort                 
	goto ende
	
	normal:
		StrCpy $isManage "-manage"
		goto ende
	manage:
		StrCpy $isManage ""
	
	ende:
FunctionEnd


Function WelcomePageSetupLinkPre
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "4" ; increase counter
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" "122" ; limit size of the upper label
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Type" "Link"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Text" "http://www.securepoint.de/"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "State" "http://www.securepoint.de/"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Left" "120"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Right" "315"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Top" "123"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Bottom" "132"
FunctionEnd

Function WelcomePageSetupLinkShow
  ; Thanks to pengyou
  ; Fix colors of added link control
  ; See http://forums.winamp.com/showthread.php?s=&threadid=205674
  Push $0

  GetDlgItem $0 $MUI_HWND 1203
  SetCtlColors $0 "0000FF" "FFFFFF"
  ; underline font
  CreateFont $1 "$(^Font)" "$(^FontSize)" "400" /UNDERLINE
  SendMessage $0 ${WM_SETFONT} $1 1
  Pop $0

FunctionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  InitPluginsDir
  SetOutPath $PLUGINSDIR
  File "/oname=$PLUGINSDIR\spltmp.bmp" "startscreen.bmp"
  Splash::Show "2000" "$PLUGINSDIR\spltmp"
  Delete "$PLUGINSDIR\spltmp.bmp"
  ClearErrors
  UserInfo::GetName
  IfErrors ok
  Pop $R0
  UserInfo::GetAccountType
  Pop $R1
  StrCmp $R1 "Admin" ok
    Messagebox MB_OK "Administrator privileges required to install Securepoint SSL VPN [$R0/$R1]"
    Abort
  ok:
FunctionEnd

Function .onInstSuccess
  SetShellVarContext current
  CreateDirectory "$APPDATA\Securepoint SSL VPN"
  CreateDirectory "$APPDATA\Securepoint SSL VPN\config"
  CopyFiles "$EXEDIR\data\config\*.*" "$APPDATA\Securepoint SSL VPN\config\"
  Delete "$EXEDIR\data\config\*.*" 
  RMDir "$EXEDIR\data\config"
  RMDir "$EXEDIR\data"
  MessageBox MB_YESNO "$(StartApp)" IDNO NoReadme
      Exec '"$INSTDIR\Spvpncl.exe" "$finishParm" "$isManage"' 
  NoReadme:
FunctionEnd

;--------------------------------
;Languages

  ;
  
; uninstall
Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) $(RemoveAppSuccess)"
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(RemoveApp1) $(^Name) $(RemoveApp2)" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ;SetShellVarContext all
  nsExec::Exec '"$INSTDIR\sc.exe" stop "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -u'
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\SPOpenVPNService.exe"
  Delete "$INSTDIR\sc.exe"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\app\bin\driver\tap0901.sys"
  Delete "$INSTDIR\app\bin\driver\tap0901.cat"
  Delete "$INSTDIR\app\bin\driver\OemWin2k.inf"
  ;64bit
  Delete "$INSTDIR\app\bin\driver\64bit\tap0901.sys"
  Delete "$INSTDIR\app\bin\driver\64bit\tap0901.cat"
  Delete "$INSTDIR\app\bin\driver\64bit\OemWin2k.inf"
  ; ende 64 bit 
  Delete "$INSTDIR\app\bin\libeay32.dll"
  Delete "$INSTDIR\app\bin\libpkcs11-helper-1.dll"
  Delete "$INSTDIR\app\bin\libssl32.dll"
  Delete "$INSTDIR\app\bin\openssl.exe"
  Delete "$INSTDIR\app\bin\openvpn.exe"
  Delete "$INSTDIR\app\bin\7za.exe"
  Delete "$INSTDIR\app\bin\openvpnserv.exe"
  Delete "$INSTDIR\app\bin\tapinstall.exe"
  Delete "$INSTDIR\SPvpncl.exe"
  Delete "$INSTDIR\proxy.ini"
  Delete "$INSTDIR\setup.txt"
  Delete "$INSTDIR\*.*"
  
  RMDir "$INSTDIR\app\bin\driver"
  RMDir "$INSTDIR\app\bin"
  RMDir "$INSTDIR\app"  
  RMDir /r "$INSTDIR"
  # little helper to delete all data
  SetShellVarContext all	  
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Uninstall.lnk"
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Securepoint.lnk"
  Delete "$DESKTOP\Securepoint SSL VPN.lnk"
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk"
  RMDir "$SMPROGRAMS\Securepoint SSL VPN"
    
  SetShellVarContext current
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Uninstall.lnk"
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Securepoint.lnk"
  Delete "$DESKTOP\Securepoint SSL VPN.lnk"
  Delete "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk"
  RMDir "$SMPROGRAMS\Securepoint SSL VPN"
  
  MessageBox MB_YESNO "$(RemoveConfigs)" IDNO NoDelete
      Delete "$APPDATA\Securepoint SSL VPN\config\*.*"
	  RMDir "$APPDATA\Securepoint SSL VPN\config"
	  RMDir /r "$APPDATA\Securepoint SSL VPN"
  NoDelete:
  
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd