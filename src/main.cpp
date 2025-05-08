#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// Firebase helpers
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// Timestamp
String getLocalTimeISO();
String getLocalTimeUNIX();

// NTP
#define NTP_SERVER "pool.ntp.org"
#define NTP_GMT_OFFSET_SEC 3600
#define NTP_DAYLIGHT_OFFSET_SEC 3600


// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Global variables
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// Definir PINes
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define PIN_RGB_ROJO_LED 4
#define PIN_RGB_VERDE_LED 16
#define PIN_RGB_AZUL_LED 17
#define PIN_ROJO_LED 32
#define PIN_VERDE_LED 33

//Variables Datos Sensor
float humedad;
float temperatura;

void setup()
{
  // Initialize Serial
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(333);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize NTP
  configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Inicialize dht
  dht.begin();

  // Inicialice PIN_LED
    pinMode(PIN_RGB_ROJO_LED, OUTPUT);
    pinMode(PIN_RGB_VERDE_LED, OUTPUT);
    pinMode(PIN_RGB_AZUL_LED, OUTPUT);
    pinMode(PIN_VERDE_LED, OUTPUT);
    pinMode(PIN_ROJO_LED, OUTPUT);

 
    

  // Sign up
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Firebase signup ok!");
    signupOK = true;
  }
  else
  {
    Serial.println("Firebase signup failed!");
  }

  // Assign callback function for token generation task
  config.token_status_callback = tokenStatusCallback;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop ()
{
  boolean Confort;
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
  String timestamp = getLocalTimeUNIX();
  static int intValue = 0;
  float floatValue = 0.0;
  static bool boolValue;
  String Fecha = getLocalTimeISO();
 
  

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3600 || sendDataPrevMillis == 0))
  
  { 
    // Firebase is ready, we are signup and 10 seconds has passed
    // Save current time
    sendDataPrevMillis = millis();
    

    // Escribir int
    //Serial.print("INT WRITE ");
    //intValue++;
    //if (Firebase.RTDB.setInt(&fbdo, "sensor/",))
    //{
      //Serial.println("OK");
      //Serial.println("  PATH: " + fbdo.dataPath());
      //Serial.println("  TYPE: " + fbdo.dataType());
      //Serial.print("  VALUE: ");
      //Serial.println(fbdo.intData());
    //}
    //else
    //{
      //Serial.println("FAILED");
      //Serial.println("  REASON: " + fbdo.errorReason());
    //}

    // 
    // String pathBase = "sensor/" + timestamp + "/";
    // Write sample float
    floatValue = 0.01 + random (0,100);
    Serial.print("FLOAT WRITE ");
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/" + timestamp + "/Humedad", humedad))
    {
      Serial.println("OK");
      Serial.println("  PATH: " + fbdo.dataPath());
      Serial.println("  TYPE: " + fbdo.dataType());
      Serial.print("  VALUE: ");
      Serial.println(fbdo.floatData());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }
floatValue = 0.01 + random (0,100);
    Serial.print("FLOAT WRITE ");
    if (Firebase.RTDB.setFloat(&fbdo, "sensor/" + timestamp + "/Temperatura", temperatura))
    {
      Serial.println("OK");
      Serial.println("  PATH: " + fbdo.dataPath());
      Serial.println("  TYPE: " + fbdo.dataType());
      Serial.print("  VALUE: ");
      Serial.println(fbdo.floatData());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Write sample boolean
    // Interpretar Confort

    if (temperatura < 25 && temperatura > 18 && humedad <60 && humedad > 40)
    {
    Confort = true;
    }
    else
    {
      Confort = false;
    }

    boolValue = !boolValue;
    Serial.print("BOOLEAN WRITE ");
    if (Firebase.RTDB.setBool(&fbdo, "sensor/" + timestamp + "/Confort", Confort))
    {
      Serial.println("OK");
      Serial.println("  PATH: " + fbdo.dataPath());
      Serial.println("  TYPE: " + fbdo.dataType());
      Serial.print("  VALUE: ");
      Serial.println(fbdo.boolData());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Write sample string
    Serial.print("STRING WRITE ");
    if (Firebase.RTDB.setString(&fbdo, "sensor/" + timestamp + "/Fecha", Fecha))
    {
      //Serial.println("OK");
      //Serial.println("  PATH: " + fbdo.dataPath());
      //Serial.println("  TYPE: " + fbdo.dataType());
      //Serial.print("  VALUE: ");
      //Serial.println(fbdo.stringData());
    //}
    //else
    //{
     // Serial.println("FAILED");
      //Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Read sample int
  

 if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3600 || sendDataPrevMillis == 0)){
    int LED_RED1;
    int LED_Blue;
    int LED_Green;

    Serial.print("read/int: ");
    if (Firebase.RTDB.getInt(&fbdo, "actuador/RGB/Red", &LED_RED1))
    {
      Serial.println(LED_RED1);
             analogWrite(PIN_RGB_ROJO_LED, LED_RED1);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }
// Read sample int
  
 
    Serial.print("read/int: ");
    if (Firebase.RTDB.getInt(&fbdo, "actuador/RGB/Blue", &LED_Blue))
    {
      Serial.println(LED_Blue);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }
    // Read sample int
  
    
    Serial.print("read/int: ");
    if (Firebase.RTDB.getInt(&fbdo, "actuador/RGB/Green", &LED_Green))
    {
      Serial.println(LED_Green);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }
    
    // Read sample float
    float floatRead = 0.0;
    Serial.print("read/float: ");
    if (Firebase.RTDB.getFloat(&fbdo,  "read/float", &floatRead))
    {
      Serial.println(floatRead);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Read sample boolean
    bool boolRead = false;
    Serial.print("read/boolean: ");
    if (Firebase.RTDB.getBool(&fbdo, "read/boolean", &boolRead))
    {
      Serial.println(boolRead);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Read sample string
    String stringRead = "";
    Serial.print("read/string: ");
    if (Firebase.RTDB.getString(&fbdo, "read/string", &stringRead))
    {
      Serial.println(stringRead);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("  REASON: " + fbdo.errorReason());
    }

    // Print time
    String timestamp_unix = getLocalTimeUNIX();
    Serial.println("UNIX: " + timestamp_unix);

    String timestamp_iso = getLocalTimeISO();
    Serial.println("ISO: " + timestamp_iso);
  }
}
}

String getLocalTimeISO()
{
  struct tm timeinfo;
  char buffer[20];

  // Get local time
  if(!getLocalTime(&timeinfo))
  {
    return "NTP Error!";
  }

  // Obtain ISO 8601 timestamp
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  return String(buffer);
}

String getLocalTimeUNIX()
{
  struct tm timeinfo;

  // Get local time
  if(!getLocalTime(&timeinfo))
  {
    return "NTP Error!";
  }

  // Obtain UNIX timestamp
  return String(mktime(&timeinfo));

  // Control LEDs


}
