#include <Joystick.h>
#include <FastLED.h>
// ----General setup---
int toggleSelector(int); //define method for later use.
void toggleStrumLight(int);
void lightState(bool, int, int);
void setHueDirection(int &, bool &, int);
bool muxButtons(bool muxSelector, int data_in);
// LED setup
#define NUM_LEDS 10       // How many LED are in the strip
#define DATA_PIN 8        //Which pin is the LED plugged into
#define SECOND_DATA_PIN 7 //Which pin is the LED plugged into
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
//----Global variables----
//Create Joystick
Joystick_ Joystick;
//upper buttons
int GREEN = 13;
int RED = 12;
int YELLOW = 11;
int BLUE = 10;
int ORANGE = 9;
//lower buttons
int lOW_GREEN = 0;
int lOW_RED = 1;
int lOW_YELLOW = 2;
int lOW_BLUE = 3;
int lOW_ORANGE = 4;
//strumbar
int STRUM_UP = 7;
int STRUM_DOWN = 6;
int TILT = 5;

/**
If I had a micro then these would be
A6 and A7, and I would put D_LEFT and D_RIGHT
to A0 and A1.
**/
int WHAMMY = A0; //A6 if using Arduino Micro
int TOGGLE_SWITCH = A1; //A6 if using Arduino Micro

//game pad
int MINUS = A2;
int PLUS = A3;
int D_UP = A4;
int D_DOWN = A5;
// int D_LEFT = A0; //only if using Arduino Micro
// int D_RIGHT = A1; //only if using Arduino Micro
 
int strobeLight = 0;
// bool isWhammy = false;
bool toggleHueUp = true;
bool strumLight = true;
void setup()
{
  // Upper buttons (SCL, SDA, D13, D12, D11)
  pinMode(GREEN, INPUT_PULLUP);
  pinMode(RED, INPUT_PULLUP);
  pinMode(YELLOW, INPUT_PULLUP);
  pinMode(BLUE, INPUT_PULLUP);
  pinMode(ORANGE, INPUT_PULLUP);
  //Lower buttons
  pinMode(lOW_GREEN, INPUT_PULLUP);
  pinMode(lOW_RED, INPUT_PULLUP);
  pinMode(lOW_YELLOW, INPUT_PULLUP);
  pinMode(lOW_BLUE, INPUT_PULLUP);
  pinMode(lOW_ORANGE, INPUT_PULLUP);
  //strumbar
  pinMode(STRUM_DOWN, INPUT_PULLUP);
  pinMode(STRUM_UP, INPUT_PULLUP);
  //Gamepad
  pinMode(PLUS, INPUT_PULLUP);
  pinMode(MINUS, INPUT_PULLUP);
  pinMode(D_UP, INPUT_PULLUP);
  pinMode(D_DOWN, INPUT_PULLUP);
  // pinMode(D_LEFT, INPUT_PULLUP); //Uncomment if using Arduino Micro
  // pinMode(D_RIGHT, INPUT_PULLUP); //Uncomment if using Arduino Micro


  //tilt
  pinMode(TILT, INPUT_PULLUP);
  // Start Joystick
  Joystick.begin();
  //set up LED
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical
}

