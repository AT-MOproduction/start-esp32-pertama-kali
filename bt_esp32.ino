#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

const int RELAY_PIN = 2; //led tertanam esp32s gpio02

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.begin(115200);
  SerialBT.begin("BT_AT-MO_PRODUCTION"); // nama Bluetooth
  Serial.println("Bluetooth SPP siap. Pair dan kirim ON/OFF.");
}

void loop() {
  // Jika ada data dari BT, echo ke Serial dan proses perintah
  if (SerialBT.available()) {
    String msg = SerialBT.readStringUntil('\n');
    msg.trim();
    Serial.print("BT <= "); Serial.println(msg);
    SerialBT.print("Echo: "); SerialBT.println(msg);

    if (msg.equalsIgnoreCase("ON")) {
      digitalWrite(RELAY_PIN, HIGH);
      SerialBT.println("Relay ON");
    } else if (msg.equalsIgnoreCase("OFF")) {
      digitalWrite(RELAY_PIN, LOW);
      SerialBT.println("Relay OFF");
    }
  }

  // Kirim data dari Serial ke BT (opsional)
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
}
