## This script installs:
#  - Arduino IDE
#  - arduino-mk
#  - Additional boards
#  Inspired from: https://github.com/adafruit/travis-ci-arduino
##

# if .travis.yml does not set version
if [ -z $ARDUINO_IDE_VERSION ]
then
   export ARDUINO_IDE_VERSION="1.8.6"
   echo "NOTE: YOUR .TRAVIS.YML DOES NOT SPECIFY ARDUINO IDE VERSION, USING $ARDUINO_IDE_VERSION"
fi

ARDUINO_DIR=${HOME}/opt/arduino-${ARDUINO_IDE_VERSION}
ARDMK_DIR=${HOME}/opt/arduino-mk

# if not already cached, download and install arduino IDE
[ -d ${ARDUINO_DIR} ] || mkdir -p ${ARDUINO_DIR}
echo "ARDUINO IDE STATUS: "
if [ ! -f ${ARDUINO_DIR}/arduino ]
then
   echo "DOWNLOADING:"
   wget https://downloads.arduino.cc/arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz
   echo "UNPACKING ARDUINO IDE:"
   tar xf arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz -C ${ARDUINO_DIR} --strip-components=1
   rm arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz
   echo "DONE"
else
   echo "CACHED"
fi

cd
echo "Arduino-mk:"
git clone https://github.com/sudar/Arduino-Makefile.git ${ARDMK_DIR} || true
cd ${ARDMK_DIR}
git pull # cache needs update?
cd

export PATH="${ARDUINO_DIR}:${ARDMK_DIR}/bin:$PATH"

# make display available for arduino CLI
/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_1.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :1 -ac -screen 0 1280x1024x16
sleep 3
export DISPLAY=:1.0

echo -e "\n########################################################################";
echo -e "INSTALLING DEPENDENCIES"
echo "########################################################################";


# install the due, esp8266, and adafruit board packages
echo "ADD BOARDS PACKAGES INDEX:"
arduino --pref "boardsmanager.additional.urls=http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json" --save-prefs

# echo "Install ESP8266:"
# if [ ! -d ~/.arduino15/packages/esp8266/hardware/esp8266 ]
# then
#    arduino --install-boards esp8266:esp8266:2.5.0-beta1
#    chmod +x /home/travis/.arduino15/packages/esp8266/hardware/esp8266/*/tools/signing.py
# else
#    echo "CACHED"
# fi

# echo "Install MiniCore:"
# if [ ! -d ~/.arduino15/packages/MiniCore/hardware/avr ]
# then
#    arduino --install-boards MiniCore:avr
# else
#    echo "CACHED"
# fi

function install_board {
   # See https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc
   # arduino --install-boards package name:platform architecture[:version]

   echo "Installing $1"

   BOARD=$1
   set -- $( echo ${BOARD} | tr ":" " " )
   CORE=$1
   ARCH=$2
   [ $# -ge 3 ] && VERS=$3 || VERS="*"

   if [ -f ~/.arduino15/packages/${CORE}/hardware/${ARCH}/${VERS}/boards.txt ]
   then
      echo "CACHED"
   else
      arduino --install-boards ${BOARD}
      #chmod +x /home/travis/.arduino15/packages/esp8266/hardware/esp8266/*/tools/signing.py
   fi
}
