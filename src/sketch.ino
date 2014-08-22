/**
 * Este programa é um software livre; você pode redistribuí-lo e/ou 
 * modificá-lo dentro dos termos da Licença Pública Geral GNU como 
 * publicada pela Fundação do Software Livre (FSF); na versão 2 da 
 * Licença, ou (na sua opinião) qualquer versão.
 * 
 * Este programa é distribuído na esperança de que possa ser útil, 
 * mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer
 * MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a
 * Licença Pública Geral GNU para maiores detalhes.
 * 
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU
 * junto com este programa, se não, escreva para a Fundação do Software
 * Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * 
 * File: sketch.ino
 * 
 * Author: Bruno Pitteli Gonçalves http://bruno.pitteli.com.br
 * Partners: Prime Seven - Web Solutions - http://primeseven.com.br
 * 
 * Repository: https://github.com/scorninpc/arduino-clock-rele
 */
 
// Adiciona a lib para comunicação I2C com o DS1307
#include <Wire.h>

// Configura o intervalo de horario de iluminação no formato Hi (8:30 = 830)
#define ILUMINACAO_INICIO 830
#define ILUMINACAO_FIM 1730

// Define o pino do rele
#define RELE_PINOUT 9

// Configura o RTC
int clockAddress = 0x68; 
byte _second, _minute, _hour, _day, _week_day, _month, _year;

/**
 * Converte binario para decimal
 */
byte binToDec(byte val)
{
  return ((val/16*10) + (val%16));
}

/**
 * Atualiza as variaveis com os dados de tempo
 */
void getDateDs1307()
{
	// Reseta o ponteiro
	Wire.beginTransmission(clockAddress);
	Wire.write(byte(0x00));
	Wire.endTransmission();
	
	// Move o ponteiro
	Wire.requestFrom(clockAddress, 7);

	// Recupera os dados de tempo
	_second = binToDec(Wire.read() & 0x7f);
	_minute = binToDec(Wire.read());
	_hour = binToDec(Wire.read() & 0x3f);  
	_week_day = binToDec(Wire.read());
	_day = binToDec(Wire.read());
	_month = binToDec(Wire.read());
	_year = binToDec(Wire.read());
}

/**
 * Configura o hardware
 */
void setup()
{
	// Configura os pinos de saida
	pinMode(RELE_PINOUT, OUTPUT);
	
	// Inicia o wire
	Wire.begin();
}

/**
 * Looping principal
 */
void loop()
{
	// Cria o horario formatado
	getDateDs1307();
	int formated_time = (_hour * 100) + _minute;
	
	// Verifica se está no intervalo configurado
	if((formated_time >= ILUMINACAO_INICIO) && (formated_time < ILUMINACAO_FIM)) {
		// Ativa o rele
		digitalWrite(RELE_PINOUT, HIGH);
	}
	else {
		// Desliga o rele
		digitalWrite(RELE_PINOUT, LOW);
	}
	
	// Espera 1 segundo
	delay(1000);
}