void loop()
{

  //set up each button variable
  int buttonGreen = digitalRead(GREEN);
  int buttonRed = digitalRead(RED);
  int buttonYellow = digitalRead(YELLOW);
  int buttonBlue = digitalRead(BLUE);
  int buttonOrange = digitalRead(ORANGE);
  //LOWER BUTTONS
  int buttonLowGreen = digitalRead(lOW_GREEN);
  int buttonLowRed = digitalRead(lOW_RED);
  int buttonLowYellow = digitalRead(lOW_YELLOW);
  int buttonLowBlue = digitalRead(lOW_BLUE);
  int buttonLowOrange = digitalRead(lOW_ORANGE);

  //strum
  int strumUp = digitalRead(STRUM_UP);
  int strumDown = digitalRead(STRUM_DOWN);
  //tilt
  int tilt = digitalRead(TILT);
  //Gamepad section
  int buttonMinus = digitalRead(MINUS); 
  int buttonPlus = digitalRead(PLUS);
  int dPadUp = digitalRead(D_UP);
  int dPadDown = digitalRead(D_DOWN);
  // int dPadLeft = digitalRead(D_LEFT); //Uncomment if using Arduino Micro
  // int dPadRight = digitalRead(D_RIGHT); //Uncomment if using Arduino Micro

  //set up Potentiometers
  int WhammyBar = analogRead(WHAMMY);
  int toggleSwitch = analogRead(TOGGLE_SWITCH);

  //DEBUGGING Potentiometers
  Serial.print("Analog: ");
  // Serial.print(WhammyBar);
  Serial.print(toggleSwitch);
  Serial.println(" ");


  //----------------Light Toggles----------------------------------------
  int lightMode = toggleSelector(toggleSwitch); 
  toggleStrumLight(strumLight);

  //--------------------GREEN BUTTON-------------------------
  if ((buttonGreen == LOW) || (buttonLowGreen == LOW))
  {
    Joystick.pressButton(0); //Button is pressed
    Serial.println("Press button 1 - GREEN ");
    lightState(true, lightMode, 0);
  }
  else
  {
    Joystick.releaseButton(0); //button is released
    lightState(false, lightMode, 0);
  }

  //--------------------RED BUTTON-------------------------
  if (buttonRed == LOW || buttonLowRed == LOW)
  {
    Joystick.pressButton(1);
    Serial.println("Press button 2 - RED ");
    lightState(true, lightMode, 1);
  }
  else
  {
    Joystick.releaseButton(1);
    lightState(false, lightMode, 1);
  }

  //--------------------YELLOW BUTTON-------------------------
  if (buttonYellow == LOW || buttonLowYellow == LOW)
  {
    Joystick.pressButton(2);
    Serial.println("Press button 3 - YELLOW ");
    lightState(true, lightMode, 2);
  }
  else
  {
    Joystick.releaseButton(2);
    lightState(false, lightMode, 2);
  }

  //--------------------BLUE BUTTON-------------------------
  if ((buttonBlue == LOW) || (buttonLowBlue == LOW))
  {
    Joystick.pressButton(3);
    Serial.println("Press button 4 - BLUE ");
    lightState(true, lightMode, 3);
  }
  else
  {
    Joystick.releaseButton(3);
    lightState(false, lightMode, 3);
  }

  //--------------------ORANGE BUTTON-------------------------
  if (buttonOrange == LOW || buttonLowOrange == LOW)
  {
    Joystick.pressButton(4);
    Serial.println("Press button 5 - ORANGE ");
    lightState(true, lightMode, 4);
  }
  else
  {
    Joystick.releaseButton(4);
    lightState(false, lightMode, 4);
  }
  //--------------------Whammy BAr-------------------------

  if (WhammyBar > 800)
  {
    Joystick.pressButton(5); //Button is pressed
    // Serial.println("Press button 6 - WHAMMY ");
  }
  else
  {

    Joystick.releaseButton(5); //button is released
  }
  //--------------------Strum Up-------------------------
  if (strumUp == LOW)
  {
    Joystick.pressButton(6);
    Serial.println("Press button 7 - Strum up ");

  }
  else
  {
    Joystick.releaseButton(6);

  }
  //--------------------Strum Down-------------------------
  if (strumDown == LOW)
  {
    Joystick.pressButton(7);
    Serial.println("Press button 8 - strum down ");

  }
  else
  {
    Joystick.releaseButton(7);

  }
  //--------------------tilt-------------------------
  if (tilt == LOW)
  {
    Joystick.pressButton(8);
    Serial.println("Press button 9 - tilt ");

  }
  else
  {
    Joystick.releaseButton(8);

  }
  // --------------------Plus-------------------------
  if (buttonPlus == LOW)
  {
    Joystick.pressButton(9);
    Serial.println("Press button 10 - PLUS ");
    Serial.println(lightMode);

  }
  else
  {
    Joystick.releaseButton(9);
  }
  //--------------------Minus-------------------------
  if (buttonMinus == LOW)
  {
    Joystick.pressButton(10);
    Serial.println("Press button 11 - MINUS ");
  }
  else
  {
    Joystick.releaseButton(10);
  }
  //--------------------D pad Up-------------------------
  if (dPadUp == LOW)
  {
    Joystick.pressButton(11);
    Serial.println("Press button 12 - Dpad  up");
  }
  else
  {
    Joystick.releaseButton(11);
  }

  //--------------------D pad Down-------------------------
  if (dPadDown == LOW)
  {
    Joystick.pressButton(12);
    Serial.println("Press button 13 - Dpad down ");
  }
  else
  {
    Joystick.releaseButton(12);
  }
  // //--------------------D pad Left------------------------- //Uncomment if using Arduino Micro
  // if (dPadLeft == LOW)
  // {
  //   Joystick.pressButton(13);
  //   Serial.println("Press button 14 - Dpad left ");
  //   // leds[4] = CRGB::Orange;
  // }
  // else
  // {
  //   Joystick.releaseButton(13);
  //   // leds[4] = CRGB::White;
  // }
  // //--------------------D pad Right------------------------- //Uncomment if using Arduino Micro
  // if (dPadRight == LOW)
  // {
  //   Joystick.pressButton(14);
  //   Serial.println("Press button 15 - Dpad right ");
  //   // leds[4] = CRGB::Orange;
  // }
  // else
  // {
  //   Joystick.releaseButton(14);
  //   // leds[4] = CRGB::White;
  // }

  FastLED.show();
  delay(10);
}

