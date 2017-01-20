#define Sensor1  A1

#define CtrlSensor1  9

#define Motor1      5

// максимальное время включения насоса в секундах
#define Motor1Time   8

//#define secInDay 1000*60*60

#define ___debug

void setup()
{

  pinMode(Sensor1, INPUT);
  pinMode(CtrlSensor1, OUTPUT);
  pinMode(Motor1, OUTPUT);
  analogWrite(Motor1, 0);
  
  #ifdef ___debug
  Serial.begin(9600);
  #endif
}

void loop()
{
  #ifdef ___debug
  Serial.print("1. ");
  #endif
  OneStep(Sensor1, CtrlSensor1, Motor1, Motor1Time, 400);

  delay(3600000);
  //delay(5000);

  
}

void OneStep(int Sensor, int CtrlSensor, int Motor, unsigned long MotorTime, int Value)
{
  float analog;
//  float cur_humidy;
  unsigned long stopTime;
  unsigned long curTime;

  // включаем питание на сенсоре
  analogWrite(CtrlSensor, 250);
  // выжидаем время, что-бы сенсор вышел на рабочий режим
  delay(3000);
  // считываем данные
  analog = analogRead(Sensor);

  #ifdef ___debug
  Serial.print("analog: ");
  Serial.println(analog);
  #endif
  // если значение больше Value - воды в поддоне нет
  if(analog > Value){
    // включаем насос, но не более чем на MotorTime сек
    #ifdef ___debug
    Serial.println(" ..motor on ");
    #endif

    stopTime = millis()+1000*MotorTime;
    curTime = millis();

    analogWrite(Motor, 255);
    while((analog > Value)&&(stopTime>curTime)){
      delay(10);
      curTime = millis();      
      analog = analogRead(Sensor);
      
      #ifdef ___debug
      Serial.print("analog: ");
      Serial.println(analog);
      #endif
    }
    // отключаем мотор
    analogWrite(Motor, 0);
    #ifdef ___debug
    Serial.println(" ..motor off ");
    #endif
  }else{
    // 
    delay(1000*MotorTime+2);
  }    
  
  // Отключаем сенсор
  analogWrite(CtrlSensor, 0);  

}
