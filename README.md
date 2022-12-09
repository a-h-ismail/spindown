# HDD Spindown

## Purpose

This repository hosts the `spindown` tool, a simple service that can keep track of disk activity and issue standby command after a specified timeout (on a per disk basis).

Some Hard disk drives firmware does not honor Linux's disk APM, which leads to disks never spinning down on idle.

## Setup and installation
Install `hdparm` (required to issue the standby command) and `g++` (to build the binary) using your package manager :

For Ubuntu/Debian:

`sudo apt install hdparm g++`

For Fedora:

`sudo dnf install hdparm g++`


Clone the project and run the install script:
```
git clone --depth 1 https://gitlab.com/a-h-ismail/spindown.git/
cd spindown
chmod +x ./install.sh
sudo ./install.sh
```
The install script will guide you to setup the configuration file and will automatically enable and start the systemd service. For non systemd users, you just need to start the binary `/usr/bin/spindownd` on boot.

## License
This project is licensed under the GNU GPL version 2 only.
