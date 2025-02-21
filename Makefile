
.PHONY:clean 

vivado/mudkip.xpr: vivado
	vivado -nolog -nojou -mode batch -source ./scripts/mudkip.tcl -tclargs './hw' './vivado'

vivado:
	mkdir -p vivado

