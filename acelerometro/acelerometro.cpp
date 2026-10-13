#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/*
  TODO: 
   - [] Refazer o sistema de display de altitude
*/

Adafruit_MPU6050 mpu;


float acc_v_inicial;
uint8_t voo_iniciado = false;
unsigned long inicio_subida, fim_subida, tempo_subida;


void voo_inicializado(sensors_event_t ac, sensors_event_t t, sensors_event_t g){
  mpu.getEvent(&ac, &g, &t);
  Serial.println(ac.acceleration.z);
    //Detectar fase de subida
    if(ac.acceleration.z < 0){
      fim_subida = millis();
      tempo_subida = fim_subida - inicio_subida;
      Serial.print("Detectada aceleracao vertical negativa: ");
      Serial.println(ac.acceleration.z, 1);
      Serial.print("Tempo de subida: ");
      Serial.print(tempo_subida/1000);
      Serial.println(" segundos");
      voo_iniciado = false;
    }
  if(t.temperature > 35){
    Serial.print("TEMPERATURA CRITICA EXCEDIDA!: ");
    Serial.println(t.temperature, 1);
  }
}

void setup() {
  sensors_event_t temp, gyro;
  voo_iniciado = false;
  Serial.begin(115200);
  Serial.println("ESP32 Inicializado");
  while(!mpu.begin()){
    Serial.println("ERROR INITIALIZING MPU6050");
  }
  Serial.println("MPU6050 Initialized");
  float accel = 0;
 while(accel < 19.6){
    sensors_event_t acc;
    mpu.getEvent(&acc, &gyro, &temp);
    accel = acc.acceleration.z;
    delay(100);
    Serial.println("check");
    Serial.println(accel, 1);
  }
  Serial.println("Aceleração vertical (z) acima de 2G durante 100ms: Inicio de voo detectado.");
  voo_iniciado = true;
  inicio_subida = millis();
}

void loop() {
    sensors_event_t acc, gyro, temp;
   if(voo_iniciado){
      
 voo_inicializado(acc, gyro, temp);
     }
  delay(10); // this speeds up the simulation
}
