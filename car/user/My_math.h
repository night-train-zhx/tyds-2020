#ifndef __MH_MATH_H_
#define __MH_MATH_H_

#include <stdint.h>
#include <string.h>
#include <math.h>


#define abs(x) 		(((x) < 0) ? (-x) : (x))


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define limit( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
#define my_pow(a) ((a)*(a))
#define safe_div(numerator,denominator,safe_value) ( (denominator == 0) ?  (safe_value) : ((numerator)/(denominator)) ) 
																	

float invSqrt(float x);
int constrain(int amt, int low, int high);	
float my_Sqrt(float x);



#endif /* __math_h */


