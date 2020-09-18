/* ***********************************************************
 * *** Inspired by Ricardo Moreno's youtube video (https://www.youtube.com/watch?v=gTQmLkwnUxk)
 *      Ricardo Moreno's youtube channel here -> (https://www.youtube.com/channel/UCbDCjdszaIdDOyXNRgMlgIg)
 *
 * Description:
 *   This sketch illustrates some of the functionality of the 
 *   RFC522 RFID module and tags. Set the card tag as a Master
 *   and use it to authorize the keychain tag. 
 *   
 *   This sketch has been expanded to act as a simple security system 
 *   for a secure lock box or secure box using RFID tags in order 
 *   to enter or unlock the box. Includes break in method. 
 *
 *  --Message from original author-- 
 *  --Statement still holds true-- 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  History:
 *  09/10/2020 v.1 by Alex Dinkelacker
 *
 * @author Alex Dinkelacker
 ********************************************************** */ 

/* ***********************************************************
 *                         Libraries                         *
 * ********************************************************* */
#include <SPI.h>
#include <MFRC522.h>


/* ***********************************************************
 *                      Global Constants                     *
 *                    Hardware Definitions                   *
 * ********************************************************* */

   // Other hardware pins
    const byte rstPin =  49;                // Reset pin
    const byte  ssPin = 53;                // Slave Select pin
    
    // Instantiate MFRC522 object class
    MFRC522 rfidReader(ssPin, rstPin);
 
    // Other Global Constants 
    const long timeout = 30000;            // Timeout in ms 


/* ***********************************************************
 *                     Liquid Crystal Display                *
 * ********************************************************* */
#include <LiquidCrystal_I2C.h>

//LCD set address
//Need to check I2C for address of device (LCD module) - mine was 0x27
//I am using LCD Display 2004A 
LiquidCrystal_I2C lcd(0x27, 20, 4);

/* ***********************************************************
 *                   Buzzer & Red LED                        *
 * ********************************************************* */
int buzzPin = 8; 
int buzzTime = 2000; 

/* ***********************************************************
 *                 Pass Pin / Green LED                      *
 * ********************************************************* */
int passPin = 7; 
int openTime = 3000; 

/* ***********************************************************
 *                     Servo Motor                           *
 * ********************************************************* */
#include <Servo.h> 

int servoPin = 9; 
int servoPos = 0;

Servo myServo;

/* ***********************************************************
 *                      HC-SR04 Sensor                       *
 *                Using this as Distance Sensor              *
 * ********************************************************* */
int trigPin = 10;
int echoPin = 11; 
int pingTravelTime; 

/* ***********************************************************
 *                      Global Variables                     *
 * ********************************************************* */
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "";


bool readRFID(long _timeout=timeout, bool useTimeout=false){
    /*  readRFID will continuously check the RFID reader for the presence of
     *    a tag from and will attempt to get tag IDs. Updates global value
     *    tagID via getTagID function.
     *  Parameters:
     *    _timeout   - [optional] the length of time before functio gives up
     *                 default value = global timout value
     *    useTimeout - [optional] boolean to enforce timout period or wait 
     *                 indefinately.  Default value = false.
     *  Returns:
     *    true  -  successfully reads tag ID
     *    false -  unsuccessful in reading the tag ID          
     */
    bool successRead = false;

    unsigned long startTime = millis();
    unsigned long currentTime = startTime;
    // S'U'+S'T'
    // T  = (currentTime-startTime) > timeout
    // T' = (currentTime-startTime) < timeout
    while (((successRead==false)&&(useTimeout==false)) || ((successRead==false)&&((currentTime - startTime) < _timeout))) {    
        if (isTagPresent() == true){ successRead = getTagID(); }
        currentTime = millis();
    }
    return successRead;
}

/* ***********************************************************
 *                         Void Setup                        *
 * ********************************************************* */
