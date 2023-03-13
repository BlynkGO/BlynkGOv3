@echo off
set COMPORT=%1
IF "%1" == "" GOTO HELP

esptool.exe --chip esp32 --port %COMPORT% --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin

GOTO:EOF

:HELP
echo.
echo Please input COM port.
echo example :  upload COM3