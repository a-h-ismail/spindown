# HDD Spindown

## Purpose

This repository hosts the `spindown` tool, a simple service that can keep track of disk activity and issue standby command to disks after a specified timeout.
Some Hard disk drives firmware does not honor Linux's disk APM, which leads to disks never spinning down on idle. This leads to a constant noise, power waste and possibly impacts the HDD lifespan.

## Setup and installation

Clone the project and run the install script:
```
git clone --depth 1 https://gitlab.com/a-h-ismail/spindown/
cd spindown
chmod +x ./install.sh
sudo ./install.sh
```

## License
This project is licensed under the GNU GPL version 2 only.
