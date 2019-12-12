// TO DO:
// ADD EXEMPTION FOR CYBERGHOSTVPN TO SUDO PASSWD
// HAVE IT AUTOMICATICALLY TEST AND SEE IF CONNECTION GOOD OR NOT WORKING, THEN TRY AGAIN

#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h> // NOT NECESSARY
#include <stdarg.h> // NOT NECESSARY

#define MAXCHAR 1024

// GLOBAL VARIABLES

char *installDir="/home/ohhaibook/Nextcloud/Coding/c/cgapplet2/";
char timeCode[MAXCHAR] = "default";
char pwdPromptCmd[MAXCHAR];
bool verbose = FALSE;
bool connectedToVPN;
GtkWidget *window, *label1, *labelStatusMessage;
GtkDialog *aboutDialog;
GtkComboBoxText *countryList, *cityList;
// GtkStatusIcon *statusIcon;
GtkImage *flagIcon;
GtkButton *connectButton;

// FUNCTIONS

void verbosePrint(char *input, ... ) {
  char formatString[MAXCHAR];// = input;
  strcpy(formatString, input);
  strcat(formatString, "\n");
  va_list arg;
  va_start(arg, input);
  if(verbose == TRUE) {
    vprintf(formatString, arg);
  }
  va_end(arg);
}

char* getFilePath(char input[MAXCHAR]) {
  static char path[MAXCHAR]="";
  strcpy(path,installDir);
  strcat(path, input);
  verbosePrint("path of file: %s", path);
  return path;
}

char* readFile(char* filePath){
    FILE *file;
    static char output[MAXCHAR];
    file = fopen(filePath, "r");
    if (file == NULL){
        printf("Could not open file %s\n",filePath);
        strcpy(output, "null");
        return NULL;
    }
    while (fgets(output, MAXCHAR, file) != NULL) {
      strtok(output, "\n");
      verbosePrint("contents of file %s: %s", filePath, output);
      //strcat(output, fgets(output, MAXCHAR, file));
    }
    fclose(file);
    return output;//strtok(output, "\n");
}

char countryCityArray[100][20][30];

void populateCountryArray(char* filePath){
  char const* const fileName = filePath; /* should check that argc > 1 */
  FILE* file = fopen(fileName, "r"); /* should check the result */
  char line[256];
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    /* note that fgets don't strip the terminating \n, checking its
       presence would allow to handle lines longer that sizeof(line) */
    verbosePrint("line %d reads: %s", i, line);
    strcpy(countryCityArray[i][0], strtok(line, "\n"));
    i++;
  }
  /* may check feof here to make a difference between eof and io failure -- network
     timeout for instance */

  fclose(file);
}

// void updateStatusMessage(char *message, char *color) {
//   char *formatting="";
//   strcat(formatting, "<span foreground=\"");
//   strcat(formatting, color);
//   strcat(formatting, "\">%s</span>");
//   char *markup = g_markup_printf_escaped (formatting, message);
//   gtk_label_set_markup (GTK_LABEL(labelStatusMessage), markup);
// }


// MAYBE MAKE CHAR SO CAN BE DIRECTLY PASSED INTO cmdToTempFile
void sudoCmd(char input[MAXCHAR]){
  char cmd[MAXCHAR];
  // strcpy(cmd, pwdPromptCmd);
  // strcat(cmd, "; sudo ");
  strcpy(cmd, "sudo ");
  strcat(cmd, input);
  system(cmd);
}

// WRITES COMMAND OUTPUT TO TIMESTAMPED TEMP FILE
// WILL NEED TO HAVE A READ FUNCTION AS WELL, OR MODIFY readFile()
void cmdToTempFile(char inputCmd[MAXCHAR], char inputFile[MAXCHAR]) {
  char cmd[MAXCHAR];
  strcpy(cmd, inputCmd);
  strcat(cmd, " > ");
  strcat(cmd, inputFile);
  // strcat(cmd, "-");
  strcat(cmd, timeCode);
  system(cmd);
}

