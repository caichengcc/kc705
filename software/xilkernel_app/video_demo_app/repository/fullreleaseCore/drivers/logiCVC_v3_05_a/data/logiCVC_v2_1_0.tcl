#*****************************************************************************
#** This confidential and proprietary code may be used only as authorized
#** by licensing agreement from Xylon d.o.o.
#** In the event of publication, the following notice is applicable:
#**
#** Copyright 2012 - Xylon d.o.o.
#** All rights reserved.
#**
#** The entire notice above must be reproduced on all authorized copies.
#*****************************************************************************/

proc xget_dname_num {driver_name param num prefix} {
    set name [string toupper $driver_name]
    set name [format "%s_%s_" $prefix $name]
    if {[string match C_* $param]} {
        set name [format "%s%d_%s" $name $num [string range $param 2 end]]
    } else {
        set name [format "%s%d_%s" $name $num $param]
    }
    return $name
}


proc xylon_xdefine_include_file {drv_handle file_name drv_string args} {
    # Open include file
    set file_handle [xopen_include_file $file_name]

    # Open config file
    set config_filename [format "src/osdrv/%s_g.c" $drv_string]
    set config_file [open $config_filename w]
    xprint_generated_header $config_file "Driver configuration parameters"

    # Get all peripherals connected to this driver
    set periphs [xget_periphs $drv_handle]

    # Handle special cases
    set arg "NUM_INSTANCES"
    set posn [lsearch -exact $args $arg]
    if {$posn > -1} {
        puts $file_handle "/* Definitions for driver [string toupper [xget_sw_name $drv_handle]] */"
        # Define NUM_INSTANCES
        puts $file_handle "#define [xget_dname $drv_string $arg] [llength $periphs]"
        set args [lreplace $args $posn $posn]
    }
    # Check if it is a driver parameter
    lappend newargs
    foreach arg $args {
        set value [xget_value $drv_handle "PARAMETER" $arg]
        if {[llength $value] == 0} {
            lappend newargs $arg
        } else {
            puts $file_handle "#define [xget_dname $drv_string $arg] [xget_value $drv_handle "PARAMETER" $arg]"
            puts $config_file "#define CORECFG_[string toupper $drv_string]_$arg [xget_value $drv_handle "PARAMETER" $arg]"
        }
    }
    set args $newargs

    # Print all parameters for all peripherals
    set device_cnt 0
    set cnt 0
    foreach periph $periphs {
        puts $file_handle ""
        puts $file_handle "/* Definitions for peripheral [string toupper [xget_hw_name $periph]] */"
        puts $config_file ""
        puts $config_file "/* Definitions for peripheral [string toupper [xget_hw_name $periph]] */"
        foreach arg $args {
            if {[string compare -nocase "C_USE_SIZE_POSITION" $arg] == 0} {
                set value $device_cnt
                incr device_cnt
            } else {
                set value [xget_param_value $periph $arg]
            }
            if {[llength $value] == 0} {
                set value 0
            }
            set value [xformat_addr_string $value $arg]
            puts $file_handle "#define [xget_dname_num $drv_string $arg $cnt "XPAR"] $value"
            puts $config_file "#define [xget_dname_num $drv_string $arg $cnt "CORECFG"] $value"
        }
        puts $file_handle ""
        puts $config_file ""
        incr cnt
    }
    puts $file_handle "\n/******************************************************************/\n"
    puts $config_file "\n/******************************************************************/\n"
    close $file_handle
    close $config_file
}

