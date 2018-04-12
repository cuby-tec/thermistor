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

	const	float measure = 91;//6.6;//91;//37;//20;//10;//0.148;

	printf("main:Tmperature:%.2f for resist:%.2f",get_temperature(measure),measure);


	return EXIT_SUCCESS;
}
