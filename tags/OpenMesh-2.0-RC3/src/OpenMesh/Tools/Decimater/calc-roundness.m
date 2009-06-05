# angle in degrees [0,60]
# 
# compute roundness of first case

A = [ 0.0335717 0.0576863 -0.0503314 ]';
B = [ 0.0325544 0.057614 -0.0504989 ]';
C = [ 0.0323531 0.057051 -0.0504476 ]';

#
vecAC=C-A;
vecAB=B-A;

aa = norm(B-C)^2;
bb = norm(vecAC)^2;
cc = norm(vecAB)^2;
AA = norm(cross(vecAC,vecAB))^2

nom   = AA * min( aa, min(bb,cc) );
denom = aa * bb * cc;
nR1   = sqrt(4.0/3.0) * sqrt(nom/denom)

