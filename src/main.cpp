#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <map>

const char* mqttServer = "44.216.84.214";
const int mqttPort = 1883;
const char* mqttTopic = "AustralFI/inel15";

WiFiClient espClient;
PubSubClient client(espClient);

#define LED_PIN 2
#define LED_PIN2 25
#define NUM_PIXELS 18
#define BUTTOM 33
#define PIN2 32

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, PIN2, NEO_GRB + NEO_KHZ800);


const char* topic = "AustralFI/inel15/receive/#";
const char* topic2 = "AustralFI/inel15/get/Pepito";
const char* stocktopic = "AustralFI/inel15/receive/get";


void publishToMQTT(const char* topic, const char* message) {
  if (client.connected()) {
    client.publish(topic, message);
  }
}

void setPixelColor(int r, int g, int b, int pixel) {
  strip1.setPixelColor(pixel, strip1.Color(r, g, b));
  strip1.fill(strip1.Color(r,g,b));
  strip1.show();
}

void setStockPixelColor(int r, int g, int b, int pixel, int qty) {
  Serial.println(qty);
  for (int i = 0; i < qty && i < 18; i++) {
    strip2.setPixelColor(i, strip2.Color(r, g, b));
  }
  strip2.show();
}  

void handleLeds(String transactionStatus) {
  Serial.println("prendiendo luces led");
  Serial.println(transactionStatus);
 // Serial.println(transactionStatus);
 if (transactionStatus == "Transaccion exitosa") {
    setPixelColor(0, 255, 0, 0);  // Verde
    delay(3000);
    setPixelColor(0,0,0,0);
    publishToMQTT(topic2,"");

  } else if (transactionStatus == "No hay stock") {
    setPixelColor(255, 0, 0, 0);  // Rojo
    delay(3000);
    setPixelColor(0,0,0,0);
  }
  else if (transactionStatus == "Reposicion existosa") {
    setPixelColor(0, 0, 255, 0);  // azul
    delay(3000);
    setPixelColor(0,0,0,0);
    publishToMQTT(topic2,"");
  }
   else {
  }
}



void showStock(int qty){
  setStockPixelColor(0,0,0,0,18);
  if (qty <= 6){
    setStockPixelColor(255, 0, 0, 0,qty);  // rojo
  }
  else if (6 < qty <= 12){
    setStockPixelColor(255, 128, 0, 0,qty);  // amarillo
  }
  else{
    setStockPixelColor(0, 255, 0, 0,qty);  // Verde
  }

}

// Función que se ejecuta cuando se conecta al broker
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Entro al callback");
    Serial.println(topic);
    


  Serial.print("Contenido del payload: ");
  const String topics = "AustralFI/inel15/receive/buy";
  const String topicRepose = "AustralFI/inel15/receive/repose";
  const String stocktopico = "AustralFI/inel15/receive/get";

  const char* myChar = topics.c_str();
  const char* reposeComp = topicRepose.c_str();
  const char* stocktopicCmp = stocktopico.c_str();
  
  
  Serial.println();


 if (strcmp(topic, myChar) == 0 || strcmp(topic,reposeComp) == 0) {
    Serial.println("buying stock");
    DynamicJsonDocument doc(512);
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
            Serial.print("Error al deserializar JSON: ");
            Serial.println(error.c_str());
            // Manejar el error según tus necesidades
        } else {
            // Acceder a la variable qty
            String response = doc["_response"];
            Serial.print("response: ");
            Serial.println(response);
            
            handleLeds(response);
   }
   }
    else if (strcmp(topic, stocktopicCmp) == 0) {
      Serial.println("Procesando JSON...");
        DynamicJsonDocument doc(512);
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
            Serial.print("Error al deserializar JSON: ");
            Serial.println(error.c_str());
            // Manejar el error según tus necesidades
        } else {
            // Acceder a la variable qty
            int qty = doc["qty"];
            Serial.print("Cantidad: ");
            Serial.println(qty);
            int qty_int = doc["qty"].as<int>();
           /* if (qty_int == 0)
            {
              handleLeds('b');
            }*/
            
            showStock(qty_int);
    }
    }
    Serial.println();
    client.loop();
}


void setup() {
  strip1.begin();
  strip1.show();

  pinMode(BUTTOM, INPUT_PULLUP);
  
  Serial.begin(115200);
  delay(1000);
  WiFi.begin("UA-Alumnos", "41umn05WLC");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  client.connect("ESP32Client");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
}


void loop() {
  if (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al servidor MQTT");
    } else {
        Serial.print("Fallo en la conexión al servidor MQTT, rc=");
        Serial.print(client.state());
        Serial.println(" Intentando de nuevo en 5 segundos");
        delay(5000);
      return;
    }
  } else {
    // Serial.println("Estás conectado al servidor MQTT");
     while (!client.subscribe(topic))
  {
    Serial.println("no se pudo suscribir");
    client.subscribe(topic);
  }
  }

 // Serial.println("ingrese lo que desea comprar");
  delay(1500);

  int buttonState = digitalRead(BUTTOM);
  if (buttonState == LOW) {
    publishToMQTT("AustralFI/inel15/buy/Pepito", "-1");
    
  }


  client.loop();
}