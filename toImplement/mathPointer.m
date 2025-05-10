% dish config
offset = 1; %dish motor offset from GPS in z dir
offX = 0; % dish GPS offset from motor in west/east
offY = 0; % dish GPS offset from motor in north/south
latConv = 110066.3829; % conversion ratio from lat deg to meters
longConv = 90369.2356; % conversion ratio from long deg to meters

% servo constants for pitch
ph = 160; % PWM value for horizontal
pv = 250; % PWM value for vertical
thetav = 90; % angle for vertical
thetah = 0; % angle for horizontal

% servo constants for yaw
pMax = 255; % PWM max value
pMin = 0; % PWM min value
thetaMax = 360; % Max angle
thetaMin = 0; % Min angle

% horizontal test case
dishPosition = [40.427364, -86.919029, 184.60]; % position of dish

% test cases, read comments and select one
targetPosition = [40.431156, -86.915763, 201.40]; % position of Neil Armstrong's Crotch
%targetPosition = [40.424083, -86.910614, 183.70]; % position of middle of PMU intersection
%targetPosition = [40.427314, -86.913871, 218.10]; % position of under bell tower
%targetPosition = [40.425521, -86.925102, 193.80]; % position of Amelia Earhart's Feet

% input data format lat, long, alt
% data format in cartesian y, x, z

% update dish pos based on offset
dishPosition(1) = dishPosition(1) + offY;
dishPosition(2) = dishPosition(2) + offX;
dishPosition(3) = dishPosition(3) + offset;

% convert to m
dishPosition(1) = latConv * dishPosition(1);
dishPosition(2) = longConv * dishPosition(2);

targetPosition(1) = latConv * targetPosition(1);
targetPosition(2) = longConv * targetPosition(2);

vectorDir = targetPosition - dishPosition;

a = vectorDir(2) > 0;
b = vectorDir(1) > 0;

angle = atand(vectorDir(1) / vectorDir(2));

% step 1: implement conversion to full circle
if (a && b) % Q1
    yaw = angle;
elseif (~a && b) % Q2
    yaw = 180 + angle;
elseif (~a && ~b) % Q3
    yaw = 180 + angle;
elseif (a && ~b) % Q4
    yaw = 360 + angle;
end


% step 2: implement conversion to azimuth

yaw = mod(90 - yaw, 360);

% step 3: pitch

dist = norm(vectorDir);
pitch = asind(vectorDir(3) / dist);

fprintf("\nyaw: %17.2f° CW from N \npitch: %15.2f° from horizon\n", yaw, pitch);
fprintf("shortest path: %7.2f m from dish\n", dist);

% calculations for PWM conversion
pwmGenPitch = (pv - ph) * pitch / (thetav - thetah) + ph;
pwmGenYaw = (pMax - pMin) * pitch / (thetaMax - thetaMin) + pMin;
fprintf("PWM value pitch %6.f\n", round(pwmGenPitch, 0));
fprintf("PWM value yaw %6.f\n", round(pwmGenYaw, 0));