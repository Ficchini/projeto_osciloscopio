#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const int wifiChannel = 6;

WebServer server(80);

static const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>AudioScope IoT</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
body{font-family:Arial;background:#111;color:white;text-align:center;}
.card{background:#222;width:700px;margin:auto;margin-top:30px;padding:20px;border-radius:10px;}
button{padding:12px 20px;margin:10px;border:none;border-radius:5px;font-size:16px;cursor:pointer;}
.on{background:green;color:white;}
.off{background:red;color:white;}
canvas{background:#fff;border-radius:10px;}
</style>
</head>
<body>
<div class="card">
<h1>AudioScope IoT</h1>
<h2>Frequencia: <span id="freq">0</span> Hz</h2>
<h2>ADC: <span id="adc">0</span></h2>
<canvas id="grafico"></canvas>
<br>
<a href="/ligar"><button class="on">LIGAR</button></a>
<a href="/desligar"><button class="off">DESLIGAR</button></a>
</div>
<script>
const ctx = document.getElementById('grafico');
let dados = [];
for(let i=0;i<50;i++){ dados.push(0); }
const chart = new Chart(ctx, {
  type: 'line',
  data: { labels: dados.map((_,i)=>i), datasets: [{ label: 'Sinal', data: dados }] },
  options: { animation:false, scales: { y: { min:0, max:4095 } } }
});
async function atualizar(){
  const response = await fetch('/dados');
  const valores = (await response.text()).split(",");
  document.getElementById("adc").innerText = valores[0];
  document.getElementById("freq").innerText = valores[1];
  dados.push(parseInt(valores[0]));
  dados.shift();
  chart.data.datasets[0].data = dados;
  chart.update();
}
setInterval(atualizar, 100);
</script>
</body>
</html>
)rawliteral";

// Pinos
const int potPin = 34;
const int buzzerPin = 23;
const int ledPin = 2;

// Variáveis
int valorPot = 0;
int frequencia = 0;
bool somLigado = true;

// PWM
const int canalPWM = 0;
const int resolucao = 8;

void paginaPrincipal();
void enviarDados();

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  // Configura PWM do buzzer
  ledcSetup(canalPWM, 1000, resolucao);
  ledcAttachPin(buzzerPin, canalPWM);

  // WiFi (canal 6 = rede Wokwi-GUEST no simulador)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, wifiChannel);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println(WiFi.localIP());

  // Rotas Web
  server.on("/", paginaPrincipal);

  server.on("/ligar", []() {
    somLigado = true;
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/desligar", []() {
    somLigado = false;
    ledcWriteTone(canalPWM, 0);
    digitalWrite(ledPin, LOW);

    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/dados", enviarDados);

  server.begin();
  Serial.println("Servidor HTTP pronto (abra http://127.0.0.1:8180)");
}

void loop() {

  valorPot = analogRead(potPin);

  // Mapeia frequência
  frequencia = map(valorPot, 0, 4095, 200, 2000);

  if (somLigado) {

    ledcWriteTone(canalPWM, frequencia);

    digitalWrite(ledPin, HIGH);

  } else {

    ledcWriteTone(canalPWM, 0);

    digitalWrite(ledPin, LOW);
  }

  for (int i = 0; i < 5; i++) {
    server.handleClient();
    yield();
  }

  delay(2);
}

// ========================
// Página principal
// ========================

void paginaPrincipal() {
  server.send_P(200, "text/html", PAGE);
}

// ========================
// Envia dados AJAX
// ========================

void enviarDados() {

  String dados = String(valorPot) + "," + String(frequencia);

  server.send(200, "text/plain", dados);
}
