#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Definisi OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definisi DHT22
#define DHTPIN 19
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Definisi LDR
#define LDRPIN 32

void setup() {
    Serial.begin(115200);

    // Inisialisasi OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Gagal menginisialisasi OLED!");
        while (true);
    }
    display.clearDisplay();
    
    // Inisialisasi DHT22
    dht.begin();
}

void loop() {
    // Baca sensor DHT22
    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    // Baca nilai LDR
    int ldrValue = analogRead(LDRPIN);
    float intensitas = (ldrValue / 4095.0) * 100; // Konversi ke persen (untuk ESP32)

    // Cek apakah pembacaan DHT22 berhasil
    if (isnan(suhu) || isnan(kelembapan)) {
        Serial.println("Gagal membaca dari DHT22!");
        return;
    }

    // Tampilkan di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print("°C | Kelembapan: ");
    Serial.print(kelembapan);
    Serial.print("% | Intensitas Cahaya: ");
    Serial.print(intensitas);
    Serial.println("%");

    // Tampilkan di OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    display.setCursor(0, 0);
    display.print("Suhu: ");
    display.print(suhu);
    display.println(" C");

    display.setCursor(0, 10);
    display.print("Kelembapan: ");
    display.print(kelembapan);
    display.println(" %");

    display.setCursor(0, 20);
    display.print("Cahaya: ");
    display.print(intensitas);
    display.println(" %");

    display.display();

    delay(2000); // Tunggu 2 detik sebelum update berikutnya
}
