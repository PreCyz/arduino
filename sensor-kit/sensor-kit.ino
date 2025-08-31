#include "Arduino_SensorKit.h"

// Uncomment line below if your SensorKit is the variant with the DHT20 sensor
// (It's marked IIC on the PCB and has a black cover, while the DHT11 sensor
// has a blue cover.)
#define Environment Environment_I2C

#define BUZZER 5
#define BUTTON  4
#define LED 6
#define POT A0
#define MIC A2
#define LIGHT A3

int pot_value;
bool button_state;
int mic_value;
int light_value;
float preassure;

void setup() {
  Serial.begin(9600);

  pinMode(MIC , INPUT);
  pinMode(LIGHT , INPUT);
  pinMode(BUTTON , INPUT);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUZZER, OUTPUT);
  
  Environment.begin();

  Oled.begin();
  Oled.setFlipMode(true);
  Oled.setFont(u8x8_font_amstrad_cpc_extended_r); 

  Accelerometer.begin();
  Pressure.begin();
}

void loop() {
  //cursor values are in characters, not pixels
  Oled.setCursor(0, 5);
  // If accelerometer and altimeter are queried too close to one another
  // this causes a hang, so we read this first.
  Oled.print("x:"); 
  Oled.print(Accelerometer.readX()); 
  Oled.print(" y:"); 
  Oled.print(Accelerometer.readY());        
  Oled.setCursor(0, 6);
  Oled.print("z:"); 
  Oled.print(Accelerometer.readZ());

  pot_value = analogRead(POT);
  button_state = digitalRead(BUTTON);

  if (button_state == true) {
    digitalWrite(LED, HIGH);
    tone(BUZZER, pot_value);
  } else {
    digitalWrite(LED, LOW);
    noTone(BUZZER);
  }

  Oled.setCursor(0, 3);
  Oled.print("Bt:"); 
  Oled.print(button_state);
  Oled.print(" BzPt: ");
  Oled.print(pot_value);
  Oled.print("Hz");

  Oled.setCursor(0, 0);
  Oled.print("Tmp: "); 
  Oled.print(Pressure.readTemperature());
  Oled.print(" ");
  Oled.print(Environment.readTemperature());

  preassure = Pressure.readPressure()/100;
  Oled.setCursor(0, 1);
  Oled.print("P: "); 
  Oled.print(preassure);
  Oled.print("hPA"); 

  Oled.setCursor(0, 2);
  Oled.print("Hum: ");
  Oled.print(Environment.readHumidity());
  Oled.print("%");

  light_value = analogRead(LIGHT);
  mic_value = analogRead(MIC);
  Oled.setCursor(0, 4);
  Oled.print("L:");
  Oled.print(light_value);
  Oled.print(" M:");
  Oled.print(mic_value);

  Oled.setCursor(0, 7);
  Oled.print("Alt:"); 
  Oled.print(Pressure.readAltitude());
  Oled.print("m"); 

  delay(100);
}
