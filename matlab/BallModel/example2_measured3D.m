close all;
clear all;

% Parameters
filePath = '../../video/tennis_ball2-cv1.dlm';
sampleRate = 30; %[Hz/FPS]
cameraWidth = 640; %[px]
cameraHeight = 480; %[px]

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
% Measured foreshortening of painted green tennis ball
% with iSight camera.
% Column 1 is distance in meters
% Column 2 is apparent diameter in pixels
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


%% Build model of foreshortening

% Plot foreshortening
subplot(1,2,1)
dist = ballForeshortening(:,1);
diam = ballForeshortening(:,2);
plot(dist,diam)
xlabel('Distance [m]');
ylabel('Apparent Diameter [px]')
title('Measured Foreshortening');
%y = 95*exp(-x/0.8)+7;

% Plot fit of diameter to distance
subplot(1,2,2)
f = fit(diam,dist,'exp1');
f2 = fit(diam,dist,'exp2');
plot(diam,f(diam),'-.r');
hold on;
plot(diam,f2(diam),'--g');
scatter(diam, dist);
xlabel('Apparent Diameter [px]')
ylabel('Distance [m]')
legend('1st Order','2nd Order','Measured');
title('Foreshortening Exponential Model');

disp('Fit exponential function to foreshortening:')
f

f2

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

% Plot detected radius vs time
subplot(2,2,2);
time_hat = time(indices);
radius_hat = ballRadius(indices);
plot(time_hat,radius_hat)
title('Apparent Ball Radius vs. Time')
xlabel('Time [sec]')
ylabel('Radius [px]')

% Estimate distance vs Xs_y
subplot(2,2,3)
d = f2(radius_hat)
plot(d,Xs_hat(:,2))
xlabel('Distance [m]');
ylabel('X^S_y [px]')
title('X^S_y vs. Estimated Distance');

subplot(2,2,4)
plot(time_hat,d)
xlabel('Time [sec]')
ylabel('Distance [m]');
title('Estimated Distance vs. Time');

% Reconstruct Pc
Xs_bar = [Xs_hat ones(pointCount,1)];
Xc=M_logitech*Xs_bar'
Xc_norm = sqrt(sum(Xc.^2,1));
alpha = d'./Xc_norm;
Pc=zeros(3,pointCount);
for i=1:pointCount
    Pc(:,i) = alpha(i)*Xc(:,i);
end
Pc

% % % Create 3dplot with estimated distance
figure()
plot3(Pc(1,:),Pc(3,:),Pc(2,:),'o-')
hold on;
plot3(Pc(1,1),Pc(3,1),Pc(2,1),'g*')
plot3(Pc(1,end),Pc(3,end),Pc(2,end),'r*')

%  Draw bounding box as a surface
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


legend('Trajectory in Time', 'Start Position', 'End Position', 'Bounding Box', 'Camera Box')

view(113,8) % az, elev 


