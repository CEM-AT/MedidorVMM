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
int16_t stateFirstIRSensor;
int16_t stateSecondIRSensor;
int16_t stateThirdIRSensor;

int16_t servoTriggerSpeed;
int16_t stateIRTrigger;

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
    void Init(byte _columns, byte _lines) {
      LiquidCrystal.begin(_columns, _lines);
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
    void setCursor(byte _columnIndex, byte _lineIndex) {
      LiquidCrystal.setCursor(_columnIndex, _lineIndex);
    }
    void print(String _content, byte _columnIndex, byte _lineIndex) {
      LiquidCrystal.setCursor(_columnIndex, _lineIndex);
      LiquidCrystal.print(_content);
    }
    void printCentered(String _content, byte _lineIndex, byte _columnCorrection) {
      if(_lineIndex == 0) Draw.Lines(Character.topLine);
      if(_lineIndex == 1) Draw.Lines(Character.bottomLine);
      LiquidCrystal.setCursor((abs(17 - _content.length()) / 2) - _columnCorrection, _lineIndex);
      LiquidCrystal.print(_content);
    }
    void printWithDelta(String _content, byte _lineIndex, byte _columnCorrection) {
      if(_lineIndex == 0) Draw.Lines(Character.topLine);
      if(_lineIndex == 1) Draw.Lines(Character.bottomLine);
      byte _columnIndex = (abs(17 - _content.length()) / 2) - _columnCorrection;
      LiquidCrystal.setCursor(_columnIndex, _lineIndex);
      LiquidCrystal.print(_content);
      Draw.Char(Character.delta, _columnIndex - 1, _lineIndex);

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
        void Char(byte _characterName, byte _columnIndex, byte _lineIndex) {
          if(_characterName == Character.brackets) {
            LiquidCrystal.setCursor(0, 0);
            LiquidCrystal.write(Character.leftTopBracket);

            LiquidCrystal.setCursor(15, 0);
            LiquidCrystal.write(Character.rightTopBracket);

            LiquidCrystal.setCursor(0, 1);
            LiquidCrystal.write(Character.leftBottomBracket);

            LiquidCrystal.setCursor(15, 1);
            LiquidCrystal.write(Character.rightBottomBracket);
          }
          else if(_characterName == Character.box) {
            LiquidCrystal.setCursor(0, 0);
            LiquidCrystal.write(Character.leftTopBracket);

            LiquidCrystal.setCursor(15, 0);
            LiquidCrystal.write(Character.rightTopBracket);

            LiquidCrystal.setCursor(0, 1);
            LiquidCrystal.write(Character.leftBottomBracket);

            LiquidCrystal.setCursor(15, 1);
            LiquidCrystal.write(Character.rightBottomBracket);

            for(uint8_t _i = 1; _i < 15; _i++) {
              LiquidCrystal.setCursor(_i, 0);
              LiquidCrystal.write(Character.topLine);
              LiquidCrystal.setCursor(_i, 1);
              LiquidCrystal.write(Character.bottomLine);
            }
          }
          else {
            LiquidCrystal.setCursor(_columnIndex, _lineIndex);
            LiquidCrystal.write(_characterName);
          }
        }

        void Lines(byte _characterName) {
          if(_characterName == Character.lines) {
            for(uint8_t _i = 1; _i < 15; _i++) {
              LiquidCrystal.setCursor(_i, 0);
              LiquidCrystal.write(Character.topLine);
              LiquidCrystal.setCursor(_i, 1);
              LiquidCrystal.write(Character.bottomLine);
            }
          }
          else if(_characterName == Character.topLine) {
            for(uint8_t _i = 1; _i < 15; _i++) {
              LiquidCrystal.setCursor(_i, 0);
              LiquidCrystal.write(Character.topLine);
            }
          }
          else if(_characterName == Character.bottomLine) {
            for(uint8_t _i = 1; _i < 15; _i++) {
              LiquidCrystal.setCursor(_i, 1);
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
    void Log(String _logContentPrefix, String _logContent) { // New if statement: (bool _isConcatenated)
      Serial.print(_logContentPrefix);
      Serial.print(F(" :: "));
      Serial.println(_logContent);
    }
    void Init() {
      Credits();
      Serial.println(F(""));
      Serial.println(F("MedidorVMM :: Iniciando setup..."));

      char* _message = "Iniciando";
      Display.print(F("Iniciando"), 3, 1);

      byte _tryCount = random(1, 10);
      byte _lineCount = _tryCount;
      byte _length = strlen(_message) + 3;
      for (byte _i = 0; _i < _tryCount; _i++) {
        if(_lineCount > 0) {
          Serial.print(F("MedidorVMM :: Finalizando em: "));
          Serial.print(_lineCount);
          Serial.print(F("s"));
        }
        for (byte _x = _length; _x < _length+3; _x++) {
          Display.print(F("."), _x, 1);
          if(_lineCount > 0) {
            Serial.print(F("."));
          }

          delay(5*115/3.112);
          Display.Draw.Char(Character.bottomLine, _x, 1);
        }
        if(_lineCount > 0) {
          Serial.print(F("\n"));
          _lineCount--;
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

      char* _msg[6];
      _msg[0] = "Medidor de";
      _msg[1] = "Velocidade";
      _msg[2] = "Media no";
      _msg[3] = "Movimento";
      _msg[4] = "-";
      _msg[5] = "MedidorVMM.";
      byte _cursorPos = 1;

      Serial.print(F("MedidorVMM :: "));
      for(byte _word = 0; _word < sizeof(_msg) / 2; _word++) {
        _cursorPos = abs(16 - (strlen(_msg[_word]))) / 2;
        for (byte _wordChar = 0; _wordChar < strlen(_msg[_word]); _wordChar++) {
          for(byte _i = _cursorPos; _i < (17 - _cursorPos); _i++) {
            Display.Draw.Char(Character.bottomLine, _i, 1);
          }

          Display.print((String)_msg[_word][_wordChar], _cursorPos, 1);
          Serial.print(_msg[_word][_wordChar]);
          delay(strlen(_msg[_word]) * strlen(_msg[_word]) * 2);
          _cursorPos++;
        }
        Serial.print(F(" "));
        delay(strlen(_msg[_word]) * strlen(_msg[_word]) * 8);
        for(byte _i = 1; _i < _cursorPos - 1; _i++) {
          Display.Draw.Char(Character.bottomLine, _i, 1);
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
        class Latest {
          public:
            byte Reading;
            double Timer[3];
            double Interval[3];
            double TimeVariation;
            double DistanceVariation;
            double AverageSpeed[2];

            byte RailFinalPosition;
            byte RailInitialPosition;
        }
        Latest;
        class Previous {
          public:
            byte Reading;
            double Timer[20][3];
            double Interval[20][3];
            double TimeVariation[20];
            double AverageSpeed[20][2];
        }
        Previous;
    }
    Data;

    void Read() {
      long _msgMillis = millis();
      byte _msgIndex = 0;
      Trigger.openGate();

      Logger.Log(F("Posicionamento"), F("Iniciando o Posicionamento."));
      Logger.Log(F("Posicionamento"), F("Posicione o Objeto e Pressione Left."));
      while(1) {
        if(millis() - _msgMillis > 1600) {
          if(_msgIndex == 0) {
            Display.printCentered(F("Posicione o"), 1, 1);
            _msgMillis = millis();
            _msgIndex++;
          }
        }
        if(millis() - _msgMillis > 1250) {
          if(_msgIndex == 1) {
            Display.printCentered(F("Objeto e"), 1, 0);
            _msgMillis = millis();
            _msgIndex++;
          }
        }
        if(millis() - _msgMillis > 1250) {
          if(_msgIndex == 2) {
            Display.printCentered(F("Pressione"), 1, 1);
            _msgMillis = millis();
            _msgIndex++;
          }
        }
        if(millis() - _msgMillis > 1200) {
          if(_msgIndex == 3) {
            Display.printCentered(F("Left."), 1, 0);
            _msgMillis = millis();
            _msgIndex = 0;
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
            delay(9000);
          }

          delay(250);
          long _msgMillis = millis();
          byte _msgIndex = 0;

          Trigger.openGate();
          Data.Latest.Timer[0] = millis();
          Logger.Log(F("MedidorVMM"), F("Iniciando leitura."));
          Logger.Log(F("MedidorVMM"), F("Sensores abertos."));
          while(1) { // Inicia a leitura e aguarda pela presenca da bolinha no primeiro sensor
            if(millis() - _msgMillis > 1300) {
              if(_msgIndex == 0) {
                Display.printCentered(F("Sensores"), 1, 0);
                _msgMillis = millis();
                _msgIndex++;
              }
            }
            if(millis() - _msgMillis > 1250) {
              if(_msgIndex == 1) {
                Display.printCentered(F("Iniciados."), 1, 0);
                _msgMillis = millis();
                _msgIndex = 0;
              }
            }

            stateFirstIRSensor = analogRead(pinFirstIRSensor);
            if(stateFirstIRSensor == 1023) { // Verifica se nao ha sinal no primeiro sensor
              Data.Latest.Timer[1] = millis();
              Logger.Log(F("Leitura"), F("Primeiro sensor detectado."));
              Display.printCentered(F("Primeiro IR"), 1, 0);
              if(Data.Latest.Reading < 20) {
                Data.Latest.Reading++;
              } else if(Data.Latest.Reading >= 20) {
                Data.Latest.Reading = 1;
              }

              while(1) { // Aguarda pela presenca da bolinha no segundo sensor
                stateSecondIRSensor = analogRead(pinSecondIRSensor);
                if(stateSecondIRSensor == 1023) { // Verifica se nao ha sinal no segundo sensor
                  Data.Latest.Timer[2] = millis();
                  Logger.Log(F("Leitura"), F("Segundo sensor detectado."));
                  Display.printCentered(F("Segundo IR"), 1, 0);

                  while(1) { // Aguarda pela presenca da bolinha no terceiro sensor
                    stateThirdIRSensor = analogRead(pinThirdIRSensor);
                    if(stateThirdIRSensor == 1023) { // Verifica se nao ha sinal no terceiro sensor
                      Data.Latest.Timer[3] = millis();
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
                      Data.Latest.Interval[1] = abs(Data.Latest.Timer[2] - Data.Latest.Timer[1]);

                      Logger.Log(F("Processamento"), F("Calculando o segundo intervalo."));
                      Data.Latest.Interval[2] = abs(Data.Latest.Timer[3] - Data.Latest.Timer[2]);

                      // Variações de tempo
                      Logger.Log(F("Processamento"), F("Calculando a variação de tempo."));
                      Data.Latest.TimeVariation = abs(Data.Latest.Timer[3] - Data.Latest.Timer[0]);

                      Logger.Log(F("Processamento"), F("Calculando a variação de distância."));
                      Data.Latest.DistanceVariation = abs(Data.Latest.RailFinalPosition - Data.Latest.RailInitialPosition);
                      Logger.Log(F("MedidorVMM"), F("Variações e intervalos calculados e processados.\n"));
                      delay(1000);

                      // Velocidades medias
                      Logger.Log(F("MedidorVMM"), F("Calculando a velocidade media em diferentes medidas."));
                      Logger.Log(F("Processamento"), F("Calculando a velocidade em CM/MS."));
                      Data.Latest.AverageSpeed[1] = abs(Data.Latest.DistanceVariation / Data.Latest.TimeVariation); // cm/ms
                      Data.Latest.AverageSpeed[2] = abs(Data.Latest.DistanceVariation / Data.Latest.TimeVariation) * 1000; // cm/s

                      Logger.Log(F("Processamento"), F("Velocidades calculadas e processadas."));
                      delay(800);
                      Logger.Log(F("MedidorVMM"), F("Processamento de dados completo.\n"));
                      delay(1000);


                      Logger.Log(F("MedidorVMM"), F("Iniciando processamento SISRAM."));
                      delay(1000);
                      // Intervalos SISRAM
                      Logger.Log(F("Processamento"), F("Salvando variações e intervalos."));
                      Logger.Log(F("Processamento"), F("Salvando o primeiro intervalo."));
                      Data.Previous.Interval[Data.Latest.Reading][1] = abs(Data.Latest.Timer[2] - Data.Latest.Timer[1]);

                      Logger.Log(F("Processamento"), F("Salvando o segundo intervalo."));
                      Data.Previous.Interval[Data.Latest.Reading][2] = abs(Data.Latest.Timer[3] - Data.Latest.Timer[2]);

                      // Variações de tempo SISRAM
                      Logger.Log(F("Processamento"), F("Salvando a variação de tempo."));
                      Data.Previous.TimeVariation[Data.Latest.Reading] = abs(Data.Latest.Timer[3] - Data.Latest.Timer[0]);

                      Logger.Log(F("Processamento"), F("Salvando a variação de distância."));

                      Logger.Log(F("MedidorVMM"), F("Intervalos e variações calculadas e processados.\n"));
                      delay(1000);

                      // Velocidades medias SISRAM
                      Logger.Log(F("MedidorVMM"), F("Salvando a velocidade media em diferentes medidas."));
                      Logger.Log(F("Processamento"), F("Salvando a velocidade em CM/MS."));
                      Data.Previous.AverageSpeed[Data.Latest.Reading][1] = abs(Data.Latest.RailFinalPosition / Data.Latest.TimeVariation); // cm/ms
                      Data.Previous.AverageSpeed[Data.Latest.Reading][2] = abs(Data.Latest.RailFinalPosition / Data.Latest.TimeVariation) * 10; // cm/s
                      Logger.Log(F("MedidorVMM"), F("Processamento SISRAM completo."));
                      Display.printCentered(F("Processado!"), 1, 0);
                      DisplayInSerial(Data.Latest.Reading);
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
      if(Data.Latest.Reading > 0) {
        String _topMessage = ((String)F("====================[ Leitura N. ") + ReadingID + F("° ]===================="));
        String _firstIntervalMessage = ((String)F("Primeiro Intervalo: ") + Data.Previous.Interval[ReadingID][1] + F("ms"));
        String _secondIntervalMessage = ((String)F("Segundo Intervalo: ") + Data.Previous.Interval[ReadingID][2] + F("ms"));
        String _timeIntervalMessage = ((String)F("Variacao de tempo: ") + Data.Previous.TimeVariation[ReadingID] + F("ms"));
        String _DistanceVariationMessage = ((String)F("Variacao de distancia: ") + Data.Latest.DistanceVariation + F("cm"));
        String _averageSpeedMessageCMMS = ((String)F("Velocidade media: ") + Data.Previous.AverageSpeed[ReadingID][1] + F("cm/ms"));
        String _averageSpeedMessageCMS = ((String)F("Velocidade media: ") + Data.Previous.AverageSpeed[ReadingID][2] + F("m/s"));

        byte _topMessageLength = _topMessage.length() - 1;
        byte _firstIntervalMessageLength = _firstIntervalMessage.length();
        byte _secondIntervalMessageLength = _secondIntervalMessage.length();
        byte _timeIntervalMessageLength = _timeIntervalMessage.length();
        byte _DistanceVariationMessageLength = _DistanceVariationMessage.length();
        byte _averageSpeedMessageCMMSLength = _averageSpeedMessageCMMS.length();
        byte _averageSpeedMessageCMSLength = _averageSpeedMessageCMS.length();

        byte _firstIntervalMessageCentered = abs(_topMessageLength - _firstIntervalMessageLength) / 2;
        byte _secondIntervalMessageCentered = abs(_topMessageLength - _secondIntervalMessageLength) / 2;
        byte _timeIntervalMessageCentered = abs(_topMessageLength - _timeIntervalMessageLength) / 2;
        byte _DistanceVariationMessageCentered = abs(_topMessageLength - _DistanceVariationMessageLength) / 2;
        byte _averageSpeedMessageCMMSCentered = abs(_topMessageLength - _averageSpeedMessageCMMSLength) / 2;
        byte _averageSpeedMessageCMSCentered = abs(_topMessageLength - _averageSpeedMessageCMSLength) / 2;

        Serial.print(F("|"));
        Serial.print(_topMessage);
        Serial.println(F("|"));

        // First interval line
        Serial.print(F("|"));
        for(byte _i = 0; _i < _firstIntervalMessageCentered; _i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Primeiro intervalo: "));
        Serial.print(Data.Previous.Interval[ReadingID][1]);
        Serial.print(F("ms"));
        for(byte _i = 0; _i < (_topMessageLength) - (_firstIntervalMessageLength + _firstIntervalMessageCentered); _i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Second interval line
        Serial.print(F("|"));
        for(byte _i = 0; _i < _secondIntervalMessageCentered; _i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Segundo intervalo: "));
        Serial.print(Data.Previous.Interval[ReadingID][2]);
        Serial.print(F("ms"));
        for(byte _i = 0; _i < (_topMessageLength) - (_secondIntervalMessageLength + _secondIntervalMessageCentered); _i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Time interval line
        Serial.print(F("|"));
        for(byte _i = 0; _i < _timeIntervalMessageCentered; _i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Variacao de tempo: "));
        Serial.print(Data.Previous.TimeVariation[ReadingID]);
        Serial.print(F("ms"));
        for(byte _i = 0; _i < _topMessageLength - (_timeIntervalMessageLength + _timeIntervalMessageCentered); _i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Distance variation line
        Serial.print(F("|"));
        for(byte _i = 0; _i < _DistanceVariationMessageCentered; _i++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Variacao de distancia: "));
        Serial.print(Data.Latest.DistanceVariation);
        Serial.print(F("cm"));
        for(byte _i = 0; _i < _topMessageLength - (_DistanceVariationMessageLength + _DistanceVariationMessageCentered); _i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Average speed line
        Serial.print(F("|"));
        for(byte _x = 0; _x < _averageSpeedMessageCMMSCentered; _x++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Velocidade media: "));
        Serial.print(Data.Previous.AverageSpeed[ReadingID][1]);
        Serial.print(F("cm/ms"));
        for(byte _i = 0; _i < _topMessageLength - (_averageSpeedMessageCMMSLength + _averageSpeedMessageCMMSCentered); _i++) {
          Serial.print(F(" "));
        }
        Serial.println(F("|"));

        // Average speed line
        Serial.print(F("|"));
        for(byte _x = 0; _x < _averageSpeedMessageCMSCentered; _x++) {
          Serial.print(F(" "));
        }
        Serial.print(F("Velocidade media: "));
        Serial.print(Data.Previous.AverageSpeed[ReadingID][2]);
        Serial.print(F("m/s"));
        for(byte _i = 0; _i < _topMessageLength - (_averageSpeedMessageCMSLength + _averageSpeedMessageCMSCentered); _i++) {
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
        byte maxRead = maxRead;
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
                Display.printCentered((String)F("1I: ") + Reader.Data.Previous.Interval[Menu().Reading.currentRead][1] + F("ms"), 0, 0);
                Display.printCentered((String)F("2I: ") + Reader.Data.Previous.Interval[Menu().Reading.currentRead][2] + F("ms"), 1, 0);

              } else if(currentPage == variationPage) {
                String firstMessage = (String)F("T: ") + Reader.Data.Previous.TimeVariation[Menu().Reading.currentRead] + F("ms");
                String secondMessage = (String)F("D: ") + Reader.Data.Latest.DistanceVariation + F("cm");

                Display.printWithDelta(firstMessage, 0, 0);
                Display.printWithDelta(secondMessage, 1, 0);

              } else if(currentPage == speedPage) {
                Display.printCentered((String)F("VM: ") + Reader.Data.Previous.AverageSpeed[Menu().Reading.currentRead][1] + F("cm/ms"), 0, 0);
                Display.printCentered((String)F("VM: ") + Reader.Data.Previous.AverageSpeed[Menu().Reading.currentRead][2] + F("m/s"), 1, 0);
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
            if(Reader.Data.Latest.Reading > 0) {
              currentEntry--;
              break;
            }
        }
      } else if(currentEntry < maxEntry) { // Home <=> Reading (from switch to)
        switch(KeypadButton.Pressed()) {
          case KeypadButton.Select:
            delay(250);
            if(Reader.Data.Latest.Reading > 0) {
              currentEntry++;
              readingEntryTimer = millis();
              Reading.latestRead();
              break;
            }
        }
      }
    }

    void nextMenu() {
      if(Reader.Data.Latest.Reading > 0)
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
      if(Reader.Data.Latest.Reading > 0) {
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
  Reader.Data.Latest.RailFinalPosition = 90;
  Reader.Data.Latest.RailInitialPosition = 0;

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

/* Working on
*/
