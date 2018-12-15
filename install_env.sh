## This script installs:
#  - Arduino IDE
#  - arduino-mk
#  - Additional boards
#  Inspired from: https://github.com/adafruit/travis-ci-arduino
##

# if .travis.yml does not set version
if [ -z $ARDUINO_IDE_VERSION ]; then
   export ARDUINO_IDE_VERSION="1.8.6"
   echo "NOTE: YOUR .TRAVIS.YML DOES NOT SPECIFY ARDUINO IDE VERSION, USING $ARDUINO_IDE_VERSION"
fi

# if newer version is requested
if [ ! -f $HOME/arduino_ide/$ARDUINO_IDE_VERSION ] && [ -f $HOME/arduino_ide/arduino ]; then
   echo "DIFFERENT VERSION OF ARDUINO IDE REQUESTED: "
   shopt -s extglob
   cd $HOME/arduino_ide/
   rm -rf *
   cd $OLDPWD
fi

# if not already cached, download and install arduino IDE
echo "ARDUINO IDE STATUS: "
if [ ! -f $HOME/arduino_ide/arduino ]; then
   echo "DOWNLOADING: "
   wget https://downloads.arduino.cc/arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz
   echo "UNPACKING ARDUINO IDE: "
   [ ! -d $HOME/arduino_ide/ ] && mkdir $HOME/arduino_ide
   tar xf arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz -C $HOME/arduino_ide/ --strip-components=1
   rm arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz
   touch $HOME/arduino_ide/$ARDUINO_IDE_VERSION
fi

cd 
git clone https://github.com/sudar/Arduino-Makefile.git arduino-mk

export PATH="$HOME/arduino_ide:$HOME/arduino-mk/bin:$PATH"

# make display available for arduino CLI
/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_1.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :1 -ac -screen 0 1280x1024x16
sleep 3
export DISPLAY=:1.0

echo -e "\n########################################################################";
echo -e "INSTALLING DEPENDENCIES"
echo "########################################################################";


# install the due, esp8266, and adafruit board packages
echo "ADD PACKAGE INDEX: "
#arduino --pref "boardsmanager.additional.urls=https://adafruit.github.io/arduino-board-index/package_adafruit_index.json,http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://dl.espressif.com/dl/package_esp32_index.json" --save-prefs

arduino --pref "boardsmanager.additional.urls=http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json" --save-prefs

echo "ESP8266:"
if [ ! -f /home/travis/.arduino15/package_esp8266com_index.json ]
then
   arduino --install-boards esp8266:esp8266
   chmod +x /home/travis/.arduino15/packages/esp8266/hardware/esp8266/2.5.0-beta1/tools/signing.py
else
   echo "CACHED"
fi

echo "MiniCore:"
if [ ! -f /home/travis/.arduino15/package_MCUdude_MiniCore_index.json ]
then
   arduino --install-boards MiniCore:avr
else
   echo "CACHED"
fi
