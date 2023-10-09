#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ ! -f ${SCRIPT_DIR}/ProDOS_2_4_2.dsk ]; then
  curl -s -L -o ${SCRIPT_DIR}/ProDOS_2_4_2.dsk https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/prodos/ProDOS_2_4_2.dsk
fi

if [ ! -f ${SCRIPT_DIR}/AppleCommander-ac-1.8.0.jar ]; then
  curl -s -L -o ${SCRIPT_DIR}/AppleCommander-ac-1.8.0.jar https://github.com/AppleCommander/AppleCommander/releases/download/1.8.0/AppleCommander-ac-1.8.0.jar
  curl -s -L -o ${SCRIPT_DIR}/AppleCommander-acx-1.8.0.jar https://github.com/AppleCommander/AppleCommander/releases/download/1.8.0/AppleCommander-acx-1.8.0.jar
fi