char* execCmdReturnOutput(char cmd[MAXCHAR], char filePath[MAXCHAR]) {
  cmdToTempFile(cmd, filePath);
  // strcat(filePath, timeCode);
  char filePath2[MAXCHAR];
  strcpy(filePath2, filePath);
  strcat(filePath2, timeCode);
  return readFile(filePath2);
}

char * getSelectedCountryID(){
  char *countrySelected = gtk_combo_box_text_get_active_text(countryList);
  int len= strlen(countrySelected);
  char *countryID = &countrySelected[len-2];
  verbosePrint("countryID is %s", countryID);
  return countryID;
}

int checkStatusIteration;
gboolean checkStatus() {
  char *formatting;// = "<span foreground=\"red\">%s</span>";
  char *statusMessage, *markup;

  // system("if cyberghostvpn --status | grep -q 'No'; then echo -n false; else echo -n true; fi > /tmp/cgapplet.txt");
  // connectedToVPN = !strcmp(readFile("/tmp/cgapplet.txt"), "true");
  char statusFile[MAXCHAR]="/tmp/cgapplet-status";
  cmdToTempFile("if cyberghostvpn --status | grep -q 'No'; then echo -n false; else echo -n true; fi", statusFile);
  strcat(statusFile, timeCode);
  connectedToVPN = !strcmp(readFile(statusFile), "true");

  if (connectedToVPN) {
    // gtk_label_set_text(GTK_LABEL(labelStatusMessage),"VPN connected!");
    statusMessage = "VPN connected!";
    formatting = "<span foreground=\"green\">%s</span>";
    markup = g_markup_printf_escaped (formatting, statusMessage);
    gtk_label_set_markup (GTK_LABEL(labelStatusMessage), markup);
    // updateStatusMessage("Secured: VPN connected!", "green");
    gtk_button_set_label(connectButton, "Disconnect");
  } else if (!connectedToVPN) {
    // gtk_label_set_text(GTK_LABEL(labelStatusMessage),"Exposed: VPN not connected!");
    statusMessage = "Exposed: VPN not connected!";
    formatting = "<span foreground=\"red\">%s</span>";
    markup = g_markup_printf_escaped (formatting, statusMessage);
    gtk_label_set_markup (GTK_LABEL(labelStatusMessage), markup);
    // updateStatusMessage("Exposed: VPN not connected!", "red");
    gtk_button_set_label(connectButton, "Connect");
  }
  if (remove(statusFile) == 0) {
    verbosePrint("deleted %s successfully", statusFile);
  } else {
    verbosePrint("unable to delete %s", statusFile);
  }
  checkStatusIteration++;
  verbosePrint("executed checkStatus, iteration %d\n=========================================", checkStatusIteration);
  return TRUE;
}


