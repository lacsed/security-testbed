
// ----------------- LIBRARIES -----------------------------//

#include <Arduino.h>
#include <U8g2lib.h>
#include "Timer.h" 

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


// ----------------- DEFINITIONS -----------------------------//

#define ON 1
#define OFF 0


//tank dimensions
#define TANK_WIDTH 22
#define TANK_HEIGHT 25
#define TANK_POSITION_X 63
#define TANK_POSITION_Y 25
#define TANK_RADIUS 4

//valve dimensions
#define VALVE_HEIGHT 7
#define VALVE_WIDTH 9

//valve positions
#define VIN_POS_X 40
#define VIN_POS_Y 10

//valve position
#define VOUT_POS_X 90
#define VOUT_POS_Y 57

//heater dimensions
#define HEATER_EXCHANGER_WIDTH 21
#define DISTANCE_TANK_TO_HEATER_EXCHANGER 14
#define HEATER_HEIGHT 5

//mixer dimensions
#define HELICES_LENGTH 2
#define MIXER_LENGTH 12

//string position
#define STR_TEMP_POS_X 1
#define STR_TEMP_POS_Y 20


// ----------------- PINS -----------------------------//

#define PIN_VIN 31
#define PIN_VOUT 33
#define PIN_MIXER 35
#define PIN_PUMP 37
#define PIN_FAN_1 39
#define PIN_FAN_2 41
#define PIN_HEATER A8

#define PIN_LEVEL 11
#define PIN_TEMPERATURE 12

#define IN_FLOW 5;
#define OUT_FLOW 3;


// ----------------- CONSTRUCTORS -----------------------------//

//display constructor
U8G2_KS0108_128X64_F u8g2(U8G2_R0, 8, 9, 2, 3, 4, 5, 6, 7, A2, A0, A3, A4, U8X8_PIN_NONE, A5);







// ----------------- GLOBAL VARIABLES -----------------------------//

int state_mixer = 0;  
int state_fan_1 = 0;
int state_fan_2 = 0;
int state_pump = 0;
int state_heater = 0;
int tank_level = 0;
bool vin_open = true; //inverse logic because of the input pullup 
bool vout_open = true; //inverse logic because of the input pullup 
bool mixer_on = true;
bool pump_on = true;
bool fan1_on = true;
bool fan2_on = true;




unsigned long previousMillis = 0;  // used to count time


const long interval = 250;  // interval at which to blink (milliseconds)


void setup(void) {

  
  u8g2.begin();  

  pinMode(PIN_VIN, INPUT_PULLUP);
  pinMode(PIN_VOUT, INPUT_PULLUP);
  pinMode(PIN_MIXER, INPUT_PULLUP);
  pinMode(PIN_PUMP, INPUT_PULLUP);
  pinMode(PIN_FAN_1, INPUT_PULLUP);
  pinMode(PIN_FAN_2, INPUT_PULLUP);

 
  Serial.begin(9600);
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}




void u8g2_tank() {
  
  //u8g2.drawFrame(63,25,23,30);
  u8g2.drawEllipse(TANK_POSITION_X + (TANK_WIDTH/2), TANK_POSITION_Y, TANK_WIDTH/2, TANK_RADIUS);
  u8g2.drawVLine(TANK_POSITION_X, TANK_POSITION_Y, TANK_HEIGHT);
  u8g2.drawVLine(TANK_POSITION_X+TANK_WIDTH, TANK_POSITION_Y, TANK_HEIGHT);
  u8g2.drawEllipse(TANK_POSITION_X + (TANK_WIDTH/2), TANK_POSITION_Y+TANK_HEIGHT, TANK_WIDTH/2, TANK_RADIUS, U8G2_DRAW_LOWER_LEFT);
  u8g2.drawEllipse(TANK_POSITION_X + (TANK_WIDTH/2), TANK_POSITION_Y+TANK_HEIGHT, TANK_WIDTH/2, TANK_RADIUS, U8G2_DRAW_LOWER_RIGHT);
  
}

void u8g2_valve(uint8_t x, uint8_t y, bool status){

  u8g2.drawVLine(x, y, VALVE_HEIGHT);
  u8g2.drawVLine(x+VALVE_WIDTH, y, VALVE_HEIGHT);
  u8g2.drawLine(x+1, y, x+VALVE_WIDTH-1, y+VALVE_HEIGHT-1);
  u8g2.drawLine(x+1, y+VALVE_HEIGHT-1, x+VALVE_WIDTH-1, y);

  if(status){
    u8g2.drawVLine(x+1, y+1, VALVE_HEIGHT-2);
    u8g2.drawVLine(x+2, y+2, VALVE_HEIGHT-4);
    u8g2.drawVLine(x+3, y+3, VALVE_HEIGHT-5);
    u8g2.drawVLine(x+6, y+3, VALVE_HEIGHT-5);
    u8g2.drawVLine(x+7, y+2, VALVE_HEIGHT-4);
    u8g2.drawVLine(x+8, y+1, VALVE_HEIGHT-2);
  }

}


