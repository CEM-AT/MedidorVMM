/*
Após o uso dos Shields as portas disponíveis são apenas:
0(TX), 1(TX), 2, 3(PWM), 11(PWM), 12 e 13
*/
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal LiquidCrystal(8, 9, 4, 5, 6, 7);
Servo Servo;

// "Pinagem" dos atuadores
#define pinFirstIRSensor A1
#define pinSecondIRSensor A2
#define pinThirdIRSensor A3

#define pinTriggerGate A5

// Estados lógicos dos atuadores
uint16_t stateFirstIRSensor;
uint16_t stateSecondIRSensor;
uint16_t stateThirdIRSensor;


// Classes e funções
/* Responsavel por armazenar algumas variaveis e informacoes
  Propertie
    Application
      .Author
      .Version
      .Rev
      .Github
  */
class Propertie {
  public:
    class Application {
      public:
        char* Author = "Abiel M. (@paodelonga)";
        char* Version = "2.0.0";
        char* Rev = "Segunda-feira, 8 de agosto de 2022 17:11";
        char* Github = "https://github.com/Paodelonga/MedidorVMM";
    }
    Application;

}
Propertie;

/* Atua em conjunto com a função de exibição de caractere da classe Display
  Character
    .lines
    .topLine
    .bottomLine
    .leftTopBracket
    .rightTopBracket
    .leftBottomBracket
    .rightBottomBracket
    .delta
    .brackets
    .box
*/
class Character {
  public:
    byte lines = 0;
    byte topLine = 1;
    byte bottomLine = 2;
    byte leftTopBracket = 3;
    byte rightTopBracket = 4;
    byte leftBottomBracket = 5;
    byte rightBottomBracket = 6;
    byte delta = 7;
    byte brackets = 8;
    byte box = 9;
}
Character;

