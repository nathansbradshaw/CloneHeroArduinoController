#include <Joystick.h>
#include <FastLED.h>
// ----General setup---
int toggleSelector(int); //define method for later use.
void toggleStrumLight(int);
void lightState(bool, int, int);
void setHueDirection(int &, bool &, int);
int slidingLight(int &, int);
// LED setup
#define NUM_LEDS 13       // How many LED are in the strip
#define DATA_PIN 8        //Which pin is the LED plugged into
#define SECOND_DATA_PIN 7 //Which pin is the LED plugged into
#define COLOR_ORDER GRB

//DEFINE LED HUE
#define GREENLED 250
#define REDLED 100
#define YELLOWLED 60
#define BLUELED 180
#define ORANGELED 80
CRGB leds[NUM_LEDS];

// DEFINE WHAT VIRTUAL BUTTON IS MAPPED TO EACH PHYSICAL BUTTON
#define GREEN_BUTTON 0
#define RED_BUTTON 1
#define YELLOW_BUTTON 2
#define BLUE_BUTTON 3
#define ORANGE_BUTTON 4
#define STRUM_UP_BUTTON 5
#define STRUM_DOWN_BUTTON 6
#define WHAMMY_BUTTON 7
#define TILT_BUTTON 8
#define PLUS_BUTTON 9
#define MINUS_BUTTON 10
#define DPAD_UP_BUTTON 11
#define DPAD_DOWN_BUTTON 12
#define DPAD_LEFT_BUTTON 13
#define DPAD_RIGHT_BUTTON 14

//----Global variables----
//Create Joystick
Joystick_ Joystick;
//upper buttons PIN NUMBERS
int GREEN = 9;
int RED = 10;
int YELLOW = 11;
int BLUE = 12;
int ORANGE = 13;
//lower buttons PIN NUMBERS
int lOW_GREEN = 4;
int lOW_RED = 3;
int lOW_YELLOW = 2;
int lOW_BLUE = 1;
int lOW_ORANGE = 0;
//strumbar PIN NUMBERS
int STRUM_UP = 7;
int STRUM_DOWN = 6;
int TILT = 5;

/**
If I had a micro then these would be
A6 and A7, and I would put D_LEFT and D_RIGHT
to A0 and A1.
**/
int WHAMMY = A0;        //A6 if using Arduino Micro
int TOGGLE_SWITCH = A1; //A6 if using Arduino Micro

//game pad
int MINUS = A2;
int PLUS = A3;
int D_UP = A4;
int D_DOWN = A5;
// int D_LEFT = A0; //only if using Arduino Micro
// int D_RIGHT = A1; //only if using Arduino Micro

//LED STROBE POWER
int strobeLight = 0;
int strumLightStrobe = 100;
int StrobeColors[6] = {GREENLED, REDLED, YELLOWLED, BLUELED, ORANGELED, 0};
int StrobeColors2[6] = {ORANGELED, BLUELED, YELLOWLED, REDLED, GREENLED, 0};
bool toggleHueUpKey[6] = {false, false, false, false, false, false};
bool WhammyStrobe = false;
int slideUp = 0;
int slideDown = 4990;
int colors[6] = {ORANGELED, BLUELED, YELLOWLED, REDLED, GREENLED, 0};
int reverseColors[6] = {GREENLED, REDLED, YELLOWLED, BLUELED, ORANGELED, 0};

