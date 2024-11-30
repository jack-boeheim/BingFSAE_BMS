clc
clear
close all 
%%Setup/Define Model Parameters
load("soc_ocv_data.mat");
load("discharge_data.mat");
dt = zeros(size(data,1),1);
for i=1:size(data,1)-1
    dt(i+1) = data.time_s(i+1) - data.time_s  (i,1); 
end 
dt = dt/3600;
OCV_SOC = fit(SOC,OCV,'poly3'); %Fit Manufacture Provided Data to 3rd Order Poly

R_0 = 2.8e-3; %Internal Resistance Per Manufacturer
R_1 = 1.6e-3; %Diffusion and Charge Transfer Parameters/Per Literature as Placeholder for now
tau_1 = 3.68;
R_2 = 7.7e-3;
tau_2 = 84.34;
Q_nom = 3000; %Capacity in Amp-Hours

Q = 0.001; %Measurement Variance
R = diag([100*Q;0.1*Q;0.01*Q]); %Process Covariance 

%%Extended Kalman Filter

x = [100; 0; 0]; %Initial State Vector
Sigma = eye(3); %Initial State Uncertainty
SOCs = zeros(size(data,1),1); 
for i=1:size(data,1)
    
    u = -data.I_mA(i); 
    A = diag([1;exp(-dt(i)/tau_1);exp(-dt(i)/tau_2)]);
    B = [-dt(i)/Q_nom;R_1*(1-exp(-dt(i)/tau_1));R_2*(1-exp(-dt(i)/tau_2))];
    x = A*x + B*u; %Prediction
    Sigma = A*Sigma*A.' + R; 
    C = [(3*OCV_SOC.p1*x(1)^2 + 2*OCV_SOC.p2*x(1) + OCV_SOC.p3), ... 
        -1,-1];%Jacobian
    K = Sigma*C.'*inv(C*Sigma*C.'+ Q); %Kalman Gain 
    V_predict = (OCV_SOC.p1*x(1)^3 + OCV_SOC.p2*x(1)^2 ...
        + OCV_SOC.p3*x(1) + OCV_SOC.p4) - x(2) - x(3) - (R_0*data.I_mA(i)/1000); 
    x = x + K*(data.Ecell_V(i) - V_predict); %Update
    SOCs(i) = x(1); 
    Sigma = (eye(3) - K*C)*Sigma; %Updated Uncertianty

end 


