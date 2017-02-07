
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

const int redPin = 4;
const int greenPin = 5;
const int yellowPin = 6;
const int whitePin = 7;
const int knappPin = 10;

int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

void setup()  
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  
  Serial.begin(9600);
  Serial.println("Adafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  
  pinMode(knappPin, INPUT);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  getFingerprintID();
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  //Serial.println("Getting image");
  switch (p) {
    case FINGERPRINT_OK:
      
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
     // Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      digitalWrite(yellowPin, HIGH);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); Serial.println(finger.confidence); 
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
    delay(5000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
     digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, HIGH);
    delay(5000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  Serial.println("Trying to get image");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("No image");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
     Serial.println("image 2 tz not working");
   return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Finger fast search failed");
    return -1;
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
