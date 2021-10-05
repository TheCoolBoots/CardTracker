// pin 3-5 = LEDS
// pin 7 = buzzer
// pin 9-11 = card sensors
// pin 13 = button

int LED_1 = 3;
int LED_2 = 4;
int LED_3 = 5;

int BUZZER = 7;

int CARD_SENSOR_1 = 9;
int CARD_SENSOR_2 = 10;
int CARD_SENSOR_3 = 11;

int BUTTON = 13;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

long timers[] = {0,0,0};
bool cardInPlace[] = {false, false, false};
bool cardInPlaceLastState[] = {true, true, true};
int LED_states[] = {0,0,0};

int blinkLED[] = {0,0,0};
bool ledOn[] = {false, false, false};

unsigned long TIME_ALLOWED_AWAY = 50000;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(CARD_SENSOR_1, INPUT);
  pinMode(CARD_SENSOR_2, INPUT);
  pinMode(CARD_SENSOR_3, INPUT);

  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(BUTTON);
  if(buttonState && !lastButtonState){
    reset();
  }

  // checkCard(0);
  checkCard(1);
  // checkCard(2);

  if(cardInPlace[0] && cardInPlace[1] && cardInPlace[2]){
    digitalWrite(BUZZER, LOW);
  }

  lastButtonState = buttonState;
}


void checkCard(int index){
  
  cardInPlace[index] = digitalRead(CARD_SENSOR_1 + index);
  
  if(!cardInPlace[index] && cardInPlaceLastState[index]){
    // when card gets taken out of wallet, start timer and start flashing LED
    LED_states[index] = 1;
    timers[index] = 0;
  }
  else if (cardInPlace[index] && !cardInPlaceLastState[index]){
    // card is returned to wallet; reset timers, turn off LEDs
    timers[index] = 0;
    LED_states[index] = 0;
  } 
  else if (!cardInPlace[index]){
    // if card is out, increment timer
    timers[index] ++;
    if(timers[index] > TIME_ALLOWED_AWAY){
      LED_states[index] = 2;
      if(timers[index] < TIME_ALLOWED_AWAY + .01 * TIME_ALLOWED_AWAY)
        digitalWrite(BUZZER, HIGH);
    }
  }
  else{
    LED_states[index] = 0;
  }


  
  if(LED_states[index] == 2){
    digitalWrite(LED_1 + index, HIGH);
  }
  else if(LED_states[index] == 1){
    blinkLEDfunc(index);
  }
  else{
    digitalWrite(LED_1 + index, LOW);
  }



  cardInPlaceLastState[index] = cardInPlace[index];
}


void blinkLEDfunc(int index){
  if(blinkLED[index] > 4000){
    ledOn[index] = !ledOn[index];
    digitalWrite(LED_1 + index, ledOn[index]);
    blinkLED[index] = 0;
  }
  blinkLED[index] += 1;
}


void reset(){
    digitalWrite(BUZZER, LOW);
}
