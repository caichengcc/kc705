# This is a generated script based on design: k7_emb_bist
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
#
# To test this script, run the following commands from Vivado Tcl console:
# source k7_emb_bist_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
#    create_project project_1 myproj -part xc7vx485tffg1157-1 -force

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design k7_emb_bist

# Creating design if needed
if { [get_files *.bd] eq "" } {
   puts "INFO: Currently there are no designs in project, so creating one..."
   create_bd_design k7_emb_bist
}


# Hierarchical cell: microblaze_0_local_memory
proc create_hier_cell_microblaze_0_local_memory { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_microblaze_0_local_memory() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 DLMB
  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 ILMB

  # Create pins
  create_bd_pin -dir I LMB_Clk
  create_bd_pin -dir I -from 0 -to 0 LMB_Rst

  # Create instance: dlmb, and set properties
  set dlmb [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 dlmb ]

  # Create instance: ilmb, and set properties
  set ilmb [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 ilmb ]

  # Create instance: LocalMemory_Cntlr_D, and set properties
  set LocalMemory_Cntlr_D [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 LocalMemory_Cntlr_D ]
  set_property -dict [ list CONFIG.C_ECC {0}  ] $LocalMemory_Cntlr_D

  # Create instance: LocalMemory_Cntlr_I, and set properties
  set LocalMemory_Cntlr_I [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 LocalMemory_Cntlr_I ]
  set_property -dict [ list CONFIG.C_ECC {0}  ] $LocalMemory_Cntlr_I

  # Create instance: lmb_bram, and set properties
  set lmb_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.0 lmb_bram ]
  set_property -dict [ list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.use_bram_block {BRAM_Controller}  ] $lmb_bram

  # Create interface connections
  connect_bd_intf_net -intf_net dlmb_bus [get_bd_intf_pins dlmb/LMB_Sl_0] [get_bd_intf_pins LocalMemory_Cntlr_D/SLMB]
  connect_bd_intf_net -intf_net ilmb_bus [get_bd_intf_pins ilmb/LMB_Sl_0] [get_bd_intf_pins LocalMemory_Cntlr_I/SLMB]
  connect_bd_intf_net -intf_net dlmb_port [get_bd_intf_pins LocalMemory_Cntlr_D/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTB]
  connect_bd_intf_net -intf_net ilmb_port [get_bd_intf_pins LocalMemory_Cntlr_I/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTA]
  connect_bd_intf_net -intf_net microblaze_0_dlmb [get_bd_intf_pins DLMB] [get_bd_intf_pins dlmb/LMB_M]
  connect_bd_intf_net -intf_net microblaze_0_ilmb [get_bd_intf_pins ILMB] [get_bd_intf_pins ilmb/LMB_M]

  # Create port connections
  connect_bd_net -net sys_clk_s [get_bd_pins LMB_Clk] [get_bd_pins dlmb/LMB_Clk] [get_bd_pins LocalMemory_Cntlr_D/LMB_Clk] [get_bd_pins ilmb/LMB_Clk] [get_bd_pins LocalMemory_Cntlr_I/LMB_Clk]
  connect_bd_net -net sys_bus_reset [get_bd_pins LMB_Rst] [get_bd_pins dlmb/SYS_Rst] [get_bd_pins LocalMemory_Cntlr_D/LMB_Rst] [get_bd_pins ilmb/SYS_Rst] [get_bd_pins LocalMemory_Cntlr_I/LMB_Rst]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: DDR3_Module
proc create_hier_cell_DDR3_Module { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_DDR3_Module() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR3
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI

  # Create pins
  create_bd_pin -dir O -type clk ui_clk
  create_bd_pin -dir I sys_clk_p
  create_bd_pin -dir I -from 0 -to 0 sys_rst
  create_bd_pin -dir I -from 11 -to 0 device_temp_i
  create_bd_pin -dir O -from 1 -to 0 const
  create_bd_pin -dir I -from 0 -to 0 aresetn
  create_bd_pin -dir I sys_clk_n
  create_bd_pin -dir O -from 2 -to 0 const1
  create_bd_pin -dir O init_calib_complete
  create_bd_pin -dir O -type clk ui_addn_clk_0

  # Create instance: ddr3_rank1_gnd, and set properties
  set ddr3_rank1_gnd [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 ddr3_rank1_gnd ]
  set_property -dict [ list CONFIG.CONST_WIDTH {3} CONFIG.CONST_VAL {0}  ] $ddr3_rank1_gnd

  # Create instance: DDR3_SDRAM, and set properties
  set DDR3_SDRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:2.0 DDR3_SDRAM ]
  set folder [pwd]
  set mig_file [glob $folder/../../../sources/constraints/k7_emb_bist_mig_a.prj]
  set mig_file_path [glob $mig_file]
  if { [file exists "$mig_file_path"] == 1 } { 
     set str_mig_folder [get_property IP_DIR [ get_ips [ get_property CONFIG.Component_Name $DDR3_SDRAM ] ] ]
     puts "Copying <$mig_file_path> to <$str_mig_folder/mig_b.prj>..."
     file copy -force $mig_file_path "$str_mig_folder/mig_b.prj"
  } else {
     puts "ERROR: Unable to find the PRJ file <$mig_file>!"
  }
  set_property -dict [ list CONFIG.XML_INPUT_FILE {mig_b.prj} CONFIG.RESET_BOARD_INTERFACE {Custom}  ] $DDR3_SDRAM

  # Create instance: ddr3_rank1_vcc, and set properties
  set ddr3_rank1_vcc [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 ddr3_rank1_vcc ]
  set_property -dict [ list CONFIG.CONST_WIDTH {2}  ] $ddr3_rank1_vcc

  # Create interface connections
  connect_bd_intf_net -intf_net ddr3_port [get_bd_intf_pins DDR3] [get_bd_intf_pins DDR3_SDRAM/DDR3]
  connect_bd_intf_net -intf_net axi4_0_ddr3 [get_bd_intf_pins S_AXI] [get_bd_intf_pins DDR3_SDRAM/S_AXI]

  # Create port connections
  connect_bd_net -net mig_clk_200 [get_bd_pins ui_clk] [get_bd_pins DDR3_SDRAM/ui_clk]
  connect_bd_net -net clk_in1_p_1 [get_bd_pins sys_clk_p] [get_bd_pins DDR3_SDRAM/sys_clk_p]
  connect_bd_net -net ext_reset_in_1 [get_bd_pins sys_rst] [get_bd_pins DDR3_SDRAM/sys_rst]
  connect_bd_net -net xadc_wiz_1_temp_out [get_bd_pins device_temp_i] [get_bd_pins DDR3_SDRAM/device_temp_i]
  connect_bd_net -net xlconstant_4_const [get_bd_pins const] [get_bd_pins ddr3_rank1_vcc/const]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins aresetn] [get_bd_pins DDR3_SDRAM/aresetn]
  connect_bd_net -net clk_in1_n_1 [get_bd_pins sys_clk_n] [get_bd_pins DDR3_SDRAM/sys_clk_n]
  connect_bd_net -net xlconstant_3_const [get_bd_pins const1] [get_bd_pins ddr3_rank1_gnd/const]
  connect_bd_net -net mig_7series_1_init_calib_complete [get_bd_pins init_calib_complete] [get_bd_pins DDR3_SDRAM/init_calib_complete]
  connect_bd_net -net clk_100Mhz [get_bd_pins ui_addn_clk_0] [get_bd_pins DDR3_SDRAM/ui_addn_clk_0]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: flash_interface
