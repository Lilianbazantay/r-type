#!/usr/bin/env bash

# detect the os and run the installation accordingly
UNAME=$(uname -s)

case "$UNAME" in
    Linux*)
        ./installation_scripts/linux.sh
        ;;
    Darwin*)
        ./installation_scripts/linux.sh
        ;;
    MINGW*|MSYS*|CYGWIN*)
        ./installation_scripts/windows.sh
        ;;
    *)
        echo "Unknown OS: $UNAME"
        ;;
esac
