/*
 * Electro Digital Therapy control
 * 
 * Control switches of EDT via relays switched from micro board, probably from web page
 * Random delayed on/off ,random mode, random start level, time based incrementing
 * V0 - On Startup:
 *    -leds for delay power on work
 *    -leds for mode change work
 *    -leds for power levels on work
 * V1 - working with time increments and changing power levels and modes
 *    - time increment achieved
 *    - power level increment achieved
 * V2 - at max power level =13, change to different mode
 */

// Define constants

const int maxTimeOn = 15;           //device max on time
const long timeMulitplier = 1000;  // mulitplier to convert minutes to mircoseconds...varible for testing
const int relayOntime = 1000;      // time to leave relays activated on 
const int delay_ms = 1000;
bool StartUp = false;              // use to set up start values
 
 //define variables
  long timeNow;           //use for millis()
  long onTime = 10000;            //ms time to stay on for. Get from analog or other 
  long timeDelayOn;       // delay before switching on
  int currentMode = 1;       // current mode set
  int nextMode;          //mode to switch to
  int startLevel = 1;        // starting power level
  long levelIncrementTime; // time between increasing levels
  long IncrementTime; // time between increasing levels
  int levelIncrements;   // number of steps increase power level after each time increment
  int currentLevel = 1;      // current power level
  int newLevel;          // next power level

 //define arrays to get random values
  int timeDelayOnArray[5] = {2.5, 3, 3.5, 4, 4.5};    // hold delay time before switching on
  int nextModeArray[] = {1,2,3,4,5,6,7,8};  // hold current mode
  int startLevelArray[] = {4,5,6,7,8};      // hold power levelto start with ;
  long levelIncrementTimeArray[] = {1,2,3}; // hold time between increasing levels
  long segmentTimeArray[] = {2, 3, 4};        // divide segemnts into timeOn between increasing levels
  int levelIncrementsArray[] = {1,2,3,4};    // increments to step level by

//define pins
//outputs to relays
const int TimeIn = 10;       //Set on analog pin if reading time in on display
const int Power = 11;
const int Mode = 2;
const int LevelUp = 3;
const int LevelDown = 4;
const int Time = 5;
// if using mag lock
const int MagLockLock1 = 6;
const int MagLockLock2 = 7;
const int MagLockSwitch1 = 8;
const int MagLockSwitch2 = 9;

//***********************************************
//preload functions
void startUp();
void timeOnDelay();
void modeChoose();
void startingLevel();
void incrementMode();
void incrementLevel();
void incrementTime();
void printSerial();

//*************************************************

//**************************************************
void setup() {
// initialize serial comms 9600 
  Serial.begin(9600);
  
//define pins input/outputs
pinMode(MagLockSwitch1, INPUT);
pinMode(MagLockSwitch2, INPUT);
pinMode(Power, OUTPUT);
pinMode(Mode, OUTPUT);
pinMode(LevelUp, OUTPUT);
pinMode(LevelDown, OUTPUT);
pinMode(Time, OUTPUT);
pinMode(MagLockLock1, OUTPUT);
pinMode(MagLockLock2, OUTPUT);

//set Output states
digitalWrite(Power, LOW);
digitalWrite(Mode, LOW);
digitalWrite(Time, LOW);
digitalWrite(LevelUp, LOW);
digitalWrite(LevelDown, LOW);
digitalWrite(MagLockLock1, LOW);
digitalWrite(MagLockLock2, LOW);


//initialise setup for power up
      timeOnDelay();
      modeChoose();
      startingLevel();
      incrementTime();
      incrementLevels();
       Serial.println("Initial setup:" );
       printSerial();
        Serial.println("........" ); Serial.println("........" );
      delay(delay_ms);   //delay for testing
      delay(delay_ms);   //delay for testing

}

void loop() 
  {

  timeNow = millis();
  //is it time to switch unit on  
  if (StartUp == false) 
      {
        
  // test for delay start time passed
        if (timeNow > timeDelayOn)
  // if true, switch on and set mode and power levels
            {  
              startUp();
              incrementMode();//mode
              incrementLevel(); //power level
              }
        }
        
   printSerial();
 delay(delay_ms);   //delay for testing
 delay(delay_ms);   //delay for testing

 // is it time to change power level?
 if ((timeNow - IncrementTime)> levelIncrementTime)
    {//increment power level by levelIncrements
      newLevel = currentLevel + levelIncrements;
          if (newLevel >= 13)
                {nextMode = nextMode +levelIncrements;
                 incrementMode();
                }
          else
              {
               //for loop to increase current Level to next Level
               for (currentLevel ;currentLevel <= newLevel; ++currentLevel)
                  {
                    digitalWrite(LevelUp, HIGH);
                    delay(relayOntime);
                    digitalWrite(LevelUp, LOW);
                    delay(delay_ms);   //delay for testing
                    Serial.print("currentLevel:" );
                    Serial.println(currentLevel);
                   }
              }
        
     currentLevel = newLevel;  
      
    }



 
  }

