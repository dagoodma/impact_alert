function [ result ] = detectImpact( Pc_traj, bboxWidth, bboxHeight )
%DETECTIMPACT Returns true if the last point in Pc_traj is within
%   the bounding box.
result = 0;
xmin = -bboxWidth/2;
xmax = bboxWidth/2;
ymin = -bboxHeight/2;
ymax = bboxHeight/2;

x = Pc_traj(1,end);
y = Pc_traj(2,end);

if ((x >= xmin && x <= xmax)...
    && (y >= ymin && y <= ymax))
    result = 1;
end

end % function