proc create_hier_cell_flash_interface { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_flash_interface() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi_mem

  # Create pins
  create_bd_pin -dir I -type rst s_axi_aresetn
  create_bd_pin -dir O -from 0 -to 0 mem_cen
  create_bd_pin -dir O -from 0 -to 0 mem_oen
  create_bd_pin -dir O mem_wen
  create_bd_pin -dir O -from 15 -to 0 mem_dq_o
  create_bd_pin -dir O -from 15 -to 0 mem_dq_t
  create_bd_pin -dir O -from 25 -to 0 Dout
  create_bd_pin -dir I -from 15 -to 0 mem_dq_i
  create_bd_pin -dir O mem_adv_ldn
  create_bd_pin -dir I rdclk

  # Create instance: Linear_Flash, and set properties
  set Linear_Flash [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_emc:2.0 Linear_Flash ]
  set_property -dict [ list CONFIG.C_WR_REC_TIME_MEM_0 {100000} CONFIG.C_TLZWE_PS_MEM_0 {35000} CONFIG.C_TWPH_PS_MEM_0 {12000} CONFIG.C_TWP_PS_MEM_0 {70000} CONFIG.C_TWC_PS_MEM_0 {70000} CONFIG.C_THZOE_PS_MEM_0 {7000} CONFIG.C_THZCE_PS_MEM_0 {35000} CONFIG.C_TAVDV_PS_MEM_0 {130000} CONFIG.C_TCEDV_PS_MEM_0 {130000} CONFIG.C_INCLUDE_DATAWIDTH_MATCHING_0 {1} CONFIG.C_MAX_MEM_WIDTH {16} CONFIG.C_MEM0_WIDTH {16} CONFIG.C_MEM0_TYPE {2}  ] $Linear_Flash

  # Create instance: flash_addr_slice, and set properties
  set flash_addr_slice [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 flash_addr_slice ]
  set_property -dict [ list CONFIG.DIN_TO {1} CONFIG.DIN_FROM {26}  ] $flash_addr_slice

  # Create interface connections
  connect_bd_intf_net -intf_net microblaze_system_m01_axi [get_bd_intf_pins s_axi_mem] [get_bd_intf_pins Linear_Flash/s_axi_mem]

  # Create port connections
  connect_bd_net -net axi_emc_1_mem_a1 [get_bd_pins Linear_Flash/mem_a] [get_bd_pins flash_addr_slice/Din]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins Linear_Flash/s_axi_aresetn]
  connect_bd_net -net axi_emc_1_mem_cen [get_bd_pins mem_cen] [get_bd_pins Linear_Flash/mem_cen]
  connect_bd_net -net axi_emc_1_mem_oen [get_bd_pins mem_oen] [get_bd_pins Linear_Flash/mem_oen]
  connect_bd_net -net axi_emc_1_mem_wen [get_bd_pins mem_wen] [get_bd_pins Linear_Flash/mem_wen]
  connect_bd_net -net axi_emc_1_mem_dq_o [get_bd_pins mem_dq_o] [get_bd_pins Linear_Flash/mem_dq_o]
  connect_bd_net -net axi_emc_1_mem_dq_t [get_bd_pins mem_dq_t] [get_bd_pins Linear_Flash/mem_dq_t]
  connect_bd_net -net axi_emc_1_mem_a [get_bd_pins Dout] [get_bd_pins flash_addr_slice/Dout]
  connect_bd_net -net mem_dq_i_1 [get_bd_pins mem_dq_i] [get_bd_pins Linear_Flash/mem_dq_i]
  connect_bd_net -net axi_emc_1_mem_adv_ldn [get_bd_pins mem_adv_ldn] [get_bd_pins Linear_Flash/mem_adv_ldn]
  connect_bd_net -net clk_100Mhz [get_bd_pins rdclk] [get_bd_pins Linear_Flash/rdclk] [get_bd_pins Linear_Flash/s_axi_aclk]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: GPIO_Interface
proc create_hier_cell_GPIO_Interface { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_GPIO_Interface() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi1
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio1
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi2
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio2
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi3
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio3
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi4
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio4

  # Create pins
  create_bd_pin -dir I -type rst s_axi_aresetn
  create_bd_pin -dir O -from 0 -to 0 const
  create_bd_pin -dir I -type clk s_axi_aclk

  # Create instance: LCD_GPIO, and set properties
  set LCD_GPIO [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 LCD_GPIO ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {7}  ] $LCD_GPIO

  # Create instance: ROTARY_GPIO, and set properties
  set ROTARY_GPIO [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 ROTARY_GPIO ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {3}  ] $ROTARY_GPIO

  # Create instance: Push_Buttons_5Bits, and set properties
  set Push_Buttons_5Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 Push_Buttons_5Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {5} CONFIG.C_ALL_INPUTS {1}  ] $Push_Buttons_5Bits

  # Create instance: DIP_Switches_4Bits, and set properties
  set DIP_Switches_4Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 DIP_Switches_4Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {4} CONFIG.C_ALL_INPUTS {1}  ] $DIP_Switches_4Bits

  # Create instance: LEDs_8Bits, and set properties
  set LEDs_8Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 LEDs_8Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {8} CONFIG.C_ALL_OUTPUTS {1}  ] $LEDs_8Bits

  # Create instance: fan_vcc, and set properties
  set fan_vcc [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 fan_vcc ]

  # Create interface connections
  connect_bd_intf_net -intf_net microblaze_system_m03_axi [get_bd_intf_pins s_axi] [get_bd_intf_pins LCD_GPIO/s_axi]
  connect_bd_intf_net -intf_net lcd_gpio_port [get_bd_intf_pins gpio] [get_bd_intf_pins LCD_GPIO/gpio]
  connect_bd_intf_net -intf_net microblaze_system_m04_axi [get_bd_intf_pins s_axi1] [get_bd_intf_pins ROTARY_GPIO/s_axi]
  connect_bd_intf_net -intf_net rotary_gpio_port [get_bd_intf_pins gpio1] [get_bd_intf_pins ROTARY_GPIO/gpio]
  connect_bd_intf_net -intf_net microblaze_system_m08_axi [get_bd_intf_pins s_axi2] [get_bd_intf_pins Push_Buttons_5Bits/s_axi]
  connect_bd_intf_net -intf_net push_buttons_5bits_gpio [get_bd_intf_pins gpio2] [get_bd_intf_pins Push_Buttons_5Bits/gpio]
  connect_bd_intf_net -intf_net microblaze_system_m05_axi [get_bd_intf_pins s_axi3] [get_bd_intf_pins DIP_Switches_4Bits/s_axi]
  connect_bd_intf_net -intf_net dip_switches_4bits_gpio [get_bd_intf_pins gpio3] [get_bd_intf_pins DIP_Switches_4Bits/gpio]
  connect_bd_intf_net -intf_net microblaze_system_m02_axi [get_bd_intf_pins s_axi4] [get_bd_intf_pins LEDs_8Bits/s_axi]
  connect_bd_intf_net -intf_net leds_8bits_gpio [get_bd_intf_pins gpio4] [get_bd_intf_pins LEDs_8Bits/gpio]

  # Create port connections
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins LEDs_8Bits/s_axi_aresetn] [get_bd_pins Push_Buttons_5Bits/s_axi_aresetn] [get_bd_pins DIP_Switches_4Bits/s_axi_aresetn] [get_bd_pins ROTARY_GPIO/s_axi_aresetn] [get_bd_pins LCD_GPIO/s_axi_aresetn]
  connect_bd_net -net xlconstant_1_const [get_bd_pins const] [get_bd_pins fan_vcc/const]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_aclk] [get_bd_pins LEDs_8Bits/s_axi_aclk] [get_bd_pins Push_Buttons_5Bits/s_axi_aclk] [get_bd_pins DIP_Switches_4Bits/s_axi_aclk] [get_bd_pins ROTARY_GPIO/s_axi_aclk] [get_bd_pins LCD_GPIO/s_axi_aclk]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: microblaze_sybsystem
