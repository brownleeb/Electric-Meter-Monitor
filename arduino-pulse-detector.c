/*---------------------------
 IR beam demo
 ----------------------------*/

#define LED 8
#define BOARD_LED 13
#define BEAM A0
int threshold = 30;
void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(BEAM,INPUT);
  pinMode(BOARD_LED,OUTPUT);
  digitalWrite(LED,LOW);
  digitalWrite(BOARD_LED,LOW);
}

static unsigned char vals[10];
#define K 5
int median5(void)
{
  int i, j, t;

  for (i = 0; i < K - 1; i++) {
    for (j = i + 1; j < K; j++) {
      if (vals[i] > vals[j]) {
        t = vals[i];
        vals[i] = vals[j];
        vals[j] = t;
      }
    }
  }
  return vals[K / 2];
}

int medianfilter5(void)
{
  int i;
  for (i = 0; i < K - 1; i++) {
    vals[i] = analogRead(BEAM);  
  }
  return median5();
}

void loop()
{  
    Serial.println(medianfilter5());
    if (medianfilter5()>20) {
    Serial.println(medianfilter5());
    digitalWrite(LED,HIGH); 
    digitalWrite(13,HIGH);
    while (medianfilter5()>10){
      if (medianfilter5()<3) {
        break;
      }
    }
  }
  digitalWrite(LED,LOW);  
  digitalWrite(BOARD_LED,LOW);

}





