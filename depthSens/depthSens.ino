 /**********************************************************************
 【改造】
 　近接すると（手をかざすと）LEDが光る


【ライセンスについて】
Copyright(c) 2022 by tamanegi
Released under the MIT license
'http://tamanegi.digick.jp/about-licence/

【マイコン基板】
Raspberry Pi Pico

【スケッチの説明】
HC-SR04 超音波センサーを使用して距離を計測します。

距離の係数は
Distance(mm) = ((331.5m/s + 0.61t * 20degC) / 2) / 1000

【ライブラリ】
 Raspberry Pi Pico/RP2040 > Raspberry Pi Pico

【準備】
HC-SR04 の電源は5Vで、信号も5Vです。
Raspberry Pi PicoのGPIOに5V印加すると故障の原因になるので、3.3Vへ電圧変換します。

Raspberry Pi Pico <-> HC-SR04
VCC(5V)             <-> VCC
GND                 <-> GND
GPIO2               <-> Echo(3.3Vへ降圧する必要がある)
GPIO3               <-> Trig

降圧例1 : 分圧抵抗
(HC-SR04) Echo -> 100Ω -> (Pico) GPIO2 -> 200Ω -> (Pico) GND

降圧例2 : ロジックレベル変換モジュール (5V -> 3V)
(HC-SR04) Echo -> ロジックレベル変換モジュール -> (Pico) GPIO2

【バージョン情報】
2023/4/9 : 新規
**********************************************************************/

#define PIN_ECHO    15   // pin20
#define PIN_TRIGGER 14   // pin19
#define TRIGGERPULSE_US   10

#define SOUNDSPEED (331.5 + 0.61 * 20) //音速 331.5m/s, 摂氏 0.61t, 20℃

void setup() 
{
  Serial.begin(115200);

  //トリガピンと、エコーピンの設定
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  //トリガはLOWに設定
  digitalWrite(PIN_TRIGGER, LOW );
  delay(100);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //距離測定、トリガ発振
  digitalWrite(PIN_TRIGGER, HIGH );
  delayMicroseconds(TRIGGERPULSE_US);
  digitalWrite(PIN_TRIGGER, LOW );

  //反射時間の読み取り (単位 us)
  double DdTwoWayTime_us = pulseIn(PIN_ECHO, HIGH);             //us
  double DdOneWayTime_us = DdTwoWayTime_us / 2;                 //往復時間の計測を片道時間にする
  double DdDistance_mm = DdOneWayTime_us * SOUNDSPEED / 1000;   // 距離（mm）を計算
  Serial.println(DdDistance_mm);

  //近接ならLEDオン　そうでないならオフ
  if( DdDistance_mm < 100 )
    {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    }
  else
    {
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    }
  
  delay(100);
}