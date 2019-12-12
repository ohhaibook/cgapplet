globalFunctions="${0%/*}/cgapplet-global.sh"
. $globalFunctions

ipaddress=$(dig @resolver1.opendns.com ANY myip.opendns.com +short)

clickConnect="<txtclick>$installDirectory/cgapplet-connect.sh</txtclick>"
clickDisconnect="<txtclick>$installDirectory/cgapplet-connect.sh disconnect</txtclick>"

if ! cyberghostConnected; then
  modifySetting "realip" "$ipaddress"
  echo "<tool>IP address: $ipaddress\nNot currently connected to VPN</tool>$clickConnect<txt><span weight=\"regular\" fgcolor=\"#CD5C5C\">Exposed</span></txt>"
else
  getSettings
  if [ "$ipaddress" = "$realip" ]; then
    echo "<tool>IP address: $ipaddress\nConnecting to VPN ($triggerCountry, $triggerCity)</tool>$clickDisconnect<txt><span weight=\"regular\" fgcolor=\"#1E90FF\">Connecting...</span></txt>"
  else
    echo "<tool>Real IP address: $realip\nIP address: $ipaddress\nConnected to: $triggerCountry, $triggerCity\nSecured with VPN!</tool>$clickDisconnect<txt><span weight=\"regular\" fgcolor=\"Light Green\">Secured</span></txt>"
  fi
fi