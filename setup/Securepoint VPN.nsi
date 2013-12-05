;--------------------------------
;Include Modern UI

!include "MUI.nsh"
!include "StrFunc.nsh"
!include "LogicLib.nsh"

${StrLoc}

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\openvpncl.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "sec-logo.bmp"

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

OutFile "Securepoint SSL VPN v1.0.1.exe"
ShowInstDetails show

;--------------------------------
;Pages

Var finishParm
Var isManage
Var isSaveData


  !define MUI_PAGE_CUSTOMFUNCTION_PRE WelcomePageSetupLinkPre
  !define MUI_PAGE_CUSTOMFUNCTION_SHOW WelcomePageSetupLinkShow
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_FINISHPAGE_TITLE_3LINES
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\eula.txt"
  Page Custom MyCustomPage MyCustomLeave
  Page Custom MyAppType MyAppTypeLeave
  Page Custom MyAppSave MyAppSaveLeave
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES  
  !define MUI_FINISHPAGE_LINK "www.securepoint.de"  
  !define MUI_FINISHPAGE_LINK_LOCATION "http://www.securepoint.de"
  ;!define MUI_FINISHPAGE_NOAUTOCLOSE
  !insertmacro MUI_PAGE_FINISH
 
 
 !insertmacro MUI_LANGUAGE "German"
 !insertmacro MUI_LANGUAGE "English"

LangString StopSecServMes ${LANG_GERMAN} "Stoppe Securepoint VPN Dienst"
LangString RemoveSecServMes ${LANG_GERMAN} "Entferne Securepoint VPN Dienst" 
LangString SetSecServMes ${LANG_GERMAN} "Setze Securepoint VPN Dienst auf automatik" 
LangString InstallSecServMes ${LANG_GERMAN} "Installiere Securepoint VPN Dienst" 
LangString StartSecServMes ${LANG_GERMAN} "Starte Securepoint VPN Dienst" 
LangString TapReturn ${LANG_GERMAN} "Tapinstall Rückgabe:" 
LangString TapRemoveOld ${LANG_GERMAN} "TAP-Win32 entferne alte TAP" 
LangString TapError ${LANG_GERMAN} "Ein Fehler ist beim Installieren vom TAP-Win32 aufgetreten." 
LangString SecAppName ${LANG_GERMAN} "Sp SSL VPN Client"
LangString SecServName ${LANG_GERMAN} "Sp SSL VPN Dienst"
LangString SecTapName ${LANG_GERMAN} "TAP-Treiber"
LangString SecOldTapName ${LANG_GERMAN} "Alte TAP-Devices entfernen"
LangString SecAllTapName ${LANG_GERMAN} "Alle TAP-Devices entfernen"
LangString Sec1Mes ${LANG_GERMAN} "Installiert den Securepoint SSL VPN Client und Dienst. Der Dienst ist nötig um Verbindungen ohne Administratorrechte herzustellen."
LangString Sec2Mes ${LANG_GERMAN} "Installiert den Securepoint SSL VPN Dienst. Dieser ist nötig um Verbindungen ohne Administratorrechte herzustellen."
LangString Sec3Mes ${LANG_GERMAN} "Installiert den OpenVPN TAP-Treiber V9. Dieses TAP-Device benötigt OpenVPN um eine Verbindung aufbauen zu können."
LangString Sec4Mes ${LANG_GERMAN} "Alle alten vorhanden Devices (<tap0901) werden entfernen."
LangString Sec5Mes ${LANG_GERMAN} "Alle vorhanden TAP-Devices werden entfernen."
LangString InstallMesHeadShort ${LANG_GERMAN} "Installationsart"
LangString InstallMesHeadLong ${LANG_GERMAN} "Bitte wählen Sie die Installationsart aus"
LangString StartMesHeadShort ${LANG_GERMAN} "Startmodus"
LangString StartMesHeadLong ${LANG_GERMAN} "Bitte wählen Sie die Startmodus aus"
LangString DataMesHeadShort ${LANG_GERMAN} "Benutzerdaten"
LangString DataMesHeadLong ${LANG_GERMAN} "Bitte wählen Sie die Betriebsart aus"
LangString RemoveApp1 ${LANG_GERMAN} "Möchten Sie "
LangString RemoveApp2 ${LANG_GERMAN} " und alle seinen Komponenten deinstallieren?"
LangString StartApp ${LANG_GERMAN} "Möchten Sie den Securepoint SSL VPN Client nun starten?"
LangString RemoveAppSuccess ${LANG_GERMAN} " wurde erfolgreich deinstalliert."
LangString RemoveConfigs ${LANG_GERMAN} "Möchten Sie die Konfigurationen mit löschen?"
LangString tapUpgrade ${LANG_GERMAN} "Der TAP32 ist zu alt und muss aktualisiert werden. Möchten Sie nun das TAP-Device aktualisieren?"
LangString rebootSetup ${LANG_GERMAN} "Es wird empfohlen einen Neustart zu machen. Möchten Sie nun neustarten?"
LangString tapUdateMessage ${LANG_GERMAN} "TAP32 wird aktualisiert. Dieses erfordert eventuell eine Bestätigung des Benutzers."

