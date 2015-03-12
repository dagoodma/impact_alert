close all;
clear all;

%% Parameters and functions
% Parameters
%filePath = '../../video/tennis_ball2-cv1.dlm';
%filePath = '../../video/tennis_ball3-cv1.dlm';
filePath = '../../video/tennis_ball4-impact-cv1.dlm';
sampleRate = 30; %[FPS]
Lx = 640; %[px]
Ly = 480; %[px]

% Settings
doFinalPrediction = 1;

% Prediction parameters
predict = 1;
predictAll = 1;
predictSteps = 200;
predictStartStep = 4;

predictTTC = 0.15; % [sec] minimum TTC to decide
predictMu = @(n) n * 3/4; % mean of trajectory hits in time
predictVar = 2.3; % variance of trajectory hits in time
predictThreshold = 0.35; % 40 percent or greater is a hit


% Bounding box and plane parameters
f_cam = 0.05; % [m] f, dist from optical center to planes
camScale = 0.25; % [m] scale camera box
bboxScale = 1.15; % [m] scale bounding box

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

% Bounding box
cs_x = abs(M_logitech(1,3)); 
cs_y = abs(M_logitech(2,3)); 
bbox_width = cs_x;
bbox_height = cs_y;

% Get filename
[dir filename ext] = fileparts(filePath);
%filename = sprintf('%s%s',filename,ext); %concat extension to filename

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


%% Using real data
% Load data
data = load(filePath) %dlmread(filePath, ',', 4,0)
ballRadius = data(:,4);
Xs = data(:,[2 3]) % ball center in pixel coordinates
[frames, ~] = size(Xs); % number of frames read
time = [0:frames-1].*(1/sampleRate); % sample time
indices = find(data(:,1)>0); % finds frame indices where ball is found
pointCount = length(indices);

% Use only data where the ball was detected
time_hat = time(indices);
radius_hat = ballRadius(indices);

% Create a 2Dplot of image plane
figure();
subplot(2,2,1);
Xs_hat = Xs(indices,:);
%Xs_hat(:,2) = Ly - Xs_hat(:,2);
plot(Xs_hat(:,1),Ly - Xs_hat(:,2),'ob-')
xlim([0 Lx]);
ylim([0 Ly]);
xlabel('X^S_x [px]')
ylabel('480 - X^S_y [px]')
title('Detected Ball Trajectory in Image')

% Plot apparent radius vs time
subplot(2,2,2);
plot(time_hat,radius_hat)
title('Apparent Ball Radius vs. Time')
xlabel('Time [sec]')
ylabel('Radius [px]')

% Estimate distance vs Xs_y
subplot(2,2,3)
d_est = foreshorteningModel(radius_hat);
plot(d_est,Ly-Xs_hat(:,2))
xlabel('Distance [m]');
ylabel('-X^S_y [px]')
title('-X^S_y vs. Estimated Distance');

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
Vc = zeros(3,1:pointCount); % [m/s^2] velocity in camera frame
Vb = zeros(3,1:pointCount); %  [m/s^2] velocity in body frame
P_est = cell(1,pointCount);
P_hit = zeros(1,pointCount);
t_0 = 0; 
t_1 = 0;
for i=1:pointCount
    t_1 = time_hat(i);
    deltaT = t_1 - t_0;
    Pc(:,i) = alpha(i)*Xc(:,i);
    ball.updatePosition(Pc(:,i),deltaT);
    
    Vc(:, i) = ball.V_C;
    Vb(:, i) = ball.V_B;
    
    % Predict trajectory n ahead
    P_traj = zeros(3,predictSteps);
    if i > predictStartStep
        P_traj = ball.predictTrajectory(predictSteps);
    
    
        % Prune trajectory points that are past the focal plane (Z <= 0)
        j_end = 0;
        for j=1:length(P_traj)
            if (P_traj(3,j) < 0)
                j_end = j - 1;
                break;
            end
        end
        
        if (j_end > 0)
            P_traj = P_traj(:,1:j_end);

            % Check for a hit
            P_hit(i) = detectImpact(P_traj, bboxScale*bbox_width,bboxScale* bbox_height);

            % Save into cell
            P_est{i} = P_traj;
        end
    end
    t_0 = t_1;
end
P_hit

%% Create velocity plots
figure()
v_cx = Vc(1,:);
v_cy = Vc(2,:);
v_cz = Vc(3,:);
v_bx = Vb(1,:);
v_by = Vb(2,:);
v_bz = Vb(3,:);

