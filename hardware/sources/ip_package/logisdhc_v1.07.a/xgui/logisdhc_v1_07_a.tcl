#Definitional proc to organize widgets for parameters.
set c_family [string tolower [get_project_property ARCHITECTURE]]

proc create_gui { ipview } {
    variable c_family
 
    set Component_Name [ipgui::add_param $ipview -parent $ipview -name Component_Name]

	   set Page0 [ ipgui::add_page $ipview  -name "IP core license and version" -layout vertical]
        set tabgroup0 [ipgui::add_group $ipview -parent $Page0 -name {IP License} -layout vertical]
            set C_IP_LICENSE_TYPE [ipgui::add_param $ipview -parent $tabgroup0 -name C_IP_LICENSE_TYPE]
            set C_IP_LICENSE_CHECK [ipgui::add_param $ipview -parent $tabgroup0 -name C_IP_LICENSE_CHECK]
        set tabgroup1 [ipgui::add_group $ipview -parent $Page0 -name {IP Version} -layout vertical]
            set C_IP_MAJOR_REVISION [ipgui::add_param $ipview -parent $tabgroup1 -name C_IP_MAJOR_REVISION]
            set C_IP_MINOR_REVISION [ipgui::add_param $ipview -parent $tabgroup1 -name C_IP_MINOR_REVISION]
            set C_IP_PATCH_LEVEL [ipgui::add_param $ipview -parent $tabgroup1 -name C_IP_PATCH_LEVEL]
 
    set Page1 [ ipgui::add_page $ipview  -name "Registers interface" -layout vertical]
        set tabgroup2 [ipgui::add_group $ipview -parent $Page1 -name {Regsiters General Settings} -layout vertical]
            set C_REGS_INTERFACE [ipgui::add_param $ipview -parent $tabgroup2 -widget comboBox -name C_REGS_INTERFACE]
        set tabgroup3 [ipgui::add_group $ipview -parent $Page1 -name {Registers Address Space} -layout vertical]
            set C_REGS_BASEADDR [ipgui::add_param $ipview -parent $tabgroup3 -name C_REGS_BASEADDR ]
                set_property tooltip {Regsiters base address - use only if OPB or SPLB bus interface is implemented.} $C_REGS_BASEADDR
            set C_REGS_HIGHADDR [ipgui::add_param $ipview -parent $tabgroup3 -name C_REGS_HIGHADDR]
                set_property tooltip {Registers high address - use only if OPB or SPLB bus interface is implemented.} $C_REGS_HIGHADDR
        set tabgroup4 [ipgui::add_group $ipview -parent $Page1 -name {AXI4_Lite Interface} -layout vertical]
            set C_S_AXI_ADDR_WIDTH [ipgui::add_param $ipview -parent $tabgroup4 -name C_S_AXI_ADDR_WIDTH]
                set_property tooltip {AXI4-Lite interface address bus width.} $C_S_AXI_ADDR_WIDTH
            set C_S_AXI_DATA_WIDTH [ipgui::add_param $ipview -parent $tabgroup4 -name C_S_AXI_DATA_WIDTH]
                set_property tooltip {AXI4-Lite interface data bus width.} $C_S_AXI_DATA_WIDTH                               
        set tabgroup5 [ipgui::add_group $ipview -parent $Page1 -name {SPLB Interface} -layout vertical]
            set C_SPLB_AWIDTH [ipgui::add_param $ipview -parent $tabgroup5 -name C_SPLB_AWIDTH]
                set_property tooltip {SPLB interface address bus width.} $C_SPLB_AWIDTH
            set C_SPLB_DWIDTH [ipgui::add_param $ipview -parent $tabgroup5 -name C_SPLB_DWIDTH]
                set_property tooltip {SPLB interface data bus width.} $C_SPLB_DWIDTH
            set C_SPLB_NATIVE_DWIDTH [ipgui::add_param $ipview -parent $tabgroup5 -name C_SPLB_NATIVE_DWIDTH]
                set_property tooltip {SPLB interface native data bus width.} $C_SPLB_NATIVE_DWIDTH
            set C_SPLB_MID_WIDTH [ipgui::add_param $ipview -parent $tabgroup5 -name C_SPLB_MID_WIDTH]
                set_property tooltip {PLB master ID bus width, the value is log2(C_SPLB_NUM_MASTERS).} $C_SPLB_MID_WIDTH
            set C_SPLB_NUM_MASTERS [ipgui::add_param $ipview -parent $tabgroup5 -name C_SPLB_NUM_MASTERS]
                set_property tooltip {Number of masters on SPLB interface bus.} $C_SPLB_NUM_MASTERS
        set tabgroup6 [ipgui::add_group $ipview -parent $Page1 -name {OPB Interface} -layout vertical]
            set C_OPB_AWIDTH [ipgui::add_param $ipview -parent $tabgroup6 -name C_OPB_AWIDTH]
                set_property tooltip {OPB interface address bus width.} $C_OPB_AWIDTH
            set C_OPB_DWIDTH [ipgui::add_param $ipview -parent $tabgroup6 -name C_OPB_DWIDTH]
                set_property tooltip {OPB interface data bus width.} $C_OPB_DWIDTH
 
    set Page2 [ ipgui::add_page $ipview  -name "Memory interface" -layout vertical]
        set tabgroup7 [ipgui::add_group $ipview -parent $Page2 -name {Memory General Settings} -layout vertical]
            set C_MEM_INTERFACE [ipgui::add_param $ipview -parent $tabgroup7 -widget comboBox -name C_MEM_INTERFACE]
            set C_MEM_BURST [ipgui::add_param $ipview -parent $tabgroup7 -widget comboBox -name C_MEM_BURST]
                set_property tooltip {Available values are: 16, 32 and 64 words.} $C_MEM_BURST
        set tabgroup8 [ipgui::add_group $ipview -parent $Page2 -name {Memory Address Space} -layout vertical]
            set C_MEM_BASEADDR [ipgui::add_param $ipview -parent $tabgroup8 -name C_MEM_BASEADDR]
            set C_MEM_HIGHADDR [ipgui::add_param $ipview -parent $tabgroup8 -name C_MEM_HIGHADDR]
        set tabgroup9 [ipgui::add_group $ipview -parent $Page2 -name {AXI4 Interface} -layout vertical]
            set C_M_AXI_ADDR_WIDTH [ipgui::add_param $ipview -parent $tabgroup9 -name C_M_AXI_ADDR_WIDTH]
                set_property tooltip {AXI4 interface address bus width.} $C_M_AXI_ADDR_WIDTH
            set C_M_AXI_DATA_WIDTH [ipgui::add_param $ipview -parent $tabgroup9 -name C_M_AXI_DATA_WIDTH]
                set_property tooltip {AXI4 interface data bus width.} $C_M_AXI_DATA_WIDTH
            set C_M_AXI_THREAD_ID_WIDTH [ipgui::add_param $ipview -parent $tabgroup9 -name C_M_AXI_THREAD_ID_WIDTH]
                set_property tooltip {AXI4 interface thread ID width.} $C_M_AXI_THREAD_ID_WIDTH
        set tabgroup10 [ipgui::add_group $ipview -parent $Page2 -name {XMB Interface} -layout vertical]
            set C_MEM_DATA_BUS_WIDTH [ipgui::add_param $ipview -parent $tabgroup10 -name C_MEM_DATA_BUS_WIDTH]
                set_property tooltip {XMB interface data bus width.} $C_MEM_DATA_BUS_WIDTH

    set Page3 [ ipgui::add_page $ipview  -name "User" -layout vertical]
        set tabgroup11 [ipgui::add_group $ipview -parent $Page3 -name {DMA Settings} -layout vertical]
            set C_USE_DMA [ipgui::add_param $ipview -parent $tabgroup11 -widget comboBox -name C_USE_DMA]
                set_property tooltip {Enable logiSDHC DMA logic for transfering data from SD card to memory interface.} $C_USE_DMA
            set C_DMA_TYPE [ipgui::add_param $ipview -parent $tabgroup11 -widget comboBox -name C_DMA_TYPE]
                set_property tooltip {Use Standard SD Specification DMA or Xylon type DMA optimized for graphic aplications.} $C_DMA_TYPE 
            set C_CONVERT_ENDIANESS [ipgui::add_param $ipview -parent $tabgroup11 -widget comboBox -name C_CONVERT_ENDIANESS]
                set_property tooltip {Convert endianness: no - endiannes not converted (big endian); yes - endianness converted (little endian).} $C_CONVERT_ENDIANESS
            set C_ROW_STRIDE [ipgui::add_param $ipview -parent $tabgroup11 -widget comboBox -name C_ROW_STRIDE]
                set_property tooltip {Distance between the same colon pixels for adjacent rows in number of pixels.} $C_ROW_STRIDE
            set C_BYTE_PER_PIXEL [ipgui::add_param $ipview -parent $tabgroup11 -name C_BYTE_PER_PIXEL]
                set_property tooltip {In case Xylon DMA is used this parameter determines how many bytes form one image pixel.} $C_BYTE_PER_PIXEL 
        set tabgroup12 [ipgui::add_group $ipview -parent $Page3 -name {SDHC Base Clock} -layout vertical]
            set C_SD_BASE_CLOCK_FREQ [ipgui::add_param $ipview -parent $tabgroup12 -name C_SD_BASE_CLOCK_FREQ]
                set_property tooltip {Recomended clock frequency is 100 MHz.} $C_SD_BASE_CLOCK_FREQ

}

