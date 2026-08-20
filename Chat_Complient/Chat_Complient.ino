#include <WiFi.h>         
#include <HTTPClient.h>        
#include <ArduinoJson.h>

const char* WIFI_SSID = "your wifi name";
const char* WIFI_PASSWORD = "your wifi password";

const char* api_key = "your api key"; //this is your api key, if you still didn't have, go to OpenAI and get the api key

const char* url = "https://openai/v1/chat/completions"; //this is the official link from OpenAI, you can change it to your own based url


void setup() {
  Serial.begin(115200);
  mulai_wifi();

  Serial.println("type your question ...");

}

void loop() {
  // This will automathicly Run in the serial monitor
  String model = "gpt-4o-mini";

  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    input.trim();

    Serial.println(String("Question : ") + input);
    Serial.print(chat(input, model)); 
    Serial.print('\n');
  }

}



void mulai_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Starting Connection");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print("><");
    delay(500);
  }
  Serial.println("\n WiFi Connected");
}



String chat(String text, String model ){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + api_key);

    String toSend =
    "{"
      "\"model\":\""+ model +"\","
      "\"messages\":["
        "{"
          "\"role\":\"user\","
          "\"content\":\""+ text +"\""
        "}"
      "]"
    "}";

    int code = http.POST(toSend);
    Serial.print(code);
    Serial.print('\n');

    if(code == 200){
      String answer = http.getString();
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, answer);

      String content = doc["choices"][0]["message"]["content"].as<String>();
      return content;
      //return answer;
    }
    else if(code == 400) return "Bad request | 400";
    else if(code == 401) return "Unauthorization | 401";
    else return "There was something error";
  }
  else{
    Serial.println("WiFi is not connected | Try again");
  }
}
