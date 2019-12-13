![cgapplet screenshot](https://i.ibb.co/h833FvC/cgapplet.png)

CGApplet is a tool that is designed to integrate with command line VPN utilities for third party VPN providers. It is essentially a graphical wrapper for the command line utility.

Currently it only supports cyberghostvpn.

First you must install the cyberghostvpn command line utility. You will also have to exempt the cyberghostvpn program from requiring a password for sudo.

You will need to compile the program with GTK

INSTALLATION

    sudo rm -rf /usr/local/cgapplet

    gcc -o cgapplet cgapplet.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

    sudo mkdir /usr/local/cgapplet

    sudo cp -r * /usr/local/cgapplet/

    sudo cp cgapplet.desktop /usr/share/applications

OR

sudo rm -rf /usr/local/cgapplet && gcc -o cgapplet cgapplet.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic && sudo mkdir /usr/local/cgapplet && sudo cp -r * /usr/local/cgapplet/ && sudo cp cgapplet.desktop /usr/share/applications
