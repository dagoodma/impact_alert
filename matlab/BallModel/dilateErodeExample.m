clear all;
%close all;

erodeRadius = 8;
dilateRadius = 4;


BW1 = imread('circbw.tif');
BW2_nodilate = imerode(BW1,strel('ball',erodeRadius,0,0));
BW2_noerode = imerode(BW1,strel('ball',erodeRadius,0,0));
BW2_erodedilate = imdilate(BW2_nodilate,strel('ball',dilateRadius,0,0));
BW2_dilateerode = imdilate(BW2_noerode,strel('ball',dilateRadius,0,0));

figure(); 
subplot(321); 
imshow(BW1);
title('Original');

subplot(323); 
imshow(BW2_nodilate);
title(sprintf('Only Eroded by r=%d',erodeRadius));

subplot(324);
imshow(BW2_erodedilate);
title(sprintf('Eroded  then by r=%d, dilated by r=%d',erodeRadius,dilateRadius));

subplot(325); 
imshow(BW2_noerode);
title(sprintf('Only Dilated by r=%d',dilateRadius));

subplot(326);
imshow(BW2_dilateerode);
title(sprintf('Dilated  then by r=%d, eroded by r=%d',dilateRadius,erodeRadius));

