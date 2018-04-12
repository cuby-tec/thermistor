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
#include <math.h>

#include "pid/PID.h"
#include "thermistor/thermo.h"

//------- defs

#define GNUPLOT "gnuplot -persist"
//#define GNUPLOT "cat"	// to test output

#define DEBUG_PID

// Structure to strore PID data and pointer to PID structure
struct pid_controller ctrldata;

_pid_t pid;

// Control loop input,output and setpoint variables
float input = 0, output = 0;
float setpoint = 240; // Температура

#define N3

#ifdef N1
// Control loop gains
float kp = 0.8;//0.25;
float ki = 3.3;//0.7;
float kd = 0.0001;//0.01;
#endif

#ifdef N2
// Control loop gains
float kp = 0.25;
float ki = 0.7;
float kd = 0.01;
#endif

#ifdef N3
float kp = 0.2;//0.25;
float ki = 14.5;//0.7;
float kd = 0.0001;//0.01;
#endif


static uint32_t ticks = 100;


static float temperature = 20;

float process_input()
{

//	temperature += 4;
//	temperature += output/25;
//	temperature -= 4;
	float s = sin((double)ticks/1000);
	temperature = output+5*s; //10

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
#define EXAMPLE1
int main(void) {
//	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	int i;

	FILE *gp;
	gp = popen(GNUPLOT, "w");	/* 'gp' is the pipe descriptor */
	if (gp == NULL) {
		printf("Error opening pipe to GNU plot.\n"
			"Install with 'sudo apt-get install gnuplot'\n");
		exit(0);
	}


#ifdef EXAMPLE2
/*
# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
# set output 'fillbetween.1.png'
set style data lines
set title "Fill area between two curves"
set xrange [ 10.0000 : * ] noreverse nowriteback
set yrange [ 0.00000 : 175.000 ] noreverse nowriteback
DEBUG_TERM_HTIC = 119
DEBUG_TERM_VTIC = 119
## Last datafile plotted: "silver.dat"
plot 'silver.dat' u 1:2:3 "%lf %lf %lf" w filledcu,       '' u 1:2 lt -1 notitle, '' u 1:3 lt -1 notitle
*/

	fprintf(gp, "set style data lines");
	fprintf(gp, "set title 'Fill area between two curves'");
	fprintf(gp, "set xrange [ 10.0000 : * ] noreverse nowriteback");
	fprintf(gp, "set yrange [ 0.00000 : 175.000 ] noreverse nowriteback");
	fprintf(gp, "DEBUG_TERM_HTIC = 119");
	fprintf(gp, "DEBUG_TERM_VTIC = 119");
	fprintf(gp, "plot 'silver.dat' u 1:2:3 '%lf %lf %lf' w filledcu,'' u 1:2 lt -1 notitle, '' u 1:3 lt -1 notitle");

#endif

#ifdef EXAMPLE3

//# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
//# set output 'simple.1.png'
	fprintf(gp,"set key fixed left top vertical Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid");
	fprintf(gp,"set samples 50, 50");
	fprintf(gp,"set title 'Simple Plots'");
	fprintf(gp,"set title  font ',20' norotate");
	fprintf(gp,"DEBUG_TERM_HTIC = 119");
	fprintf(gp,"DEBUG_TERM_VTIC = 119");
	fprintf(gp,"plot [-10:10] sin(x),atan(x),cos(atan(x))");


#endif





	// Prepare PID controller for operation
	pid = pid_create(&ctrldata, &input, &output, &setpoint, kp, ki, kd);

	// Set controler output limits from 0 to 200
	pid_limits(pid, 0, 300);

	// Allow PID to compute and change output
	pid_auto(pid);

	// MAIN CONTROL LOOP
#ifdef DEBUG_PID

	float dim[230][3];

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

		dim[i][0] = ticks/100;
		dim[i][1] = output;
		dim[i][2] = temperature;
		printf(" %d:%.2f:%.2f \n", ticks/100, output,temperature);
//		fprintf(gp, "%d %g %g\n", ticks/100, output,temperature);
		//	printf("")

		ticks += 100;

	}// for

#ifdef EXAMPLE1
	// write your gnu-plot commands here
	fprintf(gp, "set samples 100\n");
	fprintf(gp, "plot 100+abs(sin(x))\n");
//	fprintf(gp, "rep abs(cos(x))\n");

	/*
		example sample data

		test:
		echo "set style data linespoints; plot '-' \n1 10\n2 20\n3 40\n4 30"|gnuplot -persist
	*/
//	fprintf(gp, "set style data filledcurves; rep '-' lt rgb 'red' \n-10 0\n");
/*
	double rnd, dat=0.0;
	for(double i=-10.0; i<10.0; i+=0.2){
		rnd=drand48();
		dat += 0.1*(rnd-0.5);
		fprintf(gp, "%g %g\n", i, dat+.5);
	}
	fprintf(gp, "10 0\n");
*/
#define MULTI	10
#define LEVEL	2400


	fprintf(gp, "set style data lines; rep '-' lt rgb 'red' \n-10 0\n");
	for(int i=0;i<230;i++){
		if(dim[i][1]>200)
			fprintf(gp, "%g %g \n", dim[i][0], MULTI*dim[i][1]-LEVEL);
		else
			fprintf(gp, "%g %g \n", dim[i][0], dim[i][1]);
	}

	fprintf(gp, "set style data lines; rep 'A' lt rgb 'blue' \n-10 0\n");
	for(int i=0;i<230;i++){
		if(dim[i][2]>200)
			fprintf(gp, "%g %g \n", dim[i][0], MULTI*dim[i][2]-LEVEL);
		else
			fprintf(gp, "%g %g \n", dim[i][0], dim[i][2]);
	}


#endif



#endif
#ifdef DEBUG_TERMISTOR
	const	float measure = 91;//6.6;//91;//37;//20;//10;//0.148;

	printf("main:Tmperature:%.2f for resist:%.2f",get_temperature(measure),measure);
#endif

	fclose(gp);

	return EXIT_SUCCESS;
}