#
# Create configuration C file as required by Xilinx  drivers
#
proc xylon_xdefine_config_file {drv_handle drv_string args} {
    set filename [format "src/osdrv/%s_g.c" $drv_string]
    set config_file [open $filename a]
    set drv_type_prefix [string tolower $drv_string 5 6]
    puts $config_file [format "#include \"%s_g.h\"" $drv_string]
    puts $config_file "\n/*"
    puts $config_file "* The configuration table for devices"
    puts $config_file "*/\n"
    puts $config_file [format "static const %sConfigT %sConfigTable\[\] =" $drv_type_prefix $drv_type_prefix]
    puts $config_file "\{"

    set periphs [xget_periphs $drv_handle]
    set cnt 0
    set param_cnt 0
    set line_ending ""

    foreach periph $periphs {
        puts $config_file "\t\{"

        foreach arg $args {
            # Check if this is a driver parameter or a peripheral parameter
            set value [xget_value $drv_handle "PARAMETER" $arg]
            if {[llength $value] == 0} {
                set p2p_name [xget_p2p_name $periph $arg]
                if { [string compare -nocase $p2p_name ""] == 0} {
                    if {[string compare -length 7 "C_LAYER" $arg] == 0 || [string compare -length 7 "C_BUFFER" $arg] == 0} {
                        if {$param_cnt == 0} {
                            puts $config_file "\t\t\{"
                        }
                        if {$param_cnt < 4} {
                            set line_ending ","
                            incr param_cnt
                        } else {
                            set line_ending "\n\t\t\},"
                            set param_cnt 0
                        }
                        puts $config_file [format "\t\t\t%s%s" [xget_dname_num $drv_string $arg $cnt "CORECFG"] $line_ending]
                    } else {
                        puts $config_file [format "\t\t%s," [xget_dname_num $drv_string $arg $cnt "CORECFG"]]
                    }
                } else {
                    #puts $config_file [format "\t\t%s" $p2p_name]
                    puts $config_file [format "\t\t%s," [xget_dname_num $drv_string $arg $cnt "CORECFG"]]
                }
            } else {
                puts $config_file [format "\t\t%s" [xget_dname_num $drv_string $arg $cnt "CORECFG"]]
            }
        }
        puts $config_file "\t\},"
        # Increment the pheripheral counter
        incr cnt
    }

    puts $config_file "\};\n"

    # Create xxLookupConfig function for obtaining an entry from table
    puts $config_file "\n/********************* Function definitions ***********************/\n"
    puts $config_file "/*******************************************************************"
    puts $config_file "*"
    puts $config_file "* Returs device configuration based on the unique device ID."
    puts $config_file "*"
    puts $config_file "* @param\[in\]    deviceId is the unique device ID to match."
    puts $config_file "*"
    puts $config_file "* @return       Pointer to configuration data for specified instance"
    puts $config_file "*               or NULL if instance does not exist."
    puts $config_file "*"
    puts $config_file "*******************************************************************/"
    puts $config_file [format "%sConfigT const * %sLookupConfig(unsigned char deviceId)" $drv_type_prefix $drv_type_prefix]
    puts $config_file         "{"
    puts $config_file [format "    if (deviceId < CORECFG_%s_NUM_INSTANCES)" [string toupper $drv_string]]
    puts $config_file         "    {"
    puts $config_file [format "        return (%sConfigT *)&%sConfigTable\[deviceId\];" $drv_type_prefix $drv_type_prefix]
    puts $config_file         "    }"
    puts $config_file         "    else"
    puts $config_file         "    {"
    puts $config_file [format "        return (%sConfigT *)0;" $drv_type_prefix]
    puts $config_file         "    }"
    puts $config_file         "}"
    close $config_file

    # Create header file to expose xxLookupConfig function prototype
    set filename [format "src/osdrv/%s_g.h" $drv_string]
    set config_file [open $filename w]
    xprint_generated_header $config_file "Driver configuration table header file"
    puts $config_file "#ifndef __[string toupper $drv_string]_G_H__"
    puts $config_file "#define __[string toupper $drv_string]_G_H__"
    puts $config_file "\n\n/************************* Include files **************************/\n"
    puts $config_file [format "#include \"%s.h\"" $drv_string]
    puts $config_file "\n\n/********************* Constant definitions ***********************/\n"
    puts $config_file "#define CORECFG_[string toupper $drv_string]_NUM_INSTANCES [llength $periphs]"
    puts $config_file "\n\n/********************* Function declarations **********************/\n"
    puts $config_file "#ifdef __cplusplus"
    puts $config_file "extern \"C\""
    puts $config_file "#endif"
    puts $config_file "\n"
    puts $config_file [format "%sConfigT const * %sLookupConfig(unsigned char deviceId);" $drv_type_prefix $drv_type_prefix]
    puts $config_file "\n#endif"
    close $config_file
}

