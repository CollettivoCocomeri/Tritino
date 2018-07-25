#include <Max6675.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
//MAX6675
#define Ts_Pin_Dt 9
#define Ts_Pin_Ss 8
#define Ts_Pin_Clk 7
#define Ts_Offset 0 // offset for temperature measurement.1 stannds for 0.25 Celsius

Max6675 ts(Ts_Pin_Dt, Ts_Pin_Ss, Ts_Pin_Clk, Ts_Offset);

const int analogIn = A7;
int mVperAmp = 66; 
int RawValue = 0;
int ACSoffset = 2500;
double Voltage = 0;
double Amps = 0;

ClickEncoder *encoder;
int16_t last, value;

#define in1 2
#define in2 3




int rotDirection = 0;
int pressed = false;

void timerIsr() {
  encoder->service();
}

void setup() {
  Serial.begin(9600);

  //LED RGB
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite (12, HIGH);
  digitalWrite (11, HIGH);
  digitalWrite (10, HIGH);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  encoder = new ClickEncoder(A1, A0, A2);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  last = -1;
  delay (1000);
}

void loop() {


  //Encoder
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
        VERBOSECASE(ClickEncoder::Pressed);
      case ClickEncoder::Released:

        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite (12, HIGH);
        digitalWrite (11, LOW);
        digitalWrite (10, LOW);
        break;
      case ClickEncoder::Held:
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite (12, HIGH);
        digitalWrite (11, LOW);
        digitalWrite (10, HIGH);
        break;

      case ClickEncoder::Clicked:

        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite (12, HIGH);
        digitalWrite (11, LOW);
        digitalWrite (10, LOW);

        break;

      case ClickEncoder::DoubleClicked:
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite (12, HIGH);
        digitalWrite (11, HIGH);
        digitalWrite (10, HIGH);
        break;
    }
  }

  ts.getCelsius();
  if ( ts.getCelsius() > 90) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite (12, LOW);
    digitalWrite (11, LOW);
    digitalWrite (10, HIGH);
  }

  //ACS
  RawValue = analogRead(analogIn);
  Voltage = (RawValue / 1024.0) * 5000;
  Amps = ((Voltage - ACSoffset) / mVperAmp);
  Serial.print("\t Amps = "); 
  Serial.println(Amps, 3); 
  Serial.print("C = ");
  Serial.println(ts.getCelsius());

  delay(100);
  
   if ( Amps > 3) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite (12, LOW);
    digitalWrite (11, HIGH);
    digitalWrite (10, HIGH);

  }
     if ( Amps > 5) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite (12, HIGH);
    digitalWrite (11, LOW);
    digitalWrite (10, LOW);

  }
  
}

