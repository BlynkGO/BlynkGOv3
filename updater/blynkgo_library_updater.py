import os
from sys import platform
import shutil

blynkgo_ver="3.0.0 beta8"


print("")
print(r"==========================================")
print(r"     ___  __          __   _________  ")
print(r"    / _ )/ /_ _____  / /__/ ___/ __ \ ")
print(r"   / _  / / // / _ \/  '_/ (_ / /_/ / ")
print(r"  /____/_/\_, /_//_/_/\_\\___/\____/  ")
print(r"         /___/    ")
print("")
print(r"      BlynkGO Library V"+ blynkgo_ver + " Updator")
print("")
print(r"==========================================")


if platform == "linux" or platform == "linux2":
  # Linux :  /home/{username}/.arduino15
  user_home            = os.environ['HOME']
  arduino15_folder     = user_home            + "/.arduino15"
  ide2_yaml_file       = user_home            + "/.arduinoIDE/arduino-cli.yaml"   # XXX
elif platform == "darwin":
  # MAC :  /Users/"$USER"/Library/Arduino15/packages/esp32/hardware/esp32/1.0.6/tools/sdk/lib/
  username             = os.environ['USER']
  arduino15_folder     = "/Users/" + username             + "/Library/Arduino15"
  ide2_yaml_file       = "/Users/" + username + "/.arduinoIDE/arduino-cli.yaml" # XXX
  print(arduino15_folder)
elif platform == "win32":
  # Windows :  C:\Users\<USER_NAME>\AppData\Local\Arduino15  
  userprofile          = os.environ['UserProfile']
  arduino15_folder1    = userprofile          + "/Documents/ArduinoData"
  arduino15_folder2    = userprofile          + "/AppData/Local/Arduino15"
  ide2_yaml_file       = userprofile          + "/.arduinoIDE/arduino-cli.yaml" 

  arduino15_folder   = arduino15_folder1
  if(os.path.exists(arduino15_folder1)):
    arduino15_folder = arduino15_folder1
  elif(os.path.exists(arduino15_folder2)):
    arduino15_folder = arduino15_folder2

ide_esp32_basedir   = arduino15_folder + "/packages/esp32/hardware/esp32" # esp32 folder
ide_preference_file = arduino15_folder + "/preferences.txt"               # preference.txt
  
# print(esp32basedir)
print("")

if(os.path.exists(ide_esp32_basedir) is False):

  print("***************************************************************")
  print("            !!! BlynkGO-SDK Installer ERROR !!!")
  print("")
  print("             ERROR: Not Found ESP32 Core Folder")
  print("                Can't install BlynkGO-SDK.")
  print("")
  print("              Please contact blynkgo@gmail.com")
  print("")
  print("***************************************************************")

else:
  # check esp32 core version
  print("")
  # esp32_core_count = 0
  # for filename in os.listdir(esp32_basedir):
  #   esp32_core_version  = filename
  #   esp32_core_count += 1
  # if(esp32_core_count == 2):
  #   print("***************************************************************")
  #   print("            !!! BlynkGO-SDK Installer ERROR !!!")
  #   print("")
  #   print("           ERROR : ESP32 Core is MORE THAN 1 version")
  #   print("                Can't install BlynkGO-SDK.")
  #   print("")
  #   print("              Please contact blynkgo@gmail.com")
  #   print("")
  #   print("***************************************************************")

  # elif(esp32_core_count == 1 ):
  #   print("ESP32 Core Version : " + esp32_core_version)
  #   esp32_core_folder   = esp32_basedir + "/" + esp32_core_version
  #   if platform == "win32":
  #     esp32_core_folder = esp32_core_folder.replace("/","\\")

  #   print("ESP32 Core Folder  :")
  #   print("--> " + esp32_core_folder)



  esp32core_version_list = os.listdir(ide_esp32_basedir)
  # print(esp32core_version_list)
  if(len(esp32core_version_list) == 2 and platform != "darwin" )  :    # เกิดพบ 2 version ของ ESP32 core
    print("***************************************************************")
    print("            !!! BlynkGO-Library Updater ERROR !!!")
    print()
    print("        ERROR : Your ESP32 Core is MORE THAN 1 version"   )
    print("            Can't install BlynkGO-Library Updater.")
    print()
    print("              Please contact blynkgo@gmail.com")
    print()
    print("***************************************************************")
    print("Open File Explorer : \n  --> " + ide_esp32_basedir)
    print("\nand DELETE the folder [v" + "] or folder [v".join(esp32core_version_list)+ "]")
    print("And then install BlynkGO-SDK again.")

  elif(len(esp32core_version_list) == 1 or (len(esp32core_version_list) == 2 and platform == "darwin" )) :
    esp32_core_version  = esp32core_version_list[0]
    if platform == "darwin" :
      esp32_core_version  = esp32core_version_list[1]
    print("ESP32 Core Version : " + esp32_core_version)
    esp32_core_folder   = ide_esp32_basedir + "/" + esp32_core_version
    if platform == "win32":
      esp32_core_folder = esp32_core_folder.replace("/","\\")

    print("ESP32 Core Folder  :")
    print("--> " + esp32_core_folder)

    #-------------------------------------------------------------------------------
    # ESP32 Core 2.0.x  : BlynGOv3
    #------------------------
    if( esp32_core_version.startswith("2.0")):
      esp32_sdkdir        = esp32_core_folder + "/tools/sdk"
      esp32_partitiondir  = esp32_core_folder + "/tools/partitions"
      esp32_libdir        = esp32_sdkdir      + "/esp32/lib"
      esp32c3_libdir      = esp32_sdkdir      + "/esp32c3/lib"
      esp32s2_libdir      = esp32_sdkdir      + "/esp32s2/lib"
      esp32s3_libdir      = esp32_sdkdir      + "/esp32s3/lib"

      cur_dir = os.getcwd()

      print("\n\nBlynkGO Library Updating....")

      print("\n  [2] copying BlynkGO ESP32 lib")
      cur_esp32_lib_dir = cur_dir + "/esp32/lib"
      for filename in os.listdir(cur_esp32_lib_dir):
        if(filename.endswith('.a')):
          print("      - " + filename)
          shutil.copy( cur_esp32_lib_dir + "/" + filename, esp32_libdir)

      # print("\n  [3] copying BlynkGO ESP32C3 lib")
      # cur_esp32c3_lib_dir = cur_dir + "/esp32c3/lib"
      # print (cur_esp32c3_lib_dir)
      # for filename in os.listdir(cur_esp32c3_lib_dir):
      #   if(filename.endswith('.a')):
      #     print("      - " + filename)
      #     shutil.copy( cur_esp32c3_lib_dir + "/" + filename, esp32c3_libdir)

      # print("\n  [4] copying BlynkGO ESP32S2 lib")
      # cur_esp32s2_lib_dir = cur_dir + "/esp32s2/lib"
      # for filename in os.listdir(cur_esp32s2_lib_dir):
      #   if(filename.endswith('.a')):
      #     print("      - " + filename)
      #     shutil.copy( cur_esp32s2_lib_dir + "/" + filename, esp32s2_libdir)

      print("\n  [5] copying BlynkGO ESP32S3 lib")
      cur_esp32s3_lib_dir = cur_dir + "/esp32s3/lib"
      for filename in os.listdir(cur_esp32s3_lib_dir):
        if(filename.endswith('.a')):
          print("      - " + filename)
          shutil.copy( cur_esp32s3_lib_dir + "/" + filename, esp32s3_libdir)


      print ("\n\n[BlynkGO Library V"+ blynkgo_ver +"] Updated. Fun!!!")

 
print("\n")
os.system('pause')
