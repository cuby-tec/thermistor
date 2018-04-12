/*
 * thermistor_data.h
 *
 *  Created on: 12.04.2018
 *      Author: walery
 */

#ifndef THERMISTOR_THERMISTOR_DATA_H_
#define THERMISTOR_THERMISTOR_DATA_H_


struct s_thermistor_data{
	int temperature;
	float resistance;
};

//--------- vars
extern struct s_thermistor_data thermistor[];


#endif /* THERMISTOR_THERMISTOR_DATA_H_ */
