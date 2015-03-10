close all;
clear all;

%% Parameters and functions
% Parameters
filePath = '../../video/tennis_ball2-cv1.dlm';
sampleRate = 30; %[FPS]
cameraWidth = 640; %[px]
cameraHeight = 480; %[px]
deltaT = 1/sampleRate; % [sec]
predict = 1;

% intrinsic camera matrix (i-sight)
K_isight = [7.5988631728213807e+002 0. 3.1950000000000000e+002;
    0. 7.5988631728213807e+002 2.3950000000000000e+002
    0. 0. 1.];
M_isight = inv(K_isight);
% instrinsic camera matrix (logitec hd)
K_logitech = [6.8565699875178950e+002 0. 3.1950000000000000e+002
    0. 6.8565699875178950e+002 2.3950000000000000e+002
    0. 0. 1.];
M_logitech = inv(K_logitech);

ftin_to_m = @(ft,in) (ft + in/12)*0.3048;


%% Build ball model with foreshortening function

% Measured foreshortening of painted green tennis ball
% with iSight camera.
% Column 1 is distance in meters
% Column 2 is apparent radius in pixels
ballForeshortening = [ftin_to_m(1,0) 79;
    ftin_to_m(1,4) 60;
    ftin_to_m(2,2) 40;
    ftin_to_m(2,9) 32;
    ftin_to_m(3,9) 23;
    ftin_to_m(5,1) 17;
    ftin_to_m(6,7) 13;
    ftin_to_m(8,3) 10;
    ftin_to_m(10,0) 9;
    ftin_to_m(11,10) 7];
ballActualRadius = 6.54e-2; % [m]

% create models
ball = BallModel();
apparentDiameter = ballForeshortening(:,2);
distance = ballForeshortening(:,1);
foreshorteningModel = fit(apparentDiameter,distance, 'exp2');
                %ballForeshortening(:,2),'exp2');


%% Using real data
% Load data
data = dlmread(filePath, ',', 4,0)
ballRadius = data(:,4);
Xs = data(:,[2 3]) % ball center in pixel coordinates
%Xs_mask = data(:,1); %
[frames, ~] = size(Xs); % number of frames read
time = [0:frames-1].*(1/sampleRate); % sample time
indices = find(Xs(:,1)>1); % finds frame indices where ball is found
pointCount = length(indices);

% Create a 2Dplot of image plane
figure();
subplot(2,2,1);
Xs_hat = Xs(indices,:);
Xs_hat(:,2) = cameraHeight - Xs_hat(:,2);
plot(Xs_hat(:,1),Xs_hat(:,2),'ob-')
xlim([0 cameraWidth]);
ylim([0 cameraHeight]);
xlabel('X^S_x [px]')
ylabel('480 - X^S_y [px]')
title('Detected Ball Trajectory in Image')

% Plot apparent radius vs time
subplot(2,2,2);
time_hat = time(indices);
radius_hat = ballRadius(indices);
plot(time_hat,radius_hat)
title('Apparent Ball Radius vs. Time')
xlabel('Time [sec]')
ylabel('Radius [px]')

% Estimate distance vs Xs_y
subplot(2,2,3)
d_est = foreshorteningModel(radius_hat);
plot(d_est,Xs_hat(:,2))
xlabel('Distance [m]');
ylabel('X^S_y [px]')
title('X^S_y vs. Estimated Distance');

subplot(2,2,4)
plot(time_hat,d_est)
xlabel('Time [sec]')
ylabel('Distance [m]');
title('Estimated Distance vs. Time');

%% Convert Xs to Pc and run the model
Xs_bar = [Xs_hat ones(pointCount,1)];
Xc=M_logitech*Xs_bar';
Xc_norm = sqrt(sum(Xc.^2,1));
alpha = d_est'./Xc_norm;
Pc=zeros(3,pointCount);
time = 0:deltaT:(deltaT*(pointCount - 1));
Vc = zeros(3,1:pointCount); % [m/s^2] velocity in camera frame
Vb = zeros(3,1:pointCount); %  [m/s^2] velocity in body frame
P_est = cell(1,pointCount);
for i=1:pointCount
    Pc(:,i) = alpha(i)*Xc(:,i);
    ball.updatePosition(Pc(:,i),time(i));
    
    Vc(:, i) = ball.V_C;
    Vb(:, i) = ball.V_B;
    
    % Predict trajectory nSteps ahead
    if i > 5
        P_est{i} = ball.predictTrajectory(12);
    end
end

%% Create plot of model data
figure()
v_cx = Vc(1,:);
v_cy = Vc(2,:);
v_cz = Vc(3,:);
v_bx = Vb(1,:);
v_by = Vb(2,:);
v_bz = Vb(3,:);

subplot(221);
plot(time, v_cx,'b',time,v_bx,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cx', 'v_bx');
subplot(222);
plot(time, v_cy,'r--',time,v_by,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cy', 'v_by');
subplot(223);
plot(time,v_cz,'r--',time,v_bz,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cz', 'v_bz');


%% Create 3dplot with estimated distance mapped as (X,Z,Y)
figure()
plot3(Pc(1,:),Pc(3,:),Pc(2,:),'o-')
hold on;
plot3(Pc(1,1),Pc(3,1),Pc(2,1),'g*')
plot3(Pc(1,end),Pc(3,end),Pc(2,end),'r*')

% Draw velocity vectors on trajectory
q = quiver3(Pc(1,:), Pc(3,:), Pc(2,:), v_cx, v_cz, v_cy)
set(q,'Color','red');

% Draw each predicted trajectory
if (predict)
    for i=1:length(P_est)
        Pnext = P_est{i};
        if ~isempty(Pnext)
            P0= Pc(:,i);
            X = [P0(1) Pnext(1,:)];
            Y = [P0(2) Pnext(2,:)];
            Z = [P0(3) Pnext(3,:)];
            plot3(X, Z, Y,'--r');
        end
    end
end

%  Draw bounding box as a surface (flipped x, z ,y)
f_cam = 0.3; % [m] f, dist from optical center to planes
cs_x = abs(M_logitech(1,3)); 
cs_y = abs(M_logitech(2,3)); 
bbox_width = cs_x;
bbox_height = cs_y;
[xx,yy] = meshgrid([-bbox_width/2 bbox_width/2], [-bbox_height/2 bbox_height/2]);
h_bb = surf(xx,f_cam*ones(size(xx)),yy)
set(h_bb,'FaceColor',[1 0 0],'FaceAlpha',0.1)

% Draw camera box as surface
camScale = 0.25; % [m] scale camera box
Cc_cam00 = camScale*M_logitech*[0 0 1]'; % top left corner in camera coordinates
ccam_width = abs(Cc_cam00(1))*2;
ccam_height = abs(Cc_cam00(2))*2;
[xx, yy] = meshgrid([-ccam_width/2 ccam_width/2], [-ccam_height/2 ccam_height/2])
h_cam = surf(xx,f_cam*ones(size(xx)),yy);
set(h_cam,'FaceColor',[0 1 0],'FaceAlpha',0.7)


legend('Trajectory in Time', 'Start Position', 'End Position',...
    'Velocity Vector', 'Predicted Trajectory','Bounding Box', 'Camera Box')

view(113,8) % az, elev 


