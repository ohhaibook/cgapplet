sudo rm -rf /usr/local/cgapplet
gcc -o cgapplet cgapplet.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
sudo mkdir /usr/local/cgapplet
sudo cp -r * /usr/local/cgapplet/
sudo cp cgapplet.desktop /usr/share/applications
