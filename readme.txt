output = Kp * err + (Ki * int * dt) + (Kd * der /dt);

where

Kp = Proptional Constant.
Ki = Integral Constant.
Kd = Derivative Constant.
err = Expected Output - Actual Output ie. error;
int  = int from previous loop + err; ( i.e. integral error )
der  = err - err from previous loop; ( i.e. differential error)
dt = execution time of loop.


where initially 'der' and 'int' would be zero. 
If you use a delay function in code to tune the loop 
frequency to say 1 KHz then your dt would be 0.001 seconds.

I found this excellent code for PID in C, though it doesn't cover every aspect of it, its a good one nonetheless.

//get value of setpoint from user
while(1){
  // reset Timer
  // write code to escape loop on receiving a keyboard interrupt.
  // read the value of Vin from ADC ( Analogue to digital converter).
  // Calculate the output using the formula discussed previously.
  // Apply the calculated outpout to DAC ( digital to analogue converter).
  // wait till the Timer reach 'dt' seconds.
}

If we take a slow process, 
then we can use lower frequency such that 
dt >>> code execution time for single loop ( far far greater than ). 
In such cases we can do away with timer and use a delay function instead.

source: https://softwareengineering.stackexchange.com/questions/186124/programming-pid-loops-in-c
