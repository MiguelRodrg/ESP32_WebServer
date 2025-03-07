// [PROGRAMA REMOTE CONTROL SERVOS WEBSERVER ESP32 DEVKITC V4]
// Autor: David Miguel Garcia Palacios
// Universidad Nacional de Colombia Sede Bogota

// [CONFIGURACION INICIAL]

// Librerias a usar
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Objetos servo
Servo servo_1;
Servo servo_2;
Servo servo_3;

// Define pines a utilizar
const int pin_servo_1 = 11;
const int pin_servo_2 = 6;
const int pin_servo_3 = 10;
const int pin_motor_1_1 = 12;
const int pin_motor_1_2 = 7;
const int pin_motor_2_1 = 4;
const int pin_motor_2_2 = 5;
const int pin_motor_3_1 = 2;
const int pin_motor_3_2 = 3;

// Parametros y configuracion de red
const bool access_point_mode = false;
const char* ssid = "AP_ESP32_WEB_SERVER";
const char* password = "PROTOBOARDISMINE";

// Crea el servidor web en el puerto indicado
WebServer server(80);

// [FUNCIONES PARA CONTROL DE MOTORES]

// Funcion detener todos los motores
void motor_stop() {
  digitalWrite(pin_motor_1_1, LOW);
  digitalWrite(pin_motor_1_2, LOW);
  digitalWrite(pin_motor_2_1, LOW);
  digitalWrite(pin_motor_2_2, LOW);
  digitalWrite(pin_motor_3_1, LOW);
  digitalWrite(pin_motor_3_2, LOW);
}

// Funcion avanzar todos los motores
void motor_forward() {
  digitalWrite(pin_motor_1_1, HIGH);
  digitalWrite(pin_motor_1_2, LOW);
  digitalWrite(pin_motor_2_1, HIGH);
  digitalWrite(pin_motor_2_2, LOW);
  digitalWrite(pin_motor_3_1, HIGH);
  digitalWrite(pin_motor_3_2, LOW);
}

// Funcion para ajuste de angulo de los servos
void servo_angle_set(int a_1, int a_2, int a_3) {
  int servo_1_ang = servo_1.read();
  int servo_2_ang = servo_2.read();
  int servo_3_ang = servo_3.read();
  while (servo_1_ang != a_1 || servo_2_ang != a_2 || servo_3_ang != a_3) {
    if (servo_1_ang < a_1) {servo_1_ang++;}
    if (servo_1_ang > a_1) {servo_1_ang--;}
    if (servo_2_ang < a_2) {servo_2_ang++;}
    if (servo_2_ang > a_2) {servo_2_ang--;}
    if (servo_3_ang < a_3) {servo_3_ang++;}
    if (servo_3_ang > a_3) {servo_3_ang--;}
    servo_1.write(servo_1_ang);
    servo_2.write(servo_2_ang);
    servo_2.write(servo_2_ang);
    delay(5);
  }
}

// Funcion para alinear todos los servos a una direccion
void servo_align(int ang) {
  servo_angle_set(90+ang, 90+ang, 90-ang);
}

// [FUNCIONES TIPO HANDLE]

// Funcion handle de izquierda
void handle_left() {
  servo_align(45);
}

// Funcion handle de right
void handle_right() {
  servo_align(135);
}

// [SETUP DE ESP32]

void setup() {

  // Establece comunicacion
  Serial.begin(115200);
  delay(1000);
  
  // Configuracion de los pines
  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2);
  servo_3.attach(pin_servo_3);
  pinMode(pin_motor_1_1, OUTPUT);
  pinMode(pin_motor_1_2, OUTPUT);
  pinMode(pin_motor_2_1, OUTPUT);
  pinMode(pin_motor_2_2, OUTPUT);
  pinMode(pin_motor_3_1, OUTPUT);
  pinMode(pin_motor_3_2, OUTPUT);

  // Configurar el modo de red para la conexion
  if (access_point_mode) {
    Serial.println("Inciando WiFi en modo ACCESS POINT");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.print("Conexion Exitosa! | Direccion IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Inciando WiFi en modo EXTERNAL NET");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print(".");
    }
    Serial.print("\nConexión exitosa! | Direccion IP: ");
    Serial.println(WiFi.localIP());
  }

  // Declaracion de las funciones a solicitud del servidor
  server.on("/motor_stop", motor_stop);
  server.on("/motor_forward", motor_forward);
  server.on("/servo_left", handle_left);
  server.on("/servo_right", handle_right);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado con exito!");
}

// [LOOP DE ESP32]

void loop() {
  server.handleClient();
}