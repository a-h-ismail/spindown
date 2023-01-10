#!/bin/bash
if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root."
  exit 1
else
  echo 'Installing Idle disk spindown service.'
fi

g++ spindown.cpp -O2 -o spindownd
systemctl daemon-reload
mv spindownd /usr/bin
echo 'Before proceeding, take note which disks should be tracked by the service.'
echo 'For example, a disk with path /dev/sda is placed in the config as sda'
echo 'Your disks list:'
lsblk -d
read -p 'Press enter to setup your config.'
nano spindown.conf
cp spindown.conf /etc
cp spindown.service /usr/lib/systemd/system/
#For SELinux users, just in case the binary is not labeled correctly
restorecon /usr/bin/spindownd /etc/spindown.conf &> /dev/null
chmod +x /usr/bin/spindownd
systemctl enable --now spindown
echo Done!
