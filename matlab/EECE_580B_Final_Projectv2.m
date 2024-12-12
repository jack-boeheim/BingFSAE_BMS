clc 
clear
close all 

%% Setup Model Parameters
%Constants 
Q_nom = 2.9;
%SOC/OCV
SOC = [1; 0.95; 0.9; 0.8; 0.7; 0.6; 0.5; 0.4; 0.3; 0.25; 0.2; 0.15; 0.1; 0.05];
OCV = [4.17497; 4.1042; 4.05852; 3.94657; 3.86229; 3.76835; 3.66348; 3.60236; 3.55024; 3.51292; 3.45824; 3.39068; 3.34436; 3.23691];


OCV_SOC = fit(SOC,OCV,'poly3'); %Fit Estimated SOC/OCV relation to 3rd Order Poly
SOCs = 0:0.01:1;
OCV_SOC_fit = OCV_SOC.p1*SOCs.^3 + OCV_SOC.p2*SOCs.^2 ...
        + OCV_SOC.p3.*SOCs + OCV_SOC.p4;

%Plotting Fitted SOC/OCV curve from estimated relation
figure;
plot(SOCs*100,OCV_SOC_fit); hold on; plot(SOC*100,OCV,'o');
xlabel('SOC (%)');
ylabel("Estimated Open Circuit Voltage (V)");
legend('Fitted Curve','Estimated Values');

% R_0,R_1,C_1,R_2,C_2 (90% -10%)

R_0s = [.0247;.021;.0196;.0192;.0353];
R_1s = [.0107;.0122;.0114;.0133;.0483];
C_1s = [42.1654;25.2452;15.313;10.2366;18.6077];
R_2s = [.0395;.0476;.0313;.0317;.0546];
C_2s = [1155.6742;996.0583;1500.2786;1431.5031;751.7144];
tau_1s = R_1s.*C_1s;
tau_2s = R_2s.*C_2s;

%% Measurement and Noise Covariances

cov_ecm_params = zeros(5,5,5); %3D Array for Cov Matrix of Model Params at each SOC interval

cov_ecm_params(:,:,1) = diag([0;0;268.5;0;1050.2]);

cov_ecm_params(:,:,2) = diag([.0306;.0305;151.6161;.0336;706.9973]);

cov_ecm_params(:,:,3) = diag([.0306;.0305;151.6161;.03336;705.9973]);
 
cov_ecm_params (:,:,4) = diag([0;0;107.5;0;1477.3]);

cov_ecm_params(:,:,5) = diag([0;0;63;.1;1312.1]);

cov_ecm_params(:,:,6) = diag([.0189;.0185;17.15125;.0240;431.6995]);

%% Parameter Estimation via EKF 

load('HPPC_pulse_data_30.mat');
mVoltage = data.Voltage; mCurrent = data.Current; t = data.Time;
SOCs_Kalman_Dischage = kalmanSOC1(OCV_SOC,R_0s,R_1s,tau_1s,R_2s,tau_2s,Q_nom,mVoltage,mCurrent,t); 
figure;
plot(data.Time - data.Time(1), SOCs_Kalman_Dischage*100,'LineWidth',3);
SOC_0 = .3;
SOCs_CC = 100*(SOC_0 + cumtrapz(data.Time,data.Current)/(Q_nom*3600));
hold on
plot(data.Time - data.Time(1),SOCs_CC,'LineWidth',3);
legend('Kalman Filter','True SOC');
xlabel('Time (s)');
ylabel('State of Charge (%)');


load('03-18-17_02.17 25degC_Cycle_1_Pan18650PF.mat');
mVoltage = meas.Voltage(1:1e5,1); mCurrent = meas.Current(1:1e5,1); t = meas.Time(1:1e5,1);
SOCs_Kalman_C1 = kalmanSOC1(OCV_SOC,R_0s,R_1s,tau_1s,R_2s,tau_2s,Q_nom,mVoltage,mCurrent,t); 

figure;
plot(t - t(1), SOCs_Kalman_C1*100,'LineWidth',3);
SOC_0 = 1;
SOCs_CC = 100*(SOC_0 + cumtrapz(t,mCurrent)/(Q_nom*3600));
hold on
plot(t -t(1),SOCs_CC,'LineWidth',3);
legend('Kalman Filter','True SOC');
xlabel('Time (s)');
ylabel('State of Charge (%)');
title("Variable Discharge");

load('07-22-17_22.44 4020_Dis1C_1.mat');
mVoltage = meas.Voltage; mCurrent = meas.Current; t = meas.Time;
SOCs_Kalman_C2 = kalmanSOC1(OCV_SOC,R_0s,R_1s,tau_1s,R_2s,tau_2s,Q_nom,mVoltage,mCurrent,t); 
figure;
plot(t - t(1), SOCs_Kalman_C2*100,'LineWidth',3);
SOC_0 = 0.85;
SOCs_CC = 100*(SOC_0 + cumtrapz(t,mCurrent)/(Q_nom*3600));
hold on
plot(t -t(1),SOCs_CC);
legend('Kalman Filter','True SOC');
xlabel('Time (s)');
ylabel('State of Charge (%)');
title('Constant Discharge 1C');



load('03-21-17_06.44 3416_Charge3.mat');
mVoltage = meas.Voltage; mCurrent = meas.Current; t = meas.Time;
SOCs_Kalman_C2 = kalmanSOC1(OCV_SOC,R_0s,R_1s,tau_1s,R_2s,tau_2s,Q_nom,mVoltage,mCurrent,t); 
figure;
plot(t - t(1), SOCs_Kalman_C2*100,'LineWidth',3);
SOC_0 = 0.1;
SOCs_CC = 100*(SOC_0 + cumtrapz(t,mCurrent)/(Q_nom*3600));
hold on
plot(t -t(1),SOCs_CC,'LineWidth',3);
legend('Kalman Filter','True SOC');
xlabel('Time (s)');
ylabel('State of Charge (%)');
title('Charging 1C');