void populateCountryList() {
  // gtk_combo_box_text_remove_all(countryList);
  // gtk_combo_box_text_append(countryList, "1", "Any Country");

  // return to original declaration if not working for cmd[MAXCHAR]
  char cmd[MAXCHAR];// = "cyberghostvpn --country-code | grep -E '[0-9]{1}' | tr -d '| ' | sed -e 's/[0-9]\\+/ & /g' -e 's/^ \\| $//' | wc -l";
  char countryListFile[MAXCHAR] = "/tmp/cgapplet-countrylist";
  // also uncomment this
  // int numCountrys = atoi(execCmdReturnOutput(cmd, countryListFile));
  // verbosePrint("number of countries detected: %d", numCountrys);
  strcpy(cmd, "cyberghostvpn --country-code | grep -E '[0-9]{1}' | tr -d '| ' | sed -e 's/[0-9]\\+/ & /g' -e 's/^ \\| $//'");
  cmdToTempFile(cmd, countryListFile);
  strcat(countryListFile, timeCode);

// ALT MORE EFFICIENT
  populateCountryArray(countryListFile);
  char lineNumber[3]="";
  for(int i=0; i<100; i++){
    if(countryCityArray[i][0][0]=='\0'){
      break;
    }
    sprintf(lineNumber, "%d", i+1);
    gtk_combo_box_text_append(countryList, lineNumber, countryCityArray[i][0]);

    int len = strlen(countryCityArray[i][0]);
    char *countryID = &countryCityArray[i][0][len-2];
    verbosePrint("countryID is %s", countryID);

    // char cityListFile[MAXCHAR] = "/tmp/cgapplet-citylist";
    // sprintf(cmd, "%s %s %s", "cyberghostvpn --country-code", countryID, "| grep -E '[0-9]{1}' | tr -d '| [0-9]%'");
    //cmdToTempFile(cmd, cityListFile);
    // verbosePrint("%s", execCmdReturnOutput(cmd, cityListFile));
  }

// TESTED, WORKS BUT INNEFFICIENT AS FUCK
  // for(int i = 0; i < numCountrys; i++) {
      // char countryListLineFile[MAXCHAR] = "/tmp/cgapplet-countrylistline";
      // char lineNumber[3]="";
      // sprintf(lineNumber, "%d", i+1);
      // // THIS COMMAND MAY BE UNNECESSARY, SINCE A LOT OF THE DATA CONDENSING IS ALREADY DONE
      // sprintf(cmd, "%s%s%s%s%s", "grep -E '[0-9]{1}' '", countryListFile, "' | tr -d '| ' | awk '{if(NR==", lineNumber, ") print $0}'");
      // verbosePrint("executing bash: %s", cmd);
      // gtk_combo_box_text_append(countryList, lineNumber, execCmdReturnOutput(cmd, countryListLineFile););
  // }
}

// TO DO: INSERT SPACE BETWEEN aB SO THAT COMMAND WORKS WHEN MORE THAN ONE WORD IN NAME OF CITY
void populateCityList() {
  gtk_combo_box_text_remove_all(cityList);
  gtk_combo_box_text_prepend(cityList, "0", "Any City");
  if(strcmp(gtk_combo_box_text_get_active_text(countryList), "Any Country")) {
  char cmd[MAXCHAR];
    sprintf(cmd, "%s %s %s", "cyberghostvpn --country-code", getSelectedCountryID(), "| grep -E '[0-9]{1}' | tr -d '| [0-9]%' | wc -l");
    char cityListFile[MAXCHAR] = "/tmp/cgapplet-citylist";
    int numCities = atoi(execCmdReturnOutput(cmd, cityListFile));
    verbosePrint("number of cities detected: %d", numCities);
    sprintf(cmd, "%s %s %s", "cyberghostvpn --country-code", getSelectedCountryID(),"| grep -E '[0-9]{1}' | tr -d '| [0-9]%'");
    cmdToTempFile(cmd, cityListFile);
    strcat(cityListFile, timeCode);
    for(int i = 0; i < numCities; i++) {
      char cityListLineFile[MAXCHAR] = "/tmp/cgapplet-citylistline";
      char lineNumber[3]="";
      sprintf(lineNumber, "%d", i+1);
      sprintf(cmd, "%s%s%s%s%s", "cat '", cityListFile, "' | awk '{if(NR==", lineNumber, ") print $0}'");
      verbosePrint("executing bash: %s", cmd);
      gtk_combo_box_text_append(cityList, lineNumber, execCmdReturnOutput(cmd, cityListLineFile));
    }
  }
}

void countrySelected() {
  char flagPath[MAXCHAR];
  char flagID[40];

  if(!strcmp(gtk_combo_box_text_get_active_text(countryList), "Any Country")) {
    strcpy(flagID, "_earth_pernefeldt");
  } else {
    strcpy(flagID, getSelectedCountryID());
    for(int i = 0; flagID[i]; i++){
      flagID[i] = tolower(flagID[i]);
    }
  }

  sprintf(flagPath, "%s%s%s", "resources/", flagID, ".png");
  verbosePrint("flagPath is %s", flagPath);


  gtk_image_set_from_file(flagIcon, getFilePath(flagPath));
  populateCityList();
}

