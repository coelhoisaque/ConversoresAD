# Projeto BitDogLab - Controle de LEDs RGB e Display SSD1306 com RP2040

Este projeto foi desenvolvido para consolidar os conceitos sobre o uso de conversores analógico-digitais (ADC) no microcontrolador RP2040, explorando as funcionalidades da placa de desenvolvimento **BitDogLab**. O objetivo é demonstrar o controle de LEDs RGB e a integração com um display SSD1306 utilizando o protocolo I2C.

---

## **Objetivos do Projeto**

- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.
- Utilizar PWM para controlar a intensidade de dois LEDs RGB com base nos valores do joystick.
- Representar a posição do joystick no display SSD1306 por meio de um quadrado móvel.
- Aplicar o protocolo de comunicação I2C na integração com o display.

---

## **Descrição Funcional**

O joystick fornece valores analógicos correspondentes aos eixos X e Y, que são utilizados para:

1. **Controle dos LEDs RGB:**
   - **LED Azul:** A intensidade luminosa é ajustada conforme o valor do eixo Y. Quando o joystick está solto (posição central - valor 2048), o LED permanece apagado. Movendo o joystick para cima ou para baixo, o LED aumenta gradualmente seu brilho, atingindo a intensidade máxima nos extremos (0 e 4095).
   - **LED Vermelho:** Segue o mesmo princípio, mas de acordo com o eixo X. Movendo o joystick para a esquerda ou direita, o LED aumenta de brilho, sendo mais intenso nos extremos (0 e 4095).

2. **Exibição no Display SSD1306:**
   - Um quadrado de **8x8 pixels**, inicialmente centralizado, se move proporcionalmente aos valores capturados pelo joystick.

3. **Funcionalidades do Botão do Joystick:**
   - Alterna o estado do **LED Verde** a cada acionamento.
   - Modifica a borda do display para indicar quando foi pressionado, alternando entre diferentes estilos de borda.

4. **Funcionalidade do Botão A:**
   - Ativa ou desativa os LEDs controlados por PWM.

---

## **Componentes Utilizados**

- **LED RGB:** Conectado às GPIOs 11 (Vermelho), 12 (Azul) e 13 (Verde).
- **Joystick:** Conectado às GPIOs 26 (eixo X) e 27 (eixo Y).
- **Botão do Joystick:** Conectado à GPIO 22.
- **Botão A:** Conectado à GPIO 5.
- **Display SSD1306:** Conectado via I2C (GPIOs 14 e 15).

---

## **Requisitos do Projeto**

1. **Uso de Interrupções:** Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing:** Implementação obrigatória do tratamento de bouncing dos botões via software.
3. **Utilização do Display SSD1306:** Demonstração do entendimento do princípio de funcionamento do display e do protocolo I2C.
4. **Organização do Código:** O código deve estar bem estruturado e comentado para facilitar o entendimento.

---

## **Estrutura do Código**

### **Arquivos Principais**

1. **`main.c`:** Código principal que implementa todas as funcionalidades descritas.
2. **`ssd1306.h` e `ssd1306.c`:** Biblioteca para controle do display SSD1306 via I2C.
3. **`font.h`:** Fonte para exibição de caracteres no display.

---

## **Instruções de Configuração**

### **Hardware Necessário**

- Placa de desenvolvimento **BitDogLab** com microcontrolador RP2040.
- Componentes conectados conforme descrito na seção "Componentes Utilizados".

### **Dependências**

- SDK do Raspberry Pi Pico (`pico-sdk`).
- Biblioteca `ssd1306` incluída no diretório `lib`.

### **Compilação e Upload**

1. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/nome-do-repositorio.git
   cd nome-do-repositorio
   ```

2. Configure o ambiente de desenvolvimento seguindo as instruções do [Pico SDK](https://github.com/raspberrypi/pico-sdk).

3. Compile o projeto:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Faça o upload do arquivo `.uf2` gerado para a placa RP2040.

---

## **Demonstração**

O vídeo completo da demonstração do projeto pode ser acessado aqui:  
[Link do Vídeo](https://www.youtube.com/watch?v=SEULINK)

---

## **Contribuições**

Contribuições são bem-vindas! Se você encontrar algum problema ou tiver sugestões para melhorias, abra uma issue ou envie um pull request.

---

## **Licença**

Este projeto está licenciado sob a **MIT License**. Consulte o arquivo [LICENSE]() para mais detalhes.


