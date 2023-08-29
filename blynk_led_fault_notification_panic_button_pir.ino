#define LIGHT_SENSOR_NIGHT 36                     // day night detection ESP32 pin GIOP36 vp pin (ADC0)
#define LIGHT_SENSOR_A 39                         // ESP32 pin vn     fault detction
#define LIGHT_SENSOR_B 34                         // ESP32 pin 34     fault detction
#define LIGHT_SENSOR_C 35                         // ESP32 pin 35     fault detction
#define LIGHT_SENSOR_D 32                         // ESP32 pin 32     fault detction
#define DHTTYPE DHT11
#define DHTPIN 15
#define BUTTON1_PIN 16                             // ESP32 GIOP16 pin connected to button's pin
#define BUTTON2_PIN 17
#define BUZZER_PIN 4                             // ESP32 GIOP4 pin connected to Buzzer's pin
#define STATUS_PIN 2
const int PIR_SENSOR_OUTPUT_PIN = 27;  /* PIR sensor O/P pin */

// for blynk server initialization
#define BLYNK_TEMPLATE_ID "TMPL1nGImfjV"
#define BLYNK_DEVICE_NAME "led fault detection"
#define BLYNK_AUTH_TOKEN "DJ3f9UQPmfifOj0zD9ScezPVbb4SMvga"
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi Note 11";
char pass[] = "*********";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

WidgetLED led(V1);

const int PIR_SENSOR = 27; //pir
int warmup;



