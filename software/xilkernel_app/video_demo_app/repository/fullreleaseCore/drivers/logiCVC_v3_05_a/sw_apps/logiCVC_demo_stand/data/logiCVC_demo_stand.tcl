proc swapp_get_name {} {
    return "logiCVC_demo_standalone";
}

proc swapp_get_description {} {
    return "logiCVC standalone demo application";
}


proc check_standalone_os {} {
    set oslist [xget_sw_modules "type" "os"];

    if { [llength $oslist] != 1 } {
        return 0;
    }
    set os [lindex $oslist 0];

#    if { $os != "standalone" } {
#        error "supported only on the Standalone Board Support Package.";
#    }
}


proc get_os {} {
    set oslist [xget_sw_modules "type" "os"];
    set os [lindex $oslist 0];

    if { $os == "" } {
        error "No Operating System specified in the Board Support Package.";
    }

    return $os;
}

proc get_stdout {} {
    set os [get_os];
    set stdout [xget_sw_module_parameter $os "STDOUT"];
    return $stdout;
}

proc check_stdout_hw {} {
    set p7_uarts [xget_ips "type" "ps7_uart"];
    set uartlites [xget_ips "type" "uartlite"];
    set uart16550s [xget_ips "type" "uart16550"];
    set mcs_iomodule [xget_ips "type" "iomodule"];
    if { ([llength $p7_uarts] == 0) && ([llength $uartlites] == 0) &&
	 ([llength $uart16550s] == 0) && ([llength $mcs_iomodule] == 0) } {
        # Check for MDM-Uart peripheral. The MDM would be listed as a peripheral
        # only if it has a UART interface. So no further check is required
        set mdmlist [xget_ips "type" "mdm"]
        if { [llength $mdmlist] == 0 } {
	    error "This application requires a Uart IP in the hardware."
        }
    }
}

proc check_stdout_sw {} {
    set stdout [get_stdout];
    if { $stdout == "none" } {
        error "The STDOUT parameter is not set on the OS. This application requires stdout to be set."
    }
}

proc get_mem_size { memlist } {
    return [lindex $memlist 4];
}

proc require_memory {memsize} {
    set imemlist [xget_memory_ranges "access_type" "I"];
    set idmemlist [xget_memory_ranges "access_type" "ID"];
    set dmemlist [xget_memory_ranges "access_type" "D"];

    set memlist [concat $imemlist $idmemlist $dmemlist];

    while { [llength $memlist] > 3 } {
        set mem [lrange $memlist 0 4];
        set memlist [lreplace $memlist 0 4];

        if { [get_mem_size $mem] >= $memsize } {
            return 1;
        }
    }

    error "This application requires atleast $memsize bytes of memory.";
}

##############################################################

proc swapp_is_supported_hw {} {
    # check for uart peripheral
    check_stdout_hw;

    # we require atleast 30k memory
    require_memory "30000";
}

proc swapp_is_supported_sw {} {
    # check for standalone OS
    check_standalone_os;

    # check for stdout being set
    check_stdout_sw;

    # make sure xyl_oslib is available
    set librarylist [xget_sw_modules "name" "xyl_oslib"];

    if { [llength $librarylist] == 0 } {
        error "This application requires xyl_oslib library in the Board Support Package.";
    } elseif { [llength $librarylist] > 1} {
        error "Multiple xyl_oslib libraries present in the Board Support Package."
    }

}

proc generate_stdout_config { fid } {
    set stdout [get_stdout];

    # if stdout is uartlite, we don't have to generate anything
    set stdout_type [xget_ip_attribute "type" $stdout];

    if { [regexp -nocase "uartlite" $stdout_type] || [string match -nocase "mdm" $stdout_type] } {
        return;
    } elseif { [regexp -nocase "uart16550" $stdout_type] } {
	# mention that we have a 16550
        puts $fid "#define STDOUT_IS_16550";

        # and note down its base address
	set prefix "XPAR_";
	set postfix "_BASEADDR";
	set stdout_baseaddr_macro $prefix$stdout$postfix;
	set stdout_baseaddr_macro [string toupper $stdout_baseaddr_macro];
	puts $fid "#define STDOUT_BASEADDR $stdout_baseaddr_macro";
    } elseif { [regexp -nocase "ps7_uart" $stdout_type] } {
	# mention that we have a ps7_uart
        puts $fid "#define STDOUT_IS_PS7_UART";

        # and get it device id
        #set p7_uarts [lsort [xget_ips "type" "ps7_uart"]];
        #set id 0
        #foreach uart $p7_uarts {
        #    if {[string compare -nocase $uart $stdout] == 0} {
	#	puts $fid "#define UART_DEVICE_ID $id"
	#	break;
	#    }
	#    incr id
	#}
	# hack until the config table in the uart driver is fixed
	if {[string compare -nocase "ps7_uart_0" $stdout] == 0} {
	    puts $fid "#define UART_DEVICE_ID 0"
	} else {
	    puts $fid "#define UART_DEVICE_ID 1"
	}
    }
}


proc generate_cache_mask { fid } {
    set mask [format "0x%x" [xget_ppc_cache_mask]]
    puts $fid "#ifdef __PPC__"
    puts $fid "#define CACHEABLE_REGION_MASK $mask"
    puts $fid "#endif\n"
}

proc swapp_generate {} {

    set fid [open "platform_config.h" "w+"];
    puts $fid "#ifndef __PLATFORM_CONFIG_H_";
    puts $fid "#define __PLATFORM_CONFIG_H_\n";

    #puts $fid [file dirname "."];


    # if we have a uart16550 as stdout, then generate some config for that
    generate_stdout_config $fid;

    # for ppc, generate cache mask string
    generate_cache_mask $fid;

    puts $fid "#endif";
    close $fid;
}

proc swapp_get_linker_constraints {} {
    # we need a 4k stack
    #return "code_memory $code_memory data_memory $data_memory vector_section no heap 0";
    return "stack 8k heap 8k"
}