void printSerial()
  {
  Serial.print("timeDelayOn:" );
  Serial.println(timeDelayOn);
  Serial.print("currentMode:" );
  Serial.println(currentMode);
  Serial.print("nextMode:" );
  Serial.println(nextMode);
  Serial.print("currentLevel:" );
  Serial.println(currentLevel);
  Serial.print("newLevel:" );
  Serial.println(newLevel);
  }

//Get values for start up
void startUp()
{
  StartUp = true;
  digitalWrite(Power, HIGH);
  delay(relayOntime);
  digitalWrite(Power, LOW);
  delay(delay_ms);   //delay for testing
  Serial.print("Power on: " );
  Serial.println(StartUp);
}
  
void timeOnDelay()
  {
  // timeDelayOnArray[4]
  int randIndex = (int)random(0, 4);
  int timeDelay = timeDelayOnArray[randIndex];    
  timeDelayOn = timeDelay * timeMulitplier;
 //Serial.println(timeDelayOn);
  }

void modeChoose()
  {
  // nextModeArray[] = {1,2,3,4,5,6,7,8};
  int randIndex = (int)random(0, 8);
  nextMode = nextModeArray[randIndex];    
  // Serial.println(nextMode);
 }

 void startingLevel()
 {
   //startLevelArray[] = {4,5,6,7,8};
  int randIndex = (int)random(0, 5);
  newLevel = startLevelArray[randIndex];
  }

 void incrementMode(){
  if (currentMode != nextMode)
   {
     //for loop to decrease current mode to next mode
          for (currentMode ;currentMode >= nextMode; --currentMode)
              {
              digitalWrite(Mode, HIGH);
              delay(relayOntime);
              digitalWrite(Mode, LOW);
              delay(delay_ms);   //delay for testing
              Serial.print("currentMode:" );
              Serial.println(currentMode);
              }
        
    //for loop to increase current mode to next mode
          for (currentMode ; currentMode <= nextMode; ++currentMode)
              {
              digitalWrite(Mode, HIGH);
              delay(relayOntime);
              digitalWrite(Mode, LOW);
              delay(delay_ms);   //delay for testing
             Serial.print("currentMode:" );
              Serial.println(currentMode);
              }
        
     currentMode = nextMode;   
    }
 }

   void incrementLevel(){
    if (currentLevel != newLevel)
   {
     //for loop to increase current Level to next Level
          for (currentLevel ;currentLevel >= newLevel; --currentLevel)
              {
              digitalWrite(LevelDown, HIGH);
              delay(relayOntime);
              digitalWrite(LevelDown, LOW);
              delay(delay_ms);   //delay for testing
              Serial.print("currentLevel:" );
              Serial.println(currentMode);
              }
        
    //for loop to increase current Level to next Level
          for (currentLevel ;currentLevel <= newLevel; ++currentLevel)
              {
              digitalWrite(LevelUp, HIGH);
              delay(relayOntime);
              digitalWrite(LevelUp, LOW);
              delay(delay_ms);   //delay for testing
             Serial.print("currentLevel:" );
              Serial.println(currentLevel);
              }
        
     currentLevel = newLevel;   
    }
    }

 void incrementTime()
 {
  // segmentTimeArray[] = {2,2,3};
  //onTime
  int randIndex = (int)random(0, 4);
  int divideBy = segmentTimeArray[randIndex];    
  levelIncrementTime = onTime / divideBy;
 Serial.print("levelIncrementTime: ");
 Serial.println(levelIncrementTime);
 }

 void incrementLevels()
{
   // levelIncrementsArray[] = {1,2,3,4};    // increments to step level by
  int randIndex = (int)random(0, 4);
  levelIncrements = levelIncrementsArray[randIndex];    
  Serial.print("levelIncrement: ");
  Serial.println(levelIncrements);
}