//Get the current state of the toggle Selector
int toggleSelector(int toggleSwitch)
{
  if (toggleSwitch < 100)
  {
    return 0;
  }
  else if (toggleSwitch > 100 && toggleSwitch < 330)
  {
    return 1;
  }
  else if (toggleSwitch > 330 && toggleSwitch < 660)
  {
    return 2;
  }
  else if (toggleSwitch > 660 && toggleSwitch < 800)
  {
    return 3;
  }
  else
  {
    return 4;
  }
}

void toggleStrumLight(int strumLight)
{
  if (toggleStrumLight == LOW)
  {
    if (strumLight == true)
    {
      strumLight = false;
    }
    else
    {
      strumLight = true;
    }
  }
  return;
}

// This function determines what color the lights will be.
void lightState(bool isPressed, int lightMode, int key)
{
  //Key codes, 0:GREEN, 1:RED, 2.YELLOW, 3,BLUE 4:ORANGE 
  int colors[6] = {89, 200, 75, 100, 250, 0};
  if (isPressed && lightMode < 4)
  {
    leds[key] = CHSV(colors[key], 255, 255);
    leds[(9 - key)] = CHSV(colors[key], 255, 255);
    // if(isWhammy && lightMode < 3) {
    //   setHueDirection(strobeLight, toggleHueUp, 5);
    // }
    return;
  }
  else if (isPressed && lightMode == 4)
  {
    leds[key] = CHSV(strobeLight, 255, 255);
    leds[(9 - key)] = CHSV(strobeLight, 255, 0);
    setHueDirection(strobeLight, toggleHueUp, 10);
  }
  else if (!isPressed)
  {
    switch (lightMode)
    {
    case 0:
      leds[key] = CHSV(255, 0, 255);
      leds[(9 - key)] = CHSV(255, 0, 255);
      break;
    case 1:
      leds[key] = CHSV(255, 255, 0);
      leds[(9 - key)] = CHSV(255, 255, 0);
      break;
    case 2:
      leds[key] = CHSV(colors[key], 255, 100);
      leds[(9 - key)] = CHSV(colors[key], 255, 100);
      break;
    case 3:
      leds[key] = CHSV(strobeLight, 255, 255);
      leds[(9 - key)] = CHSV(strobeLight, 255, 0);
      setHueDirection(strobeLight, toggleHueUp, 1);
      break;
    case 4:
      leds[key] = CHSV(colors[key], 255, 100);
      leds[(9 - key)] = CHSV(colors[key], 255, 100);
      break;
    default:
      leds[key] = CHSV(255, 0, 255);
      leds[(9 - key)] = CHSV(255, 0, 255);
      break;
    }
    return;
  }
}

//used to control the strobe of the lights
void setHueDirection(int &counter, bool &up, int speed)
{
  if (up && counter < 255)
  {
    counter += speed;
  }
  if (up && counter > 254)
  {
    up = false;
  }
  if (!up && counter > 1)
  {
    counter -= speed;
  }
  if (!up && counter < 10)
  {
    up = true;
  }
}
