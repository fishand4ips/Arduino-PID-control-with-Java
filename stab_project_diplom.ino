#include <GyverPID.h>

volatile int val;
int numberPin = 9; // номер пина на который генерируется ШИМ
int rotate = 100;
int result = 0;
int led = 13;
String incomingString = "";

GyverPID regulator(0.1, 0.05, 0.01, 10);

void revolutions_per_minute() {
  val++;
}

void setup() {
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255

  // в процессе работы можно менять коэффициенты
  regulator.Kp = 5.2;
  regulator.Ki += 0.5;
  regulator.Kd = 0;

  Serial.begin(9600);
  attachInterrupt(0, revolutions_per_minute, CHANGE);
  // analogWrite(numberPin, rotate); // устанавливаем свои обороты в переменную rotate
}

void loop() {

  if (Serial.available() > 0) {
    incomingString = Serial.readString();
    String receiveStr = incomingString.substring(0, 9);
    String receiveInt = incomingString.substring(9);
    int incomingInteger = receiveInt.toInt();

    if (incomingString != -1 && receiveStr == "set_point") {
      int set = incomingInteger;
      regulator.setpoint = set;        // сообщаем регулятору скорость, которую он должен поддерживать
      Serial.print("set: ");
      Serial.println(set);
    }
    if (incomingString != -1 && receiveStr == "new_speed") {
      int new_sp = incomingInteger;
      regulator.input = new_sp;   // сообщаем регулятору текущую скорость
      Serial.print("new_sp: ");
      Serial.println(new_sp);
    }   
    
    // getResultTimer возвращает значение для управляющего устройства
    // (после вызова можно получать это значение как regulator.output)
    // обновление происходит по встроенному таймеру на millis()
    analogWrite(3, regulator.getResultTimer());  // отправляем на мосфет
    // .getResultTimer() по сути возвращает regulator.output
  }

  val = 0;
  delay(500);
  result = (val * 60) / 2;
  Serial.println(result); // количество импульсов на 60 секунд и делим на количество импульсов на оборот
  // Serial.print(" Оборотов в минуту(rpm): ");
  // Serial.print(val);
  // Serial.println(" val ");
}
