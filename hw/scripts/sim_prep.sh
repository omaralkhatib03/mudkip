
#!/bin/bash

TOP=$1
pwd

cd xsim
./"${TOP}.sh" -step compile
./"${TOP}.sh" -step elaborate 
