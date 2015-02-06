% Read video
xyloObj = VideoReader('v2.avi');
get(xyloObj)
nFrames = xyloObj.NumberOfFrames;
vidHeight = xyloObj.Height;
vidWidth = xyloObj.Width;
mov(1:nFrames) = ...
    struct('cdata',zeros(vidHeight,vidWidth, 3,'uint8'),'colormap',[]);
for k = 1 : nFrames
    mov(k).cdata = read(xyloObj,k);
end
hf = figure;
set(hf, 'position', [150 150 vidWidth vidHeight])
movie(hf, mov, 1, xyloObj.FrameRate);