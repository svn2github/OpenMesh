# angle in degrees [0,60]
# [replace :angle: with a value between 0 and 60]
alpha_d = :angle:;

# compute roundness of first case

alpha = pi * alpha_d/180;

A = [ 0 0 0 ]';
B = [ 2*cos(alpha) 0 0 ]';
C = [ cos(alpha) sin(alpha) 0 ]';

#
vecAC=C-A;
vecAB=B-A;

aa = norm(B-C)^2;
bb = norm(vecAC)^2;
cc = norm(vecAB)^2;
AA = norm(cross(vecAC,vecAB))^2;

nom   = AA * min( aa, min(bb,cc) );
denom = aa * bb * cc;
nR1   = sqrt(4.0/3.0) * sqrt(nom/denom)

# compute roundness of 2nd case

alpha = pi * ((180-alpha_d)/2)/180;

A = [ 0 0 0 ]';
B = [ 2*cos(alpha) 0 0 ]';
C = [ cos(alpha) sin(alpha) 0 ]';

#
vecAC=C-A;
vecAB=B-A;

aa = norm(B-C)^2;
bb = norm(vecAC)^2;
cc = norm(vecAB)^2;
AA = norm(cross(vecAC,vecAB))^2;

nom   = AA * min( aa, min(bb,cc) );
denom = aa * bb * cc;
nR2   = sqrt(4.0/3.0) * sqrt(nom/denom)

