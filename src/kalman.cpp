#include "main.h"
#include"kalman.h'

float cube(float x){ return x*x*x; };
float square(float x) {return x*x; };


float soc_ocv_fit(const float model_coeffs[4],float soc){
    return (model_coeffs[0]*cube(soc) + model_coeffs[1]*square(soc) 
    + model_coeffs[3]*soc + model_coeffs[4]);
};

