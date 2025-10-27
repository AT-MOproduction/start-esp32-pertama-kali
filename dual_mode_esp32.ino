#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID  = "ESP32-AP-STA";
const char* AP_PASS  = "12345678";
const char* STA_SSID = "GANTI DENGAN SSID DI RUMAH KAMU";
const char* STA_PASS = "GANTI DENGAN SSID DI RUMAH KAMU";

WebServer server(80);
const int RELAY_PIN = 2;

String htmlPage(const String& where) {
  bool state = digitalRead(RELAY_PIN);
  String btn = state ? "<a href=\"/off\">MATIKAN</a>" : "<a href=\"/on\">NYALAKAN</a>";
  String ips = "AP IP: " + WiFi.softAPIP().toString();
  if (WiFi.status() == WL_CONNECTED) {
    ips += " | STA IP: " + WiFi.localIP().toString();
  }
  return String() +
    "<!doctype html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'/>"
    "<style>body{font-family:Arial;padding:16px} a{display:inline-block;padding:12px 18px;border:1px solid #333;border-radius:8px;text-decoration:none}</style>"
    "</head><body>"
    "<h2>ESP32 AP+STA (" + where + ")</h2>"
    "<p>" + ips + "</p>"
    "<p>Status relay: <b>" + String(state ? "ON" : "OFF") + "</b></p>" + btn +
    "<p><a href='/'>Refresh</a></p>"
    "</body></html>";
}

void handleRoot() { server.send(200, "text/html", htmlPage("root")); }
void handleOn()   { digitalWrite(RELAY_PIN, HIGH); server.send(200, "text/html", htmlPage("on")); }
void handleOff()  { digitalWrite(RELAY_PIN, LOW);  server.send(200, "text/html", htmlPage("off")); }

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);

  // Mode ganda
  WiFi.mode(WIFI_AP_STA);

  // Start AP
  bool ap_ok = WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("AP %s, IP: %s\n", ap_ok ? "OK" : "FAILED", WiFi.softAPIP().toString().c_str());

  // Join STA
  WiFi.begin(STA_SSID, STA_PASS);
  Serial.printf("Join ke STA %s", STA_SSID);
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(500); Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("STA IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("STA gagal (tetap bisa akses via AP 192.168.4.1).");
  }

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
