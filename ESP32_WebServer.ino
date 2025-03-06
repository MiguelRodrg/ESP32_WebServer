// [PROGRAMA WEBSERVER ESP32 DEVKITC V4]
// David Miguel Garcia Palacios
// Universidad Nacional de Colombia Sede Bogota

// [CONFIGURACION INICIAL]

// Librerias a usar
#include <WiFi.h>
#include <WebServer.h>

// Define los pins a utilizar
const int pin_led = 5;

// Define nombre de red y clave
const char* net_ssid = "MotoG20_damigapa00";
const char* net_password = "CODETristy20";

// Define el metodo de conexion a red. [false] Conexion a red externa. [true] Creacion de red access point.
const bool create_net = false;

// Define el puerto del servidor
WebServer server(80);

// [FUNCIONES DE CONTROL REMOTO]

// Funcion LED ON
void remote_led_on() {
  digitalWrite(pin_led, HIGH);
  server.send(200, "text/plain", "[+] Led encendido");
}

// Funcion LED OFF
void remote_led_off() {
  digitalWrite(pin_led, LOW);
  server.send(200, "text/plain", "[+] Led apagado");
}

// Funcion BOOP (Funcion Dummy)
void remote_boop() {
  server.send(200, "text/plain", "[+] Boop! UWU");
}

// [SETUP DE ESP32]
void setup() {
  
  // Modo de los pines
  pinMode(pin_led, OUTPUT);

  // Inicio de comunicacion serial
  Serial.begin(115200);
  delay(1000);

  // Configurar la conexion de red WiFi del ESP32
  if(create_net){
    Serial.println("Configurando WiFi en modo Punto de Acceso");
    WiFi.mode(WIFI_AP);
    Serial.println("Creando la red " + String(net_ssid) + " ...");
    WiFi.softAP(net_ssid, net_password);
    Serial.println("Creacion exitosa usando direccion IP:");
    Serial.println(WiFi.softAPIP());
  }else{
    Serial.println("Configurando WiFi en modo Red Externa");
    WiFi.mode(WIFI_STA);
    Serial.println("Conectando a la red " + String(net_ssid) + " ...");
    WiFi.begin(net_ssid, net_password);
    while (WiFi.status() != WL_CONNECTED) {delay(1000); Serial.print(".");}
    Serial.println("\nConexion exitosa usando direccion IP:");
    Serial.println(WiFi.localIP());
  }
  
  // Configurar direcciones HTTP para los comandos
  server.on("/boop", HTTP_GET, remote_boop);
  server.on("/led_on", HTTP_GET, remote_led_on);
  server.on("/led_off", HTTP_GET, remote_led_off);
  
  // Inicializacion del servidor
  server.begin();
  Serial.print("El servidor ESP32 esta listo!");
}

// [LOOP DE ESP32]
void loop() {
  server.handleClient();
}