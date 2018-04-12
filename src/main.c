/*
 ============================================================================
 Name        : pid.c
 Author      : CUBY,Ltd
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "pid/PID.h"
#include "thermistor/thermo.h"

// Structure to strore PID data and pointer to PID structure
struct pid_controller ctrldata;

_pid_t pid;

// Control loop input,output and setpoint variables
float input = 0, output = 0;
float setpoint = 240; // Температура

// Control loop gains
float kp = 2.5; //2.5;
float ki = 0.50;//1.0;
float kd = 1.00;//1.0;


static uint32_t ticks = 100;


static float temperature = 20;

float process_input()
{

//	temperature += 4;
	temperature += output/25;
	temperature -= 4;


	return temperature;

}

void process_output(float out)
{
	float output;
	output = out;
}

uint32_t tick_get()
{


//	result += 100;

	return ticks;
}

int main(void) {
//	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	int i;

	// Prepare PID controller for operation
	pid = pid_create(&ctrldata, &input, &output, &setpoint, kp, ki, kd);

	// Set controler output limits from 0 to 200
	pid_limits(pid, 0, 200);

	// Allow PID to compute and change output
	pid_auto(pid);

	// MAIN CONTROL LOOP
#ifdef DEBUG_PID
	for(i=0;i<230;i++){

		// Check if need to compute PID
		if (pid_need_compute(pid)) {
			// Read process feedback
			input = process_input();
			// Compute new PID output value
			pid_compute(pid);
			//Change actuator value
			process_output(output);
		}

		printf(" %d:%.2f:%.2f \n", ticks/100, output,temperature);
		//	printf("")

		ticks += 100;

	}// for
#endif

	const	float measure = 91;//37;//20;//10;//0.148;
	const float step = 0.001;

	float res = 0;
	float temperature = 0;

	init_terminstor(measure);

	int wk1 = wk_1(9);
	int wk2 = wk1;

	float section[2][2];	// Метод секущих.

	res = thermistor[wk2].resistance;

	printf("Termistor index %d:temperature %d, resist %.2f\n",wk1,thermistor[wk1].temperature, thermistor[wk1].resistance);
//
//
//	wk1 = wk_1(24);
//
//	printf("Termistor index %d:temperature %d, resist %.2f\n",wk1,thermistor[wk1].temperature, thermistor[wk1].resistance);
	while((res - measure)>0)
	{
		wk2 = wk_1(wk1);
		if(wk2<0)
			break;
		wk1 = wk2;
		res = thermistor[wk2].resistance;
		printf("Index : %d res:%f  diff:%f\n",wk2,res,(res-measure));
	}
	printf("Termistor index %d:temperature %d, resist %.2f\n",wk1,thermistor[wk1].temperature, thermistor[wk1].resistance);




//	temperature =  101.0;//100.5852890819; //100.5921931868; //100.4;
// вычисление точного значения температуры для заданного сопротивления.
	section[0][0] = thermistor[wk1].temperature;
	section[0][1] = thermistor[wk1].resistance - measure;

	section[1][0] = section[0][0]*(1-step);
	section[1][1] = p3(wk1,section[1][0]) - measure;

	// Певая производная
	float derivative = 0;
	float x1;

	for(int i=0;i<3;i++){

		derivative = (section[1][1] - section[0][1])/(section[1][0] - section[0][0]);
		x1 = section[1][0] - section[1][1]/derivative;
		section[0][0] = section[1][0];
		section[0][1] = section[1][1];
		section[1][0] = x1;
		section[1][1] = p3(wk1,section[1][0]) - measure;
	}

//	res = p3(wk1,temperature);

	printf("Resistor: %f temperature:%f diff:%f \n",measure,section[1][0],(section[1][1]));

	return EXIT_SUCCESS;
}
