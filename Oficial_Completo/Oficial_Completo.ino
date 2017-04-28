#include <DS3231.h> //Carrega a biblioteca do temporizador
#include <LiquidCrystal.h> //Carrega a biblioteca LiquidCrystal

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

// Init a Time-data structure
Time  t;

//Botao
const int Button = 7;     //Constante Button recebe valor 7
int estado = 0;           //Variavel do botao - Ligado/Desligado

//Valvula solenoide
int porta_rele = 8;

//Sensor de umidade
#define pino_sinal_analogico A0
int valor_analogico;

void setup()
{
  //Define pinos para o rele como saida
  pinMode(porta_rele, OUTPUT); 
  
  //Define pinos para o sensor de umidade como entrada
  pinMode(pino_sinal_analogico, INPUT);
  
  //Define pino Button como entrada
  pinMode(Button, INPUT);   
  
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  
  // Setup Serial connection
  Serial.begin(115200);

  // Inicializa o temporizador
  rtc.begin();
  
  // Descomentar as linhas abaixo para setar a data correta
  //rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(15, 55, 50);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(20, 04, 2017);   // Set the date to January 1st, 2014
}

void loop()
{
  // Pega a data que esta marcada no temporizador
  t = rtc.getTime();
  
  // Printa os dados da data
  Serial.print("Today is the ");
  Serial.print(t.date, DEC);        // Dia
  Serial.print(". day of ");
  Serial.print(rtc.getMonthStr());  // Mes
  Serial.print(" in the year ");
  Serial.print(t.year, DEC);        // Ano
  Serial.println(".");
  
  // Printa os dados da data
  Serial.print("It is the "); 
  Serial.print(t.dow, DEC);         // Dia da semana  (Segunda-feira como dia 1)
  Serial.print(". day of the week (counting monday as the 1th), and it has passed ");
  Serial.print(t.hour, DEC);        // Hora
  Serial.print(" hour(s), ");
  Serial.print(t.min, DEC);         // Minutos
  Serial.print(" minute(s) and ");
  Serial.print(t.sec, DEC);         // Segundos
  Serial.println(" second(s) since midnight.");

  // Send a divider for readability
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");

  //Limpa a tela
  lcd.clear();

  valor_analogico = analogRead(pino_sinal_analogico);  //Variavel capta o valor do sensor de umidade


  //Tratamento do botao
  estado = digitalRead(Button);  /*Lê o pino Button e passa para a variável estado se HIGH ou LOW*/
  if(estado==1){    //Botao apertado
    //Imprime a data no display 
    lcd.setCursor(0,0);
    lcd.print("Hora:  ");
    lcd.print(rtc.getTimeStr());  //Printa as horas no display
    lcd.setCursor(0,1);
    lcd.print("Data: ");
    lcd.print(rtc.getDateStr());; //Printa a data no display
  }
  else{    //Botao nao apertado
    //Imprime a umidade no display 
    lcd.setCursor(0,0);
    lcd.print("Horta Interativa");
    lcd.setCursor(0,1);
    lcd.print("Solo: ");
    if (valor_analogico > 800){
      lcd.print("Seco");
    }
    else {
      lcd.print("Umido");
    }
    
  }


  //Tratamento da valvula solenoide com o tempo
  if(t.hour == 8 && t.min == 1){  //Primeiro horario que a valvula eh ligada
    digitalWrite(porta_rele, LOW);  //Liga rele
  }

  else if(t.hour == 18 && t.min == 1){ //Segundo horario que a valvula eh ligada
    digitalWrite(porta_rele, LOW);  //Liga rele
  }
  
  else{
    digitalWrite(porta_rele, HIGH);  //Desliga rele
  }

  
  // Wait one second before repeating :)
  delay (1000);
}
