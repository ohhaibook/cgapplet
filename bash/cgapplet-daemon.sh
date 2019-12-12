globalFunctions="${0%/*}/cgapplet-global.sh"
. $globalFunctions

disconnectIcon="/usr/share/icons/Papirus/48x48/emblems/vcs-conflicting.svg"
flagIconDir="/usr/share/iso-flags-png-320x240"

checkIfDaemonAlreadyRunning

while ! zenity --title="Authenticate Cyberghost" --password | sudo -S cat /dev/null >/dev/null; do
  if $(zenity --question --ellipsize --title="Wrong Password" --text="Wrong password, would you like to exit cyberghost daemon?"); then
    echo "exiting cyberghost-daemon"
    exit;
  else
    echo "trying password again"
  fi
done

sendNotification () {
  if cyberghostConnected; then
    popupNotification "Cyberghost connected! ($triggerCountry, $triggerCity)" "$flagIconDir/$triggerCountry.png"
  else
    popupNotification "Cyberghost disconnected!" $disconnectIcon    
  fi
}

executeVPN () {
  #disconnect
  if [ "$triggerChange" = "disconnect" ]; then
    if cyberghostConnected; then
      echo "Please enter your password to disconnect from Cyberghost VPN"
      sudo cyberghostvpn --stop
      sendNotification
      nmcli n off
      nmcli n on
    else
      echo "Already disconnected"
    fi
  #connect
  elif [ "$triggerChange" = "connect" ]; then
    #if only country
    if [ "$triggerCity" = "any" ]; then
      sudo cyberghostvpn --country-code $triggerCountry --connect
    #if country and city
    else
      sudo cyberghostvpn --country-code $triggerCountry --city $triggerCity --connect
    fi
    sendNotification
  fi
  disableTrigger
  waitForTrigger
}

disableTrigger () {
  modifySetting "triggerChange" "false"
}

waitForTrigger () {
  getSettings
  while [ "$triggerChange" = "false" ]; do
    getSettings
    setRunlock "on"
    sleep 1
    # sudo echo to keep sudo active, if this doesn't work then require password prompt
    sudo echo "VPN not triggered, triggerCountry=$triggerCountry, triggerCity=$triggerCity"
  done
  echo "VPN TRIGGERED, triggerCountry=$triggerCountry, triggerCity=$triggerCity"
  executeVPN
}

disableTrigger
waitForTrigger