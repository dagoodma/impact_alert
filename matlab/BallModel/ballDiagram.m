clear all;
close all;
clc;

cc_x = 0.46598;
cc_y = 0.3493;
f_cam = 1;

hFig = figure()
plot3(0,0,0,'ko','MarkerFaceColor','k'); % optical center
hold on;
color = get(hFig,'Color');
set(gca,'XColor',[1 1 1],'YColor',[1 1 1],'TickDir','out')

% Camera frame
x = [0 0.5];
y = [0 0.7];
z = [0 -0.5];
ze = [0 0];
quiver3(ze,ze,ze, x, ze, ze, '-k');
quiver3(ze,ze,ze, ze, y, ze, '-k');
quiver3(ze,ze,ze, ze, ze, z, '-k');
text(x(2)/2,0.04,0.05,'X_C');
text(0.04,y(2)/2,0.04,'Z_C');
text(0.05,0,z(2)/1.5,'Y_C');
text(-0.7,0,-0.4,'Camera Frame');

% focal plane
ccam_width = abs(cc_x)*2;
ccam_height = abs(cc_y)*2;
[xx, yy] = meshgrid([-ccam_width/2 ccam_width/2], [-ccam_height/2 ccam_height/2]);
h_cam = surf(xx,f_cam*ones(size(xx)),yy);
set(h_cam,'FaceColor',[0.2 0.2 0.2],'FaceAlpha',0.1)

% ball
Pc = [-0.5,4,0.3];
plot3(Pc(1),Pc(2),Pc(3),'ko','MarkerSize',12);
plot3(Pc(1),Pc(2),Pc(3),'bo','MarkerSize',4,'MarkerFaceColor','k');
quiver3(ze, ze, ze, 1.1*[0 Pc(1)], 1.1*[0 Pc(2)], 1.1*[0 Pc(3)]);
text(Pc(1)-.3, Pc(2), Pc(3)+.2, 'P^C');
plot3([Pc(1)-.2 Pc(1)-0.1], [Pc(2) Pc(2)], [Pc(3)+.16 Pc(3)+0.05],'-k') % line from P_c to ball

% body frame of ball
i = 0.5; % ] + [Pc(1) Pc(1)];
j = -1; %  + [Pc(2) Pc(2)];
k = 0.4; %  + [Pc(3) Pc(3)];
quiver3(Pc(1),Pc(2),Pc(3), i, 0, 0, '-k');
quiver3(Pc(1),Pc(2),Pc(3), 0, j, 0, '-k');
quiver3(Pc(1),Pc(2),Pc(3), 0, 0, k, '-k');
text(Pc(1)-0.1,Pc(2)-0.5,Pc(3),'k');
text(Pc(1)+0.06,Pc(2),Pc(3)+0.3,'J');
text(Pc(1)+0.25,Pc(2)+0.25,Pc(3),'i');
text(Pc(1)+0.35,Pc(2)+0.5,Pc(3)+0.2,'Body Frame');

% camera coordinates of ball
plot3(-0.13,f_cam,0.074,'ko','MarkerSize',4,'MarkerFaceColor','k');
quiver3(0,0,0,-0.13,f_cam,0.077,'-k');
text(-0.32,f_cam,0.05,'X^S')
plot3([-0.24 -0.17],[f_cam f_cam],[0.03 0.07],'-k');

% limits
xlim([-1.5 1.5])
ylim([-0.5 4.7]);
zlim([-0.7 1]);

            
set(gca,'YTickMode', 'manual');
set(gca,'YTick', []);
set(gca,'YTickLabel', {}); 
set(gca,'XTickMode', 'manual');
set(gca,'XTick', []);
set(gca,'XTickLabel', {}); 
set(gca,'ZTickMode', 'manual');
set(gca,'ZTick', []);
set(gca,'ZTickLabel', {});


view(6,34) % az, elev 