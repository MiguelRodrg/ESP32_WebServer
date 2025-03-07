// [PROGRAMA WEBSERVER ESP32 DEVKITC V4]
// David Miguel Garcia Palacios
// Universidad Nacional de Colombia Sede Bogota

// [CONFIGURACION INICIAL]

// Librerias a usar
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Objetos servo
Servo servo1;

// Variables a usar
int ang = 0;

// Parametros de configuracion
const int pin_led = 5;
const int pin_servo = 18;
const char* ap_ssid = "AP_ESP32_WEB_SERVER";
const char* ap_password = "PROTOBOARDISMINE";

// Crea el servidor web en el puerto indicado
WebServer server(80);

// [FUNCIONES HANDLE DE  CONTROL REMOTO]

// Funcion LED ON
void handle_led_on() {
  digitalWrite(pin_led, HIGH);
  server.send(200, "text/plain", "LED ENCENDIDO");
}

// Funcion LED OFF
void handle_led_off() {
  digitalWrite(pin_led, LOW);
  server.send(200, "text/plain", "LED APAGADO");
}

// Funcion SERVO ARRIBA
void handle_servo_up() {
  ang = servo1.read() + 5;
  Serial.print("SERVO U = "); Serial.println(ang);
  servo1.write(ang);
}

// Funcion SERVO ABAJO
void handle_servo_down() {
  ang = servo1.read() - 5;
  Serial.print("SERVO D = "); Serial.println(ang);
  servo1.write(ang);
}

// [SETUP DE ESP32]
void setup() {

  // Pines y comunicacion
  Serial.begin(115200);
  delay(1000);

  servo1.attach(pin_servo);

  pinMode(pin_led, OUTPUT);

  // Configurar el Punto de Accseso a ESP32
  Serial.println("Configurando el punto de acceso como: " + String(ap_ssid));
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Configuracion exitosa en la direccion IP: " + String(WiFi.softAPIP()));
  Serial.println(WiFi.softAPIP());

  // Declaracion de las rutas para funciones handle
  server.on("/led_on", handle_led_on);
  server.on("/led_off", handle_led_off);
  server.on("/servo_up", handle_servo_up);
  server.on("/servo_down", handle_servo_down);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado con exito!");
}

// [LOOP DE ESP32]
void loop() {
  server.handleClient();
  delay(1000);
}