proc C_SD_BASE_CLOCK_FREQ_updated {ipview} {
	# Procedure called when C_SD_BASE_CLOCK_FREQ is updated
	return true
}

proc validate_C_SD_BASE_CLOCK_FREQ {ipview} {
	# Procedure called to validate C_SD_BASE_CLOCK_FREQ
	return true
}

proc C_REGS_INTERFACE_updated {ipview} {
	# Procedure called when C_REGS_INTERFACE is updated
	return true
}

proc validate_C_REGS_INTERFACE {ipview} {
	# Procedure called to validate C_REGS_INTERFACE
	return true
}

proc C_REGS_BASEADDR_updated {ipview} {
	# Procedure called when C_REGS_BASEADDR is updated
	return true
}

proc validate_C_REGS_BASEADDR {ipview} {
	# Procedure called to validate C_REGS_BASEADDR
	return true
}

proc C_REGS_HIGHADDR_updated {ipview} {
	# Procedure called when C_REGS_HIGHADDR is updated
	return true
}

proc validate_C_REGS_HIGHADDR {ipview} {
	# Procedure called to validate C_REGS_HIGHADDR
	return true
}

proc C_OPB_AWIDTH_updated {ipview} {
	# Procedure called when C_OPB_AWIDTH is updated
	return true
}

proc validate_C_OPB_AWIDTH {ipview} {
	# Procedure called to validate C_OPB_AWIDTH
	return true
}

proc C_OPB_DWIDTH_updated {ipview} {
	# Procedure called when C_OPB_DWIDTH is updated
	return true
}

proc validate_C_OPB_DWIDTH {ipview} {
	# Procedure called to validate C_OPB_DWIDTH
	return true
}

proc C_SPLB_NUM_MASTERS_updated {ipview} {
	# Procedure called when C_SPLB_NUM_MASTERS is updated
	return true
}

proc validate_C_SPLB_NUM_MASTERS {ipview} {
	# Procedure called to validate C_SPLB_NUM_MASTERS
	return true
}

proc C_SPLB_MID_WIDTH_updated {ipview} {
	# Procedure called when C_SPLB_MID_WIDTH is updated
	return true
}

proc validate_C_SPLB_MID_WIDTH {ipview} {
	# Procedure called to validate C_SPLB_MID_WIDTH
	return true
}

proc C_SPLB_NATIVE_DWIDTH_updated {ipview} {
	# Procedure called when C_SPLB_NATIVE_DWIDTH is updated
	return true
}

proc validate_C_SPLB_NATIVE_DWIDTH {ipview} {
	# Procedure called to validate C_SPLB_NATIVE_DWIDTH
	return true
}

proc C_SPLB_AWIDTH_updated {ipview} {
	# Procedure called when C_SPLB_AWIDTH is updated
	return true
}

proc validate_C_SPLB_AWIDTH {ipview} {
	# Procedure called to validate C_SPLB_AWIDTH
	return true
}

proc C_SPLB_DWIDTH_updated {ipview} {
	# Procedure called when C_SPLB_DWIDTH is updated
	return true
}

proc validate_C_SPLB_DWIDTH {ipview} {
	# Procedure called to validate C_SPLB_DWIDTH
	return true
}

proc C_S_AXI_ADDR_WIDTH_updated {ipview} {
	# Procedure called when C_S_AXI_ADDR_WIDTH is updated
	return true
}

proc validate_C_S_AXI_ADDR_WIDTH {ipview} {
	# Procedure called to validate C_S_AXI_ADDR_WIDTH
	return true
}

proc C_S_AXI_DATA_WIDTH_updated {ipview} {
	# Procedure called when C_S_AXI_DATA_WIDTH is updated
	return true
}

proc validate_C_S_AXI_DATA_WIDTH {ipview} {
	# Procedure called to validate C_S_AXI_DATA_WIDTH
	return true
}

proc C_MEM_INTERFACE_updated {ipview} {
	# Procedure called when C_MEM_INTERFACE is updated
	return true
}

proc validate_C_MEM_INTERFACE {ipview} {
	# Procedure called to validate C_MEM_INTERFACE
	return true
}

proc C_MEM_DATA_BUS_WIDTH_updated {ipview} {
	# Procedure called when C_MEM_DATA_BUS_WIDTH is updated
	return true
}

proc validate_C_MEM_DATA_BUS_WIDTH {ipview} {
	# Procedure called to validate C_MEM_DATA_BUS_WIDTH
	return true
}

proc C_MEM_BASEADDR_updated {ipview} {
	# Procedure called when C_MEM_BASEADDR is updated
	return true
}

proc validate_C_MEM_BASEADDR {ipview} {
	# Procedure called to validate C_MEM_BASEADDR
	return true
}

proc C_MEM_HIGHADDR_updated {ipview} {
	# Procedure called when C_MEM_HIGHADDR is updated
	return true
}

proc validate_C_MEM_HIGHADDR {ipview} {
	# Procedure called to validate C_MEM_HIGHADDR
	return true
}

proc C_MEM_BURST_updated {ipview} {
	# Procedure called when C_MEM_BURST is updated
	return true
}

proc validate_C_MEM_BURST {ipview} {
	# Procedure called to validate C_MEM_BURST
	return true
}

proc C_CONVERT_ENDIANESS_updated {ipview} {
	# Procedure called when C_CONVERT_ENDIANESS is updated
	return true
}

proc validate_C_CONVERT_ENDIANESS {ipview} {
	# Procedure called to validate C_CONVERT_ENDIANESS
	return true
}

proc C_M_AXI_THREAD_ID_WIDTH_updated {ipview} {
	# Procedure called when C_M_AXI_THREAD_ID_WIDTH is updated
	return true
}

proc validate_C_M_AXI_THREAD_ID_WIDTH {ipview} {
	# Procedure called to validate C_M_AXI_THREAD_ID_WIDTH
	return true
}

proc C_M_AXI_DATA_WIDTH_updated {ipview} {
	# Procedure called when C_M_AXI_DATA_WIDTH is updated
	return true
}

proc validate_C_M_AXI_DATA_WIDTH {ipview} {
	# Procedure called to validate C_M_AXI_DATA_WIDTH
	return true
}

proc C_M_AXI_ADDR_WIDTH_updated {ipview} {
	# Procedure called when C_M_AXI_ADDR_WIDTH is updated
	return true
}

proc validate_C_M_AXI_ADDR_WIDTH {ipview} {
	# Procedure called to validate C_M_AXI_ADDR_WIDTH
	return true
}

proc C_BYTE_PER_PIXEL_updated {ipview} {
	# Procedure called when C_BYTE_PER_PIXEL is updated
	return true
}

proc validate_C_BYTE_PER_PIXEL {ipview} {
	# Procedure called to validate C_BYTE_PER_PIXEL
	return true
}

proc C_ROW_STRIDE_updated {ipview} {
	# Procedure called when C_ROW_STRIDE is updated
	return true
}

proc validate_C_ROW_STRIDE {ipview} {
	# Procedure called to validate C_ROW_STRIDE
	return true
}

proc C_USE_DMA_updated {ipview} {
	# Procedure called when C_USE_DMA is updated
	return true
}

proc validate_C_USE_DMA {ipview} {
	# Procedure called to validate C_USE_DMA
	return true
}

proc C_DMA_TYPE_updated {ipview} {
	# Procedure called when C_DMA_TYPE is updated
	return true
}

proc validate_C_DMA_TYPE {ipview} {
	# Procedure called to validate C_DMA_TYPE
	return true
}

proc C_IP_LICENSE_CHECK_updated {ipview} {
	# Procedure called when C_IP_LICENSE_CHECK is updated
	return true
}

proc validate_C_IP_LICENSE_CHECK {ipview} {
	# Procedure called to validate C_IP_LICENSE_CHECK
	return true
}

proc C_IP_PATCH_LEVEL_updated {ipview} {
	# Procedure called when C_IP_PATCH_LEVEL is updated
	return true
}

proc validate_C_IP_PATCH_LEVEL {ipview} {
	# Procedure called to validate C_IP_PATCH_LEVEL
	return true
}

proc C_IP_MINOR_REVISION_updated {ipview} {
	# Procedure called when C_IP_MINOR_REVISION is updated
	return true
}

proc validate_C_IP_MINOR_REVISION {ipview} {
	# Procedure called to validate C_IP_MINOR_REVISION
	return true
}

proc C_IP_MAJOR_REVISION_updated {ipview} {
	# Procedure called when C_IP_MAJOR_REVISION is updated
	return true
}

proc validate_C_IP_MAJOR_REVISION {ipview} {
	# Procedure called to validate C_IP_MAJOR_REVISION
	return true
}

proc C_IP_LICENSE_TYPE_updated {ipview} {
	# Procedure called when C_IP_LICENSE_TYPE is updated
	return true
}

proc validate_C_IP_LICENSE_TYPE {ipview} {
	# Procedure called to validate C_IP_LICENSE_TYPE
	return true
}

proc updateModel_C_FAMILY {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	variable c_family
	set_property modelparam_value $c_family [ipgui::get_modelparamspec C_FAMILY -of $ipview]
	return true
}

proc updateModel_C_IP_LICENSE_TYPE {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_IP_LICENSE_TYPE -of $ipview ]] [ipgui::get_modelparamspec C_IP_LICENSE_TYPE -of $ipview ]

	return true
}

proc updateModel_C_IP_MAJOR_REVISION {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_IP_MAJOR_REVISION -of $ipview ]] [ipgui::get_modelparamspec C_IP_MAJOR_REVISION -of $ipview ]

	return true
}

proc updateModel_C_IP_MINOR_REVISION {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_IP_MINOR_REVISION -of $ipview ]] [ipgui::get_modelparamspec C_IP_MINOR_REVISION -of $ipview ]

	return true
}

proc updateModel_C_IP_PATCH_LEVEL {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_IP_PATCH_LEVEL -of $ipview ]] [ipgui::get_modelparamspec C_IP_PATCH_LEVEL -of $ipview ]

	return true
}

proc updateModel_C_IP_LICENSE_CHECK {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_IP_LICENSE_CHECK -of $ipview ]] [ipgui::get_modelparamspec C_IP_LICENSE_CHECK -of $ipview ]

	return true
}

proc updateModel_C_REGS_INTERFACE {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_REGS_INTERFACE -of $ipview ]] [ipgui::get_modelparamspec C_REGS_INTERFACE -of $ipview ]

	return true
}

proc updateModel_C_REGS_BASEADDR {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_REGS_BASEADDR -of $ipview ]] [ipgui::get_modelparamspec C_REGS_BASEADDR -of $ipview ]

	return true
}

proc updateModel_C_REGS_HIGHADDR {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_REGS_HIGHADDR -of $ipview ]] [ipgui::get_modelparamspec C_REGS_HIGHADDR -of $ipview ]

	return true
}

proc updateModel_C_OPB_AWIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_OPB_AWIDTH -of $ipview ]] [ipgui::get_modelparamspec C_OPB_AWIDTH -of $ipview ]

	return true
}

proc updateModel_C_OPB_DWIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_OPB_DWIDTH -of $ipview ]] [ipgui::get_modelparamspec C_OPB_DWIDTH -of $ipview ]

	return true
}

proc updateModel_C_SPLB_NUM_MASTERS {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SPLB_NUM_MASTERS -of $ipview ]] [ipgui::get_modelparamspec C_SPLB_NUM_MASTERS -of $ipview ]

	return true
}

proc updateModel_C_SPLB_MID_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SPLB_MID_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_SPLB_MID_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_SPLB_NATIVE_DWIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SPLB_NATIVE_DWIDTH -of $ipview ]] [ipgui::get_modelparamspec C_SPLB_NATIVE_DWIDTH -of $ipview ]

	return true
}

proc updateModel_C_SPLB_AWIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SPLB_AWIDTH -of $ipview ]] [ipgui::get_modelparamspec C_SPLB_AWIDTH -of $ipview ]

	return true
}

proc updateModel_C_SPLB_DWIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SPLB_DWIDTH -of $ipview ]] [ipgui::get_modelparamspec C_SPLB_DWIDTH -of $ipview ]

	return true
}

proc updateModel_C_S_AXI_ADDR_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_S_AXI_ADDR_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_S_AXI_ADDR_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_S_AXI_DATA_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_S_AXI_DATA_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_S_AXI_DATA_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_MEM_INTERFACE {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_MEM_INTERFACE -of $ipview ]] [ipgui::get_modelparamspec C_MEM_INTERFACE -of $ipview ]

	return true
}

proc updateModel_C_MEM_BURST {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_MEM_BURST -of $ipview ]] [ipgui::get_modelparamspec C_MEM_BURST -of $ipview ]

	return true
}

proc updateModel_C_CONVERT_ENDIANESS {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_CONVERT_ENDIANESS -of $ipview ]] [ipgui::get_modelparamspec C_CONVERT_ENDIANESS -of $ipview ]

	return true
}

proc updateModel_C_MEM_BASEADDR {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_MEM_BASEADDR -of $ipview ]] [ipgui::get_modelparamspec C_MEM_BASEADDR -of $ipview ]

	return true
}

proc updateModel_C_MEM_HIGHADDR {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_MEM_HIGHADDR -of $ipview ]] [ipgui::get_modelparamspec C_MEM_HIGHADDR -of $ipview ]

	return true
}

proc updateModel_C_MEM_DATA_BUS_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_MEM_DATA_BUS_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_MEM_DATA_BUS_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_M_AXI_THREAD_ID_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_M_AXI_THREAD_ID_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_M_AXI_THREAD_ID_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_M_AXI_DATA_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_M_AXI_DATA_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_M_AXI_DATA_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_M_AXI_ADDR_WIDTH {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_M_AXI_ADDR_WIDTH -of $ipview ]] [ipgui::get_modelparamspec C_M_AXI_ADDR_WIDTH -of $ipview ]

	return true
}

proc updateModel_C_BYTE_PER_PIXEL {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_BYTE_PER_PIXEL -of $ipview ]] [ipgui::get_modelparamspec C_BYTE_PER_PIXEL -of $ipview ]

	return true
}

proc updateModel_C_ROW_STRIDE {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_ROW_STRIDE -of $ipview ]] [ipgui::get_modelparamspec C_ROW_STRIDE -of $ipview ]

	return true
}

proc updateModel_C_USE_DMA {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_USE_DMA -of $ipview ]] [ipgui::get_modelparamspec C_USE_DMA -of $ipview ]

	return true
}

proc updateModel_C_DMA_TYPE {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_DMA_TYPE -of $ipview ]] [ipgui::get_modelparamspec C_DMA_TYPE -of $ipview ]

	return true
}

proc updateModel_C_SD_BASE_CLOCK_FREQ {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_SD_BASE_CLOCK_FREQ -of $ipview ]] [ipgui::get_modelparamspec C_SD_BASE_CLOCK_FREQ -of $ipview ]

	return true
}

