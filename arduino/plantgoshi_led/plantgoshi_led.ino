char inData[4]; // Allocate some space for the string (signed char)
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

int redPin = 9;
int greenPin = 10;
int bluePin = 11;
//---------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode( redPin, OUTPUT );
  pinMode( greenPin, OUTPUT );
  pinMode( bluePin, OUTPUT );
}
//---------------------------------------------------------------
void readSerialData() {
    while (Serial.available() > 0) // Don't read unless
                                   // there you know there is data
    {
        if(index < 4) // One less than the size of the array
        {
            inChar = Serial.read(); // Read a character

            inData[index] = inChar; // Armazena o valor e forca valores entre 0 - 255
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string            
        }
    }
}
//---------------------------------------------------------------
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
//---------------------------------------------------------------
void loop()
{
  readSerialData();
  delay(10);
//  if (Comp("m1 on")==0) {
//        Serial.write("Motor 1 -> Online\n");
          setColor( int(inData[0]), int(inData[1]), int(inData[2]) );
//          setColor( 255, 0, 0 );
//        delay( 1000 );
//  
//        setColor( 0, 0, 50 );
//        delay( 1000 );
//  
//        setColor( 0, 0, 255 );
//        delay( 1000 );
//  
//        setColor( 255, 255, 0 );
//        delay( 1000 );
//  
//        setColor( 80, 0, 80 );
//        delay( 1000 );
//  
//        setColor( 0, 255, 255 );
//        delay( 1000 );
//    }
//    if (Comp("m1 off")==0) {
//        Serial.write("Motor 1 -> Offline\n");
//    }
}