/* Utilizada para melhorar a utilização do display LCD
  Display
    Init()
    setCursor()
    print()
    printCentered()
    printWithDelta()
    clear()
    ON()
    OFF()

    Draw
      Char()
      Lines()

*/
class Display {
  public:
    void Init(byte columns, byte lines) {
      LiquidCrystal.begin(columns, lines);
      delay(1000);

      byte characterTopLine[8] = {B11111, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
      byte characterBottomLine[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
      byte characterLeftTopBracket[8] = {B01111, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
      byte characterRightTopBracket[8] = {B11110, B00001, B00001, B00001, B00001, B00001, B00001, B00001};
      byte characterLeftBottomBracket[8] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B01111};
      byte characterRightBottomBracket[8] = {B00001, B00001, B00001, B00001, B00001, B00001, B00001, B11110};
      byte characterDelta[8] = {B00000, B00100, B01010, B01010, B01010, B10001, B11111, B00000};

      delay(500);

      LiquidCrystal.createChar(1, characterTopLine); delay(100);
      LiquidCrystal.createChar(2, characterBottomLine); delay(100);
      LiquidCrystal.createChar(3, characterLeftTopBracket); delay(100);
      LiquidCrystal.createChar(4, characterRightTopBracket); delay(100);
      LiquidCrystal.createChar(5, characterLeftBottomBracket); delay(100);
      LiquidCrystal.createChar(6, characterRightBottomBracket); delay(100);
      LiquidCrystal.createChar(7, characterDelta);

    }
    void setCursor(byte columnIndex, byte lineIndex) {
      LiquidCrystal.setCursor(columnIndex, lineIndex);
    }
    void print(String content, byte columnIndex, byte lineIndex) {
      LiquidCrystal.setCursor(columnIndex, lineIndex);
      LiquidCrystal.print(content);
    }
    void printCentered(String content, byte lineIndex, byte columnCorrection) {
      if(lineIndex == 0) Draw.Lines(Character.topLine);
      if(lineIndex == 1) Draw.Lines(Character.bottomLine);
      LiquidCrystal.setCursor((abs(17 - content.length()) / 2) - columnCorrection, lineIndex);
      LiquidCrystal.print(content);
    }
    void printWithDelta(String content, byte lineIndex, byte columnCorrection) {
      if(lineIndex == 0) Draw.Lines(Character.topLine);
      if(lineIndex == 1) Draw.Lines(Character.bottomLine);
      byte columnIndex = (abs(17 - content.length()) / 2) - columnCorrection;
      LiquidCrystal.setCursor(columnIndex, lineIndex);
      LiquidCrystal.print(content);
      Draw.Char(Character.delta, columnIndex - 1, lineIndex);

    }
    void clear() {
      Draw.Char(Character.box, 0, 0);
    }
    void ON() {
      LiquidCrystal.display();
    }
    void OFF() {
      LiquidCrystal.noDisplay();
    }

    class Draw {
      public:
        void Char(byte characterName, byte columnIndex, byte lineIndex) {
          if(characterName == Character.brackets) {
            LiquidCrystal.setCursor(0, 0);
            LiquidCrystal.write(Character.leftTopBracket);

            LiquidCrystal.setCursor(15, 0);
            LiquidCrystal.write(Character.rightTopBracket);

            LiquidCrystal.setCursor(0, 1);
            LiquidCrystal.write(Character.leftBottomBracket);

            LiquidCrystal.setCursor(15, 1);
            LiquidCrystal.write(Character.rightBottomBracket);
          }
          else if(characterName == Character.box) {
            LiquidCrystal.setCursor(0, 0);
            LiquidCrystal.write(Character.leftTopBracket);

            LiquidCrystal.setCursor(15, 0);
            LiquidCrystal.write(Character.rightTopBracket);

            LiquidCrystal.setCursor(0, 1);
            LiquidCrystal.write(Character.leftBottomBracket);

            LiquidCrystal.setCursor(15, 1);
            LiquidCrystal.write(Character.rightBottomBracket);

            for(uint8_t i = 1; i < 15; i++) {
              LiquidCrystal.setCursor(i, 0);
              LiquidCrystal.write(Character.topLine);
              LiquidCrystal.setCursor(i, 1);
              LiquidCrystal.write(Character.bottomLine);
            }
          }
          else {
            LiquidCrystal.setCursor(columnIndex, lineIndex);
            LiquidCrystal.write(characterName);
          }
        }

        void Lines(byte characterName) {
          if(characterName == Character.lines) {
            for(uint8_t i = 1; i < 15; i++) {
              LiquidCrystal.setCursor(i, 0);
              LiquidCrystal.write(Character.topLine);
              LiquidCrystal.setCursor(i, 1);
              LiquidCrystal.write(Character.bottomLine);
            }
          }
          else if(characterName == Character.topLine) {
            for(uint8_t i = 1; i < 15; i++) {
              LiquidCrystal.setCursor(i, 0);
              LiquidCrystal.write(Character.topLine);
            }
          }
          else if(characterName == Character.bottomLine) {
            for(uint8_t i = 1; i < 15; i++) {
              LiquidCrystal.setCursor(i, 1);
              LiquidCrystal.write(Character.bottomLine);
            }
          }
        }
    }
    Draw;
}
Display;

/* Para ações que envolvam log e print de mensagens no serial
  Logger
    Log()
    Init()
    Credits()
*/
class Logger {
  public:
    void Log(String logContentPrefix, String logContent) { // New if statement: (bool isConcatenated)
      Serial.print(logContentPrefix);
      Serial.print(F(" :: "));
      Serial.println(logContent);
    }
    void Init() {
      Credits();
      Serial.println(F(""));
      Serial.println(F("MedidorVMM :: Iniciando setup..."));

      char* message = "Iniciando";
      Display.print(F("Iniciando"), 3, 1);

      byte tryCount = random(1, 10);
      byte lineCount = tryCount;
      byte length = strlen(message) + 3;
      for (byte i = 0; i < tryCount; i++) {
        if(lineCount > 0) {
          Serial.print(F("MedidorVMM :: Finalizando em: "));
          Serial.print(lineCount);
          Serial.print(F("s"));
        }
        for (byte x = length; x < length+3; x++) {
          Display.print(F("."), x, 1);
          if(lineCount > 0) {
            Serial.print(F("."));
          }

          delay(5*115/3.112);
          Display.Draw.Char(Character.bottomLine, x, 1);
        }
        if(lineCount > 0) {
          Serial.print(F("\n"));
          lineCount--;
        }

      }
      Log(F("MedidorVMM"), F("Setup finalizado!"));

      Display.Draw.Lines(Character.bottomLine);
      Display.printCentered(F("Tudo Pronto!"), 1, 0);
      delay(1600);

      Log(F("MedidorVMM"), F("Programa iniciado.\n"));
      Display.printCentered(F("Iniciado"), 1, 0);
      delay(1950);
    }
    void Credits() {
      Display.Draw.Char(Character.box, 0, 0);
      Display.printCentered(F("MedidorVMM"), 0, 0);
      Display.Draw.Char(Character.delta, 2, 0);

      char* msg[6];
      msg[0] = "Medidor de";
      msg[1] = "Velocidade";
      msg[2] = "Media no";
      msg[3] = "Movimento";
      msg[4] = "-";
      msg[5] = "MedidorVMM.";
      byte cursorPos = 1;

      Serial.print(F("MedidorVMM :: "));
      for(byte word = 0; word < sizeof(msg) / 2; word++) {
        cursorPos = abs(16 - (strlen(msg[word]))) / 2;
        for (byte wordChar = 0; wordChar < strlen(msg[word]); wordChar++) {
          for(byte i = cursorPos; i < (17 - cursorPos); i++) {
            Display.Draw.Char(Character.bottomLine, i, 1);
          }

          Display.print((String)msg[word][wordChar], cursorPos, 1);
          Serial.print(msg[word][wordChar]);
          delay(strlen(msg[word]) * strlen(msg[word]) * 2);
          cursorPos++;
        }
        Serial.print(F(" "));
        delay(strlen(msg[word]) * strlen(msg[word]) * 8);
        for(byte i = 1; i < cursorPos - 1; i++) {
          Display.Draw.Char(Character.bottomLine, i, 1);
        }
      }
      Serial.println(F(""));
      delay(1000);

      Display.printCentered(F("Desenvolvido"), 1, 0);
      Serial.print(F("MedidorVMM :: Author: "));
      delay(1600);

      Display.printCentered(F("por Abiel M."), 1, 0);
      Serial.print(Propertie.Application.Author);
      Serial.println(F(" "));
      delay(1950);

      Display.printCentered(F("git@paodelonga"), 1, 0);
      Serial.print(F("MedidorVMM :: Github: "));
      Serial.print(Propertie.Application.Github);
      Serial.println(F(" "));
      delay(1950);

      Serial.print(F("MedidorVMM :: Version: "));
      Serial.print(Propertie.Application.Version);
      Serial.println(F(" "));

      Display.printCentered(F("Versao"), 1, 0);
      delay(1600);

      Display.Draw.Lines(Character.bottomLine);
      Display.printCentered((String)Propertie.Application.Version + ".", 1, 0);
      delay(1950);

      Serial.print(F("MedidorVMM :: Rev: "));
      Serial.print(Propertie.Application.Rev);
      Serial.println(F(" "));
    }
}
Logger;

/* Montada para utilizar os botões do shield LCD Keypad
  KeypadButton
    .buttonValue
    .pressedValue
    .toggledValue

    .Up
    .Down
    .Left
    .Right
    .Select
    .NONE

    Pressed()
    Toggled()
*/
class KeypadButton {
  public:
    int16_t buttonValue;
    byte pressedValue;
    byte toggledValue;

    static const byte Up = 1;
    static const byte Down = 2;
    static const byte Left = 3;
    static const byte Right = 4;
    static const byte Select = 5;
    static const byte NONE = 0;

    int Pressed() {
      buttonValue = analogRead(0);
      if(buttonValue > 60 and buttonValue < 200) { pressedValue = Up; }
      else if(buttonValue > 200 and buttonValue < 400) { pressedValue = Down; }
      else if(buttonValue > 400 and buttonValue < 600) { pressedValue = Left; }
      else if(buttonValue > 0 and buttonValue < 60) { pressedValue = Right; }
      else if(buttonValue > 600 and buttonValue < 800) { pressedValue = Select; }
      else if(buttonValue > 800) { pressedValue = NONE; }
      return pressedValue;
    }
    int Toggled() {
      buttonValue = analogRead(0);
      if(buttonValue > 60 and buttonValue < 200) { toggledValue = Up; }
      else if(buttonValue > 200 and buttonValue < 400) { toggledValue = Down; }
      else if(buttonValue > 400 and buttonValue < 600) { toggledValue = Left; }
      else if(buttonValue > 0 and buttonValue < 60) { toggledValue = Right; }
      else if(buttonValue > 600 and buttonValue < 800) { toggledValue = Select; }
      return toggledValue;
    }
}
KeypadButton;

/* Criada para controlar os servos, exclusivamente a cancela de liberação do objeto
  Trigger
    openGate()
    closeGate()
    calibrateGate()
    init()
*/
class Trigger {
  private:
    byte servoPin;
  public:
    Trigger(byte servoPin) {
      this->servoPin = servoPin;
    }
    void openGate() {
      // for (byte i = 1; i < 3; i++) {
      //   Servo.write(63);
      //   delay(500);
      //   Servo.write(60);
      //   delay(500);
      //   Servo.write(63);
      // }
      Servo.write(65);
      delay(100);
      Servo.write(63);
      delay(100);
      Servo.write(65);
      delay(100);
      Servo.write(63);
      Logger.Log(F("MedidorVMM"), F("Cancela aberta."));
    }
    void closeGate() {
      // for (byte i = 1; i < 3; i++) {
      //   Servo.write(10);
      //   delay(500);
      //   Servo.write(18);
      //   delay(500);
      //   Servo.write(10);
      // }
      Servo.write(20);
      delay(100);
      Servo.write(10);
      delay(100);
      Servo.write(20);
      delay(100);
      Servo.write(10);
      Logger.Log(F("MedidorVMM"), F("Cancela fechada."));
    }
    void calibrateGate() {
      Logger.Log(F("MedidorVMM"), F("Calibramento da cancela."));
      openGate();
      delay(500);
      closeGate();
      delay(500);
      openGate();
      delay(500);
      closeGate();
      for (byte i = 1; i < 3; i++) {
        Logger.Log(F("MedidorVMM"), (String)F("Calibração ") + i + F("x"));
        for (byte z = 0; z < 180; z++) {
          Servo.write(z);
          delay(5);
        }
        for (byte z = 180; z > 0; z--) {
          Servo.write(z);
          delay(5);
        }
      }
      Logger.Log(F("MedidorVMM"), F("Calibramento finalizado.\n"));
    }
    void init() {
      Servo.attach(servoPin);
      delay(500);
    }
}
Trigger(pinTriggerGate);

/* Referente a leitura; armazena, efetua a leitura e exibe os dados.
  Reader
    Data
      Latest
        .Reading
        .Timer
        .Interval
        .TimeVariation
        .DistanceVariation
        .AverageSpeed
        .RailFinalPosition
        .RailInitialPosition
      Previous
        .Reading
        .Timer
        .Interval
        .TimeVariation
        .AverageSpeed
    Read()
    DisplayInSerial()
*/
class Reader {
  public:
    class Data {
      public:
        class Reading {
          public:
            byte Latest;
            double Timer[20][3];
            double Interval[20][3];
            double TimeVariation[20];
            double DistanceVariation;
            double AverageSpeed[20][2];

            byte RailFinalPosition;
            byte RailInitialPosition;

        }
        Reading;
        // class Latest {
        //   public:
        //     byte Reading;
        //     double Timer[3];
        //     double Interval[3];
        //     double TimeVariation;
        //     double DistanceVariation;
        //     double AverageSpeed[2];
        //
        //     byte RailFinalPosition;
        //     byte RailInitialPosition;
        // }
        // Latest;
        // class Previous {
        //   public:
        //     byte Reading;
        //     double Timer[20][3];
        //     double Interval[20][3];
        //     double TimeVariation[20];
        //     double AverageSpeed[20][2];
        // }
        // Previous;
    }
    Data;

    void Read() {
      long msgMillis = millis();
      byte msgIndex = 0;
      Trigger.openGate();

      Logger.Log(F("Posicionamento"), F("Iniciando o Posicionamento."));
      Logger.Log(F("Posicionamento"), F("Posicione o Objeto e Pressione Left."));
      while(1) {
        if(millis() - msgMillis > 1600) {
          if(msgIndex == 0) {
            Display.printCentered(F("Posicione o"), 1, 1);
            msgMillis = millis();
            msgIndex++;
          }
        }
        if(millis() - msgMillis > 1250) {
          if(msgIndex == 1) {
            Display.printCentered(F("Objeto e"), 1, 0);
            msgMillis = millis();
            msgIndex++;
          }
        }
        if(millis() - msgMillis > 1250) {
          if(msgIndex == 2) {
            Display.printCentered(F("Pressione"), 1, 1);
            msgMillis = millis();
            msgIndex++;
          }
        }
        if(millis() - msgMillis > 1200) {
          if(msgIndex == 3) {
            Display.printCentered(F("Left."), 1, 0);
            msgMillis = millis();
            msgIndex = 0;
          }
        }


        if(KeypadButton.Pressed() == KeypadButton.Left) {
          delay(250);
          Trigger.closeGate();

          Logger.Log(F("Posicionamento"), F("Objeto posicionado."));
          Logger.Log(F("Posicionamento"), F("Posicionamento Completo."));

          Display.printCentered("MedidorVMM", 0, 0);
          Display.Draw.Char(Character.delta, 2, 0);

          for (byte seconds = 10; seconds > 0; seconds--) {
            Logger.Log(F("MedidorVMM"), (String)F("Iniciando a leitura em: ") + seconds + F("s."));
            Display.printCentered((String)F("Iniciando em ") + seconds, 1, 0);
            delay(1000);
          }

          if(Data.Reading.Latest < 20) {
            Data.Reading.Latest++;
          } else if(Data.Reading.Latest >= 20) {
            Data.Reading.Latest = 1;
          }

          delay(250);
          long msgMillis = millis();
          byte msgIndex = 0;

          Trigger.openGate();
          Data.Reading.Timer[Data.Reading.Latest][0] = millis();
          Logger.Log(F("MedidorVMM"), F("Iniciando leitura."));
          Logger.Log(F("MedidorVMM"), F("Sensores abertos."));
          while(1) { // Inicia a leitura e aguarda pela presenca da bolinha no primeiro sensor
            if(millis() - msgMillis > 1300) {
              if(msgIndex == 0) {
                Display.printCentered(F("Sensores"), 1, 0);
                msgMillis = millis();
                msgIndex++;
              }
            }
            if(millis() - msgMillis > 1250) {
              if(msgIndex == 1) {
                Display.printCentered(F("Iniciados."), 1, 0);
                msgMillis = millis();
                msgIndex = 0;
              }
            }

            stateFirstIRSensor = analogRead(pinFirstIRSensor);
            if(stateFirstIRSensor == 1023) { // Verifica se nao ha sinal no primeiro sensor
              Data.Reading.Timer[Data.Reading.Latest][1] = millis();
              Logger.Log(F("Leitura"), F("Primeiro sensor detectado."));
              Display.printCentered(F("Primeiro IR"), 1, 0);

              while(1) { // Aguarda pela presenca da bolinha no segundo sensor
                stateSecondIRSensor = analogRead(pinSecondIRSensor);
                if(stateSecondIRSensor == 1023) { // Verifica se nao ha sinal no segundo sensor
                  Data.Reading.Timer[Data.Reading.Latest][2] = millis();
                  Logger.Log(F("Leitura"), F("Segundo sensor detectado."));
                  Display.printCentered(F("Segundo IR"), 1, 0);

                  while(1) { // Aguarda pela presenca da bolinha no terceiro sensor
                    stateThirdIRSensor = analogRead(pinThirdIRSensor);
                    if(stateThirdIRSensor == 1023) { // Verifica se nao ha sinal no terceiro sensor
                      Data.Reading.Timer[Data.Reading.Latest][3] = millis();
                      Logger.Log(F("Leitura"), F("Terceiro sensor detectado."));
                      Display.printCentered(F("Terceiro IR"), 1, 0);
                      delay(800);
                      Logger.Log(F("MedidorVMM"), F("Leitura completa.\n"));
                      Trigger.closeGate();

                      Display.printCentered(F("Processando..."), 1, 0);
                      Logger.Log(F("MedidorVMM"), F("Iniciando processamento de dados."));
                      delay(1000);
                      // Intervalos
                      Logger.Log(F("Processamento"), F("Calculando variações e intervalos."));
                      Logger.Log(F("Processamento"), F("Calculando o primeiro intervalo."));
                      Data.Reading.Interval[Data.Reading.Latest][1] = abs(Data.Reading.Timer[Data.Reading.Latest][2] - Data.Reading.Timer[Data.Reading.Latest][1]);

                      Logger.Log(F("Processamento"), F("Calculando o segundo intervalo."));
                      Data.Reading.Interval[Data.Reading.Latest][2] = abs(Data.Reading.Timer[Data.Reading.Latest][3] - Data.Reading.Timer[Data.Reading.Latest][2]);

                      // Variações de tempo
                      Logger.Log(F("Processamento"), F("Calculando a variação de tempo."));
                      Data.Reading.TimeVariation[Data.Reading.Latest] = abs(Data.Reading.Timer[Data.Reading.Latest][3] - Data.Reading.Timer[Data.Reading.Latest][0]);

                      Logger.Log(F("Processamento"), F("Calculando a variação de distância."));
                      Data.Reading.DistanceVariation = abs(Data.Reading.RailFinalPosition - Data.Reading.RailInitialPosition);
                      Logger.Log(F("MedidorVMM"), F("Variações e intervalos calculados e processados.\n"));
                      delay(1000);

                      // Velocidades medias
                      Logger.Log(F("MedidorVMM"), F("Calculando a velocidade media em diferentes medidas."));
                      Logger.Log(F("Processamento"), F("Calculando a velocidade em CM/MS."));
                      Data.Reading.AverageSpeed[Data.Reading.Latest][1] = abs(Data.Reading.DistanceVariation / Data.Reading.TimeVariation[Data.Reading.Latest]); // cm/ms
                      Data.Reading.AverageSpeed[Data.Reading.Latest][2] = abs(Data.Reading.DistanceVariation / Data.Reading.TimeVariation[Data.Reading.Latest]) * 1000; // cm/s

                      Logger.Log(F("Processamento"), F("Velocidades calculadas e processadas."));
                      delay(800);
                      Logger.Log(F("MedidorVMM"), F("Processamento de dados completo.\n"));
                      delay(1000);


                      // Logger.Log(F("MedidorVMM"), F("Iniciando processamento SISRAM."));
                      // delay(1000);
                      // // Intervalos SISRAM
                      // Logger.Log(F("Processamento"), F("Salvando variações e intervalos."));
                      // Logger.Log(F("Processamento"), F("Salvando o primeiro intervalo."));
                      // Data.Previous.Interval[Data.Latest.Reading][1] = abs(Data.Latest.Timer[2] - Data.Latest.Timer[1]);
                      //
                      // Logger.Log(F("Processamento"), F("Salvando o segundo intervalo."));
                      // Data.Previous.Interval[Data.Latest.Reading][2] = abs(Data.Latest.Timer[3] - Data.Latest.Timer[2]);
                      //
                      // // Variações de tempo SISRAM
                      // Logger.Log(F("Processamento"), F("Salvando a variação de tempo."));
                      // Data.Previous.TimeVariation[Data.Latest.Reading] = abs(Data.Latest.Timer[3] - Data.Latest.Timer[0]);
                      //
                      // Logger.Log(F("Processamento"), F("Salvando a variação de distância."));
                      //
                      // Logger.Log(F("MedidorVMM"), F("Intervalos e variações calculadas e processados.\n"));
                      // delay(1000);
                      //
                      // // Velocidades medias SISRAM
                      // Logger.Log(F("MedidorVMM"), F("Salvando a velocidade media em diferentes medidas."));
                      // Logger.Log(F("Processamento"), F("Salvando a velocidade em CM/MS."));
                      // Data.Previous.AverageSpeed[Data.Latest.Reading][1] = abs(Data.Latest.RailFinalPosition / Data.Latest.TimeVariation); // cm/ms
                      // Data.Previous.AverageSpeed[Data.Latest.Reading][2] = abs(Data.Latest.RailFinalPosition / Data.Latest.TimeVariation) * 10; // cm/s
                      // Logger.Log(F("MedidorVMM"), F("Processamento SISRAM completo."));
                      Display.printCentered(F("Processado!"), 1, 0);
                      DisplayInSerial(Data.Reading.Latest);
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    void DisplayInSerial(byte ReadingID) {
      if(Data.Reading.Latest > 0) {
        String topMessage = ((String)F("====================[ Leitura N. ") + ReadingID + F("° ]===================="));
        String firstIntervalMessage = ((String)F("Primeiro Intervalo: ") + Data.Reading.Interval[ReadingID][1] + F("ms"));
        String secondIntervalMessage = ((String)F("Segundo Intervalo: ") + Data.Reading.Interval[ReadingID][2] + F("ms"));
        String timeIntervalMessage = ((String)F("Variacao de tempo: ") + Data.Reading.TimeVariation[ReadingID] + F("ms"));
        String DistanceVariationMessage = ((String)F("Variacao de distancia: ") + Data.Reading.DistanceVariation + F("cm"));
        String averageSpeedMessageCMMS = ((String)F("Velocidade media: ") + Data.Reading.AverageSpeed[ReadingID][1] + F("cm/ms"));
        String averageSpeedMessageCMS = ((String)F("Velocidade media: ") + Data.Reading.AverageSpeed[ReadingID][2] + F("m/s"));

        byte topMessageLength = topMessage.length() - 1;
        byte firstIntervalMessageLength = firstIntervalMessage.length();
        byte secondIntervalMessageLength = secondIntervalMessage.length();
        byte timeIntervalMessageLength = timeIntervalMessage.length();
        byte DistanceVariationMessageLength = DistanceVariationMessage.length();
        byte averageSpeedMessageCMMSLength = averageSpeedMessageCMMS.length();
        byte averageSpeedMessageCMSLength = averageSpeedMessageCMS.length();

        byte firstIntervalMessageCentered = abs(topMessageLength - firstIntervalMessageLength) / 2;
        byte secondIntervalMessageCentered = abs(topMessageLength - secondIntervalMessageLength) / 2;
        byte timeIntervalMessageCentered = abs(topMessageLength - timeIntervalMessageLength) / 2;
        byte DistanceVariationMessageCentered = abs(topMessageLength - DistanceVariationMessageLength) / 2;
        byte averageSpeedMessageCMMSCentered = abs(topMessageLength - averageSpeedMessageCMMSLength) / 2;
        byte averageSpeedMessageCMSCentered = abs(topMessageLength - averageSpeedMessageCMSLength) / 2;

        Serial.print(F("|"));
        Serial.print(topMessage);
        Serial.println(F("|"));

        // First interval line
        Serial.print(F("|"));
        for(byte i = 0; i < firstIntervalMessageCentered; i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Primeiro intervalo: "));
        Serial.print(Data.Reading.Interval[ReadingID][1]);
        Serial.print(F("ms"));
        for(byte i = 0; i < (topMessageLength) - (firstIntervalMessageLength + firstIntervalMessageCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Second interval line
        Serial.print(F("|"));
        for(byte i = 0; i < secondIntervalMessageCentered; i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Segundo intervalo: "));
        Serial.print(Data.Reading.Interval[ReadingID][2]);
        Serial.print(F("ms"));
        for(byte i = 0; i < (topMessageLength) - (secondIntervalMessageLength + secondIntervalMessageCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Time interval line
        Serial.print(F("|"));
        for(byte i = 0; i < timeIntervalMessageCentered; i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Variacao de tempo: "));
        Serial.print(Data.Reading.TimeVariation[ReadingID]);
        Serial.print(F("ms"));
        for(byte i = 0; i < topMessageLength - (timeIntervalMessageLength + timeIntervalMessageCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Distance variation line
        Serial.print(F("|"));
        for(byte i = 0; i < DistanceVariationMessageCentered; i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Variacao de distancia: "));
        Serial.print(Data.Reading.DistanceVariation);
        Serial.print(F("cm"));
        for(byte i = 0; i < topMessageLength - (DistanceVariationMessageLength + DistanceVariationMessageCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Average speed line
        Serial.print(F("|"));
        for(byte x = 0; x < averageSpeedMessageCMMSCentered; x++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Velocidade media: "));
        Serial.print(Data.Reading.AverageSpeed[ReadingID][1]);
        Serial.print(F("cm/ms"));
        for(byte i = 0; i < topMessageLength - (averageSpeedMessageCMMSLength + averageSpeedMessageCMMSCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Average speed line
        Serial.print(F("|"));
        for(byte x = 0; x < averageSpeedMessageCMSCentered; x++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Velocidade media: "));
        Serial.print(Data.Reading.AverageSpeed[ReadingID][2]);
        Serial.print(F("m/s"));
        for(byte i = 0; i < topMessageLength - (averageSpeedMessageCMSLength + averageSpeedMessageCMSCentered); i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));
        Serial.println(F("|=========================================================|\n\n"));
      }
    }
}
Reader;

/* Atua em conjunto com a classe Display para utilização de Menus
  Menu
    Reading
      Pages
        displayPage()
        nextPage()
        prevPage()
        latestPage()

      displayRead()
      latestRead()
      nextRead()
      prevRead()
    loop()
    switchMenu()
    nextMenu()
    prevMenu()
    toHome()
    toReading()
    isHome()
    isReading()
*/
class Menu {
  private:
    byte currentEntry;

    byte maxEntry = 1;
    byte minEntry = 0;

    byte homeEntry = 0;

    byte readingEntry = 1;
    long readingEntryTimer;

  public:
    class Reading {
      public:
        byte currentRead = 1;
        byte minRead = 1;
        byte maxRead = Reader.Data.Reading.Latest;
        class Pages {
          private:
            byte currentPage = 1;
            byte minPage = 1;
            byte maxPage = 3;

            byte intervalPage = 1;
            byte variationPage = 2;
            byte speedPage = 3;

          public:
            void displayPage() {
              if(currentPage == intervalPage) {
                Display.printCentered((String)F("1I: ") + Reader.Data.Reading.Interval[Menu().Reading.currentRead][1] + F("ms"), 0, 0);
                Display.printCentered((String)F("2I: ") + Reader.Data.Reading.Interval[Menu().Reading.currentRead][2] + F("ms"), 1, 0);

              } else if(currentPage == variationPage) {
                String firstMessage = (String)F("T: ") + Reader.Data.Reading.TimeVariation[Menu().Reading.currentRead] + F("ms");
                String secondMessage = (String)F("D: ") + Reader.Data.Reading.DistanceVariation + F("cm");

                Display.printWithDelta(firstMessage, 0, 0);
                Display.printWithDelta(secondMessage, 1, 0);

              } else if(currentPage == speedPage) {
                Display.printCentered((String)F("VM: ") + Reader.Data.Reading.AverageSpeed[Menu().Reading.currentRead][1] + F("cm/ms"), 0, 0);
                Display.printCentered((String)F("VM: ") + Reader.Data.Reading.AverageSpeed[Menu().Reading.currentRead][2] + F("m/s"), 1, 0);
              }
            }

            void nextPage() {
              if(currentPage < maxPage) {
                currentPage++;
                displayPage();
              }
            }
            void prevPage() {
              if(currentPage > minPage) {
                currentPage--;
                displayPage();
              }
            }

            void latestPage() {
              currentPage = minPage;
              displayPage();
            }
        }
        Pages;

        void displayRead() {
          if(maxRead > 0) {
            Display.printCentered(F("MedidorVMM"), 0, 0);
            Display.Draw.Char(Character.delta, 2, 0);
            Display.printCentered((String)F("Lendo ") + currentRead + F(" de ") + maxRead, 1, 0);
            Reader.DisplayInSerial(currentRead);
          }
        }
        void latestRead() {
          currentRead = maxRead;
          displayRead();
        }

        void nextRead() {
          if(currentRead == maxRead) {
            currentRead = minRead;
            displayRead();
          } else if(currentRead < maxRead) {
            currentRead++;
            displayRead();
          }
        }
        void prevRead() {
          if(currentRead == minRead) {
            currentRead = maxRead;
            displayRead();
          } else if(currentRead > minRead) {
            currentRead--;
            displayRead();
          }
        }
    }
    Reading;

    void loop() {
      if(isHome()) {
        Trigger.closeGate();

        Display.printCentered(F("MedidorVMM"), 0, 0);
        Display.Draw.Char(Character.delta, 2, 0);

        byte messageIndex = 0;
        long messageTime = millis();
        Logger.Log(F("MedidorVMM"), F("Pressione Left Para Iniciar."));
        while(isHome()) {
          if(millis() - messageTime > 1600) {
            if(messageIndex == 0) {
              Display.printCentered(F("Para Iniciar"), 1, 0);
              messageTime = millis();
              messageIndex++;
            }
          }
          else if(millis() - messageTime > 1400) {
            if(messageIndex == 1) {
              Display.printCentered(F("Pressione o"), 1, 0);
              messageTime = millis();
              messageIndex++;
            }
          }
          else if(millis() - messageTime > 1250) {
            if(messageIndex == 2) {
              Display.printCentered(F("Botao Left."), 1, 0);
              messageTime = millis();
              messageIndex = 0;
            }
          }

          switch(KeypadButton.Pressed()) {
            case KeypadButton.Left:
              delay(250);
              Reader.Read();
              break;
          }
          switchMenu();
        }
      }
      else if(isReading()) {
        switch(KeypadButton.Pressed()) {
          case KeypadButton.Up:
            delay(250);
            Reading.nextRead();
            readingEntryTimer = millis();
            break;

          case KeypadButton.Down:
            delay(250);
            Reading.prevRead();
            readingEntryTimer = millis();
            break;

          case KeypadButton.Left:
            delay(250);
            Reading.Pages.displayPage();
            Reading.Pages.prevPage();
            readingEntryTimer = millis();
            break;

          case KeypadButton.Right:
            delay(250);
            Reading.Pages.displayPage();
            Reading.Pages.nextPage();
            readingEntryTimer = millis();
            break;
        }
        switchMenu();
        if(millis() - readingEntryTimer >  90000) {
          toHome();
        }
      }
      switchMenu();
    }

    void switchMenu() {
      if(currentEntry > minEntry) { // Reading <=> Home (from switch to)
        switch(KeypadButton.Pressed()) {
          case KeypadButton.Select:
            delay(250);
            if(Reader.Data.Reading.Latest > 0) {
              currentEntry--;
              break;
            }
        }
      } else if(currentEntry < maxEntry) { // Home <=> Reading (from switch to)
        switch(KeypadButton.Pressed()) {
          case KeypadButton.Select:
            delay(250);
            if(Reader.Data.Reading.Latest > 0) {
              currentEntry++;
              readingEntryTimer = millis();
              Reading.latestRead();
              break;
            }
        }
      }
    }

    void nextMenu() {
      if(Reader.Data.Reading.Latest > 0)
      if(currentEntry < maxEntry) {
        currentEntry++;
      }
    }
    void prevMenu() {
      if(currentEntry > minEntry) {
        currentEntry--;
      }
    }

    void toHome() {
      currentEntry = homeEntry;
    }
    void toReading() {
      if(Reader.Data.Reading.Latest > 0) {
        currentEntry = readingEntry;
      } else {
        currentEntry = homeEntry;
      }
    }

    bool isHome() {
      if(currentEntry == homeEntry) {
        return true;
      } else {
        return false;
      }
    }
    bool isReading() {
      if(currentEntry == readingEntry) {
        return true;
      } else {
        return false;
      }
    }
}
Menu;

// Laço de configuração inicial
void setup() {
  Reader.Data.Reading.RailFinalPosition = 90;
  Reader.Data.Reading.RailInitialPosition = 0;

  pinMode(pinFirstIRSensor, INPUT);
  pinMode(pinSecondIRSensor, INPUT);
  pinMode(pinThirdIRSensor, INPUT);

  Display.Init(16, 2);
  Serial.begin(9600);
  while(!Serial);

  Logger.Init();
  Trigger.init();
  delay(1000);

  Display.Draw.Char(Character.box, 0, 0);
  Display.printCentered(F("MedidorVMM"), 0, 0);
  Display.Draw.Char(Character.delta, 2, 0);
}

// Laço infinito
void loop() {
  Menu.loop();
}
