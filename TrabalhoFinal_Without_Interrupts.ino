
// *** Definição dos pinos e variaveis ***//
#define Trigger_Pin 2  // Pino para o trigger
#define Echo_Pin 1      //Pino para o echo
#define Frequency 1000/15  // Definir quantas vezes por segundo é lido o sonar
#define Loop_Freq 1000    //  Definir quantos loops foram feitos em 1 segundo

unsigned long dist;     // Variavel para a distancia
unsigned long current_time; //variavel para ler o tempo actual
unsigned long start_time;   //variavel para ler o tempo inicial para a leitura do sonar
unsigned long s_loop_time;  //variavel para ler o tempo inicial para a leitura do loop
int count;    // contador para contar quantos loop foram feitos durante 1 segundo


void setup() {

  Serial.begin(115200);  //iniciar a porta serie a 115200 bps
  pinMode(Trigger_Pin, OUTPUT);  // definir o pino do trigger como OUTPUT
  pinMode(Echo_Pin, INPUT);     // definir o pino do echo como INPUT
  start_time=millis();    // ler o tempo inicial para o sonar
  s_loop_time=millis();  // ler o tempo inicial para o loop
  count=0;    //iniciar o contador a zero
  digitalWrite(Trigger_Pin,LOW);  // escrever o valor LOW no pino do trigger
  
}

void loop() {  
  current_time=millis();  // ler o tempo actual
  if(current_time - start_time >= Frequency){     // condição de leitura do sonar
                                                  //verificar se o tempo actual - o tempo inicial sao maiores ou iguais que 1000/15
    dist=getDistance();           // ler a distancia chamando a função getDistance()
    Serial.print("Distance: ");     
    Serial.println(dist);         // Escrever no computador o valor da distancia e efetua mudança de linha
    start_time+=Frequency;        // atualiza o tempo inicial para o sonar
  }
  count++;  // incremetar o contador do loop
  
  current_time=millis();    // ler o tempo actual
  if(current_time - s_loop_time >=Loop_Freq){  // condição de escrita do contador do loop
                                                 //verificar se o tempo actual - o tempo inicial sao maiores ou iguais que 1000
    Serial.print("Nº de Loops: ");
    Serial.println(count);        // Escrever no computador o valor da contador e efetua mudança de linha
    s_loop_time+=Loop_Freq;       // atualizar o tempo inicial para o loop
    count=0;      //atualiza o valor do contador para zero
  }  
}

// Funçao para enviar sinal do trigger ,ler o tempo de retorno do echo e calcular a distancia
int getDistance(){
    
    digitalWrite(Trigger_Pin,LOW);     // escrever o valor LOW no pino do trigger
    int distance=pulseIn(Echo_Pin,HIGH)*0.017; //ler o tempo de retorno do echo medido para isso o tempo que 
                                                //ele se encontra a HIGH e multiplicar por metade da velocidade do som em cm
    digitalWrite(Trigger_Pin,HIGH);   // escrever o valor HIGH no pino do trigger
  	return distance;	    // retorna o valor da distancia
}