void setup() {
  
    /***********************************************************
    *                         LCD Setup                        *
    ***********************************************************/
    lcd.begin();
    lcd.backlight(); 

    /***********************************************************
    *                         Buzzer Setup                     *
    ***********************************************************/
    pinMode(buzzPin, OUTPUT);
    
    /***********************************************************
    *                         Pas Pin Setup                    *
    ***********************************************************/
    pinMode(passPin, OUTPUT);
    

    /***********************************************************
    *                         Servo Setup                      *
    ***********************************************************/
    myServo.attach(servoPin);

    /***********************************************************
    *               HC-SR04(Distance) Sensor Setup             *
    ***********************************************************/
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
     /***********************************************************
    *                         RFID Setup                        *
    ***********************************************************/
    // Initiating
    Serial.begin(9600);                     // Start the serial monitor
    SPI.begin();                            // Start SPI bus
    rfidReader.PCD_Init();                  // Start MFRC522 object

    while (!Serial);                        // Do nothing if no serial port is opened
    
    // Obviously this is an over simplified sketch
    // Master tags would be save in flash storage and
    // retrieved here.  OR a special PIN entered to set
    // Master Tag.
    // But for the sake of simplicity, the sketch will 
    // obtain a new master tag when restarted.
    
    // Prints the initial message
    Serial.println(F("-No Master Tag!-"));
    Serial.println(F("    SCAN NOW"));

    //Printing message to LCD 
    lcd.setCursor(2,0);
    lcd.print("-NO MASTER TAG-"); 
    lcd.setCursor(0,2);
    lcd.print("SCAN MASTER TAG NOW");
    lcd.setCursor(2,3);
    lcd.print("TO GET STARTED");
    
    
    // readRFID will wait until a master card is scanned
    if (readRFID() == true) {
        myTags[tagsCount] = strdup(tagID.c_str()); // Sets the master tag into position 0 in the array
        Serial.println(F("Master Tag is Set!"));
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("MASTER TAG IS SET!"); 
        if (checkIfMaster() == true) {
          Serial.println("It worked");
          Serial.println(strdup(tagID.c_str()));
        }
        tagsCount++;
        
        
    }
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("PLACE ITEMS INSIDE");
    lcd.setCursor(1,2); 
    lcd.print("SLIDE DOOR CLOSED");
    while (isDoorOpen() == true) {
    
    }
    printNormalModeMessage();
    for (servoPos = 90; servoPos >=0; servoPos -= 1) {
          myServo.write(servoPos); 
          delay(3); 
    }
    
    
}

/* ***********************************************************
 *                         Void Loop                         *
 * ********************************************************* */
void loop() {
 /*
  * Your distance for your security system may be different, 
  *  you can use this print statement to measure the distance 
  *  to your security door.
  */
  //Serial.println(pingTravelTime);
  
/* 
 *  this next line is so that the red light will flash 
 *  and the buzzer will go in and out when there is a breakin
 */
  digitalWrite(buzzPin, LOW);
  if (isDoorOpen() == false) {
    
    if (isTagPresent() == true){
      getTagID();
      checkTagID();
    } else {
      delay(50);
        //return;
    }
    
 
 } else if((isDoorOpen() == true)&&(servoPos == -1))  {breakIn();}
 
else {printNormalModeMessage();}
  

}  

/* ***********************************************************
 *                         Functions                         *
 * ********************************************************* */


bool isDoorOpen(){
  /*  isDoorOpen uses the HC-SR04 sensor and determines wether
   *   or not the security door is opened or closed
   *   
   *   Parameters: (none) 
   *   Returns
   *    true - if door is open 
   *    false - if door is closed
   */
   bool returnValue = false;
   
   digitalWrite(trigPin, LOW); 
   delayMicroseconds(10); 
   digitalWrite(trigPin, HIGH); 
   delayMicroseconds(10); 
   digitalWrite(trigPin, LOW); 
   pingTravelTime = pulseIn(echoPin, HIGH); 
   if (pingTravelTime > 300) {
    //door is open
    returnValue = true;
    
   }
   if (returnValue == false) { 
    //door is closed then close the servo
   }
   return returnValue; 
}


bool isTagPresent(){
    /*  isTagPresent uses the MFRC522 methods to determine if 
     *    a tag is present or the read card serial is enabled.
     *
     *  Parameters: (none)
     *  Returns:
     *    true  - if tag detected or read card serial is true
     *    false - no tag detected or no read card serial true
     */  
     
    bool returnValue = true;
    
    // NOT a new PICC_IsNewCardPresent in RFID reader
    //OR
    // NOT a PICC_ReadCardSerial active in Serial
    if ( !rfidReader.PICC_IsNewCardPresent() || !rfidReader.PICC_ReadCardSerial() ) { 
      returnValue = false;
    }
    return returnValue;
}