void processArguments(char *argv[]) {
  char *argument[3];
  argument[0] = argv[1];
  if(argv[1] == NULL) {
    argument[0] = "null";
  }
  if(strcmp(argument[0], "verbose") == 0) {
    verbose = TRUE;
    printf("verbosity enabled\n");
  }
}

int main(int argc, char *argv[])
{
  printf("initializing program with argument %s\n", argv[1]);
  processArguments(argv);

  // GET AND STORE CURRENT TIME AT RUN AS timeCode STRING
  long int currentTime = time(NULL);
  sprintf(timeCode, "%ld", currentTime);
  verbosePrint("timecode is %s", timeCode);

  // DEFINE SUDO PROMPT COMMAND
  // GET RID OF INFINITE LOOP IN BASH SCRIPT, INSTEAD HAVE MSG "AUTHENTICATION FAILED"
  // strcpy(pwdPromptCmd, "while ! zenity --title=\"Authenticate cgapplet\" --password | sudo -S cat /dev/null >/dev/null; do if $(zenity --question --ellipsize --title=\"Wrong Password\" --text=\"Wrong password, would you like to cancel?\"); then echo \"exiting authenticator\"; exit; else echo \"trying password again\"; fi; done");
  // EVENTUALLY ADD AND READ RETURN VALUES FOR BASH SCRIPT TO DETERMINE SUCCESS, UPDATE LABEL TO SHOW AUTH FAILURE
  strcpy(pwdPromptCmd, "if ! zenity --title=\"Authenticate cgapplet\" --password | sudo -S cat /dev/null >/dev/null; then echo authentication failed; notify-send \"CGApplet authentication failed\"; exit; fi");

  GtkBuilder *builder;
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, getFilePath("cgapplet.ui"), NULL);//"/home/ohhaibook/Nextcloud/Coding/c/cgapplet/cgappletv2.glade", NULL);
  window = GTK_WIDGET(gtk_builder_get_object(builder, "cgappletWindow"));
  aboutDialog = GTK_DIALOG(gtk_builder_get_object(builder, "aboutDialog"));
  // statusIcon = GTK_STATUS_ICON(gtk_builder_get_object(builder, "statusIcon"));
  label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
  labelStatusMessage = GTK_WIDGET(gtk_builder_get_object(builder, "statusMessage"));
  flagIcon = GTK_IMAGE(gtk_builder_get_object(builder, "flagImage"));
  connectButton = GTK_BUTTON(gtk_builder_get_object(builder, "connectButton"));
  countryList = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "countryList"));
  cityList = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "cityList"));

  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);
  gtk_widget_show(window);


  populateCountryList();
  g_timeout_add(1000, checkStatus, NULL);

  gtk_main();
  // checkStatus();
  printf("program exiting\n");
  return 0;
}

void connectToVPN() {
  verbosePrint("connect button pressed, put connect process on separate thread in future");
  char cmd[MAXCHAR];
  // gtk_combo_box_get_active_id (countryList)
  if (connectedToVPN) {
    strcpy(cmd, "cyberghostvpn --stop");
    // sudoCmd("cyberghostvpn --stop");
  } else {
    sprintf(cmd, "%s %s %s %s %s", "cyberghostvpn --country-code", getSelectedCountryID(), "--city",  gtk_combo_box_text_get_active_text(cityList), "--connect");
    // gtk_button_set_label(connectButton, "Loading..."); // DOESNT WORK, LIKELY BECAUSE BUTTON GETS LOCKED BEFORE HAS A CHANCE TO RUN
    // sudoCmd("cyberghostvpn --country-code de --city frankfurt --connect");
  }
  verbosePrint("executing command %s", cmd);
  sudoCmd(cmd);
}

void showAboutDialog() {
  // gtk_widget_show(aboutDialog);
  gtk_dialog_run(aboutDialog);
}

// exit function
void exitCgapplet() {
    //gtk_widget_hide(window);
    gtk_main_quit();
}
