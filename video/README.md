# Impact Detection Videos

## First soccer ball movies

The first movie of the soccer ball being throw past the screen were recorded with John Ash's Logitech webcam. There are 4 movies with various similar trajectories and velocities. These were recorded with the names: soccer\_ball#\_orig.mov, and have the following specs:

    Frame rates: 1=29.14fps, 2=29.70fps, 3=28.32fps, 4=29.89fps
    Dimensions: 640x480
    Encoder: H.264
    Bit depth: 24
    X/Y Resolution: 72
    Avg. bitrate: 1=823kbps, 2=853kbps, 3=761kbps, 4=1.42Mbps

The audio was stripped out. The videos were converted to .avi files with a fixed frame rate (as specified above), and to .avi files with the same quality (ending with -sameq.avi). 

The 4th movie, which has the highest frame rate and bitrate, was ran through an optical flow (Horn-Schunck) simulation in MATLAB Simulink. The output was saved as soccer_ball4_thresh.avi and soccer_ball4_vector.avi. 

### Velocity threshold video

This video shows the result of squaring the magnitude of the velocity, using the mean velocity per frame across time as a threshold, and applying a median filter with a neighborhood of 3x3 to the result. This clearly shows the ball passing as a white blob across a black background. The lights can be seen flickering overhead. Notice how the soccerball appears so have a trail behind some parts of it. This could be due to the white patches on the ball being the same color as the wall in the background.

### Velocity vector video

This video shows the velocity vectors of the pixels drawn onto the original video with a motion vector gain of 20. This video also shows that the black and colored areas of the ball are being detected, while the white areas leave a trail of velocity vectors behind them.


