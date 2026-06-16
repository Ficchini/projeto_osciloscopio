# AudioScope IoT

Projeto de Internet das Coisas (IoT) desenvolvido para a disciplina de IoT da FATEC. 
O **AudioScope IoT** utiliza um ESP32 para ler um sinal analógico, gerar tom audível e exibir os dados em tempo real por meio de uma interface web.

## Descrição

O sistema funciona como um osciloscópio simplificado: um potenciômetro simula a entrada de sinal, cujo valor é lido pelo ADC do ESP32. Esse valor controla a frequência de um buzzer (entre 200 Hz e 2000 Hz) e é enviado para uma página web que exibe:

- Valor atual do ADC (0–4095)
- Frequência calculada em Hz
- Gráfico de linha atualizado a cada 100 ms (Chart.js)

A interface permite **ligar** ou **desligar** a saída de áudio e o LED indicador remotamente pelo navegador.

## Componentes

| Componente      | Pino ESP32 | Função                              |
|-----------------|------------|-------------------------------------|
| Potenciômetro   | GPIO 34    | Entrada analógica (ADC)             |
| Buzzer          | GPIO 23    | Saída PWM de áudio                  |
| LED             | GPIO 2     | Indicador visual (som ligado)       |

## Tecnologias

- **Microcontrolador:** ESP32 DevKit C v4
- **Framework:** Arduino (PlatformIO)
- **Comunicação:** Wi-Fi + servidor HTTP embutido
- **Frontend:** HTML, CSS e Chart.js (via CDN)
- **Simulação:** [Wokwi](https://wokwi.com)

## Como executar

### Pré-requisitos

- [PlatformIO](https://platformio.org/) (extensão VS Code ou CLI)
- Extensão [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode) (opcional, para simulação)

### Compilar e simular

1. Clone o repositório e abra a pasta no VS Code.
2. Compile o firmware:
   ```bash
   pio run
   ```
3. Inicie a simulação no Wokwi (F1 → **Wokwi: Start Simulator**).
4. Aguarde a conexão Wi-Fi (`Wokwi-GUEST`) e abra no navegador:
   ```
   http://127.0.0.1:8180
   ```

### Endpoints da API

| Rota        | Método | Descrição                                      |
|-------------|--------|------------------------------------------------|
| `/`         | GET    | Página principal com gráfico e controles       |
| `/dados`    | GET    | Retorna `ADC,frequencia` em texto plano        |
| `/ligar`    | GET    | Ativa buzzer e LED                             |
| `/desligar` | GET    | Desativa buzzer e LED                          |

## Estrutura do projeto

```
projeto_osciloscopio/
├── src/main.cpp      # Firmware principal
├── diagram.json      # Circuito para simulação Wokwi
├── wokwi.toml        # Configuração do simulador
├── platformio.ini    # Configuração PlatformIO
└── README.MD
```

## Autores

Erika Anastácio Creatto - 
Jônatas da Silva Pereira - 
Reinaldo Gomes de Souza - 
Vêronica Marques Palevicius - 

FATEC, 4º Semestre, IoT
