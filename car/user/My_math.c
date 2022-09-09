#include  "My_math.h"


int constrain(int amt, int low, int high)
{
		if (amt < low)
				return low;
		else if (amt > high)
				return high;
		else
				return amt;
}

/******平方根倒数******/
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;  
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

/*****平方根**********/
float my_Sqrt(float x)
{
	
 return x*invSqrt( x);

}


