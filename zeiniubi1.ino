#include <MFRC522.h>

#include <LiquidCrystal.h>

#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;


LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
////////////////////////////chong qian1////////////////////////
double balance1 = 100; //The original balance in first account(User1) zhunbei qudiao///////////////////////////////////////////
//double balance1;

char balancec1[16];

const double p1 = 2; //The price of product
const double p2 = 1.75;
const double p3 = 1.50;
const double p4 = 99.99;
double price;

const int ST = 0;
//const int I = 1;
const int S = 2;
const int G = 3;
//const int PTAP = 4;
//const int G_c = 5;
//const int G_d = 6;
const int TAP = 7;
const int Error = 8;
const int F = 9;
const int T = 10;

int item = 0;
unsigned long time1;
unsigned long time2;

int recive_a;
int recive_b;
int ssww;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  lcd.begin(16, 2);   //Defined LCD

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,OUTPUT);

  for (byte i = 0; i < 6; i++) 
    {
        key.keyByte[i] = 0xFF;
    }

  Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
  Serial.print(F("Using key (for A and B):"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
    
  Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
    
}

void loop() {
  static int state = ST;

  //store the user's choice
  static unsigned int choice;

  switch (state)
  {
    case ST:
      {
        lcd.clear();
        lcd.print("Welcome!");
        lcd.setCursor(0, 1);
        lcd.print("Please select");
        Serial.print(balance1);
     
        state = S;
        digitalWrite(A3,LOW);
      }
      
     case S:
      {
        recive_a = (analogRead(A0)>500)?1:0;
        recive_b =  (analogRead(A1)>500)?1:0;
        ssww= analogRead(A2);
 
        if (ssww)
        {
          return;
        }
        if ((recive_a==0)&&(recive_b==0))
        {
          price = p1;
          state = G;
          item = 1;
        }
        else if ((recive_a==0)&&(recive_b==1))
        {
          price = p2;
          state = G;
          item = 3;
        }
        else if ((recive_a==1)&&(recive_b==0))
        {
          price = p3;
          state = G;
          item = 2;
        }
        else if ((recive_a==1)&&(recive_b==1))
        {
          price = p4;
          state = G;
          item = 4;
        }
// item = Serial.read() - 48;
//
//        if ( item == 1)
//        {
//          price = p1;
//          state = G;
//         
//        }
//        else if (item == 2)
//        {
//          price = p2;
//          state = G;
//        }
//        else if (item == 3)
//        {
//          price = p3;
//          state = G;
//        }
//        else if (item == 4)
//        {
//          price = p4;
//          state = G;
//        }
        break;
      }
     case G:
     {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Item selected:");
        lcd.print(item);
        lcd.setCursor(0, 1);
        lcd.print("$");
        lcd.print(price);
        lcd.print("Please tap");
//        time1 = millis();

        state= TAP;
     }
//     case PTAP:
//     {
//      time2 = millis();
//      if ((time2 - time1) >= 5000)
//          state = ST;
//        else
//          state = T;
//       break;
//
//     }

     case TAP:
      {
        if ( ! mfrc522.PICC_IsNewCardPresent())
        {
          Serial.write("fan si le");
          return;
        }
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial())
        {
          Serial.write("fan si le2");
          return;
        }

        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));
         if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
        }

        String content = "";
        byte letter;
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          //     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          //     Serial.print(mfrc522.uid.uidByte[i], HEX);
          content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        content.toUpperCase();
 if (!((content.substring(1) == "55 51 12 53")||(content.substring(1) == "EB F3 14 16")))
 {
  state = Error;       
        break;
 }
        
         byte sector         = 1;
    byte blockAddr      = 4;
    byte dataBlock[]    = {
        0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
        0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
        0x08, 0x09, 0xff, 0x0b, //  9, 10, 255, 12,
        0x0c, 0x0d, 0x0e, 0x0f  // 13, 14,  15, 16
    };
    byte dataBlock0[]    = {
        0x00, 0x00, 0x00, 0x00, //  1,  2,   3,  4,
        0x00, 0x00, 0x00, 0x00, //  5,  6,   7,  8,
        0x00, 0x00, 0x00, 0x00, //  9, 10, 255, 12,
        0x00, 0x00, 0x00, 0x00  // 13, 14,  15, 16
    };
    
    byte dataBlockv [16]={};
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
      byte buffer[18];
      byte size = sizeof(buffer);

      Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
//////////////////////////chong qian2////////////////////////////////////
    dtostrf(balance1,2,2,balancec1);
    CharToByte(balancec1,dataBlockv,16);
    Serial.print(F("Adding money into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlockv, 16); Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlockv, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
/////////////////////////////////////////////////////////////////////////
/////xie qian////
//    dtostrf(balance1,2,2,balancec1);
//    CharToByte(balancec1,dataBlockv,16);
//    mfrc522.MIFARE_Write(blockAddr, dataBlockv, 16);


    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Read data from the block/////////////////////////////////////////////////////////////////////
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
///////////////////////////////////////////////////////////////////

ByteToChar(buffer,balancec1,16);
balance1 = atof(balancec1)-price;
if (balance1<0)
{
  state = F; 
  balance1 += price;
  break;
}
 

///////////////////////////////////////////////////////////////////
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write data to the block 
    dtostrf(balance1,2,2,balancec1);
    CharToByte(balancec1,dataBlockv,16);
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlockv, 16); Serial.println();
    status = (MFRC522::StatusCode)  
    mfrc522.MIFARE_Write(blockAddr, dataBlockv, 16);
    
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
     Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
        
    
        
    // Dump the sector data
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

//
//        
//
//        String content = "";
//        byte letter;
//        for (byte i = 0; i < mfrc522.uid.size; i++)
//        {
//          //     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//          //     Serial.print(mfrc522.uid.uidByte[i], HEX);
//          content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
//          content.concat(String(mfrc522.uid.uidByte[i], HEX));
//        }
        content.toUpperCase();
        if ((content.substring(1) == "55 51 12 53")||(content.substring(1) == "EB F3 14 16"))
        { 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Card balance:");
          lcd.setCursor(0,1);
          lcd.print("$");
          lcd.print(balance1);
          time1 = millis();
          
          digitalWrite(A3,HIGH);
          
          state = T;     
        }
        else
          state = Error;       
        break;

      }
      
       case Error:
      {
        lcd.clear();


        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          //    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          //    content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }



        lcd.print("Card deneied");
        time1 = millis();
        
        state = T;
        break;
      }
      case T:
      {       
        time2 = millis();
         

        if ((time2 - time1) >= 3000)
          state = ST;
        else
          state = T;
       break;
      }
      case F:
      {
        lcd.clear();
        lcd.print("Not enough mana.");
        time1 = millis();
        state = T;
        break;
      }
  }
  
  

}


void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
void CharToByte(char* chars, byte* bytes, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
        bytes[i] = (byte)chars[i];
}
void ByteToChar(byte* bytes, char* chars, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
         chars[i] = (char)bytes[i];
}
