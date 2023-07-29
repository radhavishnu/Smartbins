
// The section for library . HX711 is for load cell

#include "HX711.h"
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(7, 6); //RX, TX

/* Weight sensors
 *  DT -> 4,8
 *  SCK -> 2,7
 *  Gnd -> Gnd
 */

 int smsPrev_food=0;
 int smsPrev_solid=0;
 int smsPrev_plastic=0;
 int smsPrev_other=0;
 

// pis for reading waste fill
struct UltrasonicPins{
  int trig;
  int echo;
  };

  UltrasonicPins plastic;
  UltrasonicPins others;

//int weigh_per to store weght in percentage

int weight_per =0;
struct Weight {

  int DT;
  int SCK;
};

Weight food;
Weight solid;

EthernetClient client;

HX711 foodScale,solidScale;


void setup() {
Serial.begin(9600);
  plastic.trig = 2;
  plastic.echo=3;
  
  others.trig=4;
  others.echo=5;

 food.DT = 8;
 food.SCK = 9;

 solid.DT = 10;
 solid.SCK = 11;

    gsmSerial.begin(9600); // Setting the baud rate of GSM Module
   
   delay(1000);
   Serial.println("Preparing to send SMS");

  
  // put your setup code here, to run once:
  pinMode(plastic.trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(plastic.echo, INPUT); // Sets the echoPin as an Input
    pinMode(others.trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(others.echo, INPUT); // Sets the echoPin as an Input

  foodScale.begin(food.DT, food.SCK);
  foodScale.set_scale();
  //foodScale.tare(); 

    solidScale.begin(solid.DT, solid.SCK);
  solidScale.set_scale();
  //solidScale.tare(); 

  


  IPAddress ip(192, 168, 10, 1);
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
Ethernet.begin(mac, ip);
}

void loop() 
{
  Serial.println("hi");
  // put your main code here, to run repeatedly:
  int dist_plas = read_distance(plastic, "plastic");
  Serial.println("hi1");
  int dist_othr = read_distance(others, "other");
  Serial.println("hi2");
  int food_weight = read_weight(foodScale, "food");
  Serial.println("hi3");
 // int solid_weight = read_weight(solidScale, "solid");
   Serial.println("hi4");
  
  //Serial.println(food_w);
  //float food_perc = (food_w); //Percent
 sendEthernet(food_weight,0,dist_plas,dist_othr);
 Serial.println("hi5");
  Serial.println(dist_othr);

 smsPrev_food=food_weight;
// smsPrev_solid=solid_weight;
 smsPrev_plastic=dist_plas;
 smsPrev_other=dist_othr;
  
  delay(500);

}



//function to read weight for food
int read_weight(HX711 scale,String type)
  {
Serial.println("slld scael");
  
        float calibration_factor = 98780; // calibrated using 1.922 kgs
        scale.set_scale(calibration_factor); //Adjust to this calibration factor
        Serial.println("slld scael cccc");
        float weight = scale.get_units()*0.45359237;

        Serial.println("slld scael cccssssssssssc");
        //int weightt = floor(weight*10);
        //int weightt = round(weight);
        weight_per=map(weight,0.0,4.0,0.0,100.0);

        if (weight_per>=75)

          {
                if(type.equals("solid")&& smsPrev_solid<75)
                {
                SendMessage("Solid waste Bin Is Full at LOC 1");
                //Serial.println("solid wrs");
                }

                if(type.equals("food")&& smsPrev_food<75)
                {
                SendMessage("Food Bin Is Full at LOC 1");
                //Serial.println("food wrs");
                }
          }
          
        return weight_per;
  }



int read_distance(UltrasonicPins us, String type)

   {
        
                int trig = us.trig;
                int echo = us.echo;
                 
                
                double duration;
                double distance;
                const int bin_len = 20;
                
                
                // Clears the trigPin
                digitalWrite(trig, LOW);
                delayMicroseconds(2);
                // Sets the trigPin on HIGH state for 10 micro seconds
                digitalWrite(trig, HIGH);
                delayMicroseconds(10);
                digitalWrite(trig, LOW);
                // Reads the echoPin, returns the sound wave travel time in microseconds
                duration = pulseIn(echo, HIGH);
                // Calculating the distance
                distance= duration*0.034/2; //cm/sec
        
          if(bin_len>distance)
          {
          int percent = (bin_len - distance)*100/bin_len;

          if (percent>=75)

          {
                if(type.equals("plastic")&& smsPrev_plastic<75)
                {
                SendMessage("Plastic waste Bin Is Full at LOC 1");
                //Serial.println("plas works");
                }

                if(type.equals("other")&& smsPrev_other<75)
                {
                SendMessage("General Bin Is Full at LOC 1");
                //Serial.println("other works");
                }
          }
          return percent;
          }
          else
          {
          return 0;
          }
  
}


  


void sendEthernet(int food, int plastic, int papers, int others)
  { Serial.println("hi");


              int    HTTP_PORT   = 5050;
              String HTTP_METHOD = "GET";
              char   HOST_NAME[] = "192.168.10.2";
              String PATH_NAME   = "/setvals";
              
              if(client.connect(HOST_NAME, HTTP_PORT)){
              
                 String s1 = "?value1=";
                 String s2 = "&value2=";
                 String s3 = "&value3=";
                 String s4 = "&value4=";
              
                 String val1s = String(food);
                 String val2s = String(plastic);
                 String val3s = String(papers);
                 String val4s = String(others);
              
                 String queryString = s1+val1s+s2+val2s+s3+val3s+s4+val4s;
                 Serial.println(queryString);
              
                 client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
                 client.println("Host: " + String(HOST_NAME));
                  client.println("Connection: close");
                  client.println();
              
              }


 }

 void SendMessage(String message)
  {
         Serial.println("Setting the GSM in text mode");
         gsmSerial.println("AT+CMGF=1\r");
         delay(100);
         Serial.println("Sending SMS to the desired phone number!");
         gsmSerial.println("AT+CMGS=\"+919497188959\"\r");
         // Replace x with mobile number
         delay(100);
      
         gsmSerial.println(message);    // SMS Text
         Serial.println("Message sent");
         delay(100);
         gsmSerial.println((char)26);               // ASCII code of CTRL+Z
         
      
         delay(100);
  }



  
