/*
 * thermo.c
 *
 *  Created on: 11.04.2018
 *      Author: walery
 *      Таблица значений термистора.
 */

#include "thermo.h"


//------- defs

#define STEP	0.001

// --------- vars

int arr_index[50];

static int _ndex = 0;

static float target;

//------ function


static
void init_terminstor(float _target)
{
	target = _target;
	arr_index[_ndex++] = 7;
	arr_index[_ndex] = 8;
//	arr_index[_ndex+1] = wk_1(2);
}

/**
 * Формирование следующего индекса.
 */
static
int wk_1(int index)
{
	int a,b,result;

	float r,s;
	float u,v;
	_ndex++;
//	if (index<=2){
//		a = arr_index[0];
//		b = arr_index[1];
//	}else{
		a = arr_index[_ndex-2];
		b = arr_index[_ndex-1];
//	}

	result = b;
	s = b;

	u = thermistor[b].resistance - target;
	v = thermistor[a].resistance -target;

	if(u != v){
		r = u * (b - a)/(u - v);
		s -=r;
		s +=0.5;
		result = s;
	}
	else
		result = -1;

	arr_index[_ndex]=result;

	return result;

}

/**
 * Вычисление сопротивления для произволной температуры.
 */
static
float p3(int index, float temp)
{
	float result=0;
	struct s_thermistor_data data[4];

	float df[3][3];

	float q;

	const float step = 1.0;

//	data[3].temperature = index;	//131
//	data[2].temperature = index-1;	//130
//	data[1].temperature = index -2;	//129
//	data[0].temperature = index-3;	//128

	for(int i=0;i<4;i++ )
	{
		data[i].resistance = thermistor[index +i-3].resistance;
		data[i].temperature = thermistor[index +i-3].temperature;
	}


	df[0][0] = data[1].resistance-data[0].resistance;
	df[1][0] = data[2].resistance-data[1].resistance;
	df[2][0] = data[3].resistance-data[2].resistance;

	df[0][1] = df[1][0] - df[0][0];
	df[1][1] = df[2][0]-df[1][0];

	df[0][2] = df[1][1]-df[0][1];

	q = (temp - data[3].temperature)/step;

	result = data[3].resistance+q*df[2][0]+q*(q+1)/2*df[1][1]+q*(q+1)*(q+2)/6*df[0][2];

	return result;
}

	/**
	 * Secant method
	 * вычисление точного значения температуры для заданного сопротивления.
	 *
	 */
static
float secant(int index)
{
//	float result;
	float secant[2][2];	// Метод секущих.

	secant[0][0] = thermistor[index].temperature;
	secant[0][1] = thermistor[index].resistance - target;

	secant[1][0] = secant[0][0]*(1-STEP);
	secant[1][1] = p3(index,secant[1][0]) - target;

	// Певая производная
	float derivative = 0;
	float x1;

	for(int i=0;i<3;i++){

		derivative = (secant[1][1] - secant[0][1])/(secant[1][0] - secant[0][0]);
		x1 = secant[1][0] - secant[1][1]/derivative;
		secant[0][0] = secant[1][0];
		secant[0][1] = secant[1][1];
		secant[1][0] = x1;
		secant[1][1] = p3(index,secant[1][0]) - target;
	}

//	res = p3(wk1,temperature);

	printf("Resistor: %f temperature:%f diff:%f \n",target,secant[1][0],(secant[1][1]));


	return secant[1][0];

}

float get_temperature(float target)
{
	float res = 0;
	float temperature = 0;

	init_terminstor(target);

	int wk1 = wk_1(9);
	int wk2 = wk1;

	res = thermistor[wk2].resistance;

	printf("Termistor index %d:temperature %d, resist %.2f\n",wk1,thermistor[wk1].temperature, thermistor[wk1].resistance);

	while((res - target)>0)
	{
		wk2 = wk_1(wk1);
		if(wk2<0)
			break;
		wk1 = wk2;
		res = thermistor[wk2].resistance;
		printf("Index : %d res:%f  diff:%f\n",wk2,res,(res-target));
	}
	printf("Termistor index %d:temperature %d, resist %.2f\n",wk1,thermistor[wk1].temperature, thermistor[wk1].resistance);

	temperature = secant(wk1);

	return temperature;
}
