enum Estados{vermelho, verde, verde_fim, amarelo, noturno};
Estados estado = vermelho;

const int ledVermelhoPin = 2;
const int ledVerdePin = 4;
const int ledAmareloPin = 7;
const int ledAzulPin = 13;
const int botaoPin = 8;

unsigned long tInicio = 0;
unsigned long tInicioBotao = 0;
unsigned long piscada = 0;

bool apertou = false;

void setup() {
  pinMode(ledVermelhoPin, OUTPUT);
  pinMode(ledVerdePin, OUTPUT);
  pinMode(ledAmareloPin, OUTPUT);
  pinMode(ledAzulPin, OUTPUT);
  pinMode(botaoPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(estado);
  bool pressionado = (digitalRead(botaoPin) == LOW);
  unsigned long tAtual = millis();

  switch (estado) {
  case vermelho:
    if (!pressionado) {
      apertou = false;
      digitalWrite(ledVermelhoPin,HIGH);
      tInicioBotao = tAtual;
      if(tAtual - tInicio >= 10000){
        digitalWrite(ledVermelhoPin, LOW);
        digitalWrite(ledVerdePin, HIGH);
        tInicio = tAtual;
        estado = verde;
      }
    }else{
      if(!apertou && tAtual - tInicioBotao > 50){
        apertou = true;
        digitalWrite(ledVermelhoPin, LOW);
        digitalWrite(ledAzulPin, HIGH);
        piscada = tAtual;
        estado = noturno;
        tInicio = tAtual;
      }
    }
    break;
  case verde:
    if (!pressionado) {
      apertou = false;
      tInicioBotao = tAtual;
      if(tAtual - tInicio >= 6000){
        tInicio = tAtual;
        estado = verde_fim;
        piscada = tAtual;
      }
    }else{
      if(!apertou && tAtual - tInicioBotao > 50){
        apertou = true;
        digitalWrite(ledVerdePin, LOW);
        digitalWrite(ledAzulPin, HIGH);
        piscada = tAtual;
        tInicio = tAtual;
        estado = noturno;
      }
    }
    break;
  case verde_fim:
    if (!pressionado) {
      apertou = false;
      tInicioBotao = tAtual;
      if(tAtual - tInicio >= 2000){
        digitalWrite(ledVerdePin, LOW);
        digitalWrite(ledAmareloPin,HIGH);
        tInicio = tAtual;
        estado = amarelo;
      }else{
        if(tAtual - piscada >= 200 && digitalRead(ledVerdePin) == LOW){
          piscada = tAtual;
          digitalWrite(ledVerdePin, HIGH);
        }else if( tAtual - piscada >= 200 && digitalRead(ledVerdePin) == HIGH){
          piscada = tAtual;
          digitalWrite(ledVerdePin, LOW);
        }
      }
    }else{
      if(!apertou && tAtual - tInicioBotao > 50){
        apertou = true;
        digitalWrite(ledVerdePin, LOW);
        digitalWrite(ledAzulPin,HIGH);
        tInicio = tAtual;
        piscada = tAtual;
        estado = noturno;
      }
    }
    break;
  case amarelo:
    if (!pressionado) {
      apertou = false;
      tInicioBotao = tAtual;
      if(tAtual - tInicio > 2000){
        digitalWrite(ledAmareloPin, LOW);
        tInicio = tAtual;
        estado = vermelho;
      }
    }else{
      if(!apertou && tAtual - tInicioBotao > 50){
        apertou = true;
        digitalWrite(ledAmareloPin, LOW);
        digitalWrite(ledAzulPin, HIGH);
        tInicio = tAtual;
        piscada = tAtual;
        estado = noturno;
      }
    }
    break;
    
  case noturno:
    if(tAtual - piscada >= 1000 && digitalRead(ledAzulPin) == LOW){
        piscada = tAtual;
        digitalWrite(ledAzulPin, HIGH);
      }else if( tAtual - piscada >= 1000 && digitalRead(ledAzulPin) == HIGH){
        piscada = tAtual;
        digitalWrite(ledAzulPin, LOW);
        }
    if(apertou == true && pressionado == false){
      apertou = false;
    }

    if(!apertou){
      if(!pressionado){
        tInicioBotao = tAtual;
      }else{
        if(tAtual - tInicioBotao > 50){
          digitalWrite(ledAzulPin, LOW);
          tInicio = tAtual;
          apertou = true;
          estado = vermelho;
        }
      }
    }
    break;
  }

}
