

#define TRIG_PIN 2  // Pino para o trigger
#define ECHO_PIN 1  // Pino para o trigger
#define SONAR_FREQ 1000000/15  // Definir quantas vezes por segundo é lido o sonar (em microsegundos)
#define LOOP_FREQ 1000000     //  Definir quantos loops foram feitos em 1 segundo  (em microsegundos)
hw_timer_t* timer_n = NULL;   //definir o ponteiro do timer
unsigned long start_time;     //variavel para ler o tempo inicial para a leitura do sonar
unsigned long s_time_loop;    //variavel para ler o tempo inicial para a leitura do loop
unsigned long current_time;   //variavel para ler o tempo actual
unsigned long timer_count;    // contador para contar quantos loop foram feitos durante 1 segundo
static unsigned long s_time;   //variavel para ler o instante em que o eccho passou a HIGH
static unsigned long e_time;    //variavel para ler o instante em que o eccho passou a LOW
volatile unsigned long dist;    // Variavel parab guardar o valor da distancia
static bool state = true;       // Variavel para controlar o estado do trigger



//** Função do timer interrupt **//
void IRAM_ATTR pulseTrig(){

    digitalWrite(TRIG_PIN,state); // escrever o valor do estado (state) no pino do trigger
    state=!state;  // inverter o estado da variavel 

}
//** Função do interrupt externo**//
void IRAM_ATTR pulseEcho(){  

  if(digitalRead(ECHO_PIN)){  // condiçao para verificar qual o valor do echo (LOW ou HIGH)
                              // se o valor lido do pino do trigger for HIGH
    s_time=micros();          // vai ler o tempo inicial do echo
  }else {                     // caso o valor lido seja LOW
    e_time=micros();          // vai ler o tempo final do echo
  }
  dist = (e_time-s_time)*0.017;  // calcula a distancia subtraindo o valor do tempo inicial do echo
                                  //ao valor do tempo final do echo e multiplica pela constante da velocidade
                                  // do som em cm
}

void setup() {

  Serial.begin(115200);   //iniciar a porta serie a 115200 bps
  pinMode(TRIG_PIN,OUTPUT);    // definir o pino do trigger como OUTPUT
  pinMode(ECHO_PIN,INPUT);       // definir o pino do echo como INPUT
  digitalWrite(TRIG_PIN,LOW);   // escrever o valor LOW no pino do trigger
  timer_n=timerBegin(0,80,true);    //configura o ponteiro do timer como timer 0, com um divisor de frequencia igual
                                    // a 80 para que que o incremento seja feito em microsegundos
  timerAttachInterrupt(timer_n,pulseTrig,true);  // associa a função ao timer previamente configurado
  timerAlarmWrite(timer_n,SONAR_FREQ,true);   // configura o valor de referencia no qual o interrupt vai acontecer
  timerAlarmEnable(timer_n); //inicializa o timer
  attachInterrupt(ECHO_PIN,pulseEcho,CHANGE);  // associa a função ao interrupt externo e define o modo no qual era sera activado
                                                // neste caso será quando acontecer mudança de sinal (0 ou 1)
  start_time=micros();      // ler o tempo inicial para o sonar
  s_time_loop=micros();     // ler o tempo inicial para o loop
  timer_count=0;        //iniciar o contador a zero

}

void loop() {  

  timer_count++;  // incremetar o contador do loop
  current_time=micros();  // ler o tempo actual
  if(current_time - start_time >= SONAR_FREQ){  // condição de escrita do sonar 
                                                 //verificar se o tempo actual - o tempo inicial sao maiores ou iguais que 1000000/15   
    Serial.print("Distance: "); 
    Serial.println(dist);                   // Escrever no computador o valor da distancia e efetua mudança de linha
    start_time+=SONAR_FREQ;                 // atualiza o tempo inicial para o sonar
 }

  current_time=micros();   // ler o tempo actual
  if(current_time - s_time_loop >= LOOP_FREQ){  // condição de escrita do contador do loop
                                                //verificar se o tempo actual - o tempo inicial sao maiores ou iguais que 1000000
    Serial.print("Number of loop calls: ");
    Serial.println(timer_count);    // Escrever no computador o valor da contador e efetua mudança de linha
    timer_count=0;                  // atualizar o tempo inicial para o loop
    s_time_loop+=LOOP_FREQ;          //atualiza o valor do contador para zero
  }
}