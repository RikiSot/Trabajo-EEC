
#include "algoritmo.h"


signed int yECG[5];
unsigned int der, maximo, maximo_ant, PPM;
unsigned int flag, k, umbral;
float BCL, BCL2;


void init_algoritmo(void)
{

	for(int i=0; i<5; i++)	yECG[i] = ADC1BUF0;
	flag=0;
	maximo = 0;
	umbral = 0;
	BCL = 0;

}

int algoritmo()
{

	yECG[4] = yECG[3];
	yECG[3] = yECG[2];
	yECG[2] = yECG[1];
	yECG[1] = yECG[0];
	yECG[0] = ADC1BUF0;

	//Valor absoluto de la derivada
	der = (yECG[4] > yECG[0]) ? (yECG[4] - yECG[0]) : (yECG[0] - yECG[4]);

	if(der > maximo) maximo = der;

	if(flag == 0)
	{
		if(der > umbral)
		{
			flag = 1;
			// basal cycle length
			BCL2 = BCL;
			BCL = 0;
			BEEP(1);
		}
	}

	if (flag == 1)
	{
		k++;
	}

	//Durante 60 puntos = RR min = 0.24seg = 250 bpm
	//no se puede detectar otro punto RR
	//solo pasado ese tiempo se hace el calculo

	if(k >60)
	{
		k = 0;
		flag = 0;

	//Me quedo con la Media del incremento MAX
	//De estos 200 puntos y del incremento de los anteriores

	umbral = (maximo + maximo_ant)/2;
	maximo_ant = maximo;
	maximo = 0;

	//Reduzco el umbral al 66%

	umbral = umbral -(umbral/3);
	}

	//Si pasa un rato
	//equivaldria a un pulso de 15bpm si se detectara punto R
	//y no se ha superado el umbral se reinicia

	if(BCL > 1000)
	{
		maximo = 0;
		umbral = 0;
		flag = 1;
		BCL = 0;
	}
	//Calculo el ritmo cardiaco (si esta en rango fisiologico)
	//y si no permanece el valor anteriore
	//15000 son las muestras de 1 minuto (60000[ms]/4[ms/muestra])

	if((BCL2 > 65) && (BCL2 < 500))
	{
		PPM = (int)(15000/BCL2);
	}
return PPM;
}
