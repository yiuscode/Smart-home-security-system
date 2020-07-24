#include <Wire.h>

//vars
long randNumber;
int doorSwitch = D4;
int buzzer = D3;
int led1 = D7;
int systemState = 0;
int doorState;
int buzzerState;
int cameraState = 0;
bool notifited = false;
bool triggred = false;
String ts;


//set up the system
void setup() {
  Time.zone(10);                //set up time zone as +10
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event for its master
  pinMode(doorSwitch, INPUT_PULLUP); //Read the pin 4 on the Argon board
  pinMode(led1, OUTPUT); //set the led pin mode as output
  pinMode(buzzer, OUTPUT); //set the buzzer pin mode as output
  
  
  //Cloud functions
  Particle.function("toggleSys", toggleSys);
  Particle.function("toggleCam", toggleCam);
  Particle.function("checkCam", getCameraStat);
  Particle.function("checkSys", getSysStat);
  Particle.function("checkDoor", getDoorStat);
}


//Main logic code

void loop() {
    
    //check if the alarm system is on
    
    if (systemState == 1)
    {
        // If so, monitor the door status
        
        if (digitalRead(doorSwitch) == HIGH) {
            
            //if the door is opened
            
            if(!notifited)
            {
                //Publish a message to cloud 
                //so that IFTTT server can notify the owner's mobile phone
                
                ts = String(Time.local());
                
                Particle.publish("door", "opened at \n" + Time.format(Time.now(), "%H:%M:%S %d-%m-%Y"));
                
                //Only notify once
                notifited = true;
            }
            
            //Turn on the inbuilt led (for debug use)
            digitalWrite(led1, HIGH);
            
            //Change the door stat as 1
            doorState = 1;
            
            //Turn on the buzzer (alarm)
            tone(buzzer, 1000, 800);
            
            triggred = true;
            
        }
        else {
            
            if(triggred){
                //The buzzer will remain on if the alarm is triggred.
                tone(buzzer, 1000, 800);  
            }
            
            //reset the notification status
            notifited = false;
            
            //Turn off the inbuilt led (for debug use)
            digitalWrite(led1, LOW);
            
            //Change the door stat as 0
            doorState = 0;
        }
        
    }
    else{
        //When the system is off
        
        //reset the notification status
        notifited = false;
        triggred = false;
        
        //reset the door status
        doorState = 0;
        
        //Turn off the buzzer
        noTone(buzzer);
    }

    //The loop runs every 1 second
    delay(1000);
  
}

//Functions for mobile control
//Based on the argument to do the
//corresponding action


//Turn on/off the alarm system
int toggleSys(String arg)
{
    if(arg=="0")
    {
        return systemState = 0;
    }
    else
    {
        return systemState = 1;
    }
}


//Turn on/off the camera stream
int toggleCam(String arg)
{
    if(arg=="0")
    {
        return cameraState = 0;
    }
    else
    {
        return cameraState = 1;
    }
}


//Getters for the mobile website
int getSysStat(String check)
{
    return systemState;
}

int getDoorStat(String check)
{
    return doorState;
}

int getCameraStat(String check)
{
    return cameraState;
}

//I2C request event
//Raspberry Pi will request the camera status value every two seconds
//Argon will return the camera status to Raspberry Pi by using wire.write
//If cameraState is 1, Raspberry Pi will turn on the camera and stream the live feed
//If cameraState is 0, Raspberry Pi will turn off the camera and  stream
void requestEvent() {
  Wire.write(cameraState);
}
