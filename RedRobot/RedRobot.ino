#include <PS2X_lib.h>  //for v1.6

#include <AFMotor.h>

AF_DCMotor motorFL(2);
AF_DCMotor motorBL(3);
AF_DCMotor motorFR(1);
AF_DCMotor motorBR(4);
/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        A0      
#define PS2_CMD        A1  
#define PS2_SEL        A2  
#define PS2_CLK        A3  

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   true
//#define pressures   false
#define rumble      true
//#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
 
  Serial.begin(57600); 
  motorFL.run(RELEASE);
  motorBL.run(RELEASE);
  motorFR.run(RELEASE);
  motorBR.run(RELEASE);
  
  delay(1000);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  

  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      //Serial.print("Unknown Controller type found ");
      break;
    case 1:
      //Serial.print("DualShock Controller found ");
      break;
    case 2:
      //Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      //Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
  
  
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button

     // half speed mode or turbo speed mode 
    
    int left_spd  = (ps2x.Analog(PSS_LY)-127)*2-1;
    int right_spd = (ps2x.Analog(PSS_RY)-127)*2-1;
    if(left_spd < 0){
      motorFL.run(BACKWARD);
      motorBL.run(BACKWARD);
      if(ps2x.Button(PSB_L3)){
        left_spd = 20;
      } else if(ps2x.Button(PSB_R3)){
        left_spd = 256;
      }
      motorFL.setSpeed(-left_spd);
      motorBL.setSpeed(-left_spd);
      Serial.println(-left_spd);
    }
    else{
      motorFL.run(FORWARD);
      motorBL.run(FORWARD);
      if(ps2x.Button(PSB_L3)){
        left_spd = 20;
      } else if(ps2x.Button(PSB_R3)){
        left_spd = 256;
      }
      motorFL.setSpeed(left_spd);
      motorBL.setSpeed(left_spd);
      Serial.println(left_spd);
    }
    if(right_spd < 0){
      motorFR.run(BACKWARD);
      motorBR.run(BACKWARD);
      if(ps2x.Button(PSB_L3)){
        right_spd = 20;
      } else if(ps2x.Button(PSB_R3)){
        right_spd = 256;
      }
      motorFR.setSpeed(-right_spd);
      motorBR.setSpeed(-right_spd);
      Serial.println(-right_spd);
    }
    else{
      motorFR.run(FORWARD);
      motorBR.run(FORWARD);
      if(ps2x.Button(PSB_L3)){
        right_spd = 20;
      } else if(ps2x.Button(PSB_R3)){
        right_spd = 256;
      }
      motorFR.setSpeed(right_spd);
      motorBR.setSpeed(right_spd);
      Serial.println(right_spd);
    }  
   
	 // strict speed for DPad 
    int strict_speed = 200;
    
    if(ps2x.Button(PSB_PAD_UP)){
      motorBL.run(FORWARD);
      motorBR.run(FORWARD);
      motorBL.setSpeed(strict_speed);
      motorBR.setSpeed(strict_speed);
      Serial.println(strict_speed);
    } 
    if(ps2x.Button(PSB_PAD_DOWN)){
      motorBL.run(BACKWARD);
      motorBR.run(BACKWARD);
      motorBL.setSpeed(strict_speed);
      motorBR.setSpeed(strict_speed);
      Serial.println(strict_speed);
    }
  }  
  
    
  delay(50);  
}