void u8g2_pipes(){

  u8g2.drawHLine(0, VIN_POS_Y+2, VIN_POS_X);
  u8g2.drawHLine(0, VIN_POS_Y+4, VIN_POS_X);
  
  u8g2.drawHLine(VIN_POS_X+VALVE_WIDTH+1, VIN_POS_Y+2, TANK_POSITION_X-(VIN_POS_X+VALVE_WIDTH)+6);
  u8g2.drawHLine(VIN_POS_X+VALVE_WIDTH+1, VIN_POS_Y+4, TANK_POSITION_X-(VIN_POS_X+VALVE_WIDTH)+4);
  
  u8g2.drawVLine(VIN_POS_X+VALVE_WIDTH+TANK_POSITION_X-(VIN_POS_X+VALVE_WIDTH)+7, VIN_POS_Y+2, TANK_POSITION_Y-VIN_POS_Y-1);
  u8g2.drawVLine(VIN_POS_X+VALVE_WIDTH+TANK_POSITION_X-(VIN_POS_X+VALVE_WIDTH)+5, VIN_POS_Y+4, TANK_POSITION_Y-VIN_POS_Y-3);
  
  u8g2.setDrawColor(0);
  u8g2.drawVLine(VIN_POS_X+VALVE_WIDTH+TANK_POSITION_X-(VIN_POS_X+VALVE_WIDTH)+6, VIN_POS_Y+5, TANK_POSITION_Y-VIN_POS_Y-3);
  u8g2.setDrawColor(1);

  u8g2.drawVLine(TANK_POSITION_X+(TANK_WIDTH/2)-1, TANK_POSITION_Y+TANK_HEIGHT+5, VOUT_POS_Y-(TANK_POSITION_Y+TANK_HEIGHT)-1);
  u8g2.drawVLine(TANK_POSITION_X+(TANK_WIDTH/2)+1, TANK_POSITION_Y+TANK_HEIGHT+5, VOUT_POS_Y-(TANK_POSITION_Y+TANK_HEIGHT)-3);
  
  u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)-1, VOUT_POS_Y+4, VOUT_POS_X-TANK_POSITION_X-(TANK_WIDTH/2)+1);
  u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)+1, VOUT_POS_Y+2, VOUT_POS_X-TANK_POSITION_X-(TANK_WIDTH/2)-1);
  
  u8g2.drawHLine(VOUT_POS_X+VALVE_WIDTH+1, VOUT_POS_Y+2, 128-(VOUT_POS_X+VALVE_WIDTH));
  u8g2.drawHLine(VOUT_POS_X+VALVE_WIDTH+1, VOUT_POS_Y+4, 128-(VOUT_POS_X+VALVE_WIDTH)); 
}

void u8g2_pump(char status){
    u8g2.drawCircle(TANK_POSITION_X+TANK_WIDTH+7, TANK_POSITION_Y+TANK_HEIGHT-2, 3);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+7, TANK_POSITION_Y+TANK_HEIGHT-5, 7);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+10, TANK_POSITION_Y+TANK_HEIGHT-3, 4);
    u8g2.setDrawColor(0);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+7, TANK_POSITION_Y+TANK_HEIGHT-4, 8);
    u8g2.setDrawColor(1);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH, TANK_POSITION_Y+TANK_HEIGHT-1, 4);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH, TANK_POSITION_Y+TANK_HEIGHT+1, 6);
    u8g2.drawFrame(TANK_POSITION_X+TANK_WIDTH+14, TANK_POSITION_Y-4, HEATER_EXCHANGER_WIDTH, TANK_HEIGHT+8);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH, TANK_POSITION_Y+3, DISTANCE_TANK_TO_HEATER_EXCHANGER);
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH, TANK_POSITION_Y+5, DISTANCE_TANK_TO_HEATER_EXCHANGER);

    if(status == 1){
      u8g2.drawDisc(TANK_POSITION_X+TANK_WIDTH+7, TANK_POSITION_Y+TANK_HEIGHT-2, 3);
    }
}


