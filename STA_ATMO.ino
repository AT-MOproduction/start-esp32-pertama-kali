#include <WiFi.h>
#include <WebServer.h>

const char* STA_SSID = "GANTI DENGAN SSID DI RUMAH KAMU";
const char* STA_PASS = "GANTI DENGAN PASSWORD SSID DI RUMAH KAMU";

WebServer server(80);
const int RELAY_PIN = 2;

String htmlPage() {
  bool state = digitalRead(RELAY_PIN);
  String btn = state ? "<a href=\"/off\">MATIKAN</a>" : "<a href=\"/on\">NYALAKAN</a>";
  return String() +
    "<!doctype html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'/>"
    "<style>body{font-family:Arial;padding:16px} a{display:inline-block;padding:12px 18px;border:1px solid #333;border-radius:8px;text-decoration:none}</style>"
    "</head><body>"
    "<h2>ESP32 STA Control</h2>"
    "<p>Status relay: <b>" + String(state ? "ON" : "OFF") + "</b></p>" + btn +
    "<p><a href='/'>Refresh</a></p>"
    "</body></html>";
}

void handleRoot() { server.send(200, "text/html", htmlPage()); }
void handleOn()   { digitalWrite(RELAY_PIN, HIGH); server.send(200, "text/html", htmlPage()); }
void handleOff()  { digitalWrite(RELAY_PIN, LOW);  server.send(200, "text/html", htmlPage()); }

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASS);
  Serial.printf("Menghubungkan ke %s", STA_SSID);
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(500); Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Terhubung. IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("Gagal terhubung (cek SSID/PASS atau jarak router).");
  }

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
