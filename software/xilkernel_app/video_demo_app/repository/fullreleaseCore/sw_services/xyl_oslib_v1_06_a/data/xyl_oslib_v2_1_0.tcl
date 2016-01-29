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

# generates headers that internaly include headers with rel path
proc _gen_header {gen_path path name lib_handle} {
    #puts "_gen_header: $path + $name"
    set header [open "$gen_path/$name" w]
    puts $header "// generated file"
    if { [string equal "OslibFile.h" $name] } {
        set oslib_file_support [xget_value $lib_handle "PARAMETER" "OSLIB_FILE_SUPPORT"]
        if { [string equal "NO_FS" $oslib_file_support] } {
        } else {
            puts $header "#define OSLIB_ADD_FILE_SUPPORT"
            if { [string equal "FAT_FS" $oslib_file_support] } {
                puts $header "#define OSLIB_ADD_FATFS_SUPPORT"
            }
        }
    }
    puts $header "#include \"$path/$name\""
    close $header
}

# cretes folders and copies header files from src/$path to ../../include/xyl_oslib
proc _mkdir_copy_header_files {path} {
    set dstdir [file join .. .. include xyl_oslib $path]
    set srcdir [file join src $path]
    #puts "dirs $dstdir -> $srcdir, PWD: [pwd]"
    file mkdir $dstdir
    set sources [glob -join $srcdir *.h]
    # Copy each of the files in the list to dstdir
    foreach source $sources {
        file copy -force $source $dstdir
        #puts " copy $source -> $dstdir"
    }
}


proc generate {lib_handle} {

    set OSLIB possix

    puts "Run Xylon OS Library generate"

    set oslib_file_list [list "xyl_oslib/common/inc" "Oslib.h" \
                              "xyl_oslib/common/inc" "OslibTypes.h" \
                              "xyl_oslib/common/inc" "OslibHwAccess.h" \
                              "xyl_oslib/common/inc" "OslibHwResource.h" \
                              "xyl_oslib/common/inc" "OslibMemory.h" \
                              "xyl_oslib/common/inc" "OslibMisc.h" \
                              "xyl_oslib/common/inc" "OslibDebug.h" \
                              "xyl_oslib/common/inc" "OslibDma.h" \
                              "xyl_oslib/common/inc" "OslibSpinLock.h" \
                              "xyl_oslib/common/inc" "OslibTime.h" \
                              "xyl_oslib/common/inc" "OslibFile.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsDebug.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsDebug_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsHwAccess_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsMemory.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsMemory_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsMisc.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsMisc_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsDma.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsDma_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsSpinLock.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsSpinLock_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsTime.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsTime_inline.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsTypes.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsFile.h" \
                              "xyl_oslib/${OSLIB}/common/inc" "OsFile_fatfs.h" \
                              "xyl_oslib/${OSLIB}/common/src" "OsHwAccessObject.h" \
                              "xyl_oslib/cpuAccess/" "OsHwCpuResource.h" \
                              "xyl_oslib/${OSLIB}/cpu/inc" "OsHwCpuAccessConfig.h" \
                              "xyl_oslib/${OSLIB}/cpuVlink/inc" "OsHwCpuVlinkAccessConfig.h" \
                              "xyl_oslib/${OSLIB}/cpuVlink/inc" "OsHwCpuVlinkResource.h"]
    set gen_path  "src/gen"
    file mkdir $gen_path

    # generate headers in the $gen_path folder
    set i 0
    foreach {path name}  $oslib_file_list {
    #puts "$path $name is item number $i in list oslib_file_list"
    _gen_header $gen_path $path $name $lib_handle
    incr i
    }

    # copy all generated headers to the ../../include folder
    set dstdir [file join .. .. include]
    set srcdir [file join src gen]
    file mkdir $dstdir
    set sources [glob -join $srcdir *.h]
    # Copy each of the files in the list to dstdir
    foreach source $sources {
        file copy -force $source $dstdir
    }

    set oslib_cpu_support [xget_value $lib_handle "PARAMETER" "OSLIB_CPU_SUPPORT"]
    set oslib_cpu_vlink_support [xget_value $lib_handle "PARAMETER" "OSLIB_CPU_VLINK_SUPPORT"]
    set oslib_file_support [xget_value $lib_handle "PARAMETER" "OSLIB_FILE_SUPPORT"]


    # generate copy of the xyl_oslib include structure in the ../../include/xyl_oslib folder
    _mkdir_copy_header_files "common/inc"
    _mkdir_copy_header_files "${OSLIB}/common/inc"
    _mkdir_copy_header_files "${OSLIB}/common/src"
    if {$oslib_cpu_support == true} {
        _mkdir_copy_header_files "${OSLIB}/cpu/inc"
        _mkdir_copy_header_files "cpuAccess"
    }
    if {$oslib_cpu_vlink_support == true} {
        _mkdir_copy_header_files "${OSLIB}/cpuVlink/inc"
    }


    # generate makefile.include
    set mkfile [open [file join src makefile.include] w]
    puts $mkfile "# generated file"
    if {$oslib_cpu_support == true} {
        puts $mkfile "TCL_GEN_FLAGS+= -DOSLIB_ADD_CPU_SUPPORT"
        #puts $mkfile "LIB_SRCS+=possix/cpu/src/OsHwCpuAccessConfig.c"
        puts $mkfile "LIB_SRCS+=[file join . possix cpu src OsHwCpuAccessConfig.c]"
    }
    if {$oslib_cpu_vlink_support == true} {
        puts $mkfile "TCL_GEN_FLAGS+= -DOSLIB_ADD_CPU_VLINK_SUPPORT"
        #puts $mkfile "LIB_SRCS+=possix/cpuVlink/src/OsHwCpuVlinkAccessConfig.c"
        puts $mkfile "LIB_SRCS+=[file join . possix cpuVlink src OsHwCpuVlinkAccessConfig.c]"
    }

    if { [string equal "NO_FS" $oslib_file_support] } {
    } else {
        puts $mkfile "TCL_GEN_FLAGS+= -DOSLIB_ADD_FILE_SUPPORT"
        if { [string equal "FAT_FS" $oslib_file_support] } {
            puts $mkfile "TCL_GEN_FLAGS+= -DOSLIB_ADD_FATFS_SUPPORT"
        }
    }
    close $mkfile
}
