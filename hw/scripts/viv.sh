#!/bin/bash

SCRIPT=$1

vivado -mode batch -source "${SCRIPT}" -log ./viv_logs/vivado.log -journal ./viv_logs/vivado.jou && {
   echo "success."
   echo "all done."
} || {
   echo "error."
   echo "aborted."
}
