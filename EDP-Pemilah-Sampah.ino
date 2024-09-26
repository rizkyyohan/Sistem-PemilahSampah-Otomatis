#include <Servo.h>
#include <avr/wdt.h>  // Library untuk menggunakan Watchdog Timer

Servo servo1;  // Servo untuk sampah logam dan non-organik
Servo servo2;  // Servo untuk sampah organik

// Pin sensor dan servo
const int irSensorPin = 2;          // Pin untuk sensor Infrared
const int inductiveSensorPin = 3;   // Pin untuk sensor Proximity Induktif
const int capacitiveSensorPin = 4;  // Pin untuk sensor Proximity Kapasitif

const int servo1Pin = 9;  // Pin servo 1
const int servo2Pin = 10; // Pin servo 2

unsigned long startTime;  // Variabel untuk menyimpan waktu awal
const unsigned long rebootInterval = 86400000;  // 24 jam dalam milidetik (1000 * 60 * 60 * 24)

void setup() {
  // Inisialisasi pin sensor
  pinMode(irSensorPin, INPUT);
  pinMode(inductiveSensorPin, INPUT);
  pinMode(capacitiveSensorPin, INPUT);

  // Inisialisasi servo
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Posisikan servo ke posisi awal (0 derajat) posisi awal tergantung letak servo (bisa 90 derajat)
  servo1.write(160);
  servo2.write(140);

  Serial.begin(9600);  // Untuk debugging (opsional)

  // Inisialisasi waktu awal
  startTime = millis();
}

void loop() {
  // Cek apakah sensor IR mendeteksi ada sampah
  if (digitalRead(irSensorPin) == LOW) {
    delay(4000);  // Penundaan untuk stabilisasi sensor IR

    // Cek apakah sampah adalah logam dengan sensor Proximity Induktif
    if (digitalRead(inductiveSensorPin) == LOW) {
      Serial.println("Sampah logam terdeteksi.");
      // Gerakkan servo1 untuk memisahkan sampah logam (90 derajat)
      servo2.write(0);
      delay(4000);  // Tunggu 4 detik sebelum kembali ke posisi awal
      servo2.write(140);
      delay(2000);
    } 
    else {
      // Jika bukan logam, cek apakah sampah organik dengan sensor Kapasitif
      if (digitalRead(capacitiveSensorPin) == HIGH) {
        Serial.println("Sampah organik terdeteksi.");
        // Gerakkan servo1 untuk memisahkan sampah organik (90 derajat)
        servo1.write(0);
        delay(4000);  // Tunggu 4 detik sebelum kembali ke posisi awal
        servo1.write(160);
        delay(2000);
      } 
      else {
        Serial.println("Sampah non-organik terdeteksi.");
        // Gerakkan servo2 untuk memisahkan sampah non-organik (90 derajat)
        servo2.write(0);
        delay(4000);  // Tunggu 4 detik sebelum kembali ke posisi awal
        servo2.write(140);
        delay(2000);
      }
    }
  }

  // Cek apakah waktu 24 jam sudah terlewati
  if (millis() - startTime >= rebootInterval) {
    Serial.println("Sistem akan reboot...");

    // Reset dengan Watchdog Timer
    wdt_enable(WDTO_15MS);  // Set Watchdog Timer ke 15ms, sistem akan reboot
    while (true) {}  // Menunggu sistem reset
  }
}