byte checkMyTags(String tagID){
    /* checkMyTags function loops through the array of myTags
     *   Parameters:
     *     tagID    - a string to look for
     *   Returns:
     *     tagIndex - index in the array of myTags
     *                default 0     
     */
    byte tagIndex = 0;
    //Serial.println("checkMyTags Started");
    // Zero is reserved for master tag
    for (int i = 1; i < 100; i++) {
        if (tagID == myTags[i]) { tagIndex = i; }
    }
    //Serial.println("checkMyTags ended");
    return tagIndex;
}

void checkTagID(){
    /* checkTagID check the tag ID for authorized tag ID values
     *   if Master tag found switch to program mode
     * Parameters: (none)
     * Returns: (none)
     */
    // Checks for Master tag
    if (tagID == myTags[0]) {
        // Switch to program mode
        Serial.println(F(" Program mode:"));
        Serial.println(F(" Add/Remove Tag"));
        
        //print program mode to LCD
        lcd.clear(); 
        lcd.setCursor(2,0); 
        lcd.print("-PROGRAM MODE-");
        lcd.setCursor(1,2);
        lcd.print("ADD OR REMOVE TAG"); 
        
      
        // Now with timeout
        // readRFID will skip if timeout exceeded
        if (readRFID(timeout,true)==true) {
            //Check for authorized tag
            byte tagIndex = checkMyTags(tagID);
            if (tagIndex!=0){
                //Remove existing tag
                myTags[tagIndex] = '\0';
                Serial.println(F("  Tag Removed!"));
                lcd.clear(); 
                lcd.setCursor(3,1);
                lcd.print("TAG REMOVED");
            printNormalModeMessage();
            } else {
                //Not existing, add tag`
                myTags[tagsCount] = strdup(tagID.c_str());
                Serial.println(F("  Tag Added!"));
                lcd.clear(); 
                lcd.setCursor(3,1);
                lcd.print("TAG ADDED");
                tagsCount++;
            printNormalModeMessage();
            }
        } else {
            Serial.println(F("    timeout"));
            lcd.clear(); 
            lcd.setCursor(3,1);
            lcd.print("TIMED OUT");
        printNormalModeMessage();
        }
    } else { 
        //Check for authorized tag
        byte tagIndex = checkMyTags(tagID);
        if (tagIndex!=0){
          youAreIn();
        } else {
            //Not authorized tag
            Serial.println(F(" Access Denied!"));
            Serial.println(F(" New UID & Contents:"));
            lcd.clear(); 
            lcd.setCursor(3,0);
            lcd.print("ACCESS DENIED!");
            digitalWrite(buzzPin, HIGH);
            delay(buzzTime); 
            digitalWrite(buzzPin, LOW); 
            lcd.setCursor(1, 2);
            lcd.print("-INVALID RFID TAG-");
            rfidReader.PICC_DumpToSerial(&(rfidReader.uid));
            
       
           
            
        printNormalModeMessage();    
        }
    }
    
}

