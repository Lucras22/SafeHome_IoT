#include <WiFi.h>
#include <HTTPClient.h>

// Configurações de Wi-Fi
const char* ssid = "Lucas Galindo | Poco C65";  // Nome da rede Wi-Fi
const char* password = "lucras22";  // Senha da rede Wi-Fi

// Configurações do Bot do Telegram
const String botToken = "7580634780:AAEK0xVLd5U7IgOL8nPVwXTvMJLtVCq-Q4E";  // Token do Bot do Telegram
const String chat_id = "7003158288";  // ID do chat para enviar mensagens

// Pino do sensor magnético
const int sensorPin = 15;  // GPIO onde o sensor está conectado
int lastState = HIGH;  // Último estado do sensor

void setup() {
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi.");

  // Definir o pino do sensor magnético como entrada com pull-up
  pinMode(sensorPin, INPUT_PULLUP);
}

void loop() {
  // Ler o estado atual do sensor magnético
  int sensorState = digitalRead(sensorPin);

  // Verificar se a porta foi aberta (estado mudou de HIGH para LOW)
  if (sensorState == HIGH && lastState == LOW) {
    Serial.println("Porta aberta!");

    // Obter a data e hora do ESP32
    String message = "🚪 Alerta: Porta aberta";

    // Enviar mensagem para o Telegram
    sendMessageToTelegram(message);

    delay(1000);  // Evitar múltiplos envios em um curto período
  }

  // Atualizar o último estado do sensor
  lastState = sensorState;

  delay(100);  // Intervalo entre leituras do sensor
}

// Função para enviar mensagem ao bot no Telegram
void sendMessageToTelegram(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chat_id + "&text=" + message;
    
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Mensagem enviada ao Telegram:");
      Serial.println(response);
    } else {
      Serial.println("Erro ao enviar mensagem");
    }

    http.end();
  } else {
    Serial.println("Wi-Fi não está conectado");
  }
}
