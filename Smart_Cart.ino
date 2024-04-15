
#include <ESP8266WiFi.h> //Node MCU Library.

#include <Wire.h> //LCD Library.
#include <LiquidCrystal_I2C.h> //LCD Library.
#include <SPI.h>//LCD Library.

#include <MFRC522.h> //RFID Sensor Library.

#include<WiFiClient.h> 
#include<ESP8266WebServer.h> 


// ------------------------------------VARIABLES FROM YT------------------------------------ //
const char* ssid = "admin"; //Replace with your network SSID
const char* password = "12345678"; //Replace with your network password

ESP8266WebServer server(80); //For Server.

String page = ""; //initially page is empty.
char input[12]; //stores the uid. length of uid is assumed to be 12.
int count = 0; //used to iterate through uid.

int a; //variable to store state of remove button.
int p1=0,p2=0,p3=0,p4=0; //variables for quantity of items.
int c1=0,c2=0,c3=0,c4=0; //variables for cost of items.
double total = 0; //total cost.
int count_prod = 0; //total items.

// ------------------------------------VARIABLES------------------------------------ //
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4; 

MFRC522 rfid(D4, D3); // SS_PIN, RST_PIN
MFRC522::MIFARE_Key key;
String tag;

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
    // ---------------------- FOR LED AND BUTTON EXTRA STUFF. -------------------------- //
    pinMode(D3,INPUT_PULLUP);
    pinMode(D8,OUTPUT);
    //  --------------------------END-------------------------------- //

    WiFi.begin(ssid, password);

    // The begin call takes the width and height. This
    // Should match the number provided to the constructor.
    // lcd.begin(16,2);
    lcd.init();
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    // Turn on the backlight.
    lcd.backlight();
    //--------------------------------LCD BOOT MESSAGE---------------------------------- //
    lcd.setCursor(0, 0);
    lcd.print("   WELCOME TO       ");
    lcd.setCursor(0, 1);
    lcd.print("   SMART CART       ");
    delay(2000);

    lcd.clear();
    //  --------------------------END----------------------------------------------------- //

    // --------------------------------LCD WIFI MESSAGE---------------------------------- //
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecting...  ");
    }
    Serial.println(WiFi.localIP());
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(1000);
    //  ------------------------------------END------------------------------------------- //

    // --------------------------------LCD ADD ITEMS MESSAGE---------------------------------- //
    lcd.setCursor(0, 0);
    lcd.print(" PLZ ADD ITEMS     ");
    lcd.setCursor(0, 1);
    lcd.print("    TO CART          ");
    lcd.clear();
    //  ----------------------------------END--------------------------------------------- //

    // --------------------------------WEBPAGE CODE---------------------------------- //
    server.on("/", []()
    {
    page = "<html><head><title>E Cart using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  #3498db ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Shopping Cart using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>ITEMS</th><th>QUANTITY</th><th>COST</th></tr><tr><td>Biscuit</td><td>"+String(p1)+"</td><td>"+String(c1)+"</td></tr>";
    page += "<tr><td>Soap</td><td>"+String(p2)+"</td><td>"+String(c2)+"</td></tr><tr><td>Rice(1KG)</td><td>"+String(p3)+"</td><td>"+String(c3)+"</td>";
    page += "</tr><tr><td>Tea(50g)</td><td>"+String(p4)+"</td><td>"+String(c4)+"</td></tr><tr><th>Grand Total</th><th>"+String(count_prod)+"</th><th>"+String(total)+"</th>";
    page += "</tr></table><br><input type=\"button\" name=\"Pay Now\" value=\"Pay Now\" style=\"width: 200px;height: 50px\"></center></body></html>, page)";
    });
    server.begin();
    //  --------------------------END----------------------------------------------------- //

}
// -- Print button function for future scope. (FUTURE SCOPE) -- //

// void printButton(bool a){
//   Serial.println("THIS IS BUTTON RN: ");
//     Serial.println(a);
//     Serial.println("\n");
//     delay(5000);
// }

