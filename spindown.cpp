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
public:
    ifstream stat_file;
    int timeout_minutes, inactive_minutes;
    char old_stat[250], new_stat[250];
    string device, standby_command;
    bool spun_down;

    // The function to call in a loop to update stats and spindown the disk if needed.
    void loop()
    {
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
    // Set the device to be tracked by the object
    void set_block_device(const char *device_name)
    {
        device = device_name;
        standby_command = "hdparm -yq /dev/" + device;
        char stat_filename[17 + device.size()];
        sprintf(stat_filename, "/sys/block/%s/stat", device.c_str());
        stat_file.open(stat_filename);
    }
    block_device()
    {
        timeout_minutes = 10;
        inactive_minutes = 0;
        old_stat[0] = new_stat[0] = '\0';
    }
    block_device(block_device&)
    {
        timeout_minutes = 10;
        inactive_minutes = 0;
        old_stat[0] = new_stat[0] = '\0';
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
    vector<block_device*> disks;
    block_device *tmp;
    // Read the disk name from config and its timeout
    ifstream config;
    config.open("/etc/spindown.conf");
    string device;
    int timeout;
    while (1)
    {
        // Ignoring comments
        while (config.peek() == '#')
            config.ignore(1000, '\n');
        config >> device >> timeout;
        if (config.eof())
            break;
        tmp = new block_device(device.c_str(), timeout);
        disks.push_back(tmp);
    }
    int i;
    while (1)
    {
        for (i = 0; i < disks.size();++i)
            disks[i]->loop();
        sleep(60);
    }
    return 0;
}
