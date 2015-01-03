// Currently supported API (attached to /arduino):
//
// "/temperature" -> analogRead(0)
// "/light"       -> analogRead(1)

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <LiquidCrystal.h>

int temperaturePin = 0;
int lightPin = 1;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Set columns and rows to use for the LCD display on the Yun
  lcd.begin(16, 2);
  // Start on the first row
  lcd.print("Waiting...");

  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void process(YunClient client) {
  String command = client.readStringUntil('\r');

  if (command == "temperature") {
    analogCommand(client, 0);
  } else if (command == "light") {
    analogCommand(client, 1);
  } else {
    handleNotFound(client, command);
  }  
}

void analogCommand(YunClient client, int pin) {
  int value;
  int reading = analogRead(pin);

  client.println("Status: 200");
  client.println("Content-type: application/json; charset=utf-8");
  client.println();

  if (pin == lightPin) {
    client.print("{\"light\": " + String(reading) + ", \"measure\": \"lx\"}");
    
    lcd.setCursor(0, 0);
    lcd.print("Requested: Light");
    lcd.setCursor(0, 1);
    lcd.print("Output: " + String(reading) + " lx   ");
    
  } else if(pin == temperaturePin) {
    float temperatureInVolts = (reading / 1024.0) * 5.0;
    float temperatureInCelcius = (temperatureInVolts - 0.5) * 100;
    
    client.print("{\"temperature\": " + String(temperatureInCelcius) + ", \"measure\": \"C\"}");
    
    lcd.setCursor(0, 0);
    lcd.print("Requested: Temp ");
    lcd.setCursor(0, 1);
    lcd.print("Output: " + String(temperatureInCelcius) + " C");
  }
  // Update datastore key with the current pin value
  String key = "A";
  key += pin;
  Bridge.put(key, String(value));
}

void handleNotFound(YunClient client, String command) {
  client.println("Status: 404");
  
  lcd.setCursor(0, 0);
  lcd.print("R:" + command);
  lcd.setCursor(0, 1);
  lcd.print("Output:Not found");
}