void u8g2_fans(char status_fan_1, char status_fan_2){
    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*3/4)+1, 1);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*3/4)+5,1, HELICES_LENGTH);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*3/4)-3,1, HELICES_LENGTH);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)-4, TANK_POSITION_Y+(TANK_HEIGHT*3/4)+1, HELICES_LENGTH,1);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)+4, TANK_POSITION_Y+(TANK_HEIGHT*3/4)+1, HELICES_LENGTH,1);

    u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*1/4)-2, 1);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*1/4)+2,1, HELICES_LENGTH);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*1/4)-6,1, HELICES_LENGTH);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)-4, TANK_POSITION_Y+(TANK_HEIGHT*1/4)-2, HELICES_LENGTH,1);
    u8g2.drawEllipse(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)+4, TANK_POSITION_Y+(TANK_HEIGHT*1/4)-2, HELICES_LENGTH,1);

    if(status_fan_1 == 1){
      u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)-HELICES_LENGTH-3, TANK_POSITION_Y+(TANK_HEIGHT*1/4)-2, HELICES_LENGTH+1);
    }
    else if(status_fan_1 == 2){
      u8g2.drawVLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*1/4)-HELICES_LENGTH-5, HELICES_LENGTH+1);
    }
    else if(status_fan_1 == 3){
      u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)+3, TANK_POSITION_Y+(TANK_HEIGHT*1/4)-2, HELICES_LENGTH+1);
    }
    else if(status_fan_1 == 4){
      u8g2.drawVLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*1/4)+1, HELICES_LENGTH+1);
    }

    if(status_fan_2 == 1){
      u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)-HELICES_LENGTH-3, TANK_POSITION_Y+(TANK_HEIGHT*3/4)+1, HELICES_LENGTH+1);
    }
    else if(status_fan_2 == 2){
      u8g2.drawVLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*3/4)-HELICES_LENGTH-2, HELICES_LENGTH+1);
    }
    else if(status_fan_2 == 3){
      u8g2.drawHLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2)+3, TANK_POSITION_Y+(TANK_HEIGHT*3/4)+1, HELICES_LENGTH+1);
    }
    else if(status_fan_2 == 4){
      u8g2.drawVLine(TANK_POSITION_X+TANK_WIDTH+DISTANCE_TANK_TO_HEATER_EXCHANGER+(HEATER_EXCHANGER_WIDTH/2), TANK_POSITION_Y+(TANK_HEIGHT*3/4)+4, HELICES_LENGTH+1);
    }
    
}

void u8g2_mixer(char status){
    u8g2.drawVLine(TANK_POSITION_X+(TANK_WIDTH/2), TANK_POSITION_Y+2, MIXER_LENGTH);
    u8g2.drawEllipse(TANK_POSITION_X+(TANK_WIDTH/2)-5, TANK_POSITION_Y+MIXER_LENGTH+2, 4, 2);
    u8g2.drawEllipse(TANK_POSITION_X+(TANK_WIDTH/2)+5, TANK_POSITION_Y+MIXER_LENGTH+2, 4, 2);
    u8g2.drawFrame(TANK_POSITION_X+(TANK_WIDTH/2)-1, TANK_POSITION_Y-1, 3, 3);

    if(status == 1){
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)-8, TANK_POSITION_Y+2+MIXER_LENGTH, 7);
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)-7, TANK_POSITION_Y+1+MIXER_LENGTH, 5);
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)-7, TANK_POSITION_Y+3+MIXER_LENGTH, 5);    
    }
    else if(status == 2){
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)+2, TANK_POSITION_Y+2+MIXER_LENGTH, 7);
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)+3, TANK_POSITION_Y+1+MIXER_LENGTH, 5);
      u8g2.drawHLine(TANK_POSITION_X+(TANK_WIDTH/2)+3, TANK_POSITION_Y+3+MIXER_LENGTH, 5);    
    }
}

void u8g2_heater(char status){

    if(status == 0){
      u8g2.drawVLine(TANK_POSITION_X+(TANK_WIDTH/3), TANK_POSITION_Y+(TANK_HEIGHT*4/5), HEATER_HEIGHT);
      u8g2.drawLine(TANK_POSITION_X+(TANK_WIDTH/3), TANK_POSITION_Y+(TANK_HEIGHT*4/5), TANK_POSITION_X+(TANK_WIDTH/3)+HEATER_HEIGHT-1, TANK_POSITION_Y+(TANK_HEIGHT*4/5)+HEATER_HEIGHT-1);
      u8g2.drawVLine(TANK_POSITION_X+(TANK_WIDTH/3)+HEATER_HEIGHT, TANK_POSITION_Y+(TANK_HEIGHT*4/5), HEATER_HEIGHT);
      u8g2.drawLine(TANK_POSITION_X+(TANK_WIDTH/3)+HEATER_HEIGHT, TANK_POSITION_Y+(TANK_HEIGHT*4/5), TANK_POSITION_X+(TANK_WIDTH/3)+2*HEATER_HEIGHT-1, TANK_POSITION_Y+(TANK_HEIGHT*4/5)+HEATER_HEIGHT-1);
    }
    
}


