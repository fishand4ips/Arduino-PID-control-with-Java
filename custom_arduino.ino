volatile int val;
int numberPin = 9; // номер пина на который генерируется ШИМ
int rotate = 100;
int result = 0;
int led = 13;

String incomingString = "";

// для PID
int setpoint = 0;   // заданная величина, которую должен поддерживать регулятор
int input = 0;      // сигнал с датчика (например температура, которую мы регулируем)
int output = 0;     // выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
int pidMin = 0;     // минимальный выход с регулятора
int pidMax = 255;   // максимальный выход с регулятора

int prevInput = 0;
float integral = 0.0;
//

// коэффициенты
float Kp = 1.0;
float Ki = 1.0;
float Kd = 1.0;
float _dt_s = 0.1; // время итерации в секундах

// ПИД
// функция расчёта выходного сигнала
int computePID() {
  float error = setpoint - input;           // ошибка регулирования
  float delta_input = prevInput - input;    // изменение входного сигнала
  prevInput = input;
  output = 0;
  output += (float)error * Kp;                  // пропорционально ошибке регулирования
  output += (float)delta_input * Kd / _dt_s;    // дифференциальная составляющая
  integral += (float)error * Ki * _dt_s;        // расчёт интегральной составляющей
  // тут можно ограничить интегральную составляющую!
  output += integral;                           // прибавляем интегральную составляющую
  output = constrain(output, pidMin, pidMax);   // ограничиваем выход
  return output;
}

void revolutions_per_minute() {
  val++;
}

int receive_setpoint() {
  return set;
}

int receive_new_speed() {
  return new_sp;
}


void setup() {
  Serial.begin(9600);
  attachInterrupt(0, revolutions_per_minute, CHANGE);
  analogWrite(numberPin, rotate); // устанавливаем свои обороты в переменную rotate
}

void loop() {
  if (Serial.available() > 0) {
    incomingString = Serial.readString();   
    String receiveStr = incomingString.substring(0, 9);   
    String receiveInt = incomingString.substring(9);  
    int incomingInteger = receiveInt.toInt();  
       
    if (incomingString != -1 && receiveStr == "set_point") {
      int set = incomingInteger;
      Serial.print("set: ");
      Serial.println(set);
    }
    if (incomingString != -1 && receiveStr == "new_speed") {
      int new_sp = incomingInteger;
      Serial.print("new_sp: ");
      Serial.println(new_sp);
    }
  }



  val = 0;
  delay(500);
  result = (val * 60) / 2;
  Serial.println(result); // количество импульсов на 60 секунд и делим на количество импульсов на оборот
  // Serial.print(" Оборотов в минуту(rpm): ");
  // Serial.print(val);
  // Serial.println(" val ");
}