;Eng
;----------------------------------
LangString RemoveApp1 ${LANG_ENGLISH} "Do you want to remove " 
LangString RemoveApp2 ${LANG_ENGLISH} " with all components?"  
LangString StopSecServMes ${LANG_ENGLISH} "Stopping Securepoint VPN Service"  
LangString RemoveSecServMes ${LANG_ENGLISH} "Remove Securepoint VPN Service" 
LangString SetSecServMes ${LANG_ENGLISH} "Set Securepoint VPN Service to automatic" 
LangString InstallSecServMes ${LANG_ENGLISH} "Install Securepoint VPN Service" 
LangString StartSecServMes ${LANG_ENGLISH} "Starting Securepoint VPN Service" 
LangString TapReturn ${LANG_ENGLISH} "Tapinstall returned:" 
LangString TapRemoveOld ${LANG_ENGLISH} "TAP-Win32 REMOVE OLD TAP" 
LangString TapError ${LANG_ENGLISH} "An error occurred installing the TAP-Win32 device driver." 
LangString SecAppName ${LANG_ENGLISH} "Sp SSL VPN Client"  
LangString SecServName ${LANG_ENGLISH} "Sp SSL VPN Service"
LangString SecTapName ${LANG_ENGLISH} "TAP-Device" 
LangString SecOldTapName ${LANG_ENGLISH} "Remove old TAP-Devices"  
LangString SecAllTapName ${LANG_ENGLISH} "Remove all TAP-Devices"  
LangString Sec1Mes ${LANG_ENGLISH} "Install the Securepoint SSL VPN client and service. The service is necessary if you want to make a connection without administrator rights."
LangString Sec2Mes ${LANG_ENGLISH} "Install the Securepoint SSL VPN Service. This is necessary if you want to make a connection without administrator rights."
LangString Sec3Mes ${LANG_ENGLISH} "Install the OpenVPN TAP-Device. This is needed to make a connection with OpenVPN"
LangString Sec4Mes ${LANG_ENGLISH} "Removes all old TAP-Devices (<tap0901)."
LangString Sec5Mes ${LANG_ENGLISH} "Remove all TAP-Devices."
LangString InstallMesHeadShort ${LANG_ENGLISH} "Installations context"
LangString InstallMesHeadLong ${LANG_ENGLISH} "Please select the context of the installation"
LangString StartMesHeadShort ${LANG_ENGLISH} "Starting context"
LangString StartMesHeadLong ${LANG_ENGLISH} "Please select the context of starting the client"
LangString DataMesHeadShort ${LANG_ENGLISH} "User credentials"
LangString DataMesHeadLong ${LANG_ENGLISH} "Please select the running mode"
LangString StartApp ${LANG_ENGLISH} "Do you want to start the Securepoint SSL VPN client now?"
LangString RemoveAppSuccess ${LANG_ENGLISH} " was successfully removed."
LangString RemoveConfigs ${LANG_ENGLISH} "Do you want to delete the configurations in local appdata?"
LangString tapUpgrade ${LANG_ENGLISH} "The TAP-Device is too old. Do you want to update it now?"
LangString rebootSetup ${LANG_ENGLISH} "It is recommended that you restart the system. Do you wish to restart now?"
LangString tapUdateMessage ${LANG_ENGLISH} "TAP device update(tap0901). This may require confirmation."

  Name "${PRODUCT_NAME}"
