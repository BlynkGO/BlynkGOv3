#!/usr/bin/env python3

import sys
import os

#print(sys.argv)    # แสดงทั้งหมด
#print(sys.argv[1:]) #ตั้งแต่ตัวที่ 1 ไป ไม่รวมตัวแรก

num = len(sys.argv[1:])
#print(num)

if num == 0:
	print("\n")
	print("Please input COM port.")
	print("example :  upload.py COM3")
if num == 1:	# param ตัวที่ 1 เป็นค่า COM port
	print("Uploading 'firmware.bin' to <<BeeNeXT2.8>> : "+  sys.argv[1] +"\n\n")
	os.system('esptool.exe --chip esp32 --port '+ sys.argv[1] +' --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin')
elif num == 2:	# param ตัวที่ 2 ระบุ firmware.bin เป็นตัวอื่นๆด้วยตัวเองได้
	print("Uploading " + sys.argv[2] +" to <<BeeNeXT2.8>> : "+  sys.argv[1] +"\n\n")
	os.system('esptool.exe --chip esp32 --port '+ sys.argv[1] +' --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 '+ sys.argv[2])
elif num == 3:  # param ตัวที่ 3 ใส่ partitions.bin เป็นตัวอื่นๆด้วยตัวเองได้
	print("Uploading " + sys.argv[2] +" to <<BeeNeXT2.8>> : "+  sys.argv[1] +"\n\n")
	os.system('esptool.exe --chip esp32 --port '+ sys.argv[1] +' --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader.bin 0x8000 '+ sys.argv[3] +' 0xe000 boot_app0.bin 0x10000 '+ sys.argv[2])

print("\n\n")
os.system('pause')  # Press any key to continue . . .

