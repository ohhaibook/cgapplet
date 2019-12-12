#!/bin/bash
installDirectory="${0%/*}"
settingsFile="$installDirectory/cgapplet-settings"

getSettings () {
  . $settingsFile
}

modifySetting () {
  settingToModify=$1
  valueOfSetting=$2
  echo "Changing setting $settingToModify to $valueOfSetting"
  sed -i "s/$settingToModify=.*/$settingToModify=$valueOfSetting/" $settingsFile
}

setRunlock () {
  modifySetting "runlock" $1
}

checkIfDaemonAlreadyRunning () {
  setRunlock "off"
  sleep 3
  getSettings
  if [ "$runlock" = "on" ]; then
    echo "daemon is already running... exiting"
    exit
  elif [ [ "$runlock" = "off" ]; then
    echo "no daemon detected... continuing"
  fi
}

popupNotification () {
  notificationMessage=$1
  notificationIcon=$2
  echo "Sending notification message: \"$notificationMessage\""
  notify-send --icon=$notificationIcon "$notificationMessage"
}

cyberghostConnected () {
  if cyberghostvpn --status | grep -q 'No'; then
    false
  else
    true
  fi
}