InstallDir "$PROGRAMFILES\Securepoint SSL VPN"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
BrandingText "${PRODUCT_NAME}"

Section !$(SecAppName) s1
  ; Treiber Dateien
  SetOverwrite on
  

  DetailPrint "$(StopSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" stop "Securepoint VPN"'
  DetailPrint "$(RemoveSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -u'
  
  RMDir /r "$INSTDIR"
  
  
  SetOutPath "$INSTDIR\bin\"
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  
  IntCmp $0 0 bin-32bit
  ; Copy x64
  File "..\bin\x64\libeay32.dll"
  File "..\bin\x64\liblzo2-2.dll"
  File "..\bin\x64\libpkcs11-helper-1.dll"
  File "..\bin\x64\ssleay32.dll"
  File "..\bin\x64\openvpn.exe"
  File "..\bin\x64\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x64\driver\OemWin2k.inf"
  File "..\bin\x64\driver\tap0901.cat"
  File "..\bin\x64\driver\tap0901.sys"
  
  goto tapinstallend
  
  bin-32bit:
  ; Copy x32
  File "..\bin\x32\libeay32.dll"
  File "..\bin\x32\liblzo2-2.dll"
  File "..\bin\x32\libpkcs11-helper-1.dll"
  File "..\bin\x32\ssleay32.dll"
  File "..\bin\x32\openvpn.exe"
  File "..\bin\x32\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x32\driver\OemWin2k.inf"
  File "..\bin\x32\driver\tap0901.cat"
  File "..\bin\x32\driver\tap0901.sys"
  
  tapinstallend:
  
  ; END of bin
  
  
  ; Tap Device Testen
    IntOp $R5 0 & 0
	nsExec::ExecToStack '"$INSTDIR\bin\tapinstall.exe" hwids tap0901'
	Pop $R0 # return value/error/timeout
	IntOp $R5 $R5 | $R0
	DetailPrint "$(TapReturn) $R0"

	; If tapinstall output string contains "${PRODUCT_TAP_WIN_COMPONENT_ID}" we assume
	; that TAP device has been previously installed,
	; therefore we will update, not install.
	Push "tap0901"
	Push ">"
	Call StrLoc
	Pop $R0

	${If} $R5 == 0
		${If} $R0 == ""
			StrCpy $R1 "install"			
		${Else}
			MessageBox MB_YESNO|MB_ICONQUESTION "$(tapUpgrade)" /SD IDYES IDNO noupgrade
			DetailPrint "$(tapUdateMessage)"
			nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" update "$INSTDIR\bin\driver\OemWin2k.inf" tap0901'
			Pop $R0 # return value/error/timeout
			${If} $R0 == ""
				IntOp $R0 0 & 0				
			${EndIf}
			IntOp $R5 $R5 | $R0
			DetailPrint "$(TapReturn) $R0"
			
			noupgrade:
		${EndIf}		
	${EndIf}
       
  SetOverwrite ifnewer
  
  Delete "$INSTDIR\debug.txt"
  ClearErrors
  
  ; DLLs
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR"
  File "..\Aggregation.dll"
	File "..\CPlusPlus.dll"
	File "..\DebuggingHelper.dll"
	File "..\ExtensionSystem.dll"
	File "..\libcharset-1.dll"
	File "..\libexpat-1.dll"
	File "..\libgcc_s_dw2-1.dll"
	File "..\libgcrypt-11.dll"
	File "..\libgmp-3.dll"
	File "..\libgmpxx-4.dll"
	File "..\libgomp-1.dll"
	File "..\libgpg-error-0.dll"
	File "..\libiconv-2.dll"
	File "..\libmpfr-1.dll"
	File "..\libssh2-1.dll"
	File "..\libssh2.dll"
	File "..\libssp-0.dll"
	File "..\libstdc++-6.dll"
	File "..\mingwm10.dll"
	File "..\phonon4.dll"
	File "..\QtCore4.dll"
	File "..\QtGui4.dll"
	File "..\QtNetwork4.dll"
	File "..\QtScript4.dll"
	File "..\QtScriptTools4.dll"
	File "..\QtSql4.dll"
    File "..\QtSvg4.dll"
    File "..\QtMultimedia4.dll"	
	File "..\QtXml4.dll"
	File "..\QtXmlPatterns4.dll"
	File "..\zlib1.dll"   
    File "..\zlib.dll"         
    File "..\qca2.dll"    
	File "..\quazip.dll"    
	File "..\libssl32.dll"
	File "..\ssleay32.dll"
	File "..\libeay32.dll"		
  
  File "..\cacert.pem"
  File "..\cert.key"
  File "..\cert.pem"  
  
  
  File "..\SpSSLVpn_ger.qm"
  File "..\CPlusPlus.dll"
  File "..\Aggregation.dll"
  File "..\QtXmlPatterns4.dll"  
  ;Dienst
  File "..\sc.exe"
  File "..\SPOpenVPNService.exe"
  ;Programm Dateien
  File "..\SPvpncl.exe"  
  ;Plugins
  SetOutPath "$INSTDIR\plugins"
  File /r "..\plugins\*.*"	
  
  SetOutPath "$INSTDIR"
   
  
  ;Verknüpfungen
  CreateDirectory "$SMPROGRAMS\Securepoint SSL VPN"
  StrCmp $LANGUAGE 1031 gerini
  ; eng
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "-useEnglish $isManage $isSaveData"
  CreateShortCut "$DESKTOP\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "-useEnglish $isManage $isSaveData"
  StrCpy $finishParm "-useEnglish"
  goto lang-end

