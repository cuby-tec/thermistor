/*
 * thermo.h
 *
 *  Created on: 11.04.2018
 *      Author: walery
 */

#ifndef THERMISTOR_THERMO_H_
#define THERMISTOR_THERMO_H_


#include "thermistor_data.h"

//--------- function

//extern void init_terminstor(float _target);

//extern int wk_1(int index);

//extern float p3(int index, float temp);

/**
 * Secant method
 * вычисление точного значения температуры для заданного сопротивления.
 */
//extern float secant(int index);

/**
 * Вычисление температуры для заданного сопротивления.
 */
extern float get_temperature(float target);

#endif /* THERMISTOR_THERMO_H_ */