void loop() {
  // -------------------------------Ignore this part (FUTURE SCOPE)------------------------------------------------ //
    // a = digitalRead(D0); // D0 will have the 'REMOVE' button.
    // Serial.println(a);
    // if (currentButtonState == LOW && a == false) {
    //     // Button is clicked for the first time, toggle its state
    //     a = true;
    // } else if (currentButtonState == LOW && a == true) {
    //     // Button was clicked and its state was already true, do nothing for this click

    // } else if (currentButtonState == HIGH && a == true) {
    //     // Button was released after being clicked, reset its state
    //     a = false;
    // }
    // printButton(a);
    // count = 0;
  // -------------------------------Ignore this part (FUTURE SCOPE)------------------------------------------------ //
  
  if ( ! rfid.PICC_IsNewCardPresent())
    return; //WHEN NO CARD IS PRESENT. rest of the loop is not executed until a new card is detected.

  if (rfid.PICC_ReadCardSerial()) {
      // a = digitalRead(D0); //D0 HAS THE 'REMOVE' BUTTON (FUTURE SCOPE).
      a = 1;
    count = 0;
    //LOOP TO READ RFID.
    for (byte i = 0; i <= 4; i++) {
      tag += rfid.uid.uidByte[i];
    }

    count = tag.length();
      //-------------------------------- PRODUCT 1: T-shirt --------------------------------//
      if ((tag == "1631031851490") && (a == 1))
      {
        Serial.println("");

        Serial.println("T-shirt ADDED ");
        Serial.println("Price(Rs):350.00      ");

        lcd.setCursor(0, 0);
        lcd.print("T-shirt Added       ");
        lcd.setCursor(0, 1);
        lcd.print("Price(Rs):350.00      ");
        p1++;
        digitalWrite(D8,HIGH);
        delay(2000);
        total = total + 350.00;
        count_prod++;
        digitalWrite(D8,LOW);
        lcd.clear();
       
      }
      else if ((tag == "1631031851490") && (a == 0))
      {
        if(p1>0)
        {
            Serial.println("");

            Serial.println("T-shirt REMOVED");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("T-shirt Removed!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            p1--;
            total = total - 350.00;
            count_prod--;
            lcd.clear();
            digitalWrite(D8,LOW);
            
        }
        else
        {
            Serial.println("");

            Serial.println("T-shirt CALLED ELSE IF ELSE");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not in cart!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            digitalWrite(D8,LOW);
            lcd.clear();
            
        }
      }
        //-------------------------------- PRODUCT 2: Cargo --------------------------------//

      else if ((tag == "147185117150") && (a==1))

      {
        Serial.println("Cargo ADDED ");
        Serial.println("Price(Rs):400.00      ");
        lcd.setCursor(0, 0);
        lcd.print("Cargo Added          ");
        lcd.setCursor(0, 1);
        lcd.print("Price(Rs):400.00         ");
        total = total + 400.00;
        digitalWrite(D8,HIGH);
        delay(2000);
        p2++;
        count_prod++;
        digitalWrite(D8,LOW);
        lcd.clear();
      }
      else if ((tag == "147185117150") && (a==0))
      {
        if(p2>0)
        {
            Serial.println("Cargo removed ");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Cargo Removed!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            p2--;
            total = total - 400.00;
            count_prod--;
            lcd.clear();
            digitalWrite(D8,LOW);
        }
        else
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not in cart!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            lcd.clear();
            digitalWrite(D8,LOW);
        }
      }
        //-------------------------------- PRODUCT 3: Shoes --------------------------------//

      else if ((tag == "195229106150") && (a == 1))
      {
        Serial.println("Shoes ADDED ");
        Serial.println("Price(Rs):550.00      ");
        lcd.setCursor(0, 0);
        lcd.print("Shoes Added       ");
        lcd.setCursor(0, 1);
        lcd.print("Price(Rs):550.00      ");
        total = total + 550.00;
        digitalWrite(D8,HIGH);
        delay(2000);
        count_prod++;
        p3++;
        lcd.clear();
        digitalWrite(D8,LOW);
      }
      else if ((tag == "195229106150") && (a ==0))
      {
        if(p3>0)
        {
            Serial.println("Shoes REMOVED ");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Shoes Removed!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            total = total - 550.00;
            p3--;
            count_prod--;
            lcd.clear();
            digitalWrite(D8,LOW);
        }
        else
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not in cart!!!        ");
            digitalWrite(D8,HIGH);
            delay(2000);
            lcd.clear();
            digitalWrite(D8,LOW);
        }
      }
    // Master Card.
      else if (tag == "147762041660")
      {
        Serial.println("Total Prod: ");
        Serial.println(count_prod);
        Serial.print("Price:");
        Serial.print(total);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Total Prod:");
        lcd.setCursor(11, 0);
        lcd.print(count_prod);
        lcd.setCursor(0, 1);
        lcd.print("Price:");
        lcd.setCursor(6, 1);
        lcd.print(total);
        
        digitalWrite(D8,HIGH);
        delay(2000);

        lcd.clear();
        digitalWrite(D8,LOW);
        Serial.println("\nThank you  ");
        Serial.println(" for Shopping   ");

        lcd.setCursor(0, 0);
        lcd.print("   Thank you        ");
        lcd.setCursor(0, 1);
        lcd.print("  for Shopping        ");
        digitalWrite(D8,LOW);
      }

    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
  }
}