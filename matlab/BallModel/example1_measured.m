close all;
clear all;

filePath = '../../video/tennis_ball2-cv1.dlm';
sampleRate = 30; %[Hz/FPS]
cameraWidth = 640; %[px]
cameraHeight = 480; %[px]

% Load data
data = dlmread(filePath, ',', 4,0)
ballRadius = data(:,4);
Xs = data(:,[2 3]) % ball center in pixel coordinates
%Xs_mask = data(:,1); %
[frames, ~] = size(Xs); % number of frames read
time = [0:frames-1].*(1/sampleRate); % sample time
indices = find(Xs(:,1)>1); % finds frame indices where ball is found

% Create a 2Dplot of image plane
figure();
subplot(3,1,1);
Xs_hat = Xs(indices,:);
Xs_hat(:,2) = cameraHeight - Xs_hat(:,2);
plot(Xs_hat(:,1),Xs_hat(:,2),'ob-')
xlim([0 cameraWidth]);
ylim([0 cameraHeight]);
xlabel('Xs_x [px]')
ylabel('480px - Xs_y [px]')

% Plot detected radius vs time
subplot(3,1,2);
plot(time(indices),ballRadius(indices))
xlabel('Time [sec]')
ylabel('Ball Radius [px]')

subplot(3,1,3)
%plot(time(indices),25.*exp(time(indices)/0.5)-25)
%  -1  10  11  11  13  13  13  15  16  17  18  20  23  26  30  35  46  55  66  -1  -1
distance = [-1 6.5 6.3 6.1 5.7 5.4 5.1 4.5 4.0 3.0 2.6 2.2 2.0 1.8 1.6 1.4 1.0 0.6 0.35 -1 -1]*0.305;
plot(ballRadius(indices), distance(indices))
hold on
plot(ballRadius(indices), 3*exp(-ballRadius(indices)/20))

% % Create 2dplot against time
% figure()
% plot3(time(indices), Xs_hat(:,1), Xs_hat(:,2))
% [x, y, z] = sphere(30)
% hold on;
% for i_hat=1:length(indices)
%     i = indices(i_hat);
%     mesh(x+time(i),y+Xs_hat(i,1),z+Xs_hat(i,2));
% end
% hold off;
% xlabel('Xs_x')
% ylabel('Xs_y')
% zlabel('Time [sec]')
