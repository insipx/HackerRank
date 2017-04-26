#! /bin/bash


for i in `seq 0 99`; do
  if [ $(($i % 2)) != 0 ]; then
    echo $i
  fi
done