gerini:
  CreateShortCut "$SMPROGRAMS\Securepoint SSL VPN\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "$isManage $isSaveData"
  CreateShortCut "$DESKTOP\Securepoint SSL VPN.lnk" "$INSTDIR\SPvpncl.exe" "$isManage $isSaveData"
  StrCpy $finishParm ""
lang-end:
    
  DetailPrint "$(StopSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" stop "Securepoint VPN"'
  DetailPrint "$(RemoveSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -u'
  DetailPrint "$(InstallSecServMes)"
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -install'
  DetailPrint "$(SetSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\sc.exe" config "Securepoint VPN" start= auto'
  DetailPrint "$(StartSecServMes)"
  nsExec::Exec '"$INSTDIR\sc.exe" start "Securepoint VPN"'
  ; Only for win 8
  nsExec::Exec '"$INSTDIR\sc.exe" stop Netman'
  nsExec::Exec '"$INSTDIR\sc.exe" query Netman'
  nsExec::Exec '"$INSTDIR\sc.exe" config Netman start= auto'
  nsExec::Exec '"$INSTDIR\sc.exe" query Netman'
  nsExec::Exec '"$INSTDIR\sc.exe" start Netman' 
  
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

Section /o $(SecAllTapName) s2 
  SectionGetFlags ${s2} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

  goto insttap

 copyf:
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR\bin\"
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  
  IntCmp $0 0 bin-32bit
  ; Copy x64    
  
  File "..\bin\x64\libeay32.dll"
  File "..\bin\x64\liblzo2-2.dll"
  File "..\bin\x64\libpkcs11-helper-1.dll"
  File "..\bin\x64\ssleay32.dll"
  File "..\bin\x64\openvpn.exe"
  File "..\bin\x64\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x64\driver\OemWin2k.inf"
  File "..\bin\x64\driver\tap0901.cat"
  File "..\bin\x64\driver\tap0901.sys"
  
  goto tapinstallend
  
  bin-32bit:
  ; Copy x32
  DetailPrint "Installation des TAP-Treibers 32bit."
  DetailPrint "TAP-Win32 INSTALL (${TAP})"
  
  File "..\bin\x32\libeay32.dll"
  File "..\bin\x32\liblzo2-2.dll"
  File "..\bin\x32\libpkcs11-helper-1.dll"
  File "..\bin\x32\ssleay32.dll"
  File "..\bin\x32\openvpn.exe"
  File "..\bin\x32\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x32\driver\OemWin2k.inf"
  File "..\bin\x32\driver\tap0901.cat"
  File "..\bin\x32\driver\tap0901.sys"
   
  tapinstallend:
  insttap:
  
    DetailPrint "$(TapRemoveOld)"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAP'
    Pop $R0 # return value/error/timeout
    DetailPrint "$(TapReturn) $R0"
    nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove TAPDEV'
    Pop $R0 # return value/error/timeout
    DetailPrint "$(TapReturn) $R0"
	nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" remove tap0901'
    Pop $R0 # return value/error/timeout
    DetailPrint "$(TapReturn) $R0" 
	