void u8g2_tank_level(char level){ //level between 0 and 100
  u8g2.drawVLine(TANK_POSITION_X-1, TANK_POSITION_Y, TANK_HEIGHT);
  u8g2.setDrawColor(0);
  u8g2.drawVLine(TANK_POSITION_X-1, TANK_POSITION_Y, (100-level)*TANK_HEIGHT/100);
  u8g2.setDrawColor(1);
}

void u8g2_strings(int temp, int level, int heater){
  u8g2.drawStr(STR_TEMP_POS_X, STR_TEMP_POS_Y, "Level:");
  u8g2.drawStr(STR_TEMP_POS_X, STR_TEMP_POS_Y+10, "Temp:");
  u8g2.drawStr(STR_TEMP_POS_X, STR_TEMP_POS_Y+20, "Heater:");

  u8g2.setCursor(STR_TEMP_POS_X+42, STR_TEMP_POS_Y);
  u8g2.print(level);  
  u8g2.setCursor(STR_TEMP_POS_X+42, STR_TEMP_POS_Y+10);
  u8g2.print(temp);
  u8g2.setCursor(STR_TEMP_POS_X+42, STR_TEMP_POS_Y+20);
  u8g2.print(heater);
  
}

void u8g2_strings_message(char message[]){
  u8g2.setCursor(STR_TEMP_POS_X, STR_TEMP_POS_Y+30);
  u8g2.print(message);
}

int aux = -1;

void loop(void) {
  u8g2.firstPage();
  u8g2_prepare();

  unsigned long currentMillis = millis();

  vin_open = !digitalRead(PIN_VIN);
  vout_open = !digitalRead(PIN_VOUT);
  mixer_on = !digitalRead(PIN_MIXER);
  pump_on = !digitalRead(PIN_PUMP);
  fan1_on = !digitalRead(PIN_FAN_1);
  fan2_on = !digitalRead(PIN_FAN_2);
  

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    // if (state_mixer == 0) {
    //   state_mixer = 1;
    // } else {
    //   state_mixer = 0;
    // }

    if(mixer_on){
      if(state_mixer == 2){
        state_mixer = 1;
      }
      else{
        state_mixer++;
      }
    }
    else{
      state_mixer = 0;
    }

    if(fan1_on){
      if(state_fan_1 == 4){
        state_fan_1 = 1;
      }
      else{
        state_fan_1++;
      }  
    }
    else{
      state_fan_1 = 0;
    }
    
    if(fan2_on){
      if(state_fan_2 == 4){
        state_fan_2 = 1;
      }
      else{
        state_fan_2++;
      }  
    }
    else{
      state_fan_2 = 0;
    }
    
    if(pump_on){
      if(state_pump == 1){
        state_pump = 0;
      }
      else{
        state_pump++;
      }
    }
    else{
      state_pump = 0;
    }
    
    
    if(state_heater == 1){
      state_heater = 0;
    }
    else{
      state_heater++;
    }

    if(!vin_open && tank_level <= 100){
      tank_level += IN_FLOW;
    }
    else if(!vout_open && tank_level >= 0){
      tank_level -= OUT_FLOW;
    }

    
    

    //Serial.println(tank_level);

  }


  do {
    u8g2_tank();
    u8g2_valve(VIN_POS_X, VIN_POS_Y, vin_open);
    u8g2_valve(VOUT_POS_X, VOUT_POS_Y, vout_open);
    u8g2_pipes();
    u8g2_pump(state_pump);
    u8g2_fans(state_fan_1, state_fan_2);
    u8g2_mixer(state_mixer);
    u8g2_heater(state_heater);
    u8g2_tank_level(tank_level);
    u8g2_strings(tank_level, tank_level, tank_level);

    if(tank_level>100){
      u8g2_strings_message("OVRFLW");
      Serial.println("teste");
    }    
    else if(tank_level<0){
      u8g2_strings_message("UNDRFLW");
    }    
    


  } while ( u8g2.nextPage() );
  //delay(1000);
}