const int ledPinA = 18;                         // LEDA corresponds to GPIO18
const int ledPinB = 19;                         // LEDB corresponds to GPIO19
const int ledPinC = 21;                         // LEDC corresponds to GPIO21
const int ledPinD = 3;                          // LEDD corresponds to rx0 pin

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);


  pinMode(PIR_SENSOR, INPUT); //pir

  
  pinMode(BUTTON1_PIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(BUTTON2_PIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(BUZZER_PIN, OUTPUT);       // set ESP32 pin to output mode
  pinMode(STATUS_PIN, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPinA, ledChannel);
  ledcAttachPin(ledPinB, ledChannel);
  ledcAttachPin(ledPinC, ledChannel);
  ledcAttachPin(ledPinD, ledChannel);

  dht.begin();
  timer.setInterval(1000L, sendSensor);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(500L, mysensor);
}


void loop() {
   Blynk.run();
   timer.run();
   digitalWrite(STATUS_PIN, HIGH); // turn on
   
 int buttonState1 = digitalRead(BUTTON1_PIN); // read new state

  if (buttonState1 == LOW) {
    Serial.println("The button 1 is being pressed");
     digitalWrite(BUZZER_PIN, HIGH); // turn on
    Blynk.virtualWrite(V3,"EMERGENCY ALERT!!");
    Blynk.virtualWrite(V4,"Near lamp pole A");
      for(int dutyCycle =230 ; dutyCycle <= 255; dutyCycle++)
          {
          // changing the LED brightness with PWM
          ledcWrite(ledChannel, dutyCycle);
          delay(5);
          }

          for(float dutyCycle = 10; dutyCycle >= 0; dutyCycle--)
          {
          // changing the LED brightness with PWM
          ledcWrite(ledChannel, dutyCycle);
          delay(5);
          }
    
  }
  else
  if (buttonState1 == HIGH) {
    digitalWrite(BUZZER_PIN, LOW);  // turn off
    Blynk.virtualWrite(V3,"   ");
    Blynk.virtualWrite(V4,"  ");
  }

//button 2
   int buttonState2 = digitalRead(BUTTON2_PIN); // read new state

  if (buttonState2 == LOW) {
    Serial.println("The button 2 is being pressed");
     digitalWrite(BUZZER_PIN, HIGH); // turn on
    Blynk.virtualWrite(V3,"EMERGENCY ALERT!!");
    Blynk.virtualWrite(V4,"Near lamp pole C");
      for(int dutyCycle =230 ; dutyCycle <= 255; dutyCycle++)
          {
          // changing the LED brightness with PWM
          ledcWrite(ledChannel, dutyCycle);
          delay(5);
          }

          for(float dutyCycle = 10; dutyCycle >= 0; dutyCycle--)
          {
          // changing the LED brightness with PWM
          ledcWrite(ledChannel, dutyCycle);
          delay(5);
          }
    
  }
  else
  if (buttonState2 == HIGH) {
    digitalWrite(BUZZER_PIN, LOW);  // turn off
    Blynk.virtualWrite(V3,"   ");
    Blynk.virtualWrite(V4,"  ");
  }
 }

  
void mysensor()
{

  // reads the input on analog pin (value between 0 and 4095)
  int analogValue = analogRead(LIGHT_SENSOR_NIGHT);

  //Serial.print("Analog Value = ");
  Serial.print(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 500) {
    Blynk.virtualWrite(V0, "Night");
    led.on();
    Serial.println("                                        =>Night time");
    for(int dutyCycle =240 ; dutyCycle <= 255; dutyCycle++)
    {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(5);
    }
    
   //fault detection logic
  // reads the input on analog pin (value between 0 and 4095)
  int analogValue_A = analogRead(LIGHT_SENSOR_A);
  int analogValue_B = analogRead(LIGHT_SENSOR_B);
  int analogValue_C = analogRead(LIGHT_SENSOR_C);
  int analogValue_D = analogRead(LIGHT_SENSOR_D);
  
  // the raw analog reading
 /* Serial.print("\nAnalog Value A = ");
  Serial.print(analogValue_A);   
  Serial.print("\nAnalog Value B = ");
  Serial.print(analogValue_B);   
  Serial.print("\nAnalog Value C = ");
  Serial.print(analogValue_C);   
  Serial.print("\nAnalog Value D = ");
  Serial.print(analogValue_D);   */ 

  // lamp fault detection
  
    if (analogValue_A < 2400){ 
    Blynk.virtualWrite(V2,"Lamp A is Fault");
    Blynk.logEvent("faulty_light");
    Serial.println(" \n=> fault on lamp A !!!");
    } else if (analogValue_B < 2400) {
    Blynk.virtualWrite(V2,"Lamp B is Fault");
    Blynk.logEvent("faulty_light");
    Serial.println(" \n=> Fault on Lamp B !!!");
    }else if (analogValue_D < 2400) {
    Blynk.virtualWrite(V2,"Lamp D is Fault");
    Blynk.logEvent("faulty_light");
    Serial.println(" \n=> fault on lamp D !!!");
    }else if (analogValue_C < 2100) {
    Blynk.virtualWrite(V2,"Lamp C is Fault");
    Blynk.logEvent("faulty_light");
    Serial.println(" \n=> fault on lamp C !!!");
    }else {
    Blynk.virtualWrite(V2,"Everything Fine");
    Serial.println("           => All lamps are Fine");
     }
  delay(30);

 
}

    /*else if(analogValue < 1000){
      Serial.println("                                         =>Cloudy");
      Blynk.virtualWrite(V0,"Cloudy");

      for(int dutyCycle =40 ; dutyCycle <= 55; dutyCycle++)
    {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(5);
    }
 } */

      
      else{
    Serial.println("                                         =>Day time");
    Blynk.virtualWrite(V0,"Day");
    led.off();
      //Blynk.virtualWrite(V2,"   ");
    // decrease the LED brightness
    for(float dutyCycle = 0.3; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(1);
    }
  }

}



void sendSensor()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

BLYNK_WRITE(V7){
  if(param.asInt()==1)
  {
   int sensor_out;
   sensor_out = digitalRead(PIR_SENSOR_OUTPUT_PIN);
   if(sensor_out == LOW )
   {
    if( warmup == 1)
    {
      warmup = 0;
      delay(1000);
    }
    Serial.print("no human\n")
   }
    else
    {
      S
      }

}