proc create_hier_cell_microblaze_sybsystem { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_microblaze_sybsystem() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_DC
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_IC
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M01_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M05_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M06_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M03_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M02_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M08_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M07_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M04_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M09_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M13_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M14_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M11_AXI
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M12_AXI

  # Create pins
  create_bd_pin -dir I LMB_Clk
  create_bd_pin -dir I -from 0 -to 0 LMB_Rst
  create_bd_pin -dir I -type rst processor_rst
  create_bd_pin -dir I -from 0 -to 0 -type rst peripheral_aresetn
  create_bd_pin -dir O Debug_SYS_Rst
  create_bd_pin -dir I -from 0 -to 0 Interconnect_aresetn
  create_bd_pin -dir I Lite_Slave_ACLK
  create_bd_pin -dir I lite_Master_ACLK
  create_bd_pin -dir I -from 0 -to 0 interrupt_4
  create_bd_pin -dir I -from 0 -to 0 interrupt_5
  create_bd_pin -dir I -from 0 -to 0 interrupt_6
  create_bd_pin -dir I -from 0 -to 0 interrupt_7
  create_bd_pin -dir I -from 0 -to 0 interrupt_0
  create_bd_pin -dir I -from 0 -to 0 interrupt_2
  create_bd_pin -dir I -from 0 -to 0 interrupt_1
  create_bd_pin -dir I -from 0 -to 0 interrupt_3

  # Create instance: microblaze_0, and set properties
  set microblaze_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.1 microblaze_0 ]
  set_property -dict [ list CONFIG.C_FAULT_TOLERANT {0} CONFIG.C_D_AXI {1} CONFIG.C_D_LMB {1} CONFIG.C_I_LMB {1} CONFIG.C_DEBUG_ENABLED {1} CONFIG.C_ICACHE_BASEADDR {0x80000000} CONFIG.C_ICACHE_HIGHADDR {0xFFFFFFFF} CONFIG.C_USE_ICACHE {1} CONFIG.C_ICACHE_LINE_LEN {8} CONFIG.C_ICACHE_ALWAYS_USED {1} CONFIG.C_ICACHE_FORCE_TAG_LUTRAM {1} CONFIG.C_DCACHE_BASEADDR {0x80000000} CONFIG.C_DCACHE_HIGHADDR {0xFFFFFFFF} CONFIG.C_USE_DCACHE {1} CONFIG.C_DCACHE_LINE_LEN {8} CONFIG.C_DCACHE_ALWAYS_USED {1} CONFIG.C_DCACHE_FORCE_TAG_LUTRAM {1}  ] $microblaze_0

  # Create instance: debug_module, and set properties
  set debug_module [ create_bd_cell -type ip -vlnv xilinx.com:ip:mdm:3.0 debug_module ]
  set_property -dict [ list CONFIG.C_USE_UART {1}  ] $debug_module

  # Create instance: Interrupt_Cntlr, and set properties
  set Interrupt_Cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:3.1 Interrupt_Cntlr ]
  set_property -dict [ list CONFIG.C_HAS_FAST {0}  ] $Interrupt_Cntlr

  # Create instance: microblaze_0_local_memory
  create_hier_cell_microblaze_0_local_memory $hier_obj microblaze_0_local_memory

  # Create instance: axi4lite_0, and set properties
  set axi4lite_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi4lite_0 ]
  set_property -dict [ list CONFIG.NUM_MI {15}  ] $axi4lite_0

  # Create instance: int_ctrl_concat, and set properties
  set int_ctrl_concat [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 int_ctrl_concat ]
  set_property -dict [ list CONFIG.NUM_PORTS {8}  ] $int_ctrl_concat

  # Create interface connections
  connect_bd_intf_net -intf_net axi_intc_1_interrupt [get_bd_intf_pins Interrupt_Cntlr/interrupt] [get_bd_intf_pins microblaze_0/INTERRUPT]
  connect_bd_intf_net -intf_net microblaze_0_dlmb [get_bd_intf_pins microblaze_0/DLMB] [get_bd_intf_pins microblaze_0_local_memory/DLMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb [get_bd_intf_pins microblaze_0/ILMB] [get_bd_intf_pins microblaze_0_local_memory/ILMB]
  connect_bd_intf_net -intf_net microblaze_0_debug [get_bd_intf_pins debug_module/MBDEBUG_0] [get_bd_intf_pins microblaze_0/DEBUG]
  connect_bd_intf_net -intf_net axi4lite_0_m00_axi [get_bd_intf_pins debug_module/S_AXI] [get_bd_intf_pins axi4lite_0/M00_AXI]
  connect_bd_intf_net -intf_net microblaze_0_m_axi_dp [get_bd_intf_pins microblaze_0/M_AXI_DP] [get_bd_intf_pins axi4lite_0/S00_AXI]
  connect_bd_intf_net -intf_net axi4lite_0_m10_axi [get_bd_intf_pins Interrupt_Cntlr/s_axi] [get_bd_intf_pins axi4lite_0/M10_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_dc [get_bd_intf_pins M_AXI_DC] [get_bd_intf_pins microblaze_0/M_AXI_DC]
  connect_bd_intf_net -intf_net axi_mm_mb_ic [get_bd_intf_pins M_AXI_IC] [get_bd_intf_pins microblaze_0/M_AXI_IC]
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins M01_AXI] [get_bd_intf_pins axi4lite_0/M01_AXI]
  connect_bd_intf_net -intf_net Conn2 [get_bd_intf_pins M05_AXI] [get_bd_intf_pins axi4lite_0/M05_AXI]
  connect_bd_intf_net -intf_net Conn3 [get_bd_intf_pins M06_AXI] [get_bd_intf_pins axi4lite_0/M06_AXI]
  connect_bd_intf_net -intf_net Conn4 [get_bd_intf_pins M03_AXI] [get_bd_intf_pins axi4lite_0/M03_AXI]
  connect_bd_intf_net -intf_net Conn5 [get_bd_intf_pins M02_AXI] [get_bd_intf_pins axi4lite_0/M02_AXI]
  connect_bd_intf_net -intf_net Conn6 [get_bd_intf_pins M08_AXI] [get_bd_intf_pins axi4lite_0/M08_AXI]
  connect_bd_intf_net -intf_net Conn7 [get_bd_intf_pins M07_AXI] [get_bd_intf_pins axi4lite_0/M07_AXI]
  connect_bd_intf_net -intf_net Conn8 [get_bd_intf_pins M04_AXI] [get_bd_intf_pins axi4lite_0/M04_AXI]
  connect_bd_intf_net -intf_net Conn9 [get_bd_intf_pins M09_AXI] [get_bd_intf_pins axi4lite_0/M09_AXI]
  connect_bd_intf_net -intf_net Conn10 [get_bd_intf_pins M13_AXI] [get_bd_intf_pins axi4lite_0/M13_AXI]
  connect_bd_intf_net -intf_net Conn11 [get_bd_intf_pins M14_AXI] [get_bd_intf_pins axi4lite_0/M14_AXI]
  connect_bd_intf_net -intf_net Conn12 [get_bd_intf_pins M11_AXI] [get_bd_intf_pins axi4lite_0/M11_AXI]
  connect_bd_intf_net -intf_net Conn13 [get_bd_intf_pins M12_AXI] [get_bd_intf_pins axi4lite_0/M12_AXI]

  # Create port connections
  connect_bd_net -net clk_150Mhz [get_bd_pins LMB_Clk] [get_bd_pins microblaze_0_local_memory/LMB_Clk] [get_bd_pins Interrupt_Cntlr/processor_clk] [get_bd_pins microblaze_0/Clk]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins LMB_Rst] [get_bd_pins microblaze_0_local_memory/LMB_Rst]
  connect_bd_net -net proc_sys_reset_1_mb_reset [get_bd_pins processor_rst] [get_bd_pins Interrupt_Cntlr/processor_rst] [get_bd_pins microblaze_0/Reset]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins peripheral_aresetn] [get_bd_pins Interrupt_Cntlr/s_axi_aresetn] [get_bd_pins debug_module/S_AXI_ARESETN]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins Debug_SYS_Rst] [get_bd_pins debug_module/Debug_SYS_Rst]
  connect_bd_net -net aresetn_1 [get_bd_pins Interconnect_aresetn] [get_bd_pins axi4lite_0/ARESETN] [get_bd_pins axi4lite_0/S00_ARESETN] [get_bd_pins axi4lite_0/M00_ARESETN] [get_bd_pins axi4lite_0/M01_ARESETN] [get_bd_pins axi4lite_0/M02_ARESETN] [get_bd_pins axi4lite_0/M03_ARESETN] [get_bd_pins axi4lite_0/M04_ARESETN] [get_bd_pins axi4lite_0/M05_ARESETN] [get_bd_pins axi4lite_0/M06_ARESETN] [get_bd_pins axi4lite_0/M07_ARESETN] [get_bd_pins axi4lite_0/M08_ARESETN] [get_bd_pins axi4lite_0/M09_ARESETN] [get_bd_pins axi4lite_0/M10_ARESETN] [get_bd_pins axi4lite_0/M11_ARESETN] [get_bd_pins axi4lite_0/M12_ARESETN] [get_bd_pins axi4lite_0/M13_ARESETN] [get_bd_pins axi4lite_0/M14_ARESETN]
  connect_bd_net -net s00_aclk_1 [get_bd_pins Lite_Slave_ACLK] [get_bd_pins axi4lite_0/S00_ACLK]
  connect_bd_net -net aclk_1 [get_bd_pins lite_Master_ACLK] [get_bd_pins axi4lite_0/ACLK] [get_bd_pins axi4lite_0/M00_ACLK] [get_bd_pins axi4lite_0/M01_ACLK] [get_bd_pins axi4lite_0/M02_ACLK] [get_bd_pins axi4lite_0/M03_ACLK] [get_bd_pins axi4lite_0/M04_ACLK] [get_bd_pins axi4lite_0/M05_ACLK] [get_bd_pins axi4lite_0/M06_ACLK] [get_bd_pins axi4lite_0/M07_ACLK] [get_bd_pins axi4lite_0/M08_ACLK] [get_bd_pins axi4lite_0/M09_ACLK] [get_bd_pins axi4lite_0/M10_ACLK] [get_bd_pins axi4lite_0/M11_ACLK] [get_bd_pins axi4lite_0/M12_ACLK] [get_bd_pins axi4lite_0/M13_ACLK] [get_bd_pins axi4lite_0/M14_ACLK] [get_bd_pins Interrupt_Cntlr/s_axi_aclk] [get_bd_pins debug_module/S_AXI_ACLK]
  connect_bd_net -net in4_1 [get_bd_pins interrupt_4] [get_bd_pins int_ctrl_concat/In4]
  connect_bd_net -net in5_1 [get_bd_pins interrupt_5] [get_bd_pins int_ctrl_concat/In5]
  connect_bd_net -net in6_1 [get_bd_pins interrupt_6] [get_bd_pins int_ctrl_concat/In6]
  connect_bd_net -net in7_1 [get_bd_pins interrupt_7] [get_bd_pins int_ctrl_concat/In7]
  connect_bd_net -net int_ctrl_concat_dout [get_bd_pins int_ctrl_concat/dout] [get_bd_pins Interrupt_Cntlr/intr]
  connect_bd_net -net in0_1 [get_bd_pins interrupt_0] [get_bd_pins int_ctrl_concat/In0]
  connect_bd_net -net in2_1 [get_bd_pins interrupt_2] [get_bd_pins int_ctrl_concat/In2]
  connect_bd_net -net in1_1 [get_bd_pins interrupt_1] [get_bd_pins int_ctrl_concat/In1]
  connect_bd_net -net in3_1 [get_bd_pins interrupt_3] [get_bd_pins int_ctrl_concat/In3]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: BRAM
