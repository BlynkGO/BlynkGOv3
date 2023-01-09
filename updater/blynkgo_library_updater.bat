@echo off
set blynkgo_ver=3.0.0 beta1
cls
echo.
echo ==========================================
echo     ___  __          __   _________ 
echo    / _ )/ /_ _____  / /__/ ___/ __ \
echo   / _  / / // / _ \/  '_/ (_ / /_/ /
echo  /____/_/\_, /_//_/_/\_\\___/\____/ 
echo        /___/ BlynkGO Library V%blynkgo_ver%
echo.
echo       BlynkGO Library Updator V%blynkgo_ver% 
echo.
echo ==========================================
echo.
set esp32basedir=%UserProfile%\AppData\Local\Arduino15\packages\esp32\hardware\esp32
IF EXIST %esp32basedir% (
	GOTO FOUND_ESP32BASEDIR
)
rem กรณี ไม่พบ ที่ Arduino15 ให้ลองเช็คอีกที่
set esp32basedir=%UserProfile%\Documents\ArduinoData\packages\esp32\hardware\esp32
IF EXIST %esp32basedir% (
	GOTO FOUND_ESP32BASEDIR
)
GOTO ERROR_NOT_FOUND


:FOUND_ESP32BASEDIR
echo Found ESP32 Core Folder :
echo --^> %esp32basedir%
echo.
echo.
for /d %%d in (%esp32basedir%\*.*) do (
  set esp32dir=%%d
)

IF EXIST %esp32dir% (
	GOTO COPY_FILES
)

GOTO ERROR_NOT_FOUND



:COPY_FILES
echo Updating...
set esp32sdkdir=%esp32dir%\tools\sdk
set esp32partitiondir=%esp32dir%\tools\partitions
set esp32libdir=%esp32sdkdir%\esp32\lib
set esp32c3libdir=%esp32sdkdir%\esp32c3\lib
set esp32s2libdir=%esp32sdkdir%\esp32s2\lib
set esp32s3libdir=%esp32sdkdir%\esp32s3\lib
echo.
copy .\esp32\lib\libblynkgolib.a %esp32libdir%\libblynkgolib.a  /Y
rem xcopy /s /v .\esp32\lib %esp32sdkdir%\esp32\lib\ /K /H /Y /d
rem xcopy /s /v .\esp32c3\lib %esp32sdkdir%\esp32c3\lib\ /K /H /Y /d
rem xcopy /s /v .\esp32s2\lib %esp32sdkdir%\esp32s2\lib\ /K /H /Y /d
rem xcopy /s /v .\esp32s3\lib %esp32sdkdir%\esp32s3\lib\ /K /H /Y /d
echo.
echo BlynkGO Library V%blynkgo_ver% Updated. Fun!!!
echo.
echo.
pause
GOTO:EOF 


:ERROR_NOT_FOUND
echo ******************************************
echo    !!! BlynkGO Libary Update ERROR !!!    
echo.
echo     Not Found ESP32 Core Folder
echo     Can't Update BlynkGO Library.
echo.
echo     Please contact blynkgo@gmail.com
echo.
echo ******************************************
echo.
echo.

pause
GOTO:EOF