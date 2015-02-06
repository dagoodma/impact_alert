function varargout = object_tracking(varargin)
% _____________________________________________________
%Author: Diego Barragán.
% www.matpic.com
% _____________________________________________________
% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @object_tracking_OpeningFcn, ...
    'gui_OutputFcn',  @object_tracking_OutputFcn, ...
    'gui_LayoutFcn',  [] , ...
    'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before object_tracking is made visible.
function object_tracking_OpeningFcn(hObject, eventdata, handles, varargin)
movegui(hObject,'center')
imaqreset
% ID of video source
handles.fuente=2;
%Disable "Start" and "Stop" buttons
set(handles.inicio,'Enable','off');
set(handles.parar,'Enable','off');
set(hObject,'UserData',0)
set(handles.axes1,'XTickLabel',[],'YTickLabel',[])
% Choose default command line output for object_tracking
handles.output = hObject;
% Update handles structure
guidata(hObject, handles);

% --- Outputs from this function are returned to the command line.
function varargout = object_tracking_OutputFcn(hObject, eventdata, handles)
% Get default command line output from handles structure
varargout{1} = handles.output;

% --- FUNCTION TO GET BACKGROUND
function cap_fondo_Callback(hObject, eventdata, handles)
% Reset imaq device
imaqreset
set(hObject,'UserData',0) %User data 0 (1 stop capture)
% Enable "Start" and "Stop" buttons
set(handles.inicio,'Enable','off');
set(handles.parar,'Enable','off');
% Disable current button
set(hObject,'Enable','off');
% Get default source
sel_fuente=handles.fuente;
switch sel_fuente
    % _________________________________________________________________
    case 1 %WEB CAM        
        % Open GUI to select the camera to use
        sel_camera
        %
        uiwait
        % Bring the camera features
        % id= Camera ID
        % es_web_ext= indicator if laptop or external camera
        global id es_web_ext
        % Determine format depending on the type of camera to use
        if es_web_ext==0
            formato='YUY2_176x144';
        else
            formato='RGB24_320x240';
        end
        try
            % Create video object
            %vid = videoinput('winvideo',id,formato);
            vid = videoinput('macvideo',id,'YCbCr422_640x480');
            % Update handles
            guidata(hObject, handles);            
        catch
            % Message on error
            msgbox('Check the connection of the camera','Camera')
            % Remove axis labels
            set(handles.axes1,'XTick',[ ],'YTick',[ ])
        end
        % Specify how often to acquire frame from video stream
        vid.FrameGrabInterval = 1;
        %set(vid,'TriggerRepeat',Inf);
        % Start capture
        % _______Get Background_________
        vid.FramesPerTrigger=50;
        start(vid);
        data = getdata(vid,50);
        if es_web_ext==0
            fondo=double(ycbcr2rgb(data(:,:,:,50)));
        else
            fondo=double(data(:,:,:,50));
        end
        % Set last image as background
        % Show background
        imshow(uint8(fondo))
        % Reset video object
        stop(vid);
        clear vid
        imaqreset
    case 2%VIDEO AVI
        [nombre, ruta]=uigetfile('*.avi','SELECT VIDEO AVI');
        if nombre == 0 %If press cancel button, return
            set(hObject,'Enable','on');
            set(handles.inicio,'Enable','on');
            set(handles.parar,'Enable','on');
            return
        end
        %------------------------------------------
        handles.xyloObj = VideoReader(fullfile(ruta,nombre));
        axes(handles.axes1)
        % Show image
        fondo=double(read(handles.xyloObj,10));
        imagesc(uint8(fondo))
        axis image off
        handles.ruta=ruta;
        handles.nombre=nombre;
end
% Save background
handles.backg=fondo;
guidata(hObject,handles)
% Enable buttons
set(hObject,'Enable','on');
set(handles.inicio,'Enable','on');
set(handles.parar,'Enable','on');

% --- FUNCTION THAT START OBJECT TRACKING
function inicio_Callback(hObject, eventdata, handles)
% Disable button
set(handles.inicio,'Enable','off');
% Get threshold
umbral=str2double(get(handles.umbral,'String'));
% Verify whether threshold is numerical value
if umbral<0 || isnan(umbral) || isempty(umbral)
    errordlg('Input numerical value','ERROR')
    return
end
% User data 0 (1 stop capture)
set(handles.parar,'UserData',0)
% Get video source
sel_fuente=handles.fuente;
%Get background
fondo=handles.backg;
% _____________________________
switch sel_fuente
    case 1 % WEB CAM        
        global id es_web_ext
        
        if es_web_ext==0
            formato='YUY2_176x144';
        else
            formato='RGB24_320x240';
        end
        try
            vid = videoinput('winvideo',id,formato);
            guidata(hObject, handles);
        catch
            msgbox('Check the connection of the camera','Camera')
            set(handles.axes1,'XTick',[ ],'YTick',[ ])
        end
        % Specify how often to acquire frame from video stream
        vid.FrameGrabInterval = 2;
        set(vid,'TriggerRepeat',Inf);
        %Start capture
        %  try
        start(vid);
        while 1
            if get(handles.parar,'UserData') % Data from "Stop" button
                break
            end
            % Get image
            if es_web_ext==0
                imagen = ycbcr2rgb(getdata(vid,1));
            else
                imagen = getdata(vid,1);
            end
            % Show image
            image(imagen)
            % Convert image to double
            im_ent = double(imagen);
            axis image off
            % Call "compare" function
            compare(im_ent,fondo,umbral);
            drawnow
        end
        stop(vid);
        delete(vid)
        clear vid
        imaqreset
        % _________________________________________________________________
    case 2 % VIDEO AVI        
        nFrames = handles.xyloObj.NumberOfFrames;
        for cnt = 1:2:nFrames
            if get(handles.parar,'UserData')% Stop whether "stop" button is pressed
                break
            end
            % Show image
            the_image=read(handles.xyloObj,cnt);
            image(the_image);
            axis image off
            im_ent=double(the_image);
            % Call "compare" function
            compare(im_ent,fondo,umbral);%
            drawnow;
        end        
end
% Enable "Start"  button
set(handles.inicio,'Enable','on');
% User data 0 (1 stop capture)
set(handles.parar,'UserData',0)
guidata(hObject, handles);

% --- STOP BUTTON ---.
function parar_Callback(hObject, eventdata, handles)
set(hObject,'userdata',1)
guidata(hObject, handles)

% --- SELECTION OF VIDEO SOURCE
function uipanel1_SelectionChangeFcn(hObject, eventdata, handles)
if hObject==handles.video_op %VIDEO AVI
    handles.fuente=2;
else
    handles.fuente=1;    % WEBCAM
end
guidata(hObject,handles)

function umbral_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function umbral_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
