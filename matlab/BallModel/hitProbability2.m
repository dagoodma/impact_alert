% Designining predictor from results for three videos for |Pc| >= 2 [m]
close all;
clc;
clear all;

% tennis_ball2-cv1 (impact):
P_hit2=[0     0     0     0     0     0     0     0     0     0     1     1   0     0];

% tennis_ball4-impact-cv1 (impact):
P_hit4=[0     0     0     0     0     0     0     0     1     0     0     1  0     1     0     1     0     0     1     0];
    
% predictor settings
mu = @(n) n * 3/4; % mean (pick which trajectory predictions are favored)
var = 2.5; % variance (pick spread around mean)

%% tennis_ball2-cv1 (impact):
figure();
n = length(P_hit2);
x = [1:n];
subplot(221);
plot(x, P_hit2,'*');
title('tennis-ball2-cv1 (impact)');
hold on;

norm2 = normpdf(x,mu(n),var);
plot(x,norm2,'g');
disp('Probability of hit2: ');
P_hit2*norm2'

%% tennis_ball3-cv1 (non-impact):
n = length(P_hit3);
x = [1:n];
subplot(222);
plot(x, P_hit3,'*');
title('tennis-ball3-cv1 (non-impact)');
hold on;

norm3 = normpdf(x,mu(n),var);
plot(x,norm3,'g');
disp('Probability of hit3: ');
P_hit3*norm3'

%% tennis_ball4-impact-cv1 (impact):
n = length(P_hit4);
x = [1:n];
subplot(223);
plot(x, P_hit4,'*');
title('tennis-ball4-impact-cv1 (impact)');
hold on;

norm4 = normpdf(x,mu(n),var);
plot(x,norm4,'g');
disp('Probability of hit4: ');
P_hit4*norm4'