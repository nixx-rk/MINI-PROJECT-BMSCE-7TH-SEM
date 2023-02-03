#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "Knight";//Replace with your network SSID
const char* password = "7676370637";//Replace with your network password

ESP8266WebServer server(80);

String page = "",pg1="",pg2="",pg3="",pg4="",pgb="",pgp="";
char input[12];
int count = 0;

int tags[]={0,0,0,0,0,0,0,0,0};
int a;
int p1=0,p2=0,p3=0,p4=0;
int c1=0,c2=0,c3=0,c4=0;

double total = 0;
int count_prod = 0;
int items=0,amount=0;
void setup()
{
  pinMode(D3,INPUT_PULLUP);
  pinMode(D4,OUTPUT);
  pinMode(D8,OUTPUT);
  digitalWrite(D8,LOW);
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Wire.begin(D2, D1);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO       ");
  lcd.setCursor(0, 1);
  lcd.print("   SMART CART       ");

  delay(2000);

  lcd.clear();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connecting...  ");
  }
  Serial.print(" \n");
  Serial.print(WiFi.localIP());
  Serial.print(" \n");
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1000);
  
  lcd.setCursor(0, 0);
  lcd.print(" PLZ ADD ITEMS     ");
  lcd.setCursor(0, 1);
  lcd.print("    TO CART          ");
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

server.on("/", hc);
  server.begin();
}
void hc()
  {
    page = "<html><head><title>E Cart using IoT</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  #3498db ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Shopping Cart using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>ITEMS</th><th>QUANTITY</th><th>COST</th></tr>";
    //<tr><td>Biscuit</td><td>"+String(tags[1])+"</td><td>"+String(30)+"</td></tr>"
    // page += "<tr><td>Soap</td><td>"+String(tags[2])+"</td><td>"+String(25)+"</td></tr><tr><td>Rice(1KG)</td><td>"+String(tags[3])+"</td><td>"+String(20)+"</td>";
    // page += "</tr><tr><td>Tea(50g)</td><td>"+String(tags[4])+"</td><td>"+String(15)+"</td></tr>";
     pgp="<tr><th>Grand Total</th><th>"+String(items)+"</th><th>"+String(amount)+"</th></tr></table>";
    pgp += "<br><input type=\"button\" name=\"Pay Now\" value=\"Pay Now\" style=\"width: 200px;height: 50px\"></center></body></html>";
    pgp+="<meta http-eqiv=\"refresh\" content=\"1\">";
    pgb=page+pg1+pg2+pg3+pg4+pgp;
    server.send(200,"text/html",pgb);
  }
void loop()
{
  server.handleClient();
  int a=digitalRead(D0);
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.print("\n"+tag+"\t");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ITEM TAG ID:   ");
    lcd.setCursor(0, 1);
    lcd.print(tag);
    digitalWrite(D8,HIGH);
  delay(500);
  digitalWrite(D8,LOW);
  
  
    if(tag=="9911519546"){
      if(a && tags[1]>0){
        amount-=30;
        items--;
        tags[1]--;
        if (tags[1]==0) pg1="";
        else{
          pg1="<tr><td>Burger</td><td>"+String(tags[1])+"</td><td>"+String(30)+"</td></tr>";
        }         
      }
      else if(!a){
        amount+=30;
        tags[1]++;
        items++;
        pg1="<tr><td>Burger</td><td>"+String(tags[1])+"</td><td>"+String(30)+"</td></tr>";
      }
      lcd.clear();
      Serial.print("Item: Burger\tRs.30\tTotal Amount: Rs."+(String)amount+"\tItems: "+(String)items);
      lcd.setCursor(0,0);
      lcd.print("Item: Burger");
      lcd.setCursor(0,1);
      lcd.print("Price: Rs.30");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Items count: "+(String)items);
      lcd.setCursor(0,1);
      lcd.print("Tot_Amt: Rs."+(String)amount);
    }
    else if(tag=="1157218546"){
      if(a && tags[2]>0){
        amount-=25;
        items--;
        tags[2]--;
        if (tags[2]==0) pg2=""; 
        else{
          pg2="<tr><td>Maggie</td><td>"+String(tags[2])+"</td><td>"+String(25)+"</td></tr>";          
        }
      }
      else if(!a){
        amount+=25;
        tags[2]++;
        items++;
        pg2="<tr><td>Maggie</td><td>"+String(tags[2])+"</td><td>"+String(25)+"</td></tr>";
      }
      lcd.clear();
      Serial.print("Item: Maggie\trs.25\tTotal Amount: Rs."+(String)amount+"\tItems: "+(String)items);
      lcd.setCursor(0,0);
      lcd.print("Item: Maggie");
      lcd.setCursor(0,1);
      lcd.print("Price: Rs.25");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Items count: "+(String)items);
      lcd.setCursor(0,1);
      lcd.print("Tot_Amt: Rs."+(String)amount);
      
    }
    else if(tag=="21042828"){
      if(a && tags[3]>0){
        amount-=20;
        tags[3]--;
        items--;
        if (tags[3]==0) pg3="";
        else{
          pg3="<tr><td>Pepsi</td><td>"+String(tags[3])+"</td><td>"+String(20)+"</td></tr>";
        } 
      }
      else if(!a){
        amount+=20;
        tags[3]++;
        items++;
        pg3="<tr><td>Pepsi</td><td>"+String(tags[3])+"</td><td>"+String(20)+"</td></tr>";
      }
        lcd.clear();
        Serial.print("Item: Pepsi\tRs.20\tTotal Amount: Rs."+(String)amount+"\tItems: "+(String)items);
        lcd.setCursor(0,0);
        lcd.print("Item: Pepsi");
        lcd.setCursor(0,1);
        lcd.print("Price: Rs.20");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Items count: "+(String)items);
        lcd.setCursor(0,1);
        lcd.print("Tot_Amt: Rs."+(String)amount);
      

    }
    else if(tag=="991785946"){
      if(a && tags[4]>0){
        amount-=15;
        tags[4]--;
        items--;
        if (tags[4]==0) pg4="";
        else{
          pg4="<tr><td>Chips</td><td>"+String(tags[4])+"</td><td>"+String(15)+"</td></tr>";
        }        
      }
      else if(!a){
        amount+=15;
        tags[4]++;
        items++;
        pg4="<tr><td>Chips</td><td>"+String(tags[4])+"</td><td>"+String(15)+"</td></tr>";
        
      }
        lcd.clear();
        Serial.print("Item: Chips\tRs.15\tTotal Amount: Rs."+(String)amount+"\tItems: "+(String)items);
        lcd.setCursor(0,0);
        lcd.print("Item: Chips");
        lcd.setCursor(0,1);
        lcd.print("Price: Rs.15");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Items count: "+(String)items);
        lcd.setCursor(0,1);
        lcd.print("Tot_Amt: Rs."+(String)amount);
      
      
    }
  tag = "";
  
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }    
   
}
