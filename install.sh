#!/bin/bash

echo Removing cgapplet directory in case of older install...
sudo rm -rf /usr/local/cgapplet

echo Compiling...
gcc -o cgapplet cgapplet.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

echo Creating new directory...
sudo mkdir /usr/local/cgapplet

echo Moving files into directory...
sudo cp -r cgapplet cgapplet.ui resources /usr/local/cgapplet/

echo Creating .desktop file...
sudo cp cgapplet.desktop /usr/share/applications

echo Installed cgapplet!