proc generate {drv_handle} {

    file copy -force [file join src inc logiCVC.h] [file join .. .. include]
    file copy -force [file join src osdrv logiCVC_init.h] [file join .. .. include]

    # generate makefile.include
    set flip_reg_endian [xget_value $drv_handle "PARAMETER" "FLIP_REG_ENDIAN"]
    set oslib_cpu_support [xget_value $drv_handle "PARAMETER" "OSLIB_CPU_SUPPORT"]
    set mkfile [open [file join src makefile.include] w]
    puts $mkfile "# generated file"
    if {$flip_reg_endian == true} {
        puts $mkfile "TCL_GEN_FLAGS+= -DCVC_FLIP_REG_ENDIAN"
    }
    if {$oslib_cpu_support == true} {
        puts $mkfile "TCL_GEN_FLAGS+= -DOSLIB_ADD_CPU_SUPPORT"
    }
    close $mkfile

    xylon_xdefine_include_file $drv_handle "xparameters.h" "logiCVC" "NUM_INSTANCES" \
                 "C_IP_MAJOR_REVISION" "C_IP_MINOR_REVISION" "C_IP_PATCH_LEVEL" "C_REGS_BASEADDR" "C_REGS_HIGHADDR" \
                 "C_VMEM_BASEADDR" "C_VMEM_HIGHADDR" "C_ROW_STRIDE" "C_NUM_OF_LAYERS" "C_USE_BACKGROUND" "C_READABLE_REGS" \
                 "C_LAYER_0_DATA_WIDTH" "C_LAYER_1_DATA_WIDTH" "C_LAYER_2_DATA_WIDTH" "C_LAYER_3_DATA_WIDTH" "C_LAYER_4_DATA_WIDTH" \
                 "C_LAYER_0_TYPE" "C_LAYER_1_TYPE" "C_LAYER_2_TYPE" "C_LAYER_3_TYPE" "C_LAYER_4_TYPE" \
                 "C_LAYER_0_ALPHA_MODE" "C_LAYER_1_ALPHA_MODE" "C_LAYER_2_ALPHA_MODE" "C_LAYER_3_ALPHA_MODE" "C_LAYER_4_ALPHA_MODE" \
                 "C_LAYER_0_OFFSET" "C_LAYER_1_OFFSET" "C_LAYER_2_OFFSET" "C_LAYER_3_OFFSET" "C_LAYER_4_OFFSET" \
                 "C_BUFFER_0_OFFSET" "C_BUFFER_1_OFFSET" "C_BUFFER_2_OFFSET" "C_BUFFER_3_OFFSET" "C_BUFFER_4_OFFSET" \
                 "C_USE_SIZE_POSITION"

    xylon_xdefine_config_file $drv_handle "logiCVC" \
                 "C_IP_MAJOR_REVISION" "C_IP_MINOR_REVISION" "C_IP_PATCH_LEVEL" "C_REGS_BASEADDR" "C_REGS_HIGHADDR" \
                 "C_VMEM_BASEADDR" "C_VMEM_HIGHADDR" "C_ROW_STRIDE" "C_NUM_OF_LAYERS" "C_USE_BACKGROUND" "C_READABLE_REGS" \
                 "C_LAYER_0_DATA_WIDTH" "C_LAYER_1_DATA_WIDTH" "C_LAYER_2_DATA_WIDTH" "C_LAYER_3_DATA_WIDTH" "C_LAYER_4_DATA_WIDTH" \
                 "C_LAYER_0_TYPE" "C_LAYER_1_TYPE" "C_LAYER_2_TYPE" "C_LAYER_3_TYPE" "C_LAYER_4_TYPE" \
                 "C_LAYER_0_ALPHA_MODE" "C_LAYER_1_ALPHA_MODE" "C_LAYER_2_ALPHA_MODE" "C_LAYER_3_ALPHA_MODE" "C_LAYER_4_ALPHA_MODE" \
                 "C_LAYER_0_OFFSET" "C_LAYER_1_OFFSET" "C_LAYER_2_OFFSET" "C_LAYER_3_OFFSET" "C_LAYER_4_OFFSET" \
                 "C_BUFFER_0_OFFSET" "C_BUFFER_1_OFFSET" "C_BUFFER_2_OFFSET" "C_BUFFER_3_OFFSET" "C_BUFFER_4_OFFSET"
}