bool getTagID() {
    /*  getTagID retrieves the tag ID.  Modifies global variable tagID
     *           
     *    Parameters: (none)    
     *    Returns: true
     */

    tagID = "";
    
    Serial.print(F(" UID tag: "));
    for (byte i = 0; i < rfidReader.uid.size; i++){
        // The MIFARE PICCs that we use have 4 byte UID
        Serial.print(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfidReader.uid.uidByte[i], HEX);
        // Adds the bytes in a single String variable
        tagID.concat(String(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
        tagID.concat(String(rfidReader.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.println();
    tagID.toUpperCase();
    rfidReader.PICC_HaltA();                     // Stop reading
    return true;
}

void youAreIn(){
    /* youAreIn gets called when you scan a proper tag that has been 
     *  added to the system. It allows the user to enter. 
     *  
     *  Parameters: (none) 
     *  Return: void -> (none) 
     * 
     */
            Serial.println(F(" Access Granted!"));
            digitalWrite(passPin, HIGH);
            lcd.clear(); 
            lcd.setCursor(2,0);
            lcd.print("ACCESS GRANTED!");
            for (servoPos = 0; servoPos <=100; servoPos +=1) {
              myServo.write(servoPos); 
              delay(5);
            }
            while (isDoorOpen() == false) {
              lcd.setCursor(3,1); 
              lcd.print("OPEN DOOR");
            }
            lcd.setCursor(1,2);
            lcd.print("RETRIEVE ITEMS AND");
            lcd.setCursor(3,3);
            lcd.print("CLOSE DOOR");  
            while (isDoorOpen() == true) {
            }
            for (servoPos = 100; servoPos >=0; servoPos -=1) {
              myServo.write(servoPos); 
              delay(5);
            }
            digitalWrite(passPin, LOW); 
            printNormalModeMessage(); 
            
            
            
}

void printNormalModeMessage() {
    /*  printNormalModeMessage sends the standard greeting
     *    to the serial monitor.
     *  Parameters: (none)
     *  Returns: (none)
     */
    delay(1500);
    Serial.println();
    Serial.println(F("-Access Control-"));
    Serial.println(F(" Scan Your Tag!"));
    lcd.clear(); 
    lcd.setCursor(1,0);
    lcd.print("CONTROLLED ACCESS"); 
    lcd.setCursor(1,2); 
    lcd.print("SCAN TAG TO ENTER");
}

void printDoorOpen() {
  /*  printDoorOpen will send a stand message to the user 
   *   that the door has been opened. And will begin scanning 
   *   again once the door is closed. 
   *   Paramerters: (none)
   *   Returns: void -> (none)
   */
   delay(500); 
   lcd.clear(); 
   lcd.setCursor(1, 0); 
   lcd.print("THE DOOR IS OPEN"); 
   lcd.setCursor(1,2);
   lcd.print("RETRIEVE ITEMS AND");
   lcd.setCursor(3,3);
   lcd.print("CLOSE DOOR"); 
}

bool checkIfMaster(){
  /* checkIfMaster checks to see if tag that is read 
   *    is the master tag 
   *  Parameters: (none)
   *  Return: 
   *    true: is master tag is read 
   *    false: if tag is not master tag
   */
  if (tagID == myTags[0]){
    return true;
  }
  else {return false;}
}

void breakIn() { 
  /* breakIn is a function that will run a
   *    break in script - ringing the buzzer and 
   *    flashing the buzzer light on and off 
   *    It will also write to the lcd reporting of 
   *    a break in and demand that the master key be 
   *    scanned in order to reset the system. 
   *    
   *    Parameters: (none) 
   *    Return: void -> (none) 
   * 
   */
  while (servoPos == -1){
        digitalWrite(buzzPin, HIGH);
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("THERE IS A BREAK IN!");
        lcd.setCursor(0,1);
        lcd.print("SCAN MASTER TAG NOW!");
        lcd.setCursor(0,2);
        lcd.print("AUTHORITIES HAVE");
        lcd.setCursor(1,3);
        lcd.print("BEEN CONTACTED!");
        digitalWrite(buzzPin, LOW);
        if (isTagPresent() == true) {
                getTagID();
                if (checkIfMaster()==true){
                  digitalWrite(buzzPin,LOW);
                  //open servo to replace/fix door
                  for (servoPos = 0; servoPos <=100; servoPos +=1) {
                    myServo.write(servoPos); 
                    delay(5);
                  }
                  lcd.clear();
                  lcd.print("CLOSE DOOR TO RESET");
                  while (isDoorOpen() == true){
                  }
                  for (servoPos = 100; servoPos >=-4; servoPos -=1) {
                    myServo.write(servoPos); 
                    delay(5);
                  }
                  printNormalModeMessage();
                 }
                 else{
                  lcd.clear();
                  lcd.setCursor(3,0);
                  lcd.print("YOU NEED THE");
                  lcd.setCursor(4,2);
                  lcd.print("MASTER KEY");
                  delay(1800);
                  }
        }
  }
    for (servoPos = 50; servoPos >=0; servoPos -= 1) {
          myServo.write(servoPos); 
          delay(2); 
    }
}
