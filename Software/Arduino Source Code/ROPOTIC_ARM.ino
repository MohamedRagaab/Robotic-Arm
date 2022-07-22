

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SimpleModbusSlave.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
 uint8_t servonum = 0;
 int pulse  = 100;


 enum 
{     
  // just add or remove registers and you're good to go...
  // The first register starts at address 1
  ADC0,   // address in scada 4x1  
  ADC1,   // address in scada 4x2     
  ADC2,   // address in scada 4x3
  ADC3,   // address in scada 4x4
  ADC4,   // address in scada 4x5
  ADC5,   // address in scada 4x6
  angle1,  // address in scada 4x7
  angle2,   // address in scada 4x8
  angle3,    // address in scada 4x9
  grib,     // address in scada 4x10
  seq,    // // address in scada 4x10
  // leave this one
  TOTAL_REGS_SIZE 
  // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[TOTAL_REGS_SIZE]; // function 3 and 16 register array

void setup() {

  modbus_configure(9600, 1, 2, TOTAL_REGS_SIZE, 0);
  //Serial.begin(9600);
  //Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates

  yield();
  holdingRegs[angle1] = 100;
  holdingRegs[angle2] = 200;
}
void up_date(void);
// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 50;   // 60 Hz
  //Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
 // Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
 // Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  //holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
   //setServoPulse(1, 0.003) ;
  
   // pwm.setPWM(0, 0, 500);     //SG90 PULSE MAX = 550  PULSE MIN = 100
   // pwm.setPWM(1, 0, 500);     // MG995 CCW HIGH = 500 CCW LOW = 380   CW HIGH 100 CW LOW 300 


  modbus_update(holdingRegs); 
  //delayMicroseconds(50);
  
  pwm.setPWM(0, 0,holdingRegs[angle1]);
  pwm.setPWM(1, 0,holdingRegs[angle2]);
  pwm.setPWM(2, 0,holdingRegs[angle3]);
  pwm.setPWM(3, 0,holdingRegs[grib]);
 // pwm.setPWM(0, 0,500);

 if (holdingRegs[seq] == 1)
 
 {
   for(int i = 0 ; i<= 4 ; i++){
     holdingRegs[angle1] = 100;
     holdingRegs[angle2] = 280;
     holdingRegs[angle3] = 240;
     holdingRegs[grib] = 300;
     up_date();
     delay(2000);
     
     holdingRegs[grib] = 100;
     up_date();
     delay(1000);
     holdingRegs[angle3] = 160;
     holdingRegs[angle2] = 100;
     holdingRegs[angle1] = 500;
     holdingRegs[seq] = 0;
      up_date();
      delay(1000);
      holdingRegs[grib] = 300;
     up_date();
     delay(4000);
   }
     
  }
  
}

//********************************************************************
void setServoAngle(uint8_t n, double angle) {


  double pulse = ((angle*35)/18) + 150 ;
  
  
  pwm.setPWM(n, 0, pulse);
}
//**********************************************************************
void up_date(void)
{
  pwm.setPWM(0, 0,holdingRegs[angle1]);
  pwm.setPWM(1, 0,holdingRegs[angle2]);
  pwm.setPWM(2, 0,holdingRegs[angle3]);
  pwm.setPWM(3, 0,holdingRegs[grib]);
  modbus_update(holdingRegs);
  }
  //**********************************************************************