SectionEnd

Section $(SecTapName) s3
  ;
  ; install
  ;
  SectionGetFlags ${s3} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" notap notap
  
  SectionGetFlags ${s1} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  IntCmp $R0 ${SF_SELECTED} "" copyf copyf

    goto installtap

 copyf:
  SetOverwrite ifnewer
  
  SetOutPath "$INSTDIR\bin\"
  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  
  IntCmp $0 0 bin-32bit
  ; Copy x64
  DetailPrint "Installation des TAP-Treibers 64bit."
  DetailPrint "TAP-Win32 INSTALL (${TAP})"
  
  File "..\bin\x64\libeay32.dll"
  File "..\bin\x64\liblzo2-2.dll"
  File "..\bin\x64\libpkcs11-helper-1.dll"
  File "..\bin\x64\ssleay32.dll"
  File "..\bin\x64\openvpn.exe"
  File "..\bin\x64\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x64\driver\OemWin2k.inf"
  File "..\bin\x64\driver\tap0901.cat"
  File "..\bin\x64\driver\tap0901.sys"
  
  goto tapinstallend
  
  bin-32bit:
  ; Copy x32
  DetailPrint "Installation des TAP-Treibers 32bit."
  DetailPrint "TAP-Win32 INSTALL (${TAP})"
  
  File "..\bin\x32\libeay32.dll"
  File "..\bin\x32\liblzo2-2.dll"
  File "..\bin\x32\libpkcs11-helper-1.dll"
  File "..\bin\x32\ssleay32.dll"
  File "..\bin\x32\openvpn.exe"
  File "..\bin\x32\tapinstall.exe"
  SetOutPath "$INSTDIR\bin\driver"
  File "..\bin\x32\driver\OemWin2k.inf"
  File "..\bin\x32\driver\tap0901.cat"
  File "..\bin\x32\driver\tap0901.sys"

 tapinstallend:
 installtap:
  nsExec::ExecToLog '"$INSTDIR\bin\tapinstall.exe" install "$INSTDIR\bin\driver\OemWin2k.inf" ${TAP}'
  Pop $R0 # return value/error/timeout
  IntOp $5 $5 | $R0
  DetailPrint "$(TapReturn) $R0"

  
; tapinstall_check_error:
    DetailPrint "$(TapReturn) $5"
    IntCmp $5 0 notap
    MessageBox MB_OK "$(TapError)"
 notap:
 
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${s1} "$(Sec1Mes)"  
  !insertmacro MUI_DESCRIPTION_TEXT ${s2} "$(Sec5Mes)"  
  !insertmacro MUI_DESCRIPTION_TEXT ${s3} "$(Sec3Mes)"     
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

Function MyAppSave
  ; Wenn Deutsch eingestellt ist deutsche Datei öffnen, sont eng
  !insertmacro MUI_HEADER_TEXT "$(DataMesHeadShort)" "$(DataMesHeadLong)"
  StrCmp $LANGUAGE 1031 gerini
  
  ; eng
  ReserveFile "eng_data.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "eng_data.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "eng_data.ini"

  goto lang-end

gerini:

  ReserveFile "data.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "data.ini"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "data.ini"
  
lang-end:
  
