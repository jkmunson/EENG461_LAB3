#!/bin/bash
sudo stty -parenb -F /dev/ttyACM0 921600
#921600
sudo cat /dev/ttyACM0
#sudo screen /dev/ttyACM0 921600
