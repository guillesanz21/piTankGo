/** File Name    : Joystick-Torreta.ino
 *  Description   : Se encarga de detectar los movimientos (análogicos) del joystick y enviarle los datos (digitales) a la Raspberry a través de los pines.
 */

/* Arduino pin numbers
------------------------------------------------------------------*/
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A1; // analog pin connected to X output
const int Y_pin = A0; // analog pin connected to Y output
const int Y_positivo = 4; 
const int Y_negativo = 5;
const int X_positivo = 7;
const int X_negativo = 6;
const int Btn_central = 3;

/*
 * @brief   Configuracion de los pines digitales de Arduino, inicializándolos con 0 
 * excepto el botón del Joystick, que se inicializa con 1
 */

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  digitalWrite(SW_pin, HIGH);
  pinMode(Y_positivo, OUTPUT);
  digitalWrite(Y_positivo, LOW);
  pinMode(Y_negativo, OUTPUT);
  digitalWrite(Y_negativo, LOW);
  pinMode(X_positivo, OUTPUT);
  digitalWrite(X_positivo, LOW);
  pinMode(X_negativo, OUTPUT);
  digitalWrite(X_negativo, LOW);
  pinMode(Btn_central, OUTPUT);
  digitalWrite(Btn_central, LOW);
  Serial.begin(9600);

}

/*
 * @brief   El Joystick que se está usando es un Joystick analógico, por eso necesitamos usar el Arduino como intermediario.
 * La posicion neutra del Joystick para ambos ejes son 500 mV. El umbral, sin embargo, lo hemos puesto en 900 para las coordenadas positivas 
 * y en 100 para las negativas para así, poder evitar movimientos indeseados de la torreta.
 */

void loop() {
  if(analogRead(Y_pin) > 900){
    digitalWrite(Y_positivo, HIGH);
  } else {
    digitalWrite(Y_positivo, LOW);
  }
  if(analogRead(Y_pin) < 100){
    digitalWrite(Y_negativo, HIGH);
  } else {
    digitalWrite(Y_negativo, LOW);
  }
  if(analogRead(X_pin) > 900){
    digitalWrite(X_positivo, HIGH);
  } else {
    digitalWrite(X_positivo, LOW);
  }
  if(analogRead(X_pin) < 100){
    digitalWrite(X_negativo, HIGH);
  } else {
    digitalWrite(X_negativo, LOW);
  }
    if(digitalRead(SW_pin) == 0){
    digitalWrite(Btn_central, HIGH);
  } else {
    digitalWrite(Btn_central, LOW);
  }

  delay(100);

 
}
