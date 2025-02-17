#ifndef KALMAN_H
#define KALMAN_H 

const float soc_ocv_fit_coeffs[4] = {
    1.119, -1.727, 
    1.607, 3.184
};

float cube(float x);
float square(float x);
float soc_ocv_fit(const float model_coeffs[4],float soc);


#endif