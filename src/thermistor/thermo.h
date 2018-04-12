/*
 * thermo.h
 *
 *  Created on: 11.04.2018
 *      Author: walery
 */

#ifndef THERMISTOR_THERMO_H_
#define THERMISTOR_THERMO_H_

struct s_thermistor_data{
	int temperature;
	float resistance;
};

//--------- vars
extern struct s_thermistor_data thermistor[];

//--------- function

extern void init_terminstor(float _target);

extern int wk_1(int index);

extern float p3(int index, float temp);

#endif /* THERMISTOR_THERMO_H_ */
