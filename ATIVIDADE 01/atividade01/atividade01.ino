int LED = 13;
int BOTAO = 2; 

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT);
}
////1
void loop() {
  
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

////2
//void loop(){
//  int estado = digitalRead(BOTAO);
//
//  if(estado == HIGH){
//     digitalWrite(LED, HIGH);
//  } 
//  else{
//    digitalWrite(LED, LOW);
//  }
//}

////3
//int op = 1;
//void loop(){
//  int estado_botao = digitalRead(BOTAO);
//  int estado_led = digitalRead(LED);
//  
//  if(estado_botao == HIGH){
//    if(op == 1){
//      if(estado_led == HIGH){
//        digitalWrite(LED, LOW);
//        op = 0;
//      }
//      else{
//        digitalWrite(LED, HIGH);
//        op = 0;
//      }
//    }
//  }
//  else{
//    op = 1;
//  }
//  delay(50);
//}

////4
//int op = 1;
//int cont = 0;
//
//void loop(){
//  int estado_botao = digitalRead(BOTAO);
//  int estado_led = digitalRead(LED);
//  
//  if(estado_botao == HIGH){
//    if(op == 1){
//      cont++;
//      op = 0;
//      if(cont == 3){
//        if(estado_led == HIGH){
//          digitalWrite(LED, LOW);
//          
//        }
//        else{
//          digitalWrite(LED, HIGH);
//      
//        }
//        cont = 0;
//      }
//    }
//  }
//  else{
//    op = 1;
//  }
//}
