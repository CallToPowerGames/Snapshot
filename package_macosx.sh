#!/bin/bash
#
# Snapshot package script for Mac OS X
#
# by Denis Meyer
#

set -o errexit
set -o pipefail

#############
# Variables #
#############

SYSTEM_NAME=$(uname -s)
SCRIPT_VERSION="1.0.0"
FOLDER_NAME_APP="build-Snapshot-Desktop_Qt_5_2_1_clang_64bit-Release"
APP_VOLUME_NAME="Snapshot v1.0.2"
APP_VOLUME_NAME_DMG="Snapshot_v1-0-2"
MACDEPLOYQT="/bin/macdeployqt"
OPERATING_SYSTEM_OK=0

#############
# Functions #
#############

function print_logo()
{
	echo ""
	echo "########################################"
	echo "#                                      #"
	echo "# Snapshot package script for Mac OS X #"
	echo "#                                      #"
	echo "########################################"
	echo "Script version $SCRIPT_VERSION by Denis Meyer"
	echo ""
}

function check_operatingSystem()
{
	case $SYSTEM_NAME in
	  Darwin)
		echo "1"
	    ;;
	  *)
		echo "0"
	    ;;
	esac
}

function exit_error()
{
	exit 2
}

##########
# Script #
##########

print_logo

# check the operating system
echo "Checking the operating system..."
OPERATING_SYSTEM_OK=$(check_operatingSystem)
if [[ $OPERATING_SYSTEM_OK == 1 ]];
then
	echo "Operating system supported."
else
	echo "Error: Unsupported platform: $SYSTEM_NAME" >&2
	exit_error
fi

rm -rf $FOLDER_NAME_APP/*.o $FOLDER_NAME_APP/*.cpp $FOLDER_NAME_APP/*.h $FOLDER_NAME_APP/Makefile
cp bundling-resources/mac/Info.plist $FOLDER_NAME_APP/Snapshot.app/Contents/
cp bundling-resources/mac/icons.icns $FOLDER_NAME_APP/Snapshot.app/Contents/Resources/
cp CHANGELOG.txt $FOLDER_NAME_APP
cp README.txt $FOLDER_NAME_APP
$MACDEPLOYQT $FOLDER_NAME_APP/Snapshot.app/
hdiutil create -fs HFS+ -srcfolder $FOLDER_NAME_APP -volname "$APP_VOLUME_NAME" $APP_VOLUME_NAME_DMG.dmg
rm -rf $FOLDER_NAME_APP
