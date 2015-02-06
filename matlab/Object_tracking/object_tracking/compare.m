function [indicador]=compare(input_image,background,threshold)
indicador = 0;
% 
diference = (abs(input_image(:,:,1)-background(:,:,1)) > threshold) | (abs(input_image(:,:,2) - background(:,:,2)) > threshold) ...
                    | (abs(input_image(:,:,3) - background(:,:,3)) > threshold);             
% Remove noise (eliminating little holes and refill openings)
% a = bwlabel(diference,8);
% Performs morphological closing (dilation followed by erosion).
b = bwmorph(diference,'close');
% Performs morphological opening (erosion followed by dilation).
diference = bwmorph(b,'open');        
diference = bwmorph(diference,'erode',2); 
% Seelct the biggest object
etiqueta = bwlabel(diference,8);
% Measure properties of image regions such as: 'Area', 'Centroid', and 'Box'
objeto = regionprops(etiqueta);
N = size(objeto,1); % Number of objects in in the image.
if N < 1||isempty(objeto) % Return whether no object in the image
    return
end
% Remove holes less than 200 pixels
s=find([objeto.Area]<200);
if ~isempty(s)
    objeto(s)=[ ];
end
N=size(objeto,1);% Count objects
if N < 1 || isempty(objeto)
    return
end
% Draw a rectangle and center point for every object in the image
for n=1:N
    hold on
    centroid = objeto(n).Centroid;
    C_X = centroid(1);
    C_Y = centroid(2);
    rectangle('Position',objeto(n).BoundingBox,'EdgeColor','g','LineWidth',2)    
    plot(C_X,C_Y,'Color','g','Marker','+','LineWidth',2)
    title(['X= ',num2str(round(C_X)),' Y= ',num2str(round(C_X))])
    hold off
end
indicador = 1;
return