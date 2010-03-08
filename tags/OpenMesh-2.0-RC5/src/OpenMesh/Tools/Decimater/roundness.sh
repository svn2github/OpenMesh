#!/bin/sh

i=0
while ((i <= 60)); do
  cat roundness.m | perl -pe "s/:angle:/$i/" > tmp.m
  echo $i `octave -q tmp.m 2> /dev/null | grep -v "nR2" | perl -pe 's/^nR1 = (.*)$/\1/'`
  i=$((++i))
done
