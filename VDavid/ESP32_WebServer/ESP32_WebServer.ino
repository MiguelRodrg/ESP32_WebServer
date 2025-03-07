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
const char* ap_ssid = "Perry";
const char* ap_password = "t4y5u6i7o8";

// Crea el servidor web en el puerto indicado
WebServer server(80);


// Función para manejar CORS
void handleCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// Funcion LED ON
void handle_led_on() {
  digitalWrite(pin_led, HIGH);
  handleCors();  // Agregar encabezados CORS
  server.send(200, "text/plain", "LED ENCENDIDO");
  Serial.println("LED ENCENDIDO");
}

// Funcion LED OFF
void handle_led_off() {
  digitalWrite(pin_led, LOW);
  handleCors();
  server.send(200, "text/plain", "LED APAGADO");
  Serial.println("LED APAGADO");
}

// Funcion SERVO ARRIBA
void handle_servo_up() {
  ang = servo1.read() + 5;
  Serial.print("SERVO U = "); Serial.println(ang);
  servo1.write(ang);
  handleCors();
  server.send(200, "text/plain", "SERVO ARRIBA");
}

// Funcion SERVO ABAJO
void handle_servo_down() {
  ang = servo1.read() - 5;
  Serial.print("SERVO D = "); Serial.println(ang);
  servo1.write(ang);
  handleCors();
  server.send(200, "text/plain", "SERVO ABAJO");
}

// Manejo de solicitudes OPTIONS para CORS
void handleOptions() {
  handleCors();
  server.send(204); // Respuesta sin contenido
}

// [SETUP DE ESP32]
void setup() {
  // Pines y comunicación
  Serial.begin(115200);
  delay(1000);

  servo1.attach(pin_servo);
  pinMode(pin_led, OUTPUT);

  // Configurar el Punto de Accseso a ESP32
  Serial.println("Configurando el punto de acceso como: " + String(ap_ssid));
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Configuracion exitosa en la direccion IP: ");
  Serial.println(WiFi.softAPIP());

  // Declaracion de las rutas para funciones handle
  server.on("/led_on", HTTP_GET, handle_led_on);
  server.on("/led_off", HTTP_GET, handle_led_off);
  server.on("/servo_up", HTTP_GET, handle_servo_up);
  server.on("/servo_down", HTTP_GET, handle_servo_down);

  // Manejo de preflight requests para CORS
  server.onNotFound(handleOptions);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado con éxito!");
}

// [LOOP DE ESP32]
void loop() {
  server.handleClient();
}
