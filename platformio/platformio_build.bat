rem Install Python27 platformio
rem C:\Python27\Scripts\pip.exe install -U platformio

rem Set Python path
set PATH=%PATH%;C:\Python27
set PATH=%PATH%;C:\Python27\Scripts

rem Update library
rem git fetch
rem git pull

rem Build example(s)
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Brightness/Brightness.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Buttons/Buttons.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Counter/Counter.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Date/Date.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Demo/Demo.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Temperature/Temperature.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/TestLEDs/TestLEDs.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Time/Time.ino