proc create_hier_cell_BRAM { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_BRAM() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI

  # Create pins
  create_bd_pin -dir I -type clk S_AXI_ACLK
  create_bd_pin -dir I -type rst S_AXI_ARESETN

  # Create instance: Internal_BRAM, and set properties
  set Internal_BRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:3.0 Internal_BRAM ]
  set_property -dict [ list CONFIG.SUPPORTS_NARROW_BURST {0}  ] $Internal_BRAM

  # Create instance: Internal_BRAM_block, and set properties
  set Internal_BRAM_block [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.0 Internal_BRAM_block ]
  set_property -dict [ list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Enable_32bit_Address {true} CONFIG.use_bram_block {BRAM_Controller}  ] $Internal_BRAM_block

  # Create interface connections
  connect_bd_intf_net -intf_net internal_bram_bram_porta [get_bd_intf_pins Internal_BRAM_block/BRAM_PORTA] [get_bd_intf_pins Internal_BRAM/BRAM_PORTA]
  connect_bd_intf_net -intf_net internal_bram_bram_portb [get_bd_intf_pins Internal_BRAM_block/BRAM_PORTB] [get_bd_intf_pins Internal_BRAM/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi_mm_mb_m00_axi [get_bd_intf_pins S_AXI] [get_bd_intf_pins Internal_BRAM/S_AXI]

  # Create port connections
  connect_bd_net -net clk_100Mhz [get_bd_pins S_AXI_ACLK] [get_bd_pins Internal_BRAM/S_AXI_ACLK]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins S_AXI_ARESETN] [get_bd_pins Internal_BRAM/S_AXI_ARESETN]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: Ethernet_Controller
proc create_hier_cell_Ethernet_Controller { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_Ethernet_Controller() - Empty argument(s)!"
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI_LITE
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_SG
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_MM2S
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gmii_rtl:1.0 gmii

  # Create pins
  create_bd_pin -dir I -type clk m_axi_mm2s_aclk
  create_bd_pin -dir I -type clk ref_clk
  create_bd_pin -dir O -type intr mm2s_introut
  create_bd_pin -dir IO mdio
  create_bd_pin -dir I -type clk gtx_clk
  create_bd_pin -dir O mdc
  create_bd_pin -dir I -from 0 -to 0 -type rst s_axi_lite_resetn
  create_bd_pin -dir I -type clk s_axi_lite_clk
  create_bd_pin -dir O interrupt
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir O phy_rst_n

  # Create instance: AXI_DMA_Ethernet, and set properties
  set AXI_DMA_Ethernet [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.0 AXI_DMA_Ethernet ]
  set_property -dict [ list CONFIG.c_sg_length_width {16} CONFIG.c_include_mm2s_dre {1} CONFIG.c_sg_use_stsapp_length {1} CONFIG.c_include_s2mm_dre {1}  ] $AXI_DMA_Ethernet

  # Create instance: Soft_Ethernet_MAC_0, and set properties
  set Soft_Ethernet_MAC_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_ethernet:5.0 Soft_Ethernet_MAC_0 ]
  set_property -dict [ list CONFIG.Statistics_Counters {false}  ] $Soft_Ethernet_MAC_0

  # Create interface connections
  connect_bd_intf_net -intf_net ethernet_txd [get_bd_intf_pins AXI_DMA_Ethernet/M_AXIS_MM2S] [get_bd_intf_pins Soft_Ethernet_MAC_0/axi_str_txd]
  connect_bd_intf_net -intf_net ethernet_rxd [get_bd_intf_pins Soft_Ethernet_MAC_0/axi_str_rxd] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net ethernet_cntrl [get_bd_intf_pins AXI_DMA_Ethernet/M_AXIS_CNTRL] [get_bd_intf_pins Soft_Ethernet_MAC_0/axi_str_txc]
  connect_bd_intf_net -intf_net ethernet_sts [get_bd_intf_pins Soft_Ethernet_MAC_0/axi_str_rxs] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXIS_STS]
  connect_bd_intf_net -intf_net axi4lite_0_dma [get_bd_intf_pins S_AXI_LITE] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXI_LITE]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_sg [get_bd_intf_pins M_AXI_SG] [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_SG]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_mm2s [get_bd_intf_pins M_AXI_MM2S] [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_MM2S]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_S2MM]
  connect_bd_intf_net -intf_net microblaze_0_ethernet_axi [get_bd_intf_pins s_axi] [get_bd_intf_pins Soft_Ethernet_MAC_0/s_axi]
  connect_bd_intf_net -intf_net soft_ethernet_mac_gmii [get_bd_intf_pins gmii] [get_bd_intf_pins Soft_Ethernet_MAC_0/gmii]

  # Create port connections
  connect_bd_net -net AXI_STR_TXD_ARESETN [get_bd_pins AXI_DMA_Ethernet/mm2s_prmry_reset_out_n] [get_bd_pins Soft_Ethernet_MAC_0/axi_txd_arstn]
  connect_bd_net -net AXI_STR_TXC_ARESETN [get_bd_pins AXI_DMA_Ethernet/mm2s_cntrl_reset_out_n] [get_bd_pins Soft_Ethernet_MAC_0/axi_txc_arstn]
  connect_bd_net -net AXI_STR_RXD_ARESETN [get_bd_pins AXI_DMA_Ethernet/s2mm_prmry_reset_out_n] [get_bd_pins Soft_Ethernet_MAC_0/axi_rxd_arstn]
  connect_bd_net -net AXI_STR_RXS_ARESETN [get_bd_pins AXI_DMA_Ethernet/s2mm_sts_reset_out_n] [get_bd_pins Soft_Ethernet_MAC_0/axi_rxs_arstn]
  connect_bd_net -net clk_150Mhz [get_bd_pins m_axi_mm2s_aclk] [get_bd_pins AXI_DMA_Ethernet/m_axi_mm2s_aclk] [get_bd_pins AXI_DMA_Ethernet/m_axi_s2mm_aclk] [get_bd_pins Soft_Ethernet_MAC_0/axis_clk] [get_bd_pins AXI_DMA_Ethernet/m_axi_sg_aclk]
  connect_bd_net -net mig_clk_200 [get_bd_pins ref_clk] [get_bd_pins Soft_Ethernet_MAC_0/ref_clk]
  connect_bd_net -net dma_mm2s_introut [get_bd_pins mm2s_introut] [get_bd_pins AXI_DMA_Ethernet/mm2s_introut]
  connect_bd_net -net Net [get_bd_pins mdio] [get_bd_pins Soft_Ethernet_MAC_0/mdio]
  connect_bd_net -net ethernet_clk_125 [get_bd_pins gtx_clk] [get_bd_pins Soft_Ethernet_MAC_0/gtx_clk]
  connect_bd_net -net soft_ethernet_mac_mdc [get_bd_pins mdc] [get_bd_pins Soft_Ethernet_MAC_0/mdc]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_lite_resetn] [get_bd_pins Soft_Ethernet_MAC_0/s_axi_lite_resetn] [get_bd_pins AXI_DMA_Ethernet/axi_resetn]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_lite_clk] [get_bd_pins Soft_Ethernet_MAC_0/s_axi_lite_clk] [get_bd_pins AXI_DMA_Ethernet/s_axi_lite_aclk]
  connect_bd_net -net axi_ethernet_interrupt [get_bd_pins interrupt] [get_bd_pins Soft_Ethernet_MAC_0/interrupt]
  connect_bd_net -net dma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins AXI_DMA_Ethernet/s2mm_introut]
  connect_bd_net -net soft_ethernet_mac_phy_rst_n [get_bd_pins phy_rst_n] [get_bd_pins Soft_Ethernet_MAC_0/phy_rst_n]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}


# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set LCD_GPIO_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 LCD_GPIO_IO ]
  set ROTARY_GPIO_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 ROTARY_GPIO_IO ]
  set IIC_MAIN [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:iic_rtl:1.0 IIC_MAIN ]
  set DDR3 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR3 ]
  set Soft_Ethernet_MAC [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gmii_rtl:1.0 Soft_Ethernet_MAC ]
  set Push_Buttons_5Bits_TRI_I [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 Push_Buttons_5Bits_TRI_I ]
  set LEDs_8Bits_TRI_O [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 LEDs_8Bits_TRI_O ]
  set DIP_Switches_TRI_I [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 DIP_Switches_TRI_I ]

  # Create ports
  set RESET [ create_bd_port -dir I -type rst RESET ]
  set_property -dict [ list CONFIG.POLARITY {ACTIVE_HIGH}  ] $RESET
  set RS232_UART_1_sout [ create_bd_port -dir O RS232_UART_1_sout ]
  set RS232_UART_1_sin [ create_bd_port -dir I RS232_UART_1_sin ]
  set Linear_Flash_we_n [ create_bd_port -dir O Linear_Flash_we_n ]
  set Linear_Flash_oe_n [ create_bd_port -dir O -from 0 -to 0 Linear_Flash_oe_n ]
  set Linear_Flash_ce_n [ create_bd_port -dir O -from 0 -to 0 Linear_Flash_ce_n ]
  set Linear_Flash_Data_T [ create_bd_port -dir O -from 15 -to 0 Linear_Flash_Data_T ]
  set Linear_Flash_Data_O [ create_bd_port -dir O -from 15 -to 0 Linear_Flash_Data_O ]
  set Linear_Flash_Data_I [ create_bd_port -dir I -from 15 -to 0 Linear_Flash_Data_I ]
  set Linear_Flash_address [ create_bd_port -dir O -from 0 -to 25 Linear_Flash_address ]
  set Linear_Flash_adv_ldn [ create_bd_port -dir O Linear_Flash_adv_ldn ]
  set IIC_MUX_RESET_B [ create_bd_port -dir O -from 0 -to 0 IIC_MUX_RESET_B ]
  set sm_fan_pwm [ create_bd_port -dir O -from 0 -to 0 sm_fan_pwm ]
  set Soft_Ethernet_MAC_MDIO_pin [ create_bd_port -dir IO Soft_Ethernet_MAC_MDIO_pin ]
  set Soft_Ethernet_MAC_MDC_pin [ create_bd_port -dir O Soft_Ethernet_MAC_MDC_pin ]
  set sd_clk [ create_bd_port -dir O sd_clk ]
  set sd_dat_o [ create_bd_port -dir O -from 3 -to 0 sd_dat_o ]
  set sd_dat_t [ create_bd_port -dir O -from 3 -to 0 sd_dat_t ]
  set sd_dat_i [ create_bd_port -dir I -from 3 -to 0 sd_dat_i ]
  set sd_wp [ create_bd_port -dir I sd_wp ]
  set sd_cd_n [ create_bd_port -dir I sd_cd_n ]
  set sd_cmd_i [ create_bd_port -dir I sd_cmd_i ]
  set sd_cmd_o [ create_bd_port -dir O sd_cmd_o ]
  set sd_cmd_t [ create_bd_port -dir O sd_cmd_t ]
  set axi_xadc_0_VAUXP [ create_bd_port -dir I axi_xadc_0_VAUXP ]
  set axi_xadc_0_VAUXN [ create_bd_port -dir I axi_xadc_0_VAUXN ]
  set axi_xadc_8_VAUXP [ create_bd_port -dir I axi_xadc_8_VAUXP ]
  set axi_xadc_8_VAUXN [ create_bd_port -dir I axi_xadc_8_VAUXN ]
  set Soft_Ethernet_MAC_RST_n_pin [ create_bd_port -dir O Soft_Ethernet_MAC_RST_n_pin ]
  set init_calib_complete [ create_bd_port -dir O init_calib_complete ]
  set ddr3_1_n [ create_bd_port -dir O -from 2 -to 0 ddr3_1_n ]
  set CLK_N [ create_bd_port -dir I CLK_N ]
  set ddr3_1_p [ create_bd_port -dir O -from 1 -to 0 ddr3_1_p ]
  set CLK_P [ create_bd_port -dir I CLK_P ]

  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]

  # Create instance: clock_generator_1, and set properties
  set clock_generator_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.0 clock_generator_1 ]
  set_property -dict [ list CONFIG.PRIM_IN_FREQ {200.000} CONFIG.CLKOUT2_USED {true} CONFIG.CLKOUT3_USED {false} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {150.000} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {125.000} CONFIG.CLKOUT3_REQUESTED_OUT_FREQ {100.000} CONFIG.PRIM_SOURCE {No_buffer} CONFIG.CLKOUT2_DRIVES {No_buffer} CONFIG.USE_RESET {false}  ] $clock_generator_1

  # Create instance: RS232_Uart_1, and set properties
  set RS232_Uart_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uart16550:2.0 RS232_Uart_1 ]

  # Create instance: Dual_Timer_Counter, and set properties
  set Dual_Timer_Counter [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 Dual_Timer_Counter ]

  # Create instance: IIC_EEPROM, and set properties
  set IIC_EEPROM [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_iic:2.0 IIC_EEPROM ]

  # Create instance: axi_mm_mb, and set properties
  set axi_mm_mb [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi_mm_mb ]
  set_property -dict [ list CONFIG.NUM_SI {5} CONFIG.NUM_MI {2} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {32} CONFIG.M00_HAS_REGSLICE {1} CONFIG.M01_HAS_REGSLICE {1} CONFIG.S01_HAS_REGSLICE {1} CONFIG.S02_HAS_REGSLICE {1} CONFIG.S03_HAS_REGSLICE {1} CONFIG.S04_HAS_REGSLICE {1}  ] $axi_mm_mb

  # Create instance: axi4_0, and set properties
  set axi4_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi4_0 ]
  set_property -dict [ list CONFIG.NUM_MI {1} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {512}  ] $axi4_0

  # Create instance: logisdhc_0, and set properties
  set logisdhc_0 [ create_bd_cell -type ip -vlnv xylon:logicbricks:logisdhc:1.07.a logisdhc_0 ]
  set_property -dict [ list CONFIG.C_USE_DMA {0}  ] $logisdhc_0

  # Create instance: axi_xadc_0, and set properties
  set axi_xadc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xadc_wiz:3.0 axi_xadc_0 ]
  set_property -dict [ list CONFIG.INTERFACE_SELECTION {Enable_AXI} CONFIG.XADC_STARUP_SELECTION {channel_sequencer} CONFIG.CHANNEL_ENABLE_VAUXP0_VAUXN0 {true} CONFIG.CHANNEL_ENABLE_VAUXP8_VAUXN8 {true} CONFIG.AVERAGE_ENABLE_VAUXP0_VAUXN0 {false} CONFIG.ENABLE_TEMP_BUS {true}  ] $axi_xadc_0

  # Create instance: Ethernet_Controller
  create_hier_cell_Ethernet_Controller [current_bd_instance .] Ethernet_Controller

  # Create instance: BRAM
  create_hier_cell_BRAM [current_bd_instance .] BRAM

  # Create instance: microblaze_sybsystem
  create_hier_cell_microblaze_sybsystem [current_bd_instance .] microblaze_sybsystem

  # Create instance: GPIO_Interface
  create_hier_cell_GPIO_Interface [current_bd_instance .] GPIO_Interface

  # Create instance: flash_interface
  create_hier_cell_flash_interface [current_bd_instance .] flash_interface

  # Create instance: DDR3_Module
  create_hier_cell_DDR3_Module [current_bd_instance .] DDR3_Module

  # Create interface connections
  connect_bd_intf_net -intf_net axi_mm_mb_dma_sg [get_bd_intf_pins Ethernet_Controller/M_AXI_SG] [get_bd_intf_pins axi_mm_mb/S02_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_mm2s [get_bd_intf_pins Ethernet_Controller/M_AXI_MM2S] [get_bd_intf_pins axi_mm_mb/S03_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_s2mm [get_bd_intf_pins Ethernet_Controller/M_AXI_S2MM] [get_bd_intf_pins axi_mm_mb/S04_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_m01_axi [get_bd_intf_pins axi_mm_mb/M01_AXI] [get_bd_intf_pins axi4_0/S00_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_m00_axi [get_bd_intf_pins axi_mm_mb/M00_AXI] [get_bd_intf_pins BRAM/S_AXI]
  connect_bd_intf_net -intf_net s_axi_1 [get_bd_intf_pins Ethernet_Controller/s_axi] [get_bd_intf_pins microblaze_sybsystem/M12_AXI]
  connect_bd_intf_net -intf_net s_axi_lite_1 [get_bd_intf_pins Ethernet_Controller/S_AXI_LITE] [get_bd_intf_pins microblaze_sybsystem/M11_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m14_axi [get_bd_intf_pins logisdhc_0/s_axi] [get_bd_intf_pins microblaze_sybsystem/M14_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m13_axi [get_bd_intf_pins axi_xadc_0/s_axi_lite] [get_bd_intf_pins microblaze_sybsystem/M13_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m09_axi [get_bd_intf_pins RS232_Uart_1/s_axi] [get_bd_intf_pins microblaze_sybsystem/M09_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m07_axi [get_bd_intf_pins Dual_Timer_Counter/s_axi] [get_bd_intf_pins microblaze_sybsystem/M07_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m01_axi [get_bd_intf_pins flash_interface/s_axi_mem] [get_bd_intf_pins microblaze_sybsystem/M01_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_ic [get_bd_intf_pins axi_mm_mb/S00_AXI] [get_bd_intf_pins microblaze_sybsystem/M_AXI_IC]
  connect_bd_intf_net -intf_net axi_mm_mb_dc [get_bd_intf_pins axi_mm_mb/S01_AXI] [get_bd_intf_pins microblaze_sybsystem/M_AXI_DC]
  connect_bd_intf_net -intf_net axi4_0_ddr3 [get_bd_intf_pins axi4_0/M00_AXI] [get_bd_intf_pins DDR3_Module/S_AXI]
  connect_bd_intf_net -intf_net microblaze_system_m05_axi [get_bd_intf_pins microblaze_sybsystem/M05_AXI] [get_bd_intf_pins GPIO_Interface/s_axi]
  connect_bd_intf_net -intf_net microblaze_system_m06_axi [get_bd_intf_pins microblaze_sybsystem/M06_AXI] [get_bd_intf_pins GPIO_Interface/s_axi1]
  connect_bd_intf_net -intf_net microblaze_system_m03_axi [get_bd_intf_pins microblaze_sybsystem/M03_AXI] [get_bd_intf_pins GPIO_Interface/s_axi2]
  connect_bd_intf_net -intf_net microblaze_system_m02_axi [get_bd_intf_pins microblaze_sybsystem/M02_AXI] [get_bd_intf_pins GPIO_Interface/s_axi3]
  connect_bd_intf_net -intf_net microblaze_system_m08_axi [get_bd_intf_pins microblaze_sybsystem/M08_AXI] [get_bd_intf_pins GPIO_Interface/s_axi4]
  connect_bd_intf_net -intf_net microblaze_system_m04_axi [get_bd_intf_pins microblaze_sybsystem/M04_AXI] [get_bd_intf_pins IIC_EEPROM/s_axi]
  connect_bd_intf_net -intf_net lcd_gpio_port [get_bd_intf_ports LCD_GPIO_IO] [get_bd_intf_pins GPIO_Interface/gpio]
  connect_bd_intf_net -intf_net rotary_gpio_port [get_bd_intf_ports ROTARY_GPIO_IO] [get_bd_intf_pins GPIO_Interface/gpio1]
  connect_bd_intf_net -intf_net eeprom_port [get_bd_intf_ports IIC_MAIN] [get_bd_intf_pins IIC_EEPROM/iic]
  connect_bd_intf_net -intf_net ddr3_port [get_bd_intf_ports DDR3] [get_bd_intf_pins DDR3_Module/DDR3]
  connect_bd_intf_net -intf_net soft_ethernet_mac_gmii [get_bd_intf_ports Soft_Ethernet_MAC] [get_bd_intf_pins Ethernet_Controller/gmii]
  connect_bd_intf_net -intf_net push_buttons_5bits_gpio [get_bd_intf_ports Push_Buttons_5Bits_TRI_I] [get_bd_intf_pins GPIO_Interface/gpio2]
  connect_bd_intf_net -intf_net leds_8bits_gpio [get_bd_intf_ports LEDs_8Bits_TRI_O] [get_bd_intf_pins GPIO_Interface/gpio4]
  connect_bd_intf_net -intf_net dip_switches_4bits_gpio [get_bd_intf_ports DIP_Switches_TRI_I] [get_bd_intf_pins GPIO_Interface/gpio3]

  # Create port connections
  connect_bd_net -net clk_wiz_1_locked [get_bd_pins clock_generator_1/locked] [get_bd_pins proc_sys_reset_0/dcm_locked] [get_bd_pins logisdhc_0/sd_base_lock]
  connect_bd_net -net clk_100Mhz [get_bd_pins DDR3_Module/ui_addn_clk_0] [get_bd_pins proc_sys_reset_0/slowest_sync_clk] [get_bd_pins RS232_Uart_1/s_axi_aclk] [get_bd_pins Dual_Timer_Counter/s_axi_aclk] [get_bd_pins IIC_EEPROM/s_axi_aclk] [get_bd_pins axi_mm_mb/M00_ACLK] [get_bd_pins logisdhc_0/S_AXI_ACLK] [get_bd_pins logisdhc_0/sd_base_clk] [get_bd_pins axi_xadc_0/s_axi_aclk] [get_bd_pins Ethernet_Controller/s_axi_lite_clk] [get_bd_pins BRAM/S_AXI_ACLK] [get_bd_pins microblaze_sybsystem/lite_Master_ACLK] [get_bd_pins GPIO_Interface/s_axi_aclk] [get_bd_pins flash_interface/rdclk]
  connect_bd_net -net proc_sys_reset_1_interconnect_aresetn [get_bd_pins proc_sys_reset_0/interconnect_aresetn] [get_bd_pins axi_mm_mb/ARESETN] [get_bd_pins axi4_0/ARESETN] [get_bd_pins microblaze_sybsystem/Interconnect_aresetn]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins proc_sys_reset_0/peripheral_aresetn] [get_bd_pins RS232_Uart_1/s_axi_aresetn] [get_bd_pins Dual_Timer_Counter/s_axi_aresetn] [get_bd_pins IIC_EEPROM/s_axi_aresetn] [get_bd_pins axi_mm_mb/S00_ARESETN] [get_bd_pins axi_mm_mb/S01_ARESETN] [get_bd_pins axi_mm_mb/M00_ARESETN] [get_bd_pins axi_mm_mb/M01_ARESETN] [get_bd_pins axi4_0/M00_ARESETN] [get_bd_pins axi4_0/S00_ARESETN] [get_bd_pins axi_mm_mb/S02_ARESETN] [get_bd_pins axi_mm_mb/S03_ARESETN] [get_bd_pins axi_mm_mb/S04_ARESETN] [get_bd_pins logisdhc_0/S_AXI_ARESETN] [get_bd_pins axi_xadc_0/s_axi_aresetn] [get_bd_pins Ethernet_Controller/s_axi_lite_resetn] [get_bd_pins BRAM/S_AXI_ARESETN] [get_bd_pins microblaze_sybsystem/peripheral_aresetn] [get_bd_pins GPIO_Interface/s_axi_aresetn] [get_bd_pins flash_interface/s_axi_aresetn] [get_bd_pins DDR3_Module/aresetn]
  connect_bd_net -net ext_reset_in_1 [get_bd_ports RESET] [get_bd_pins proc_sys_reset_0/ext_reset_in] [get_bd_pins DDR3_Module/sys_rst]
  connect_bd_net -net axi_uart16550_1_sout [get_bd_ports RS232_UART_1_sout] [get_bd_pins RS232_Uart_1/sout]
  connect_bd_net -net sin_1 [get_bd_ports RS232_UART_1_sin] [get_bd_pins RS232_Uart_1/sin]
  connect_bd_net -net clk_150Mhz [get_bd_pins clock_generator_1/clk_out1] [get_bd_pins axi_mm_mb/ACLK] [get_bd_pins axi_mm_mb/S00_ACLK] [get_bd_pins axi_mm_mb/S01_ACLK] [get_bd_pins axi_mm_mb/S02_ACLK] [get_bd_pins axi_mm_mb/S03_ACLK] [get_bd_pins axi_mm_mb/S04_ACLK] [get_bd_pins Ethernet_Controller/m_axi_mm2s_aclk] [get_bd_pins microblaze_sybsystem/LMB_Clk] [get_bd_pins microblaze_sybsystem/Lite_Slave_ACLK]
  connect_bd_net -net axi_emc_1_mem_wen [get_bd_ports Linear_Flash_we_n] [get_bd_pins flash_interface/mem_wen]
  connect_bd_net -net axi_emc_1_mem_oen [get_bd_ports Linear_Flash_oe_n] [get_bd_pins flash_interface/mem_oen]
  connect_bd_net -net axi_emc_1_mem_cen [get_bd_ports Linear_Flash_ce_n] [get_bd_pins flash_interface/mem_cen]
  connect_bd_net -net axi_emc_1_mem_dq_t [get_bd_ports Linear_Flash_Data_T] [get_bd_pins flash_interface/mem_dq_t]
  connect_bd_net -net axi_emc_1_mem_dq_o [get_bd_ports Linear_Flash_Data_O] [get_bd_pins flash_interface/mem_dq_o]
  connect_bd_net -net mem_dq_i_1 [get_bd_ports Linear_Flash_Data_I] [get_bd_pins flash_interface/mem_dq_i]
  connect_bd_net -net axi_emc_1_mem_a [get_bd_ports Linear_Flash_address] [get_bd_pins flash_interface/Dout]
  connect_bd_net -net axi_emc_1_mem_adv_ldn [get_bd_ports Linear_Flash_adv_ldn] [get_bd_pins flash_interface/mem_adv_ldn]
  connect_bd_net -net axi_iic_1_gpo [get_bd_ports IIC_MUX_RESET_B] [get_bd_pins IIC_EEPROM/gpo]
  connect_bd_net -net xlconstant_1_const [get_bd_ports sm_fan_pwm] [get_bd_pins GPIO_Interface/const]
  connect_bd_net -net mig_clk_200 [get_bd_pins DDR3_Module/ui_clk] [get_bd_pins clock_generator_1/clk_in1] [get_bd_pins axi4_0/ACLK] [get_bd_pins axi_mm_mb/M01_ACLK] [get_bd_pins axi4_0/S00_ACLK] [get_bd_pins axi4_0/M00_ACLK] [get_bd_pins Ethernet_Controller/ref_clk]
  connect_bd_net -net Net [get_bd_ports Soft_Ethernet_MAC_MDIO_pin] [get_bd_pins Ethernet_Controller/mdio]
  connect_bd_net -net soft_ethernet_mac_mdc [get_bd_ports Soft_Ethernet_MAC_MDC_pin] [get_bd_pins Ethernet_Controller/mdc]
  connect_bd_net -net ethernet_clk_125 [get_bd_pins clock_generator_1/clk_out2] [get_bd_pins Ethernet_Controller/gtx_clk]
  connect_bd_net -net logisdhc_0_sd_clk [get_bd_ports sd_clk] [get_bd_pins logisdhc_0/sd_clk]
  connect_bd_net -net logisdhc_0_sd_dat_o [get_bd_ports sd_dat_o] [get_bd_pins logisdhc_0/sd_dat_o]
  connect_bd_net -net logisdhc_0_sd_dat_t [get_bd_ports sd_dat_t] [get_bd_pins logisdhc_0/sd_dat_t]
  connect_bd_net -net sd_dat_i_1 [get_bd_ports sd_dat_i] [get_bd_pins logisdhc_0/sd_dat_i]
  connect_bd_net -net sd_wp_1 [get_bd_ports sd_wp] [get_bd_pins logisdhc_0/sd_wp]
  connect_bd_net -net sd_cd_n_1 [get_bd_ports sd_cd_n] [get_bd_pins logisdhc_0/sd_cd_n]
  connect_bd_net -net sd_cmd_i_1 [get_bd_ports sd_cmd_i] [get_bd_pins logisdhc_0/sd_cmd_i]
  connect_bd_net -net logisdhc_0_sd_cmd_o [get_bd_ports sd_cmd_o] [get_bd_pins logisdhc_0/sd_cmd_o]
  connect_bd_net -net logisdhc_0_sd_cmd_t [get_bd_ports sd_cmd_t] [get_bd_pins logisdhc_0/sd_cmd_t]
  connect_bd_net -net vauxp0_1 [get_bd_ports axi_xadc_0_VAUXP] [get_bd_pins axi_xadc_0/vauxp0]
  connect_bd_net -net vauxn0_1 [get_bd_ports axi_xadc_0_VAUXN] [get_bd_pins axi_xadc_0/vauxn0]
  connect_bd_net -net vauxp8_1 [get_bd_ports axi_xadc_8_VAUXP] [get_bd_pins axi_xadc_0/vauxp8]
  connect_bd_net -net vauxn8_1 [get_bd_ports axi_xadc_8_VAUXN] [get_bd_pins axi_xadc_0/vauxn8]
  connect_bd_net -net soft_ethernet_mac_phy_rst_n [get_bd_ports Soft_Ethernet_MAC_RST_n_pin] [get_bd_pins Ethernet_Controller/phy_rst_n]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins microblaze_sybsystem/Debug_SYS_Rst] [get_bd_pins proc_sys_reset_0/mb_debug_sys_rst]
  connect_bd_net -net proc_sys_reset_1_mb_reset [get_bd_pins proc_sys_reset_0/mb_reset] [get_bd_pins microblaze_sybsystem/processor_rst]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins proc_sys_reset_0/bus_struct_reset] [get_bd_pins microblaze_sybsystem/LMB_Rst]
  connect_bd_net -net dual_timer_counter_interrupt [get_bd_pins Dual_Timer_Counter/interrupt] [get_bd_pins microblaze_sybsystem/interrupt_4]
  connect_bd_net -net ethernet_controller_interrupt [get_bd_pins Ethernet_Controller/interrupt] [get_bd_pins microblaze_sybsystem/interrupt_5]
  connect_bd_net -net ethernet_controller_s2mm_introut [get_bd_pins Ethernet_Controller/s2mm_introut] [get_bd_pins microblaze_sybsystem/interrupt_6]
  connect_bd_net -net ethernet_controller_mm2s_introut [get_bd_pins Ethernet_Controller/mm2s_introut] [get_bd_pins microblaze_sybsystem/interrupt_7]
  connect_bd_net -net axi_xadc_0_ip2intc_irpt [get_bd_pins axi_xadc_0/ip2intc_irpt] [get_bd_pins microblaze_sybsystem/interrupt_0]
  connect_bd_net -net rs232_uart_1_ip2intc_irpt [get_bd_pins RS232_Uart_1/ip2intc_irpt] [get_bd_pins microblaze_sybsystem/interrupt_2]
  connect_bd_net -net logisdhc_0_sd_int [get_bd_pins logisdhc_0/sd_int] [get_bd_pins microblaze_sybsystem/interrupt_1]
  connect_bd_net -net iic_eeprom_iic2intc_irpt [get_bd_pins IIC_EEPROM/iic2intc_irpt] [get_bd_pins microblaze_sybsystem/interrupt_3]
  connect_bd_net -net mig_7series_1_init_calib_complete [get_bd_ports init_calib_complete] [get_bd_pins DDR3_Module/init_calib_complete]
  connect_bd_net -net xlconstant_3_const [get_bd_ports ddr3_1_n] [get_bd_pins DDR3_Module/const1]
  connect_bd_net -net clk_in1_n_1 [get_bd_ports CLK_N] [get_bd_pins DDR3_Module/sys_clk_n]
  connect_bd_net -net xlconstant_4_const [get_bd_ports ddr3_1_p] [get_bd_pins DDR3_Module/const]
  connect_bd_net -net xadc_wiz_1_temp_out [get_bd_pins axi_xadc_0/temp_out] [get_bd_pins DDR3_Module/device_temp_i]
  connect_bd_net -net clk_in1_p_1 [get_bd_ports CLK_P] [get_bd_pins DDR3_Module/sys_clk_p]

  # Create address segments
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_SG] [get_bd_addr_segs DDR3_Module/DDR3_SDRAM/memmap/memaddr] SEG2
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_SG] [get_bd_addr_segs BRAM/Internal_BRAM/S_AXI/Mem0] SEG4
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_MM2S] [get_bd_addr_segs DDR3_Module/DDR3_SDRAM/memmap/memaddr] SEG3
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_MM2S] [get_bd_addr_segs BRAM/Internal_BRAM/S_AXI/Mem0] SEG5
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_S2MM] [get_bd_addr_segs DDR3_Module/DDR3_SDRAM/memmap/memaddr] SEG4
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces Ethernet_Controller/AXI_DMA_Ethernet/Data_S2MM] [get_bd_addr_segs BRAM/Internal_BRAM/S_AXI/Mem0] SEG6
  create_bd_addr_seg -range 0x2000 -offset 0x0 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_sybsystem/microblaze_0_local_memory/LocalMemory_Cntlr_D/SLMB/Mem] SEG2
  create_bd_addr_seg -range 0x10000 -offset 0x40200000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_sybsystem/debug_module/S_AXI/Reg] SEG3
  create_bd_addr_seg -range 0x10000 -offset 0x40600000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs GPIO_Interface/LEDs_8Bits/s_axi/Reg] SEG1
  create_bd_addr_seg -range 0x10000 -offset 0x40400000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs RS232_Uart_1/s_axi/Reg] SEG4
  create_bd_addr_seg -range 0x10000 -offset 0x40300000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs Dual_Timer_Counter/s_axi/Reg] SEG5
  create_bd_addr_seg -range 0x10000 -offset 0x40500000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs GPIO_Interface/Push_Buttons_5Bits/s_axi/Reg] SEG6
  create_bd_addr_seg -range 0x10000 -offset 0x40700000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs GPIO_Interface/DIP_Switches_4Bits/s_axi/Reg] SEG7
  create_bd_addr_seg -range 0x10000 -offset 0x40800000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs GPIO_Interface/LCD_GPIO/s_axi/Reg] SEG8
  create_bd_addr_seg -range 0x10000 -offset 0x40900000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs GPIO_Interface/ROTARY_GPIO/s_axi/Reg] SEG9
  create_bd_addr_seg -range 0x8000000 -offset 0x48000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs flash_interface/Linear_Flash/S_AXI_MEM/MEM0] SEG10
  create_bd_addr_seg -range 0x10000 -offset 0x40A00000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs IIC_EEPROM/s_axi/Reg] SEG11
  create_bd_addr_seg -range 0x10000 -offset 0x40100000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_sybsystem/Interrupt_Cntlr/s_axi/Reg] SEG12
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs BRAM/Internal_BRAM/S_AXI/Mem0] SEG13
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs DDR3_Module/DDR3_SDRAM/memmap/memaddr] SEG14
  create_bd_addr_seg -range 0x10000 -offset 0x50000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs Ethernet_Controller/AXI_DMA_Ethernet/S_AXI_LITE/Reg] SEG15
  create_bd_addr_seg -range 0x40000 -offset 0x50100000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs Ethernet_Controller/Soft_Ethernet_MAC_0/axi_ethernet_buffer/S_AXI_Ethernet/Axi_Ethernet_Mem0] SEG16
  create_bd_addr_seg -range 0x10000 -offset 0xB000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs logisdhc_0/s_axi/Reg] SEG17
  create_bd_addr_seg -range 0x10000 -offset 0xD000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Data] [get_bd_addr_segs axi_xadc_0/s_axi_lite/Reg] SEG18
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Instruction] [get_bd_addr_segs DDR3_Module/DDR3_SDRAM/memmap/memaddr] SEG1
  create_bd_addr_seg -range 0x2000 -offset 0x0 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Instruction] [get_bd_addr_segs microblaze_sybsystem/microblaze_0_local_memory/LocalMemory_Cntlr_I/SLMB/Mem] SEG2
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_sybsystem/microblaze_0/Instruction] [get_bd_addr_segs BRAM/Internal_BRAM/S_AXI/Mem0] SEG3
  

  # Restore current instance
  current_bd_instance $oldCurInst
}


#####################################################
# Main Flow
#####################################################

create_root_design ""
