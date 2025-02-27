# 🌱 Sistema Embarcado para Monitoramento Ambiental

## 📌 Descrição do Projeto
Este projeto consiste no desenvolvimento de um **sistema embarcado** para **monitoramento ambiental**, utilizando o **Raspberry Pi Pico W (RP2040)**. O sistema simula a leitura de temperatura e umidade através de um **joystick analógico**, exibe os dados em um **display OLED** e aciona **LEDs RGB e um buzzer** para alertar sobre mudanças no estado ambiental.

## 🎯 Objetivos do Projeto

### Objetivo Geral
- Desenvolver um **sistema embarcado** capaz de monitorar temperatura e umidade, exibindo os dados em um **display OLED** e indicando estados ambientais por meio de **LEDs RGB e um buzzer**.

### Objetivos Específicos
- **Simular a leitura de temperatura e umidade** usando um **joystick analógico** e converter os valores com o **ADC do RP2040**.
- **Implementar um modelo de estados ambientais**:
  - **Normal**: LED verde → Ambiente seguro.
  - **Alerta**: LED amarelo → Variações detectadas.
  - **Crítico**: LED vermelho + buzzer → Ação imediata necessária.
- **Exibir os valores monitorados no display OLED**, utilizando **I²C** para comunicação.
- **Integrar LEDs RGB para sinalização visual dos estados ambientais**.
- **Acionar um buzzer para alertas sonoros quando o sistema entrar no estado crítico**.
- **Implementar um mecanismo de segurança**:
  - Pressionar **Botão A** → **Desativa** o sistema.
  - Pressionar **Botão B** → **Reativa** o sistema.

## 🔧 Especificação do Hardware

### 🖥️ Componentes Utilizados
| **Componente**        | **Modelo/Especificação** |
|----------------------|----------------------|
| **Microcontrolador** | Raspberry Pi Pico W (RP2040) |
| **Display**         | OLED 128x64 (I²C) |
| **LED RGB**         | Anodo comum (BitDogLab) |
| **Buzzer**         | Piezoelétrico (PWM) |
| **Botões**         | Push-button com pull-up |
| **Simulador de sensores** | Joystick analógico |

### 📡 Configuração dos Pinos do RP2040
| **Componente** | **Pino RP2040** | **Descrição** |
|--------------|----------------|-------------|
| **LED R**   | GP13 | Controle do LED vermelho |
| **LED G**   | GP11 | Controle do LED verde |
| **LED B**   | GP12 | Controle do LED azul |
| **Buzzer**  | GP10 | Controle via PWM |
| **Joystick X (Temp.)** | GP27 (ADC1) | Leitura da temperatura |
| **Joystick Y (Umid.)** | GP26 (ADC0) | Leitura da umidade |
| **Botão A** | GP5 | Desativa o sistema |
| **Botão B** | GP6 | Reativa o sistema |
| **OLED SDA** | GP14 | Comunicação I²C |
| **OLED SCL** | GP15 | Comunicação I²C |

### 📜 Diagrama em Blocos
![Diagrama do Sistema](caminho_para_a_imagem_do_diagrama.png)

## 🔨 Especificação do Firmware

### 🏗️ Blocos Funcionais
- **Leitura de sensores**: Obtém valores do joystick e os converte para temperatura e umidade.
- **Processamento de estados**: Determina se o sistema está em **Normal, Alerta ou Crítico**.
- **Controle dos LEDs**: Ajusta as cores dos LEDs conforme o estado.
- **Controle do buzzer**: Ativa/desativa o buzzer em estado crítico.
- **Interface OLED**: Exibe os valores monitorados em tempo real.
- **Mecanismo de segurança**: Botões físicos para ativar/desativar o sistema.

### 📍 Fluxograma do Software
![Fluxograma](caminho_para_o_fluxograma.png)

## ✅ Execução do Projeto

### 📌 Metodologia
1. **Definição do escopo**: Monitoramento ambiental com alertas visuais e sonoros.
2. **Pesquisa de projetos similares**: Estudos sobre IoT e sistemas embarcados.
3. **Especificação de hardware e software**: Seleção de componentes e desenvolvimento do código.
4. **Implementação e testes**: Validação dos estados e respostas do sistema.
5. **Documentação**: Relatório técnico e README para repositório GitHub.

### 🔍 Testes de Validação
| **Teste**  | **Objetivo**  | **Resultado** |
|-----------|-------------|------------|
| **Leitura dos Sensores** | Verificar a precisão da leitura do joystick | Precisão superior a 95% |
| **Mudança de Estados** | Validar a resposta dos LEDs e buzzer | Funcionamento 100% correto |
| **Ação dos Botões** | Desativação/ativação do sistema | Sistema desliga e religa corretamente |

## 📌 Instruções de Uso
1. **Faça o clone do repositório**:
   ```bash
   git clone https://github.com/seu_usuario/nome_do_repositorio.git
   cd nome_do_repositorio
   Compile e carregue o código no Raspberry Pi Pico utilizando a IDE apropriada.
2. **Conecte os periféricos conforme a tabela de configuração dos pinos**.
3. **Utilize o joystick para simular temperatura e umidade**:
- Movendo para a direita/esquerda → Simula temperatura.
- Movendo para cima/baixo → Simula umidade.
4. **Acompanhe a leitura no display OLED e verifique os alertas visuais e sonoros**.
## 🎬 Demonstração do Projeto
**Assista ao vídeo completo no YouTube**:
[Assista ao vídeo do projeto no YouTube](https://youtu.be/pCoXMdYecGQ)