// bool isWhammy = false;
bool toggleHueUp = true;
bool toggleHueDown = false;
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

  // //DEBUGGING Potentiometers
  // Serial.print("Analog: ");
  // // Serial.print(WhammyBar);
  // Serial.print(toggleSwitch);
  // Serial.println(" ");

  //----------------Light Toggles----------------------------------------
  int lightMode = toggleSelector(toggleSwitch);
  toggleStrumLight(strumLight);

  //--------------------GREEN BUTTON-------------------------
  if ((buttonGreen == LOW) || (buttonLowGreen == LOW))
  {
    Joystick.pressButton(GREEN_BUTTON); //Button is pressed
    Serial.println("Press button 1 - GREEN ");
    lightState(true, lightMode, 4);
  }
  else
  {
    Joystick.releaseButton(GREEN_BUTTON); //button is released
    lightState(false, lightMode, 4);
  }

  //--------------------RED BUTTON-------------------------
  if (buttonRed == LOW || buttonLowRed == LOW)
  {
    Joystick.pressButton(RED_BUTTON);
    // Serial.println("Press button 2 - RED ");
    lightState(true, lightMode, 3);
  }
  else
  {
    Joystick.releaseButton(RED_BUTTON);
    lightState(false, lightMode, 3);
  }

  //--------------------YELLOW BUTTON-------------------------
  if (buttonYellow == LOW || buttonLowYellow == LOW)
  {
    Joystick.pressButton(YELLOW_BUTTON);
    // Serial.println("Press button 3 - YELLOW ");
    lightState(true, lightMode, 2);
  }
  else
  {
    Joystick.releaseButton(YELLOW_BUTTON);
    lightState(false, lightMode, 2);
  }

  //--------------------BLUE BUTTON-------------------------
  if ((buttonBlue == LOW) || (buttonLowBlue == LOW))
  {
    Joystick.pressButton(BLUE_BUTTON);
    // Serial.println("Press button 4 - BLUE ");
    lightState(true, lightMode, 1);
  }
  else
  {
    Joystick.releaseButton(BLUE_BUTTON);
    lightState(false, lightMode, 1);
  }

  //--------------------ORANGE BUTTON-------------------------
  if (buttonOrange == LOW || buttonLowOrange == LOW)
  {
    Joystick.pressButton(ORANGE_BUTTON);
    // Serial.println("Press button 5 - ORANGE ");
    lightState(true, lightMode, 0);
  }
  else
  {
    Joystick.releaseButton(ORANGE_BUTTON);
    lightState(false, lightMode, 0);
  }
  //--------------------Whammy BAr-------------------------

  if (WhammyBar > 800)
  {
    Joystick.pressButton(WHAMMY_BUTTON); //Button is pressed
    // Serial.println("Press button 6 - WHAMMY ");
    WhammyStrobe = true;
  }
  else
  {

    Joystick.releaseButton(WHAMMY_BUTTON); //button is released
    WhammyStrobe = false;
  }
  //--------------------Strum Up-------------------------
  if (strumUp == LOW)
  {
    Joystick.pressButton(STRUM_UP_BUTTON);
    // Serial.println("Press button 7 - Strum up ");
    leds[10] = CHSV(strumLightStrobe, 255, 255);
    leds[11] = CHSV(strumLightStrobe, 255, 255);
    setHueDirection(strumLightStrobe, toggleHueUp, 10);
  }
  else
  {
    Joystick.releaseButton(STRUM_UP_BUTTON);
    leds[10] = CHSV(255, 255, 0);
    leds[11] = CHSV(255, 255, 0);
  }
  //--------------------Strum Down-------------------------
  if (strumDown == LOW)
  {
    Joystick.pressButton(STRUM_DOWN_BUTTON);
    // Serial.println("Press button 8 - strum down ");
    leds[10] = CHSV(strumLightStrobe, 255, 255);
    leds[11] = CHSV(strumLightStrobe, 255, 255);
    setHueDirection(strumLightStrobe, toggleHueUp, 15);
  }
  else
  {
    Joystick.releaseButton(STRUM_DOWN_BUTTON);
    leds[10] = CHSV(255, 255, 0);
    leds[11] = CHSV(255, 255, 0);
  }
  //--------------------tilt-------------------------
  if (tilt == LOW)
  {
    Joystick.pressButton(TILT_BUTTON);
    // Serial.println("Press button 9 - tilt ");
  }
  else
  {
    Joystick.releaseButton(TILT_BUTTON);
  }
  // --------------------Plus-------------------------
  if (buttonPlus == LOW)
  {
    Joystick.pressButton(PLUS_BUTTON);
    // Serial.println("Press button 10 - PLUS ");
    Serial.println(lightMode);
  }
  else
  {
    Joystick.releaseButton(PLUS_BUTTON);
  }
  //--------------------Minus-------------------------
  if (buttonMinus == LOW)
  {
    Joystick.pressButton(MINUS_BUTTON);
    // Serial.println("Press button 11 - MINUS ");
  }
  else
  {
    Joystick.releaseButton(MINUS_BUTTON);
  }
  //--------------------D pad Up-------------------------
  if (dPadUp == LOW)
  {
    Joystick.pressButton(DPAD_UP_BUTTON);
    // Serial.println("Press button 12 - Dpad  up");
  }
  else
  {
    Joystick.releaseButton(DPAD_UP_BUTTON);
  }

  //--------------------D pad Down-------------------------
  if (dPadDown == LOW)
  {
    Joystick.pressButton(DPAD_DOWN_BUTTON);
    // Serial.println("Press button 13 - Dpad down ");
  }
  else
  {
    Joystick.releaseButton(DPAD_DOWN_BUTTON);
  }
  // //--------------------D pad Left------------------------- //Uncomment if using Arduino Micro
  // if (dPadLeft == LOW)
  // {
  //   Joystick.pressButton(DPAD_LEFT_BUTTON);
  // //   Serial.println("Press button 14 - Dpad left ");
  //   // leds[4] = CRGB::Orange;
  // }
  // else
  // {
  //   Joystick.releaseButton(DPAD_LEFT_BUTTON);
  //   // leds[4] = CRGB::White;
  // }
  // //--------------------D pad Right------------------------- //Uncomment if using Arduino Micro
  // if (dPadRight == LOW)
  // {
  //   Joystick.pressButton(DPAD_RIGHT_BUTTON);
  //  // Serial.println("Press button 15 - Dpad right ");
  //   // leds[4] = CRGB::Orange;
  // }
  // else
  // {
  //   Joystick.releaseButton(DPAD_RIGHT_BUTTON);
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
  int slideLightUp = slidingLight(slideUp, 32);
  int slideLightDown = slidingLight(slideDown, -37);
  //Key codes
  if (isPressed && lightMode < 4)
  {
    if (!WhammyStrobe || lightMode >= 3)
    {
      leds[key] = CHSV(colors[key], 255, 255);
      leds[(9 - key)] = CHSV(colors[key], 255, 255);
    }

    if ((WhammyStrobe) && (lightMode < 3))
    {
      leds[key] = CHSV(StrobeColors[key], 255, 255);
      leds[(9 - key)] = CHSV(StrobeColors2[key], 255, 255);
      setHueDirection(StrobeColors[key], toggleHueUp, -20);
      setHueDirection(StrobeColors2[key], toggleHueDown, 20);
    }
    return;
  }
  else if (isPressed && lightMode == 4)
  {
    leds[key] = CHSV(StrobeColors[key], 255, 255);
    leds[(9 - key)] = CHSV(StrobeColors[key], 255, 255);
    setHueDirection(StrobeColors[key], toggleHueUpKey[key], 20);
  }
  else if (!isPressed)
  {
    switch (lightMode)
    {
    case 0:
      leds[key] = CHSV(colors[key], 255, 0);
      leds[(9 - key)] = CHSV(colors[key], 255, 0);
      leds[slideLightDown] = CHSV(colors[slideLightDown], 255, 130);
      leds[(9 - slideLightUp)] = CHSV(colors[slideLightUp], 255, 130);
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
      leds[key] = CHSV(StrobeColors[key], 255, 175);
      leds[(9 - key)] = CHSV(StrobeColors2[key], 255, 175);
      setHueDirection(StrobeColors[key], toggleHueUp, -10);
      setHueDirection(StrobeColors2[key], toggleHueDown, 10);

      break;
    case 4:
      leds[key] = CHSV(colors[key], 255, 100);
      leds[(9 - key)] = CHSV(colors[key], 255, 100);
      break;
    default:
      leds[key] = CHSV(255, 0, 105);
      leds[(9 - key)] = CHSV(255, 0, 105);
      break;
    }
    return;
  }
}

//used to control the strobe of the lights
void setHueDirection(int &counter, bool &up, int speed)
{
  if (up && counter < 214)
  {
    counter += speed;
  }
  if (up && counter > 214)
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

int slidingLight(int &counter, int slideDirection)
{
  
  if (counter > 5000)
  {
    counter = 1;
  }
  else if (counter < 0)
  {
    counter = 4999;
  }
  counter += slideDirection;
  if (counter < 1000)
  {
    return 0;
  }
  else if (counter < 2000)
  {
    return 1;
  }
  else if (counter < 3000)
  {
    return 2;
  }
  else if (counter < 4000)
  {
    return 3;
  }
  else if (counter < 5000)
  {
    return 4;
  }
  return 0;
}
