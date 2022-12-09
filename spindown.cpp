/*
Copyright (C) 2022 Ahmad Ismail
SPDX-License-Identifier: GPL-2.0
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <unistd.h>
using namespace std;
class block_device
{
private:
    string standby_command;
    string device;
    ifstream stat_file;
    char old_stat[250], new_stat[250];
    bool spun_down;

public:
    int timeout_minutes, inactive_minutes;

    // The function to call in a loop to update stats and spindown the disk if needed.
    void loop()
    {
        if (stat_file.fail())
            return;
        stat_file.sync();
        stat_file.getline(new_stat, 249);
        stat_file.seekg(0);
        if (strcmp(old_stat, new_stat) == 0)
            ++inactive_minutes;
        else
        {
            strcpy(old_stat, new_stat);
            spun_down = false;
            inactive_minutes = 0;
        }
        if (inactive_minutes == timeout_minutes && spun_down == false)
        {
            system(standby_command.c_str());
            spun_down = true;
        }
    }
    string get_device()
    {
        return device;
    }
    // Set the device to be tracked by the object
    void set_block_device(const char *device_name)
    {
        device = device_name;
        standby_command = "hdparm -yq /dev/" + device;
        char stat_filename[17 + device.size()];
        sprintf(stat_filename, "/sys/block/%s/stat", device.c_str());
        stat_file.open(stat_filename);
        if (stat_file.fail())
        {
            cout << "The block device " << device << "does not exist, ignoring..." << endl;
        }
    }
    block_device()
    {
        timeout_minutes = 10;
        inactive_minutes = 0;
        old_stat[0] = new_stat[0] = '\0';
    }
    block_device(block_device &source)
    {
        timeout_minutes = source.timeout_minutes;
        inactive_minutes = source.inactive_minutes;
        device = source.device;
        set_block_device(device.c_str());
        strcpy(old_stat, source.old_stat);
        strcpy(new_stat, source.new_stat);
    }
    block_device(const char *device_name, int inactivity_delay_minutes = 10)
    {
        set_block_device(device_name);
        timeout_minutes = inactivity_delay_minutes;
        inactive_minutes = 0;
        old_stat[0] = new_stat[0] = '\0';
    }
    // Clean up
    ~block_device()
    {
        stat_file.close();
    }
};
int main()
{
    vector<block_device *> disks;
    block_device *tmp;
    // Read the disk name from config and its timeout
    ifstream config;
    config.open("/etc/spindown.conf");
    string device;
    int timeout;
    do
    {
        // Ignoring comments
        while (config.peek() == '#')
            config.ignore(1000, '\n');
        config >> device >> timeout;
        // When the stream fails either due to reaching eof or an error, break out of the loop.
        if (config.fail())
            break;
        tmp = new block_device(device.c_str(), timeout);
        disks.push_back(tmp);
    } while (!config.eof());
    int i;
    // Main loop
    while (1)
    {
        for (i = 0; i < disks.size(); ++i)
            disks[i]->loop();
        sleep(60);
    }
    for (i = 0; i < disks.size(); ++i)
        delete disks[i];
    return 0;
}
