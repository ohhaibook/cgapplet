#installDir="/home/ohhaibook/custom/cyberghost"
installDir="${0%/*}"
globalFunctions="$installDir/cgapplet-global.sh"
. $globalFunctions

getSettings

country="$1"
city="$2"

confirmDialog () {
  if [ $1 = "connect" ]; then
    action="$1 to $country, $city"
    icon="system-lock-screen"
  elif [ $1 = "disconnect" ]; then
    action="$1"
    icon="process-stop"
  fi
  if $(zenity --question --ellipsize --icon-name=$icon --title="Confirm" --text="Are you sure you want to $action?"); then
    echo "Attempting request"
    commitChanges "$1"
    exit
  else
    echo "Cancelling"
    exit
  fi
}

commitChanges () {
  modifySetting "triggerCountry" $country
  modifySetting "triggerCity" $city
  modifySetting "triggerChange" $1
}

#disconnect
if [ "$country" = "disconnect" ]; then
  if cyberghostConnected; then
    confirmDialog "disconnect"    
  else
    #disconnect
    exit    
  fi
#defaults
elif [ -z "$country" ]; then
  country="de"
  city="frankfurt"
  confirmDialog "connect"
#if only country
elif [ -z "$city" ]; then
  city="any"
  confirmDialog "connect"
#if country and city
else
  confirmDialog "connect"
fi