FunctionEnd
 
Function MyAppSaveLeave
  # Form validation here. Call Abort to go back to the page.
  # Use !insertmacro MUI_INSTALLOPTIONS_READ $Var "InstallOptionsFile.ini" ...
  # to get values.
  StrCmp $LANGUAGE 1031 gerini  
  ; eng
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "eng_data.ini" "Field 3" "State"
  
  goto lang-end

gerini:
  !insertmacro MUI_INSTALLOPTIONS_READ $R1 "data.ini" "Field 3" "State"
lang-end:
	StrCmp $R1 0 nosave
	StrCmp $R1 1 normal
	Abort                 
	goto ende
	
	normal:
		StrCpy $isSaveData ""
		goto ende
	nosave:
		StrCpy $isSaveData "-disableSaveData"		
	
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
  FindProcDLL::FindProc "SPvpncl.exe"
   IntCmp $R0 1 0 notRunning
		MessageBox MB_OK|MB_ICONEXCLAMATION "The VPN client is still running. Please close it first" /SD IDOK
		Abort
   notRunning:
FunctionEnd

Function .onInstSuccess

  SetShellVarContext current
  CreateDirectory "$APPDATA\Securepoint SSL VPN"
  CreateDirectory "$APPDATA\Securepoint SSL VPN\config"
  CopyFiles "$EXEDIR\data\config\*.*" "$APPDATA\Securepoint SSL VPN\config\"
  Delete "$EXEDIR\data\config\*.*" 
  RMDir "$EXEDIR\data\config"
  RMDir "$EXEDIR\data"
  
  MessageBox MB_YESNO|MB_ICONQUESTION "$(rebootSetup)" /SD IDYES IDNO noreboot
		Reboot
  
  noreboot:
  ;MessageBox MB_YESNO "$(StartApp)" IDNO NoReadme  
  ;StrCmp $LANGUAGE 1031 gerini  
;	Exec '"$INSTDIR\Spvpncl.exe" -useEnglish $isManage $isSaveData' 	
;	goto lang-end
	
;gerini:
 ;;   Exec '"$INSTDIR\Spvpncl.exe" $isManage $isSaveData' 
;lang-end:
   
 ;   NoReadme:
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
  !insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(RemoveApp1) $(^Name) $(RemoveApp2)" IDYES +2
  Abort
FunctionEnd

Section Uninstall
   FindProcDLL::FindProc "SPvpncl.exe"
   IntCmp $R0 1 0 notRunning
		MessageBox MB_OK|MB_ICONEXCLAMATION "The VPN client is still running. Please close it first" /SD IDOK
		Abort
   notRunning:
  ;SetShellVarContext all
  nsExec::Exec '"$INSTDIR\sc.exe" stop "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\sc.exe" query "Securepoint VPN"'
  nsExec::Exec '"$INSTDIR\SPOpenVPNService.exe" -u'
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\QtXml4.dll"  
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\qca2.dll"  
  Delete "$INSTDIR\quazip.dll"  
  Delete "$INSTDIR\SPOpenVPNService.exe"
  Delete "$INSTDIR\sc.exe"
  Delete "$INSTDIR\CPlusPlus.dll"
  Delete "$INSTDIR\Aggregation.dll"
  Delete "$INSTDIR\QtXmlPatterns4.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  
  Delete "$INSTDIR\cacert.pem"
  Delete "$INSTDIR\cert.key"
  Delete "$INSTDIR\cert.pem"  
  
  RMDir /r "$INSTDIR\bin\"
  
  Delete "$INSTDIR\SPvpncl.exe"
  Delete "$INSTDIR\proxy.ini"
  Delete "$INSTDIR\setup.txt"
  Delete "$INSTDIR\*.*"
  
  RMDir "$INSTDIR\bin\driver"
  RMDir "$INSTDIR\bin"  
  RMDir /r "$INSTDIR"
  # litle helper to delete all data
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
	  Delete "$APPDATA\Securepoint SSL VPN\*.*"
	  RMDir /r "$APPDATA\Securepoint SSL VPN"
  NoDelete:
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd