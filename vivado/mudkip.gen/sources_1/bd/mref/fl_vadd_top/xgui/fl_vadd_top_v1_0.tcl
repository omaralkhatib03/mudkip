# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "AXI_DDR_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_DDR_DATAWIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_DDR_DEPTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_PS_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_PS_DATAWIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_PS_DEPTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_VECTOR_WIDTH" -parent ${Page_0}


}

proc update_PARAM_VALUE.AXI_DDR_ADDR_WIDTH { PARAM_VALUE.AXI_DDR_ADDR_WIDTH } {
	# Procedure called to update AXI_DDR_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_DDR_ADDR_WIDTH { PARAM_VALUE.AXI_DDR_ADDR_WIDTH } {
	# Procedure called to validate AXI_DDR_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_DDR_DATAWIDTH { PARAM_VALUE.AXI_DDR_DATAWIDTH } {
	# Procedure called to update AXI_DDR_DATAWIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_DDR_DATAWIDTH { PARAM_VALUE.AXI_DDR_DATAWIDTH } {
	# Procedure called to validate AXI_DDR_DATAWIDTH
	return true
}

proc update_PARAM_VALUE.AXI_DDR_DEPTH { PARAM_VALUE.AXI_DDR_DEPTH } {
	# Procedure called to update AXI_DDR_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_DDR_DEPTH { PARAM_VALUE.AXI_DDR_DEPTH } {
	# Procedure called to validate AXI_DDR_DEPTH
	return true
}

proc update_PARAM_VALUE.AXI_PS_ADDR_WIDTH { PARAM_VALUE.AXI_PS_ADDR_WIDTH } {
	# Procedure called to update AXI_PS_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_PS_ADDR_WIDTH { PARAM_VALUE.AXI_PS_ADDR_WIDTH } {
	# Procedure called to validate AXI_PS_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_PS_DATAWIDTH { PARAM_VALUE.AXI_PS_DATAWIDTH } {
	# Procedure called to update AXI_PS_DATAWIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_PS_DATAWIDTH { PARAM_VALUE.AXI_PS_DATAWIDTH } {
	# Procedure called to validate AXI_PS_DATAWIDTH
	return true
}

proc update_PARAM_VALUE.AXI_PS_DEPTH { PARAM_VALUE.AXI_PS_DEPTH } {
	# Procedure called to update AXI_PS_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_PS_DEPTH { PARAM_VALUE.AXI_PS_DEPTH } {
	# Procedure called to validate AXI_PS_DEPTH
	return true
}

proc update_PARAM_VALUE.AXI_VECTOR_WIDTH { PARAM_VALUE.AXI_VECTOR_WIDTH } {
	# Procedure called to update AXI_VECTOR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_VECTOR_WIDTH { PARAM_VALUE.AXI_VECTOR_WIDTH } {
	# Procedure called to validate AXI_VECTOR_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.AXI_VECTOR_WIDTH { MODELPARAM_VALUE.AXI_VECTOR_WIDTH PARAM_VALUE.AXI_VECTOR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_VECTOR_WIDTH}] ${MODELPARAM_VALUE.AXI_VECTOR_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_PS_DEPTH { MODELPARAM_VALUE.AXI_PS_DEPTH PARAM_VALUE.AXI_PS_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_PS_DEPTH}] ${MODELPARAM_VALUE.AXI_PS_DEPTH}
}

proc update_MODELPARAM_VALUE.AXI_PS_DATAWIDTH { MODELPARAM_VALUE.AXI_PS_DATAWIDTH PARAM_VALUE.AXI_PS_DATAWIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_PS_DATAWIDTH}] ${MODELPARAM_VALUE.AXI_PS_DATAWIDTH}
}

proc update_MODELPARAM_VALUE.AXI_DDR_DATAWIDTH { MODELPARAM_VALUE.AXI_DDR_DATAWIDTH PARAM_VALUE.AXI_DDR_DATAWIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_DDR_DATAWIDTH}] ${MODELPARAM_VALUE.AXI_DDR_DATAWIDTH}
}

proc update_MODELPARAM_VALUE.AXI_DDR_DEPTH { MODELPARAM_VALUE.AXI_DDR_DEPTH PARAM_VALUE.AXI_DDR_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_DDR_DEPTH}] ${MODELPARAM_VALUE.AXI_DDR_DEPTH}
}

proc update_MODELPARAM_VALUE.AXI_DDR_ADDR_WIDTH { MODELPARAM_VALUE.AXI_DDR_ADDR_WIDTH PARAM_VALUE.AXI_DDR_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_DDR_ADDR_WIDTH}] ${MODELPARAM_VALUE.AXI_DDR_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_PS_ADDR_WIDTH { MODELPARAM_VALUE.AXI_PS_ADDR_WIDTH PARAM_VALUE.AXI_PS_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_PS_ADDR_WIDTH}] ${MODELPARAM_VALUE.AXI_PS_ADDR_WIDTH}
}

