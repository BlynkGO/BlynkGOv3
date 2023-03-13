@echo off
setlocal

set esptool=esptool.exe
set chip=esp32
set baud=921600
set before=default_reset
set after=hard_reset
set flash_mode=dio
set flash_freq=80m
set flash_size=4MB

set bin_0=b.bin
set bin_1=p.bin
set bin_2=a.bin
set bin_3=f.bin

set com_port=%1

if "%com_port%" == "" (
    echo Please specify the COM port as the first parameter
    echo Usage: beenext_flasher.bat COM_PORT
    goto end
)

%esptool% --chip %chip% --port %com_port% --baud %baud% --before %before% --after %after% write_flash -z --flash_mode %flash_mode% --flash_freq %flash_freq% --flash_size %flash_size% 0x1000 %bin_0% 0x8000 %bin_1% 0xe000 %bin_2% 0x10000 %bin_3%

:end
echo Press any key to close this window.
pause >nul
endlocal
