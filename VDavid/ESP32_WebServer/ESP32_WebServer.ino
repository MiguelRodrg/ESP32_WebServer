// Universidad Nacional de Colombia Sede Bogotá

// [CONFIGURACION INICIAL]

// Librerías a usar
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Objetos servo
Servo servo1;

// Variables a usar
int ang = 0;
bool forceAPMode = false;  // Bandera para forzar el modo AP

// Parámetros de configuración
const int pin_led = 5;
const int pin_servo = 18;
const char* ap_ssid = "ESP32_robot";         // Nombre de la red propia (AP)
const char* ap_password = "Design2024"; // Contraseña del AP

const char* wifi_ssid = "Mimi 5G";       // Nombre de la red WiFi existente
const char* wifi_password = "iOL34kmDfG12"; // Contraseña de la red WiFi existente

// Crea el servidor web en el puerto indicado
WebServer server(80);

// Función para manejar CORS
void handleCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// Funciones de control
void handle_led_on() {
  digitalWrite(pin_led, HIGH);
  handleCors();
  server.send(200, "text/plain", "LED ENCENDIDO");
  Serial.println("LED ENCENDIDO");
}

void handle_led_off() {
  digitalWrite(pin_led, LOW);
  handleCors();
  server.send(200, "text/plain", "LED APAGADO");
  Serial.println("LED APAGADO");
}

void handle_servo_up() {
  ang = servo1.read() + 5;
  Serial.print("SERVO U = "); Serial.println(ang);
  servo1.write(ang);
  handleCors();
  server.send(200, "text/plain", "SERVO ARRIBA");
}

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
  server.send(204);
}

// Conectar a una red WiFi existente
bool connectToWiFi() {
  Serial.print("Conectando a red WiFi: ");
  Serial.println(wifi_ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 10) {
    delay(1000);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("\nError: No se pudo conectar a WiFi.");
  return false;
}

// Configurar el modo AP (punto de acceso)
void setupAPMode() {
  Serial.println("Iniciando modo AP...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Modo AP activo en IP: ");
  Serial.println(WiFi.softAPIP());
}

// [SETUP DE ESP32]
void setup() {
  Serial.begin(115200);
  delay(1000);

  servo1.attach(pin_servo);
  pinMode(pin_led, OUTPUT);

  // Intentar conectar a WiFi existente, si no, activar AP
  if (!forceAPMode && connectToWiFi()) {
    Serial.println("Modo STA (WiFi existente) activo.");
  } else {
    setupAPMode();
  }

  // Definir rutas
  server.on("/led_on", HTTP_GET, handle_led_on);
  server.on("/led_off", HTTP_GET, handle_led_off);
  server.on("/servo_up", HTTP_GET, handle_servo_up);
  server.on("/servo_down", HTTP_GET, handle_servo_down);
  server.onNotFound(handleOptions);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado con éxito!");
}

// [LOOP DE ESP32]
void loop() {
  server.handleClient();

  // Verificar comandos desde el Monitor Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "SET_AP_MODE") {
      Serial.println("Cambiando a modo AP...");
      forceAPMode = true;
      setupAPMode();
    }
  }
}
