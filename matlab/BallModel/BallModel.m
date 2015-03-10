classdef BallModel < handle
    %BALLMODEL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        % Variables
        V_B % [m/s^2] 3x1, velocity vector (i,j,k) in body frame
        %V_Blast % [m/s^2] 3x1, velocity vector (i,j,k) in body frame
        V_C % [m/s^2] 3x1, velocity vector (x,y,z) in camera frame
        P_C % [m] 3x1, current position in camera frame
        P_C0 % [m] 3x1, initial position in camera frame
        P_Clast % [m] 3x1, last position in camera frame
        updates; % number of position updates
        update_time % [sec] time difference between P_C and P_Clast
        % Constants
        C_d = 0.5; % Coefficient of drag
        rho = 1.21 % [kg/m^3] Density of fluid (air at ~25degC)
        S = 0.0034 % [m^2] cross-sectional area of ball
        m = 0.055 % [kg] mass of ball
        g = 9.8; % [m/s^2] standard gravity of Earth
        f_s = 500; % [Hz] sample rate for predictions
    end
    
    methods
        % Constructor initializes variables
        function ball = BallModel()
            ball.V_B = zeros(3,1);
            ball.V_C = zeros(3,1);
            ball.P_C = zeros(3,1);
            ball.P_C0 = zeros(3,1);
            ball.P_Clast = zeros(3,1);
            ball.updates = 0;
        end
        
        % Updates the ball's position in camera coordinates over time delta_t
        function updatePosition(obj, position, delta_t)
            obj.updates = obj.updates + 1;
            obj.P_Clast = obj.P_C;
            % Sets initial position
            if (obj.updates == 1)
                obj.P_C0 = position;
            end
            obj.P_C = position;
            
            % Update velocity if we have at least two position updates
            if (obj.hasState())
                obj.updateVelocity(delta_t);
            end
        end
        
        % Calculates the ball's velocity in the body and camera frames
        function updateVelocity(obj, delta_t)
            if (delta_t > 0)
                obj.V_C = (obj.P_C - obj.P_Clast)*1/delta_t;
                obj.V_B = [1 0 0; 0 -1 0; 0 0 -1] * obj.V_C;
                obj.update_time = delta_t;
            end
        end
        
        
        % If the ball's position has been updated twice, all state
        % variables are set, and the model is ready for predictions.
        function result = hasState(obj)
            result = obj.updates > 2;
        end
        
        % Uses a model of the ball to predict nSteps forward in time.
        % Returns a 3xn vector of positions in camera coordinates.
        function P = predictTrajectory(obj,nSteps,scaleVelocity)
            X_est = zeros(3,nSteps);
            C = (obj.rho * obj.S * obj.C_d)/(2*obj.m);
            dt = 1/obj.f_s;
            
            %X_0 = obj.P_Clast; % don't need since we have V already
            X_0 = obj.P_C;
            Xdot_0 = obj.V_C * scaleVelocity;
            
            for i=1:nSteps
                Xdot_0_mag = norm(Xdot_0);
                Xdot_0_norm = Xdot_0 / Xdot_0_mag;
                
                % Forward Euler to solve for next position
                Xddot_0 = [0 -obj.g 0]' - C*Xdot_0_mag^2*Xdot_0_norm;
                Xdot_1 = Xdot_0 + dt*Xddot_0;
                
                X_1 = X_0 + dt*Xdot_1*10;
                
                X_est(:,i) = X_1;
                
                % Update current variable window
                %X_0 = X_1;
                X_0 = X_1;
                Xdot_0 = Xdot_1;
            end
            
            P = X_est;
        end % function predictTrajectory()
        
    end
    
end

