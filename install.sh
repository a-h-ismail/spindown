#!/bin/bash
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
cp spindown.conf /etc
cp spindown.service /usr/lib/systemd/system/
#For SELinux users, just in case the binary is not labeled correctly
restorecon /usr/bin/spindownd /etc/spindown.conf &> /dev/null
chmod +x /usr/bin/spindownd
systemctl enable --now spindown
echo Done!
