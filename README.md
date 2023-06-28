# HDD Spindown

## Purpose

This repository hosts the `spindown` tool, a simple service that can keep track of disk activity and issue standby command after a specified timeout (on a per disk basis).<br>
Some Hard disk drives firmware does not honor Linux's disk APM, which leads to disks never spinning down on idle.

## Setup and installation

### Required packages

Install `hdparm` (required to issue the standby command) and `g++` (to build the binary) using your package manager :

#### Ubuntu/Debian

`sudo apt install hdparm g++`

#### Fedora

`sudo dnf install hdparm g++`<br>

### Build from source

Clone the project and run the install script:

```sh
git clone --depth 1 https://gitlab.com/a-h-ismail/spindown.git/
cd spindown
chmod +x ./install.sh
sudo ./install.sh
```

The install script will guide you to setup the configuration file and will automatically enable and start the systemd service. For non systemd users, you just need to start the binary `/usr/bin/spindownd` on boot (as a daemon).

## Notes

- The configuration file is located at `/etc/spindown.conf`. After editing it you need to reload the binary for changes to take effect:
`sudo systemctl restart spindown`

- The tool may not be aware of some SMART self tests since they may not change usage statistics in the kernel. This leads to aborting the test due to the tool not detecting any activity and sending standby command. Temporarily stop the service during self tests as follows:

```sh
# Stop the service
sudo systemctl stop spindown

# Start the service AFTER completing the test
sudo systemctl start spindown
```

## License

This project is licensed under the GNU GPL version 2 only.
