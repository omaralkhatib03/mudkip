PROB=${1:-"afiro.mtx"}
FT=$2
PROB_NO_EXT="${PROB%.mtx}"

scp mtx$FT/$PROB Versal:/home/petalinux/cltb/mtx
ssh -t Versal "cd /home/petalinux/cltb2/ && ./run_app.sh $PROB_NO_EXT"