subplot(221);
plot(time_hat, v_cx,'b',time_hat,v_bx,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cx', 'v_bx');
subplot(222);
plot(time_hat, v_cy,'r--',time_hat,v_by,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cy', 'v_by');
subplot(223);
plot(time_hat,v_cz,'r--',time_hat,v_bz,'r');
xlabel('Time [sec]');
ylabel('Ball Velocity [m/s^2]');
legend('v_cz', 'v_bz');
subplot(224);
plot(time_hat,sqrt(sum(Vc,1).^2));
xlabel('Time [sec]');
ylabel('|V^C| [m/s^2]');


%% Create 3dplot with estimated distance mapped as (X,Z,Y)
figure()
plot3(Pc(1,:),Pc(3,:),-Pc(2,:),'o-')
hold on;
plot3(Pc(1,1),Pc(3,1),-Pc(2,1),'g*')
plot3(Pc(1,end),Pc(3,end),-Pc(2,end),'r*')

% Draw velocity vectors on trajectory
q = quiver3(Pc(1,:), Pc(3,:), -Pc(2,:), v_cx, v_cz, -v_cy);
set(q,'Color','red');

%% Draw camera bounding boxes
%  Draw bounding box as a surface (flipped x, z ,y)
[xx,yy] = meshgrid([-bbox_width/2 bbox_width/2], [-bbox_height/2 bbox_height/2]);
h_bb = surf(xx,f_cam*ones(size(xx)),yy);
set(h_bb,'FaceColor',[1 0 0],'FaceAlpha',0.1)

% Draw camera box as surface
Cc_cam00 = camScale*M_logitech*[0 0 1]'; % top left corner in camera coordinates
ccam_width = abs(Cc_cam00(1))*2;
ccam_height = abs(Cc_cam00(2))*2;
[xx, yy] = meshgrid([-ccam_width/2 ccam_width/2], [-ccam_height/2 ccam_height/2]);
h_cam = surf(xx,f_cam*ones(size(xx)),yy);
set(h_cam,'FaceColor',[0 1 0],'FaceAlpha',0.7)

%% Draw each predicted trajectory
if (predict)
    for i=1:length(P_est)
        if (~predictAll && mod(i,2) > 0) % skip odds
            continue;
        end
            
        Pnext = P_est{i};
        if ~isempty(Pnext)
            P0= Pc(:,i);
            X = [P0(1) Pnext(1,:)];
            Y = [P0(2) Pnext(2,:)];
            Z = [P0(3) Pnext(3,:)];
            % Draw trajectory as black or red if it hits
            col = 'k';
            if (P_hit(i))
                col = 'r';
            end
            plot3(X, Z, -Y,sprintf('--%c',col));
        end
    end
end

% Set legend and view
legend('Trajectory in Time', 'Start Position', 'End Position',...
    'Velocity Vector','Bounding Box', 'Camera Box', 'Predicted Trajectory')
th = title(sprintf('3D Trajectory Model of ''%s''',filename));
set(th,'interpreter','none')
set(gca,'xgrid','on')
box on;

view(113,8) % az, elev 

% Get window limits
xl = xlim();
yl = ylim();
zl = zlim();

% Set window limits
yl_min = 0;
yl_max = Pc(3,1) + 0.4;
% Don't set xlim
% xl_min
% xl_max
zl_min = -Pc(2,1);
zl_max = 0.4;

ylim([yl_min yl_max]);
zlim([zl_min zl_max]);


%% Find TTC
figure();
TTC = d_est'.*(1./sqrt(sum(Vc,1).^2));
subplot(221);
plot(d_est, TTC);
xlabel('Estimated Distance [m]');
ylabel('TTC [sec]');
ylim([0 4]);

subplot(222)
n = length(P_hit);
x = [1:n];
plot(TTC, P_hit,'*');
xlabel('TTC [sec]');
ylabel('Hit detected');

subplot(223)
plot(x, P_hit,'*');
th = title(filename);
set(th,'interpreter','none');
ylabel('Hit detected');

subplot(224)
plot(d_est, P_hit,'*');
xlabel('Estimated Distance [m]');
ylabel('Hit detected');

%% Draw trajectories back on image plane in movie sequence
if doFinalPrediction
    impactPredicted = 0;
    impactProbability = 0;
    impactTTC = 0;
    
    figure();
    for i=1:length(time_hat)
        plot(Xs_hat(i,1), Ly-Xs_hat(i,2),'bo'); % plot the ball
        distance = sqrt(sum(Pc(:,i).^2,1)); % find distance
        ttc = distance/sqrt(sum(Vc(:,i)).^2);
        
        % Plot trajectory
        Pc_traj = P_est{i};
        if (~isempty(Pc_traj))
            hold on;
            Xs_traj = K_logitech*Pc_traj;
            Xs_traj = Xs_traj*diag(1./Xs_traj(3,:)); % homogeneous normalization
            Xs_traj = Xs_traj(1:2,:); % remove third row 
            %Xs_traj = [Xs_traj(1,:); Ly - Xs_traj(2,:)]; % invert y for plotting
            plot(Xs_traj(1,:),Ly-Xs_traj(2,:),'r');
            hold off;
        end
        xlim([0 Lx]);
        ylim([0 Ly]);
        th = title(sprintf('Trajectory Predictions in ''%s''',filename));
        set(th,'interpreter','none');
        
        % Make hit prediction if we're under TTC threshold
        if (ttc <= predictTTC)
            figure();
            n = i;
            x=1:n;
            stem(time_hat(x),P_hit(1:n),'*:');
            hold on;
            norm = normpdf(x,predictMu(n),predictVar);
            plot(time_hat(x),norm,'g');
            xlabel('Time [sec]');
            ylabel('Hit Predicted');
            set(gca,'YTickMode', 'manual');
            set(gca,'YTick', [0 1]);
            set(gca,'YTickLabel', {'no', 'yes'});
            
            title('Hit Predictions Over Time with Weighting Function')
            legend('Hit Prediction', 'Weighting Function');
            disp('Probability of hit2: ');
            impactProbability = P_hit(1:n)*norm';
            impactTTC = ttc;
            
            if (impactProbability >= predictThreshold)
                impactPredicted = 1;
            end
            
            break
        else
            pause(0.1)
        end
    end
    
    if ~impactPredicted
        predictionStr = sprintf('No impact detected! (TTC = %.2f, P = %.2f)\n', impactTTC, impactProbability);
        fprintf(predictionStr);
        text(0.1,0.6,predictionStr,'Color','g','FontSize',14);
    else
        predictionStr = sprintf('Impact is detected! (TTC = %.2f, P = %.2f)\n', impactTTC, impactProbability);
        fprintf(predictionStr);
        text(0.1,0.6,predictionStr,'Color','r','FontSize',14);
    end
end