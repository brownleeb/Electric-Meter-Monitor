#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <syslog.h>

char powerfilename[60];
int kilowatts;
time_t lastwritetime;
int pulseFound = 0;
int pulsecount = 0;
int pin = -1;

void pulse()
{
  while (digitalRead(pin)==1) { delay(2); }
  pulseFound++;
}

int main(int argc, const char* argv[])
{

  if (argc == 2) {
    pin = atoi(argv[1]);
  }
  if (pin == -1 || argc > 2) {
    syslog(LOG_ERR, "usage: power <wiringPi pin number (3-6)>\n");
    exit(EXIT_FAILURE);
  }

  char pinStr[3];
  sprintf(pinStr, "%d", pin);

  sprintf(powerfilename,"/srv/http/secure-lanonly.8080/power/power/power%d.txt",pin);
  FILE *powerfile = fopen (powerfilename, "r");
  fscanf (powerfile, "%d", &kilowatts);
  fclose (powerfile);
  time(&lastwritetime);

  if (piHiPri(99) != 0)
  {
    syslog(LOG_ERR, "pin%s wiringPi setting high-priority failed %s", pinStr, strerror(errno)); }

  if (wiringPiSetup() != 0)
  {
    syslog(LOG_ERR, "pin%s wiringPi setup failed %s", pinStr, strerror(errno));
    exit(EXIT_FAILURE);
  }
  pullUpDnControl(pin,PUD_DOWN);
  if (wiringPiISR(pin, INT_EDGE_FALLING, &pulse) != 0)
  {
    syslog(LOG_ERR, "pin%s failed setup ISR %s", pinStr, strerror(errno));
    exit(EXIT_FAILURE);
  }

  syslog(LOG_ERR, "Setup Complete");
  for (;;)
  {
    if (pulseFound > 0) {
      kilowatts=kilowatts+pulseFound;
      pulsecount=pulseFound+pulsecount;
      pulseFound=0;
      
      if (((time(NULL) - lastwritetime) > 3600) || (pulsecount > 100))  {
           FILE *powerfile = fopen (powerfilename, "w");
           fprintf (powerfile, "%d", kilowatts);
           fclose (powerfile);
           time(&lastwritetime);
           pulsecount = 0;
           }

      }

  sleep(10);

  }
  exit(EXIT_SUCCESS);
}

