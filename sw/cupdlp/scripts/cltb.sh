make build/cltb
scp ./build/cltb Versal:/home/petalinux/
# scp ./build/kernel.xclbin Versal:/home/petalinux/
ssh -t Versal "cd /home/petalinux/ && ./cltb kernel.xclbin"
