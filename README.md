![cgapplet screenshot](https://i.ibb.co/h833FvC/cgapplet.png)

# About the project

CGApplet is a tool that is designed to integrate with command line VPN utilities for third party VPN providers. It is essentially a graphical wrapper for the command line utility.

Currently it only supports cyberghostvpn.

## Future features/known bugs

* Add support for text file definitions that allow the program to integrate with other command line utilities simply by loading the definitions into the program
* Add support for openvpn files
* Fix any country, any city selections so that they actually work
* Options menu with certain options such as restart network manager on disconnect

# Installation

## Dependencies

You will need the GTK dev tools in order to compile it. As it currently only works with Cyberghost VPN via their command line utility for Linux, cyberghostvpn, you will also need to install that. It is written in Python so you may also need to install Python. Set cyberghostvpn up with your user account. Make sure the cyberghostvpn command line utility is working before proceeding with installation.

## Step by step

Then, clone the git repo and run install.sh
```
git clone https://github.com/ohhaibook/cgapplet.git

cd cgapplet

chmod a+X install.sh && bash install.sh
```
## One line installation command
```
git clone https://github.com/ohhaibook/cgapplet.git && cd cgapplet && chmod a+X install.sh && bash install.sh
```
