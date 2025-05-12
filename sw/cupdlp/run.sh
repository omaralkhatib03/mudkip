PROB=${1:-"afiro.mps"}
PROB_NO_EXT="${PROB%.mps}"

scp build/plc Versal:/home/petalinux/cupdlp/
scp problems/$PROB Versal:/home/petalinux/cupdlp/problems

ssh -t Versal "cd /home/petalinux/cupdlp/ && ./run_app.sh $PROB"
ssh -t Versal "cd /home/petalinux/cupdlp/ && scp $PROB_NO_EXT.json ghambir:$PWD/solutions"
