//*****   Definiçao dos pinos dos leds, do trigger e do echo *****//
#define Led_Pin_0 5
#define Led_Pin_1 6
#define Led_Pin_2 7
#define Led_Pin_3 8
#define Led_Pin_4 9
#define Led_Pin_5 10
#define Led_Pin_6 11
#define Led_Pin_7 12
#define Trigger_Pin 2
#define Echo_Pin 1
//***************************************************************//                                 
unsigned long transition_time;  // variavel para guardar o tempo de transição entre os leds
unsigned long dist;             // variavel para guardar o valor da distancia que irá ser medida
unsigned long current_time;     //variavel para ler o tempo actual
unsigned long start_time;        //variavel para ler o tempo inicial
unsigned char PIN_NUM[8]={Led_Pin_0,Led_Pin_1,Led_Pin_2,Led_Pin_3,Led_Pin_4,Led_Pin_5,Led_Pin_6,Led_Pin_7}; // array que irá conter os pinos referentes aos leds
                                                                                                            
bool bright = false;  // variavel que irá conter o estado do led(HIGH ou LOW) -- Aceso ou apagado
bool direction = true;    // Variavel para controlar a direção dos leds
int nLeds;    // indice para ler a posiçao dos leds no array
int count;    // contador para controlar o estado do led


void setup() {
  Serial.begin(115200); //iniciar a porta serie a 115200 bps
  //***** definir os pinos dos leds como OUTPUT *****//
  pinMode(Led_Pin_0,OUTPUT);  
  pinMode(Led_Pin_1,OUTPUT);
  pinMode(Led_Pin_2,OUTPUT);
  pinMode(Led_Pin_3,OUTPUT); 
  pinMode(Led_Pin_4,OUTPUT);
  pinMode(Led_Pin_5,OUTPUT);
  pinMode(Led_Pin_6,OUTPUT);
  pinMode(Led_Pin_7,OUTPUT); 
  //************************************************//
  pinMode(Trigger_Pin, OUTPUT); //definir o pino do trigger como OUTPUT
  pinMode(Echo_Pin, INPUT);    //definir o pino do echo como INPUT
  start_time=millis();          // ler o tempo inicial
  nLeds=0;          // inicializar o indice do array a zero -- primeira posiçao do array
  count=0;          // inicializar o contador a zero
}

void loop() {    
  current_time= millis();  // ler o tempo inicial
  dist=getDistance();  // ler a distancia chamando a função getDistance()
  if(dist<3){ // condiçao de validaçao se distancia for inferior a 3 -- distancia minima de leitura do sonar
    dist=3;   // se a condiçao for verificada a distancia assume o valor 3
  } 
  Serial.print("dist: ");
  Serial.println(dist);  // Escrever no computador o valor da distancia e efetua mudança de linha  
  transition_time = map(dist,0,400,0,1000);  // mapear o valor da distancia para um valor maximo de 1 segundo de transição  
  if(current_time - start_time >= 100){ // condiçao para a transição entre leds
                                                    // se condiçao se verificar:
    bright=!bright;                               // inverter o valor do estado do led
    digitalWrite(PIN_NUM[nLeds],bright);          // escrever o valor do bright no led referenciado no array
    count++;      // incrementar contador  
    if(direction){      // condição de direção
                        // se condição de direção for true
      if(count==2){     //condição de controlo de estado do led
                        // se o contador for igual a dois, significa que o led acende e apagou e é a altura de transitar para o led seguinte
        count=0;        // recolocar o contador a zero
        nLeds++;          // incrementar o indice do array para passar para o led seguinte
        if(nLeds>6){        // condiçao de inversao de direção
                            // se o indice for maior que 6 significa que estamos no led da posiçao 7 e ultima do array e temos que iniciar mudança de direção
          direction=!direction;        // inverter o valor da direção 
        }
      } 
    }else{  // se condiçao de direção for false
        if(count==2){ //condição de controlo de estado do led
                        // se o contador for igual a dois, significa que o led acende e apagou e é a altura de transitar para o led seguinte
        count=0;    // recolocar o contador a zero
        nLeds--;    // decrementar o indice do array para passar para o led seguinte, de forma descendente
        if(nLeds==0){  // condiçao de inversao de direção
                            // se o indice for igual a zero significa que estamos no led da primeira posição e temos que iniciar mudança de direção
          direction=!direction; // inverter o valor da direção
        }
      }
    }    
    start_time+=100;  //atualiza o tempo inicial somando o valor da transição
  }
}

// Funçao para enviar sinal do trigger ,ler o tempo de retorno do echo e calcular a distancia
int getDistance(){
    digitalWrite(Trigger_Pin,LOW);    // escrever o valor LOW no pino do trigger
    int distance=pulseIn(Echo_Pin,HIGH)*0.017;  //ler o tempo de retorno do echo medido para isso o tempo que 
                                                //ele se encontra a HIGH e multiplicar por metade da velocidade do som em cm
    digitalWrite(Trigger_Pin,HIGH);  // escrever o valor HIGH no pino do trigger
  	return distance;	// retorna o valor da distancia
}


