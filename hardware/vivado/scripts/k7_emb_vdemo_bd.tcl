# This is a generated script based on design: k7_emb_vdemo
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
#
# To test this script, run the following commands from Vivado Tcl console:
# source k7_emb_vdemo_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
#    create_project project_1 myproj -part xc7vx485tffg1157-1 -force

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design k7_emb_vdemo

# Creating design if needed
if { [get_files *.bd] eq "" } {
   puts "INFO: Currently there are no designs in project, so creating one..."
   create_bd_design k7_emb_vdemo
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

# Hierarchical cell: DDR_Interface
proc create_hier_cell_DDR_Interface { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_DDR_Interface() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI1
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR3

  # Create pins
  create_bd_pin -dir I sys_clk_p
  create_bd_pin -dir I sys_clk_n
  create_bd_pin -dir O -from 2 -to 0 ddr3_1_n
  create_bd_pin -dir O -from 1 -to 0 ddr3_1_p
  create_bd_pin -dir O -type clk ui_clk
  create_bd_pin -dir I -from 0 -to 0 -type rst core_aresetn
  create_bd_pin -dir I -from 0 -to 0 aresetn
  create_bd_pin -dir O init_calib_complete
  create_bd_pin -dir O -type clk ui_addn_clk_0
  create_bd_pin -dir I -from 11 -to 0 device_temp_i
  create_bd_pin -dir I -type rst sys_rst

  # Create instance: axi_perf_mon_1, and set properties
  set axi_perf_mon_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_perf_mon:4.0 axi_perf_mon_1 ]
  set_property -dict [ list CONFIG.C_NUM_OF_COUNTERS {4}  ] $axi_perf_mon_1

  # Create instance: ddr3_rank1_vcc, and set properties
  set ddr3_rank1_vcc [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 ddr3_rank1_vcc ]
  set_property -dict [ list CONFIG.CONST_WIDTH {2}  ] $ddr3_rank1_vcc

  # Create instance: DDR3_SDRAM, and set properties
  set DDR3_SDRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:2.0 DDR3_SDRAM ]
  set folder [pwd]
  set mig_file "../../../sources/constraints/k7_emb_vdemo_mig_a.prj"
  set mig_file_path [glob $folder/$mig_file]
  if { [file exists "$mig_file_path"] == 1 } { 
     set str_mig_folder [get_property IP_DIR [ get_ips [ get_property CONFIG.Component_Name $DDR3_SDRAM ] ] ]
     puts "Copying <$mig_file_path> to <$str_mig_folder/mig_b.prj>..."
     file copy -force $mig_file_path "$str_mig_folder/mig_b.prj"
  } else {
     puts "ERROR: Unable to find the PRJ file <$mig_file>!"
  }
  set_property -dict [ list CONFIG.XML_INPUT_FILE {mig_b.prj} CONFIG.RESET_BOARD_INTERFACE {Custom}  ] $DDR3_SDRAM

  # Create instance: ddr3_rank1_gnd, and set properties
  set ddr3_rank1_gnd [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 ddr3_rank1_gnd ]
  set_property -dict [ list CONFIG.CONST_WIDTH {3} CONFIG.CONST_VAL {0}  ] $ddr3_rank1_gnd

  # Create interface connections
  connect_bd_intf_net -intf_net axi4_0_m00 [get_bd_intf_pins S_AXI1] [get_bd_intf_pins DDR3_SDRAM/S_AXI]
  connect_bd_intf_net -intf_net axi4_0_m00 [get_bd_intf_pins S_AXI1] [get_bd_intf_pins axi_perf_mon_1/SLOT_0_AXI]
  connect_bd_intf_net -intf_net axi4lite_1_m13_axi [get_bd_intf_pins s_axi] [get_bd_intf_pins axi_perf_mon_1/s_axi]
  connect_bd_intf_net -intf_net ddr3_port [get_bd_intf_pins DDR3] [get_bd_intf_pins DDR3_SDRAM/DDR3]

  # Create port connections
  connect_bd_net -net clk_p_1 [get_bd_pins sys_clk_p] [get_bd_pins DDR3_SDRAM/sys_clk_p]
  connect_bd_net -net clk_n_1 [get_bd_pins sys_clk_n] [get_bd_pins DDR3_SDRAM/sys_clk_n]
  connect_bd_net -net xlconstant_3_const [get_bd_pins ddr3_1_n] [get_bd_pins ddr3_rank1_gnd/const]
  connect_bd_net -net xlconstant_4_const [get_bd_pins ddr3_1_p] [get_bd_pins ddr3_rank1_vcc/const]
  connect_bd_net -net mig_clk_200 [get_bd_pins ui_clk] [get_bd_pins DDR3_SDRAM/ui_clk] [get_bd_pins axi_perf_mon_1/core_aclk] [get_bd_pins axi_perf_mon_1/slot_0_axi_aclk]
  connect_bd_net -net proc_sys_reset_1_interconnect_aresetn [get_bd_pins core_aresetn] [get_bd_pins axi_perf_mon_1/core_aresetn] [get_bd_pins axi_perf_mon_1/slot_0_axi_aresetn] [get_bd_pins axi_perf_mon_1/s_axi_aresetn]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins aresetn] [get_bd_pins DDR3_SDRAM/aresetn]
  connect_bd_net -net mig_7series_1_init_calib_complete [get_bd_pins init_calib_complete] [get_bd_pins DDR3_SDRAM/init_calib_complete]
  connect_bd_net -net clk_100Mhz [get_bd_pins ui_addn_clk_0] [get_bd_pins DDR3_SDRAM/ui_addn_clk_0] [get_bd_pins axi_perf_mon_1/s_axi_aclk]
  connect_bd_net -net xadc_wiz_1_temp_out [get_bd_pins device_temp_i] [get_bd_pins DDR3_SDRAM/device_temp_i]
  connect_bd_net -net ext_reset_in_1 [get_bd_pins sys_rst] [get_bd_pins DDR3_SDRAM/sys_rst]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: Display_Controller
proc create_hier_cell_Display_Controller { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_Display_Controller() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi1

  # Create pins
  create_bd_pin -dir O -type intr cvc_interrupt
  create_bd_pin -dir I -type clk M_AXI_ACLK
  create_bd_pin -dir O hsync_o
  create_bd_pin -dir O blank_o
  create_bd_pin -dir O vsync_o
  create_bd_pin -dir O -from 15 -to 0 d_pix_o
  create_bd_pin -dir O pix_clk_o
  create_bd_pin -dir O -from 0 -to 0 inline_scaler_1_reset_n
  create_bd_pin -dir O -from 0 -to 0 video_mux_sel_1
  create_bd_pin -dir O -from 0 -to 0 video_mux_sel_2
  create_bd_pin -dir O -from 0 -to 0 inline_scaler_2_reset_n
  create_bd_pin -dir I -from 0 -to 0 -type rst s_axi_aresetn
  create_bd_pin -dir I -from 0 -to 0 rst
  create_bd_pin -dir I -type clk s_axi_aclk
  create_bd_pin -dir I -from 0 -to 0 e_sw_vbuff_3
  create_bd_pin -dir I -from 0 -to 0 e_sw_vbuff_4
  create_bd_pin -dir I -from 0 -to 0 e_sw_vbuff_1
  create_bd_pin -dir I -from 0 -to 0 e_sw_vbuff_2
  create_bd_pin -dir O -from 0 -to 0 mem_scaler_1_reset_n
  create_bd_pin -dir O -from 0 -to 0 mem_scaler_2_reset_n

  # Create instance: logicvc_1, and set properties
  set logicvc_1 [ create_bd_cell -type ip -vlnv xylon:logicbricks:logicvc:3.01.a logicvc_1 ]
  set_property -dict [ list CONFIG.C_BUFFER_3_OFFSET {1080} CONFIG.C_BUFFER_2_OFFSET {1080} CONFIG.C_BUFFER_1_OFFSET {1080} CONFIG.C_BUFFER_0_OFFSET {1080} CONFIG.C_LAYER_4_OFFSET {25920} CONFIG.C_LAYER_3_OFFSET {19440} CONFIG.C_LAYER_2_OFFSET {12960} CONFIG.C_LAYER_1_OFFSET {6480} CONFIG.C_USE_XTREME_DSP {1} CONFIG.C_USE_BACKGROUND {1} CONFIG.C_LAYER_4_DATA_WIDTH {24} CONFIG.C_LAYER_3_DATA_WIDTH {24} CONFIG.C_LAYER_2_DATA_WIDTH {24} CONFIG.C_LAYER_1_DATA_WIDTH {24} CONFIG.C_LAYER_0_DATA_WIDTH {24} CONFIG.C_NUM_OF_LAYERS {5} CONFIG.C_DISPLAY_COLOR_SPACE {1} CONFIG.C_ROW_STRIDE {2048} CONFIG.C_PIXEL_DATA_WIDTH {16} CONFIG.C_M_AXI_DATA_WIDTH {128} CONFIG.C_INCREASE_FIFO {4} CONFIG.C_VMEM_HIGHADDR {0x9FFFFFFF} CONFIG.C_VMEM_BASEADDR {0x80000000}  ] $logicvc_1

  # Create instance: video_mux_sel_1, and set properties
  set video_mux_sel_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 video_mux_sel_1 ]
  set_property -dict [ list CONFIG.DIN_TO {0} CONFIG.DIN_FROM {0} CONFIG.DIN_WIDTH {8}  ] $video_mux_sel_1

  # Create instance: gpio_rst_mux, and set properties
  set gpio_rst_mux [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 gpio_rst_mux ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {8} CONFIG.C_ALL_OUTPUTS {1}  ] $gpio_rst_mux

  # Create instance: inline_rst_1, and set properties
  set inline_rst_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 inline_rst_1 ]
  set_property -dict [ list CONFIG.DIN_TO {5} CONFIG.DIN_FROM {5} CONFIG.DIN_WIDTH {8}  ] $inline_rst_1

  # Create instance: inline_rst_2, and set properties
  set inline_rst_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 inline_rst_2 ]
  set_property -dict [ list CONFIG.DIN_TO {7} CONFIG.DIN_FROM {7} CONFIG.DIN_WIDTH {8}  ] $inline_rst_2

  # Create instance: video_mux_sel_2, and set properties
  set video_mux_sel_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 video_mux_sel_2 ]
  set_property -dict [ list CONFIG.DIN_TO {3} CONFIG.DIN_FROM {3} CONFIG.DIN_WIDTH {8}  ] $video_mux_sel_2

  # Create instance: cvc_switch, and set properties
  set cvc_switch [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 cvc_switch ]
  set_property -dict [ list CONFIG.NUM_PORTS {5}  ] $cvc_switch

  # Create instance: logic_zero, and set properties
  set logic_zero [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 logic_zero ]

  # Create instance: memscaler_rst_1, and set properties
  set memscaler_rst_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 memscaler_rst_1 ]
  set_property -dict [ list CONFIG.DIN_TO {4} CONFIG.DIN_FROM {4} CONFIG.DIN_WIDTH {8}  ] $memscaler_rst_1

  # Create instance: memscaler_rst_2, and set properties
  set memscaler_rst_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 memscaler_rst_2 ]
  set_property -dict [ list CONFIG.DIN_TO {6} CONFIG.DIN_FROM {6} CONFIG.DIN_WIDTH {8}  ] $memscaler_rst_2

  # Create interface connections
  connect_bd_intf_net -intf_net s02_axi_1 [get_bd_intf_pins m_axi] [get_bd_intf_pins logicvc_1/m_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m03_axi [get_bd_intf_pins s_axi] [get_bd_intf_pins logicvc_1/s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m00_axi [get_bd_intf_pins s_axi1] [get_bd_intf_pins gpio_rst_mux/s_axi]

  # Create port connections
  connect_bd_net -net gpio_rst_mux_gpio_io_o [get_bd_pins gpio_rst_mux/gpio_io_o] [get_bd_pins video_mux_sel_1/Din] [get_bd_pins video_mux_sel_2/Din] [get_bd_pins inline_rst_1/Din] [get_bd_pins inline_rst_2/Din] [get_bd_pins memscaler_rst_1/Din] [get_bd_pins memscaler_rst_2/Din]
  connect_bd_net -net logicvc_1_interrupt [get_bd_pins cvc_interrupt] [get_bd_pins logicvc_1/interrupt]
  connect_bd_net -net clk_150Mhz [get_bd_pins M_AXI_ACLK] [get_bd_pins logicvc_1/M_AXI_ACLK] [get_bd_pins logicvc_1/vclk]
  connect_bd_net -net logicvc_2_hsync_o [get_bd_pins hsync_o] [get_bd_pins logicvc_1/hsync_o]
  connect_bd_net -net logicvc_2_blank_o [get_bd_pins blank_o] [get_bd_pins logicvc_1/blank_o]
  connect_bd_net -net logicvc_2_vsync_o [get_bd_pins vsync_o] [get_bd_pins logicvc_1/vsync_o]
  connect_bd_net -net logicvc_2_d_pix_o [get_bd_pins d_pix_o] [get_bd_pins logicvc_1/d_pix_o]
  connect_bd_net -net logicvc_2_pix_clk_o [get_bd_pins pix_clk_o] [get_bd_pins logicvc_1/pix_clk_o]
  connect_bd_net -net xlslice_1_dout [get_bd_pins inline_scaler_1_reset_n] [get_bd_pins inline_rst_1/Dout]
  connect_bd_net -net video_src_0_sel [get_bd_pins video_mux_sel_1] [get_bd_pins video_mux_sel_1/Dout]
  connect_bd_net -net video_mux_sel_2_dout [get_bd_pins video_mux_sel_2] [get_bd_pins video_mux_sel_2/Dout]
  connect_bd_net -net xlslice_2_dout [get_bd_pins inline_scaler_2_reset_n] [get_bd_pins inline_rst_2/Dout]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins gpio_rst_mux/s_axi_aresetn] [get_bd_pins logicvc_1/S_AXI_ARESETN] [get_bd_pins logicvc_1/M_AXI_ARESETN]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins rst] [get_bd_pins logicvc_1/rst]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_aclk] [get_bd_pins gpio_rst_mux/s_axi_aclk] [get_bd_pins logicvc_1/S_AXI_ACLK]
  connect_bd_net -net cvc_switch_dout [get_bd_pins cvc_switch/dout] [get_bd_pins logicvc_1/e_sw_vbuff]
  connect_bd_net -net in3_1 [get_bd_pins e_sw_vbuff_3] [get_bd_pins cvc_switch/In3]
  connect_bd_net -net in4_1 [get_bd_pins e_sw_vbuff_4] [get_bd_pins cvc_switch/In4]
  connect_bd_net -net in1_1 [get_bd_pins e_sw_vbuff_1] [get_bd_pins cvc_switch/In1]
  connect_bd_net -net in2_1 [get_bd_pins e_sw_vbuff_2] [get_bd_pins cvc_switch/In2]
  connect_bd_net -net logic_zero_const [get_bd_pins logic_zero/const] [get_bd_pins cvc_switch/In0]
  connect_bd_net -net memscaler_rst_1_dout [get_bd_pins mem_scaler_1_reset_n] [get_bd_pins memscaler_rst_1/Dout]
  connect_bd_net -net memscaler_rst_2_dout [get_bd_pins mem_scaler_2_reset_n] [get_bd_pins memscaler_rst_2/Dout]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: inline_scaler_2
proc create_hier_cell_inline_scaler_2 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_inline_scaler_2() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 ctrl

  # Create pins
  create_bd_pin -dir I -type clk m_axi_s2mm_aclk
  create_bd_pin -dir O s2mm_fsync_out
  create_bd_pin -dir I -type clk s_axi_lite_aclk
  create_bd_pin -dir I -from 0 -to 0 -type rst axi_resetn
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I -from 0 -to 0 rst
  create_bd_pin -dir I vid_vsync
  create_bd_pin -dir I vid_hsync
  create_bd_pin -dir I vid_active_video
  create_bd_pin -dir I -from 0 -to 0 scaler_reset_n

  # Create instance: vid_source_3, and set properties
  set vid_source_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tpg:5.0 vid_source_3 ]
  set_property -dict [ list CONFIG.has_axi4_lite {true} CONFIG.has_axi4s_slave {true} CONFIG.pattern_control {10} CONFIG.active_cols {1920} CONFIG.active_rows {1080} CONFIG.s_video_format {2} CONFIG.m_video_format {2} CONFIG.enable_motion {true} CONFIG.motion_speed {0}  ] $vid_source_3

  # Create instance: SCALER_3_VDMA, and set properties
  set SCALER_3_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 SCALER_3_VDMA ]
  set_property -dict [ list CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_s2mm_fsync {2} CONFIG.c_s2mm_genlock_mode {0} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_include_mm2s {0} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_11 {1}  ] $SCALER_3_VDMA

  # Create instance: xlconstant_2, and set properties
  set xlconstant_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 xlconstant_2 ]
  set_property -dict [ list CONFIG.CONST_WIDTH {4} CONFIG.CONST_VAL {15}  ] $xlconstant_2

  # Create instance: v_vid_in_axi4s_1, and set properties
  set v_vid_in_axi4s_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_vid_in_axi4s:3.0 v_vid_in_axi4s_1 ]

  # Create instance: axi_scaler_3, and set properties
  set axi_scaler_3 [ create_bd_cell -type ip -vlnv xilinx.com:axi_scaler:axi_scaler:2.0 axi_scaler_3 ]

  # Create instance: axis_subset_converter_3, and set properties
  set axis_subset_converter_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_subset_converter:1.0 axis_subset_converter_3 ]
  set_property -dict [ list CONFIG.S_TDATA_NUM_BYTES {3} CONFIG.M_TDATA_NUM_BYTES {4} CONFIG.TDATA_REMAP {8'b11111111,tdata[23:16],tdata[7:0],tdata[15:8]}  ] $axis_subset_converter_3

  # Create instance: xlconstant_1, and set properties
  set xlconstant_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 xlconstant_1 ]
  set_property -dict [ list CONFIG.CONST_WIDTH {24}  ] $xlconstant_1

  # Create interface connections
  connect_bd_intf_net -intf_net v_vid_in_axi4s_1_video_out [get_bd_intf_pins v_vid_in_axi4s_1/video_out] [get_bd_intf_pins vid_source_3/video_in]
  connect_bd_intf_net -intf_net axi_scaler_1_s_axis_s2mm [get_bd_intf_pins axi_scaler_3/s_axis_s2mm] [get_bd_intf_pins SCALER_3_VDMA/S_AXIS_S2MM]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi_scaler_1_s_axis_s2mm]
  connect_bd_intf_net -intf_net vid_source_3_video_out [get_bd_intf_pins axis_subset_converter_3/S_AXIS] [get_bd_intf_pins vid_source_3/video_out]
  connect_bd_intf_net -intf_net axis_subset_converter_1_m_axis [get_bd_intf_pins axis_subset_converter_3/M_AXIS] [get_bd_intf_pins axi_scaler_3/m_axis_mm2s]
  connect_bd_intf_net -intf_net axi4lite_1_m07 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins SCALER_3_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net tpg_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins SCALER_3_VDMA/M_AXI_S2MM]
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins ctrl] [get_bd_intf_pins vid_source_3/ctrl]

  # Create port connections
  connect_bd_net -net clk_150Mhz [get_bd_pins m_axi_s2mm_aclk] [get_bd_pins SCALER_3_VDMA/m_axi_s2mm_aclk] [get_bd_pins SCALER_3_VDMA/s_axis_s2mm_aclk] [get_bd_pins vid_source_3/aclk] [get_bd_pins v_vid_in_axi4s_1/vid_io_in_clk] [get_bd_pins v_vid_in_axi4s_1/aclk] [get_bd_pins axi_scaler_3/m_axi_s2mm_aclk] [get_bd_pins axi_scaler_3/m_axi_mm2s_aclk] [get_bd_pins axis_subset_converter_3/aclk]
  connect_bd_net -net scaler_1_vdma_s2mm_fsync_out [get_bd_pins s2mm_fsync_out] [get_bd_pins SCALER_3_VDMA/s2mm_fsync_out]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_lite_aclk] [get_bd_pins SCALER_3_VDMA/s_axi_lite_aclk] [get_bd_pins vid_source_3/s_axi_aclk]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins axi_resetn] [get_bd_pins SCALER_3_VDMA/axi_resetn] [get_bd_pins vid_source_3/s_axi_aresetn] [get_bd_pins vid_source_3/aresetn] [get_bd_pins v_vid_in_axi4s_1/aresetn] [get_bd_pins axis_subset_converter_3/aresetn]
  connect_bd_net -net scaler_1_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins SCALER_3_VDMA/s2mm_introut]
  connect_bd_net -net rst_1 [get_bd_pins rst] [get_bd_pins v_vid_in_axi4s_1/rst]
  connect_bd_net -net vid_vsync_1 [get_bd_pins vid_vsync] [get_bd_pins v_vid_in_axi4s_1/vid_vsync]
  connect_bd_net -net vid_hsync_1 [get_bd_pins vid_hsync] [get_bd_pins v_vid_in_axi4s_1/vid_hsync]
  connect_bd_net -net vid_active_video_1 [get_bd_pins vid_active_video] [get_bd_pins v_vid_in_axi4s_1/vid_active_video]
  connect_bd_net -net xlconstant_2_const [get_bd_pins xlconstant_2/const] [get_bd_pins axi_scaler_3/m_axis_mm2s_tkeep]
  connect_bd_net -net axi_scaler_1_s_axis_s2mm_tuser [get_bd_pins axi_scaler_3/s_axis_s2mm_tuser] [get_bd_pins SCALER_3_VDMA/s_axis_s2mm_tuser]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_nets axi_scaler_1_s_axis_s2mm_tuser]
  connect_bd_net -net mm2s_prmry_reset_out_n_1 [get_bd_pins scaler_reset_n] [get_bd_pins axi_scaler_3/mm2s_prmry_reset_out_n] [get_bd_pins axi_scaler_3/s_axis_s2mm_aresetn]
  connect_bd_net -net xlconstant_1_const [get_bd_pins xlconstant_1/const] [get_bd_pins v_vid_in_axi4s_1/vid_data]
  connect_bd_net -net axi_scaler_3_s_axis_s2mm_tdata [get_bd_pins axi_scaler_3/s_axis_s2mm_tdata] [get_bd_pins SCALER_3_VDMA/s_axis_s2mm_tdata]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: mem_scaler_2
proc create_hier_cell_mem_scaler_2 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_mem_scaler_2() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_MM2S
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM

  # Create pins
  create_bd_pin -dir I -type clk m_axis_mm2s_aclk
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I fsync
  create_bd_pin -dir O -type intr mm2s_introut
  create_bd_pin -dir I -type clk s_axi_aclk
  create_bd_pin -dir I -from 0 -to 0 -type rst s_axi_aresetn
  create_bd_pin -dir I -from 0 -to 0 scaler_reset_n

  # Create instance: SCALER_2_VDMA, and set properties
  set SCALER_2_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 SCALER_2_VDMA ]
  set_property -dict [ list CONFIG.c_m_axi_mm2s_data_width {64} CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_mm2s_fsync {1} CONFIG.c_use_s2mm_fsync {1} CONFIG.c_mm2s_genlock_mode {1} CONFIG.c_s2mm_genlock_mode {1} CONFIG.c_mm2s_linebuffer_depth {4096} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_mm2s_max_burst_length {256} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_0 {1} CONFIG.c_enable_debug_info_7 {0} CONFIG.c_enable_debug_info_8 {1} CONFIG.c_enable_debug_info_9 {0} CONFIG.c_enable_debug_info_10 {0} CONFIG.c_enable_debug_info_11 {1} CONFIG.c_enable_debug_info_12 {1} CONFIG.c_enable_debug_info_13 {1} CONFIG.c_enable_debug_info_14 {0} CONFIG.c_include_mm2s_dre {1}  ] $SCALER_2_VDMA

  # Create instance: axi_scaler_2, and set properties
  set axi_scaler_2 [ create_bd_cell -type ip -vlnv xilinx.com:axi_scaler:axi_scaler:2.0 axi_scaler_2 ]

  # Create interface connections
  connect_bd_intf_net -intf_net axi_scaler_1_s_axis_s2mm [get_bd_intf_pins axi_scaler_2/s_axis_s2mm] [get_bd_intf_pins SCALER_2_VDMA/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net scaler_2_vdma_m_axis_mm2s [get_bd_intf_pins SCALER_2_VDMA/M_AXIS_MM2S] [get_bd_intf_pins axi_scaler_2/m_axis_mm2s]
  connect_bd_intf_net -intf_net axi4lite_1_m02 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins SCALER_2_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net scaler_0_vdma_m_axi_mm2s [get_bd_intf_pins M_AXI_MM2S] [get_bd_intf_pins SCALER_2_VDMA/M_AXI_MM2S]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets scaler_0_vdma_m_axi_mm2s]
  connect_bd_intf_net -intf_net scaler_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins SCALER_2_VDMA/M_AXI_S2MM]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets scaler_0_vdma_m_axi_s2mm]

  # Create port connections
  connect_bd_net -net clk_150Mhz [get_bd_pins m_axis_mm2s_aclk] [get_bd_pins SCALER_2_VDMA/m_axis_mm2s_aclk] [get_bd_pins SCALER_2_VDMA/m_axi_s2mm_aclk] [get_bd_pins SCALER_2_VDMA/s_axis_s2mm_aclk] [get_bd_pins SCALER_2_VDMA/m_axi_mm2s_aclk] [get_bd_pins axi_scaler_2/m_axi_mm2s_aclk] [get_bd_pins axi_scaler_2/m_axi_s2mm_aclk]
  connect_bd_net -net scaler_0_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins SCALER_2_VDMA/s2mm_introut]
  connect_bd_net -net dvi_in_0_vdma_s2mm_fsync_out [get_bd_pins fsync] [get_bd_pins SCALER_2_VDMA/mm2s_fsync] [get_bd_pins SCALER_2_VDMA/s2mm_fsync]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_nets dvi_in_0_vdma_s2mm_fsync_out]
  connect_bd_net -net scaler_0_vdma_mm2s_introut [get_bd_pins mm2s_introut] [get_bd_pins SCALER_2_VDMA/mm2s_introut]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_aclk] [get_bd_pins SCALER_2_VDMA/s_axi_lite_aclk]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins SCALER_2_VDMA/axi_resetn]
  connect_bd_net -net scaler_reset_n_1 [get_bd_pins scaler_reset_n] [get_bd_pins axi_scaler_2/mm2s_prmry_reset_out_n] [get_bd_pins axi_scaler_2/s_axis_s2mm_aresetn]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: v_in_pipe_2
proc create_hier_cell_v_in_pipe_2 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_v_in_pipe_2() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 tpg_s_axi
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM

  # Create pins
  create_bd_pin -dir I -from 7 -to 0 blue
  create_bd_pin -dir I -from 0 -to 0 vsync_1
  create_bd_pin -dir I -from 0 -to 0 video_clk_0
  create_bd_pin -dir O s2mm_fsync_out
  create_bd_pin -dir I -from 0 -to 0 de_1
  create_bd_pin -dir I -from 0 -to 0 video_clk_1
  create_bd_pin -dir I -from 7 -to 0 green
  create_bd_pin -dir I -from 0 -to 0 -type rst rst
  create_bd_pin -dir I -from 0 -to 0 hsync_1
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I -type clk s_axi_lite_aclk
  create_bd_pin -dir I -from 7 -to 0 red
  create_bd_pin -dir I -from 0 -to 0 -type rst aresetn
  create_bd_pin -dir I -from 0 -to 0 de_0
  create_bd_pin -dir I -from 0 -to 0 vsync_0
  create_bd_pin -dir I -from 0 -to 0 hsync_0
  create_bd_pin -dir I -from 0 -to 0 video_sel

  # Create instance: video_data_1, and set properties
  set video_data_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 video_data_1 ]
  set_property -dict [ list CONFIG.NUM_PORTS {3}  ] $video_data_1

  # Create instance: vid_source_2, and set properties
  set vid_source_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tpg:5.0 vid_source_2 ]
  set_property -dict [ list CONFIG.pattern_control {10} CONFIG.active_cols {1920} CONFIG.active_rows {1080} CONFIG.s_video_format {2}  ] $vid_source_2

  # Create instance: video_mux_1, and set properties
  set video_mux_1 [ create_bd_cell -type ip -vlnv xilinx.com:user:vsrc_sel:1.0 video_mux_1 ]

  # Create instance: DVI2AXIS_1, and set properties
  set DVI2AXIS_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_vid_in_axi4s:3.0 DVI2AXIS_1 ]
  set_property -dict [ list CONFIG.C_M_AXIS_VIDEO_FORMAT {2} CONFIG.RAM_ADDR_BITS {12}  ] $DVI2AXIS_1

  # Create instance: DVI_IN_1_VDMA, and set properties
  set DVI_IN_1_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 DVI_IN_1_VDMA ]
  set_property -dict [ list CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_s2mm_fsync {2} CONFIG.c_s2mm_genlock_mode {0} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_include_mm2s {0} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_11 {1}  ] $DVI_IN_1_VDMA

  # Create instance: axis_subset_converter_2, and set properties
  set axis_subset_converter_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_subset_converter:1.0 axis_subset_converter_2 ]
  set_property -dict [ list CONFIG.S_TDATA_NUM_BYTES {3} CONFIG.M_TDATA_NUM_BYTES {4} CONFIG.TDATA_REMAP {8'b11111111,tdata[23:16],tdata[7:0],tdata[15:8]}  ] $axis_subset_converter_2

  # Create interface connections
  connect_bd_intf_net -intf_net v_vid_in_axi4s_1_video_out [get_bd_intf_pins DVI2AXIS_1/video_out] [get_bd_intf_pins vid_source_2/video_in]
  connect_bd_intf_net -intf_net axis_subset_converter_1_m_axis [get_bd_intf_pins axis_subset_converter_2/M_AXIS] [get_bd_intf_pins DVI_IN_1_VDMA/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net vid_source_2_video_out [get_bd_intf_pins axis_subset_converter_2/S_AXIS] [get_bd_intf_pins vid_source_2/video_out]
  connect_bd_intf_net -intf_net axi4lite_1_m05 [get_bd_intf_pins tpg_s_axi] [get_bd_intf_pins vid_source_2/ctrl]
  connect_bd_intf_net -intf_net axi4lite_1_m01 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins DVI_IN_1_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net dvi_in_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins DVI_IN_1_VDMA/M_AXI_S2MM]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets dvi_in_0_vdma_m_axi_s2mm]

  # Create port connections
  connect_bd_net -net video_data_1_dout [get_bd_pins video_data_1/dout] [get_bd_pins DVI2AXIS_1/vid_data]
  connect_bd_net -net video_mux_0_video_clk [get_bd_pins video_mux_1/video_clk] [get_bd_pins DVI2AXIS_1/vid_io_in_clk]
  connect_bd_net -net video_mux_0_hsync [get_bd_pins video_mux_1/hsync] [get_bd_pins DVI2AXIS_1/vid_hsync]
  connect_bd_net -net video_mux_0_vsync [get_bd_pins video_mux_1/vsync] [get_bd_pins DVI2AXIS_1/vid_vsync]
  connect_bd_net -net video_mux_0_de [get_bd_pins video_mux_1/de] [get_bd_pins DVI2AXIS_1/vid_active_video]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_red_1 [get_bd_pins blue] [get_bd_pins video_data_1/In1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_vsync_1 [get_bd_pins vsync_1] [get_bd_pins video_mux_1/vsync_2]
  connect_bd_net -net clk_150Mhz [get_bd_pins video_clk_0] [get_bd_pins video_mux_1/video_clk_1] [get_bd_pins DVI2AXIS_1/aclk] [get_bd_pins DVI_IN_1_VDMA/m_axi_s2mm_aclk] [get_bd_pins DVI_IN_1_VDMA/s_axis_s2mm_aclk] [get_bd_pins vid_source_2/aclk] [get_bd_pins axis_subset_converter_2/aclk]
  connect_bd_net -net dvi_in_0_vdma_s2mm_fsync_out [get_bd_pins s2mm_fsync_out] [get_bd_pins DVI_IN_1_VDMA/s2mm_fsync_out]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_de_1 [get_bd_pins de_1] [get_bd_pins video_mux_1/de_2]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_clk_1 [get_bd_pins video_clk_1] [get_bd_pins video_mux_1/video_clk_2]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins rst] [get_bd_pins DVI2AXIS_1/rst]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_hsync_1 [get_bd_pins hsync_1] [get_bd_pins video_mux_1/hsync_2]
  connect_bd_net -net dvi_in_0_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins DVI_IN_1_VDMA/s2mm_introut]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_lite_aclk] [get_bd_pins DVI_IN_1_VDMA/s_axi_lite_aclk] [get_bd_pins vid_source_2/s_axi_aclk]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_green_1 [get_bd_pins red] [get_bd_pins video_data_1/In0]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins aresetn] [get_bd_pins DVI2AXIS_1/aresetn] [get_bd_pins DVI_IN_1_VDMA/axi_resetn] [get_bd_pins vid_source_2/s_axi_aresetn] [get_bd_pins vid_source_2/aresetn] [get_bd_pins axis_subset_converter_2/aresetn]
  connect_bd_net -net v_tc_1_active_video_out [get_bd_pins de_0] [get_bd_pins video_mux_1/de_1]
  connect_bd_net -net v_tc_1_vsync_out [get_bd_pins vsync_0] [get_bd_pins video_mux_1/vsync_1]
  connect_bd_net -net v_tc_1_hsync_out [get_bd_pins hsync_0] [get_bd_pins video_mux_1/hsync_1]
  connect_bd_net -net video_sel_1 [get_bd_pins video_sel] [get_bd_pins video_mux_1/video_sel]
  connect_bd_net -net green_1 [get_bd_pins green] [get_bd_pins video_data_1/In2]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: inline_scaler_1
proc create_hier_cell_inline_scaler_1 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_inline_scaler_1() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 ctrl

  # Create pins
  create_bd_pin -dir I -type clk m_axi_s2mm_aclk
  create_bd_pin -dir O s2mm_fsync_out
  create_bd_pin -dir I -type clk s_axi_lite_aclk
  create_bd_pin -dir I -from 0 -to 0 -type rst axi_resetn
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I -from 0 -to 0 rst
  create_bd_pin -dir I vid_vsync
  create_bd_pin -dir I vid_hsync
  create_bd_pin -dir I vid_active_video
  create_bd_pin -dir I -from 0 -to 0 scaler_reset_n

  # Create instance: vid_source_1, and set properties
  set vid_source_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tpg:5.0 vid_source_1 ]
  set_property -dict [ list CONFIG.has_axi4_lite {true} CONFIG.has_axi4s_slave {true} CONFIG.pattern_control {10} CONFIG.active_cols {1920} CONFIG.active_rows {1080} CONFIG.s_video_format {2} CONFIG.m_video_format {2} CONFIG.enable_motion {true} CONFIG.motion_speed {0}  ] $vid_source_1

  # Create instance: SCALER_1_VDMA, and set properties
  set SCALER_1_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 SCALER_1_VDMA ]
  set_property -dict [ list CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_s2mm_fsync {2} CONFIG.c_s2mm_genlock_mode {0} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_include_mm2s {0} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_11 {1}  ] $SCALER_1_VDMA

  # Create instance: xlconstant_2, and set properties
  set xlconstant_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 xlconstant_2 ]
  set_property -dict [ list CONFIG.CONST_WIDTH {4} CONFIG.CONST_VAL {15}  ] $xlconstant_2

  # Create instance: v_vid_in_axi4s_1, and set properties
  set v_vid_in_axi4s_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_vid_in_axi4s:3.0 v_vid_in_axi4s_1 ]

  # Create instance: axi_scaler_1, and set properties
  set axi_scaler_1 [ create_bd_cell -type ip -vlnv xilinx.com:axi_scaler:axi_scaler:2.0 axi_scaler_1 ]

  # Create instance: axis_subset_converter_1, and set properties
  set axis_subset_converter_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_subset_converter:1.0 axis_subset_converter_1 ]
  set_property -dict [ list CONFIG.S_TDATA_NUM_BYTES {3} CONFIG.M_TDATA_NUM_BYTES {4} CONFIG.TDATA_REMAP {8'b11111111,tdata[23:16],tdata[7:0],tdata[15:8]}  ] $axis_subset_converter_1

  # Create instance: xlconstant_3, and set properties
  set xlconstant_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 xlconstant_3 ]
  set_property -dict [ list CONFIG.CONST_WIDTH {24}  ] $xlconstant_3

  # Create interface connections
  connect_bd_intf_net -intf_net v_vid_in_axi4s_1_video_out [get_bd_intf_pins v_vid_in_axi4s_1/video_out] [get_bd_intf_pins vid_source_1/video_in]
  connect_bd_intf_net -intf_net axi_scaler_2_s_axis_s2mm [get_bd_intf_pins axi_scaler_1/s_axis_s2mm] [get_bd_intf_pins SCALER_1_VDMA/S_AXIS_S2MM]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi_scaler_2_s_axis_s2mm]
  connect_bd_intf_net -intf_net vid_source_1_video_out [get_bd_intf_pins axis_subset_converter_1/S_AXIS] [get_bd_intf_pins vid_source_1/video_out]
  connect_bd_intf_net -intf_net axis_subset_converter_1_m_axis [get_bd_intf_pins axis_subset_converter_1/M_AXIS] [get_bd_intf_pins axi_scaler_1/m_axis_mm2s]
  connect_bd_intf_net -intf_net axi4lite_1_m07 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins SCALER_1_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net tpg_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins SCALER_1_VDMA/M_AXI_S2MM]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets tpg_0_vdma_m_axi_s2mm]
  connect_bd_intf_net -intf_net Conn1 [get_bd_intf_pins ctrl] [get_bd_intf_pins vid_source_1/ctrl]

  # Create port connections
  connect_bd_net -net clk_150Mhz [get_bd_pins m_axi_s2mm_aclk] [get_bd_pins SCALER_1_VDMA/m_axi_s2mm_aclk] [get_bd_pins SCALER_1_VDMA/s_axis_s2mm_aclk] [get_bd_pins vid_source_1/aclk] [get_bd_pins v_vid_in_axi4s_1/aclk] [get_bd_pins v_vid_in_axi4s_1/vid_io_in_clk] [get_bd_pins axi_scaler_1/m_axi_s2mm_aclk] [get_bd_pins axi_scaler_1/m_axi_mm2s_aclk] [get_bd_pins axis_subset_converter_1/aclk]
  connect_bd_net -net scaler_1_vdma_s2mm_fsync_out [get_bd_pins s2mm_fsync_out] [get_bd_pins SCALER_1_VDMA/s2mm_fsync_out]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_nets scaler_1_vdma_s2mm_fsync_out]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_lite_aclk] [get_bd_pins SCALER_1_VDMA/s_axi_lite_aclk] [get_bd_pins vid_source_1/s_axi_aclk]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins axi_resetn] [get_bd_pins SCALER_1_VDMA/axi_resetn] [get_bd_pins vid_source_1/s_axi_aresetn] [get_bd_pins v_vid_in_axi4s_1/aresetn] [get_bd_pins vid_source_1/aresetn] [get_bd_pins axis_subset_converter_1/aresetn]
  connect_bd_net -net scaler_1_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins SCALER_1_VDMA/s2mm_introut]
  connect_bd_net -net rst_1 [get_bd_pins rst] [get_bd_pins v_vid_in_axi4s_1/rst]
  connect_bd_net -net vid_vsync_1 [get_bd_pins vid_vsync] [get_bd_pins v_vid_in_axi4s_1/vid_vsync]
  connect_bd_net -net vid_hsync_1 [get_bd_pins vid_hsync] [get_bd_pins v_vid_in_axi4s_1/vid_hsync]
  connect_bd_net -net vid_active_video_1 [get_bd_pins vid_active_video] [get_bd_pins v_vid_in_axi4s_1/vid_active_video]
  connect_bd_net -net axi_scaler_1_s_axis_s2mm_tdata [get_bd_pins axi_scaler_1/s_axis_s2mm_tdata] [get_bd_pins SCALER_1_VDMA/s_axis_s2mm_tdata]
  connect_bd_net -net axi_scaler_1_s_axis_s2mm_tuser [get_bd_pins axi_scaler_1/s_axis_s2mm_tuser] [get_bd_pins SCALER_1_VDMA/s_axis_s2mm_tuser]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_nets axi_scaler_1_s_axis_s2mm_tuser]
  connect_bd_net -net mm2s_prmry_reset_out_n_1 [get_bd_pins scaler_reset_n] [get_bd_pins axi_scaler_1/mm2s_prmry_reset_out_n] [get_bd_pins axi_scaler_1/s_axis_s2mm_aresetn]
  connect_bd_net -net xlconstant_2_const [get_bd_pins xlconstant_2/const] [get_bd_pins axi_scaler_1/m_axis_mm2s_tkeep]
  connect_bd_net -net xlconstant_3_const [get_bd_pins xlconstant_3/const] [get_bd_pins v_vid_in_axi4s_1/vid_data]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: mem_scaler_1
proc create_hier_cell_mem_scaler_1 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_mem_scaler_1() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_MM2S
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM

  # Create pins
  create_bd_pin -dir I -type clk m_axis_mm2s_aclk
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I fsync
  create_bd_pin -dir O -type intr mm2s_introut
  create_bd_pin -dir I -type clk s_axi_aclk
  create_bd_pin -dir I -from 0 -to 0 -type rst s_axi_aresetn
  create_bd_pin -dir I -from 0 -to 0 scaler_reset_n

  # Create instance: SCALER_0_VDMA, and set properties
  set SCALER_0_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 SCALER_0_VDMA ]
  set_property -dict [ list CONFIG.c_m_axi_mm2s_data_width {64} CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_mm2s_fsync {1} CONFIG.c_use_s2mm_fsync {1} CONFIG.c_mm2s_genlock_mode {1} CONFIG.c_s2mm_genlock_mode {1} CONFIG.c_mm2s_linebuffer_depth {4096} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_mm2s_max_burst_length {256} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_0 {1} CONFIG.c_enable_debug_info_7 {0} CONFIG.c_enable_debug_info_8 {1} CONFIG.c_enable_debug_info_9 {0} CONFIG.c_enable_debug_info_10 {0} CONFIG.c_enable_debug_info_11 {1} CONFIG.c_enable_debug_info_12 {1} CONFIG.c_enable_debug_info_13 {1} CONFIG.c_enable_debug_info_14 {0} CONFIG.c_include_mm2s_dre {1}  ] $SCALER_0_VDMA

  # Create instance: axi_scaler_0, and set properties
  set axi_scaler_0 [ create_bd_cell -type ip -vlnv xilinx.com:axi_scaler:axi_scaler:2.0 axi_scaler_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net scaler_0_vdma_m_axis_mm2s [get_bd_intf_pins SCALER_0_VDMA/M_AXIS_MM2S] [get_bd_intf_pins axi_scaler_0/m_axis_mm2s]
  connect_bd_intf_net -intf_net axi_scaler_1_s_axis_s2mm [get_bd_intf_pins axi_scaler_0/s_axis_s2mm] [get_bd_intf_pins SCALER_0_VDMA/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net axi4lite_1_m02 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins SCALER_0_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net scaler_0_vdma_m_axi_mm2s [get_bd_intf_pins M_AXI_MM2S] [get_bd_intf_pins SCALER_0_VDMA/M_AXI_MM2S]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets scaler_0_vdma_m_axi_mm2s]
  connect_bd_intf_net -intf_net scaler_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins SCALER_0_VDMA/M_AXI_S2MM]

  # Create port connections
  connect_bd_net -net clk_150Mhz [get_bd_pins m_axis_mm2s_aclk] [get_bd_pins SCALER_0_VDMA/m_axis_mm2s_aclk] [get_bd_pins SCALER_0_VDMA/m_axi_s2mm_aclk] [get_bd_pins SCALER_0_VDMA/s_axis_s2mm_aclk] [get_bd_pins SCALER_0_VDMA/m_axi_mm2s_aclk] [get_bd_pins axi_scaler_0/m_axi_s2mm_aclk] [get_bd_pins axi_scaler_0/m_axi_mm2s_aclk]
  connect_bd_net -net scaler_0_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins SCALER_0_VDMA/s2mm_introut]
  connect_bd_net -net dvi_in_0_vdma_s2mm_fsync_out [get_bd_pins fsync] [get_bd_pins SCALER_0_VDMA/mm2s_fsync] [get_bd_pins SCALER_0_VDMA/s2mm_fsync]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_nets dvi_in_0_vdma_s2mm_fsync_out]
  connect_bd_net -net scaler_0_vdma_mm2s_introut [get_bd_pins mm2s_introut] [get_bd_pins SCALER_0_VDMA/mm2s_introut]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_aclk] [get_bd_pins SCALER_0_VDMA/s_axi_lite_aclk]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_axi_aresetn] [get_bd_pins SCALER_0_VDMA/axi_resetn]
  connect_bd_net -net scaler_reset_n_1 [get_bd_pins scaler_reset_n] [get_bd_pins axi_scaler_0/mm2s_prmry_reset_out_n] [get_bd_pins axi_scaler_0/s_axis_s2mm_aresetn]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: v_in_pipe_1
proc create_hier_cell_v_in_pipe_1 { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_v_in_pipe_1() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 tpg_s_axi
  create_bd_intf_pin -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 vdma_s_axi
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M_AXI_S2MM

  # Create pins
  create_bd_pin -dir I -from 7 -to 0 blue
  create_bd_pin -dir I -from 0 -to 0 vsync_1
  create_bd_pin -dir I -from 0 -to 0 video_clk_0
  create_bd_pin -dir O s2mm_fsync_out
  create_bd_pin -dir I -from 0 -to 0 de_1
  create_bd_pin -dir I -from 0 -to 0 video_clk_1
  create_bd_pin -dir I -from 7 -to 0 green
  create_bd_pin -dir I -from 0 -to 0 -type rst rst
  create_bd_pin -dir I -from 0 -to 0 hsync_1
  create_bd_pin -dir O -type intr s2mm_introut
  create_bd_pin -dir I -type clk s_axi_lite_aclk
  create_bd_pin -dir I -from 7 -to 0 red
  create_bd_pin -dir I -from 0 -to 0 -type rst aresetn
  create_bd_pin -dir I -from 0 -to 0 de_0
  create_bd_pin -dir I -from 0 -to 0 vsync_0
  create_bd_pin -dir I -from 0 -to 0 hsync_0
  create_bd_pin -dir I -from 0 -to 0 video_sel

  # Create instance: video_data_1, and set properties
  set video_data_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 video_data_1 ]
  set_property -dict [ list CONFIG.NUM_PORTS {3}  ] $video_data_1

  # Create instance: vid_source_0, and set properties
  set vid_source_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tpg:5.0 vid_source_0 ]
  set_property -dict [ list CONFIG.pattern_control {10} CONFIG.active_cols {1920} CONFIG.active_rows {1080} CONFIG.s_video_format {2}  ] $vid_source_0

  # Create instance: video_mux_0, and set properties
  set video_mux_0 [ create_bd_cell -type ip -vlnv xilinx.com:user:vsrc_sel:1.0 video_mux_0 ]

  # Create instance: DVI2AXIS_0, and set properties
  set DVI2AXIS_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_vid_in_axi4s:3.0 DVI2AXIS_0 ]
  set_property -dict [ list CONFIG.C_M_AXIS_VIDEO_FORMAT {2} CONFIG.RAM_ADDR_BITS {12}  ] $DVI2AXIS_0

  # Create instance: DVI_IN_0_VDMA, and set properties
  set DVI_IN_0_VDMA [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.0 DVI_IN_0_VDMA ]
  set_property -dict [ list CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_use_s2mm_fsync {2} CONFIG.c_s2mm_genlock_mode {0} CONFIG.c_s2mm_linebuffer_depth {4096} CONFIG.c_include_mm2s {0} CONFIG.c_s2mm_max_burst_length {256} CONFIG.c_enable_debug_info_11 {1}  ] $DVI_IN_0_VDMA

  # Create instance: axis_subset_converter_0, and set properties
  set axis_subset_converter_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_subset_converter:1.0 axis_subset_converter_0 ]
  set_property -dict [ list CONFIG.S_TDATA_NUM_BYTES {3} CONFIG.M_TDATA_NUM_BYTES {4} CONFIG.TDATA_REMAP {8'b11111111,tdata[23:16],tdata[7:0],tdata[15:8]}  ] $axis_subset_converter_0

  # Create interface connections
  connect_bd_intf_net -intf_net v_vid_in_axi4s_1_video_out [get_bd_intf_pins DVI2AXIS_0/video_out] [get_bd_intf_pins vid_source_0/video_in]
  connect_bd_intf_net -intf_net vid_source_0_video_out [get_bd_intf_pins axis_subset_converter_0/S_AXIS] [get_bd_intf_pins vid_source_0/video_out]
  connect_bd_intf_net -intf_net axis_subset_converter_1_m_axis [get_bd_intf_pins axis_subset_converter_0/M_AXIS] [get_bd_intf_pins DVI_IN_0_VDMA/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net axi4lite_1_m05 [get_bd_intf_pins tpg_s_axi] [get_bd_intf_pins vid_source_0/ctrl]
  connect_bd_intf_net -intf_net axi4lite_1_m01 [get_bd_intf_pins vdma_s_axi] [get_bd_intf_pins DVI_IN_0_VDMA/S_AXI_LITE]
  connect_bd_intf_net -intf_net dvi_in_0_vdma_m_axi_s2mm [get_bd_intf_pins M_AXI_S2MM] [get_bd_intf_pins DVI_IN_0_VDMA/M_AXI_S2MM]

  # Create port connections
  connect_bd_net -net video_data_1_dout [get_bd_pins video_data_1/dout] [get_bd_pins DVI2AXIS_0/vid_data]
  connect_bd_net -net video_mux_0_video_clk [get_bd_pins video_mux_0/video_clk] [get_bd_pins DVI2AXIS_0/vid_io_in_clk]
  connect_bd_net -net video_mux_0_hsync [get_bd_pins video_mux_0/hsync] [get_bd_pins DVI2AXIS_0/vid_hsync]
  connect_bd_net -net video_mux_0_vsync [get_bd_pins video_mux_0/vsync] [get_bd_pins DVI2AXIS_0/vid_vsync]
  connect_bd_net -net video_mux_0_de [get_bd_pins video_mux_0/de] [get_bd_pins DVI2AXIS_0/vid_active_video]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_vsync_1 [get_bd_pins vsync_1] [get_bd_pins video_mux_0/vsync_2]
  connect_bd_net -net clk_150Mhz [get_bd_pins video_clk_0] [get_bd_pins video_mux_0/video_clk_1] [get_bd_pins DVI2AXIS_0/aclk] [get_bd_pins DVI_IN_0_VDMA/m_axi_s2mm_aclk] [get_bd_pins DVI_IN_0_VDMA/s_axis_s2mm_aclk] [get_bd_pins vid_source_0/aclk] [get_bd_pins axis_subset_converter_0/aclk]
  connect_bd_net -net dvi_in_0_vdma_s2mm_fsync_out [get_bd_pins s2mm_fsync_out] [get_bd_pins DVI_IN_0_VDMA/s2mm_fsync_out]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_de_1 [get_bd_pins de_1] [get_bd_pins video_mux_0/de_2]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_clk_1 [get_bd_pins video_clk_1] [get_bd_pins video_mux_0/video_clk_2]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins rst] [get_bd_pins DVI2AXIS_0/rst]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_hsync_1 [get_bd_pins hsync_1] [get_bd_pins video_mux_0/hsync_2]
  connect_bd_net -net dvi_in_0_vdma_s2mm_introut [get_bd_pins s2mm_introut] [get_bd_pins DVI_IN_0_VDMA/s2mm_introut]
  connect_bd_net -net clk_100Mhz [get_bd_pins s_axi_lite_aclk] [get_bd_pins DVI_IN_0_VDMA/s_axi_lite_aclk] [get_bd_pins vid_source_0/s_axi_aclk]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_green_1 [get_bd_pins red] [get_bd_pins video_data_1/In0]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins aresetn] [get_bd_pins DVI2AXIS_0/aresetn] [get_bd_pins DVI_IN_0_VDMA/axi_resetn] [get_bd_pins vid_source_0/s_axi_aresetn] [get_bd_pins vid_source_0/aresetn] [get_bd_pins axis_subset_converter_0/aresetn]
  connect_bd_net -net v_tc_1_active_video_out [get_bd_pins de_0] [get_bd_pins video_mux_0/de_1]
  connect_bd_net -net v_tc_1_vsync_out [get_bd_pins vsync_0] [get_bd_pins video_mux_0/vsync_1]
  connect_bd_net -net v_tc_1_hsync_out [get_bd_pins hsync_0] [get_bd_pins video_mux_0/hsync_1]
  connect_bd_net -net video_sel_1 [get_bd_pins video_sel] [get_bd_pins video_mux_0/video_sel]
  connect_bd_net -net blue_1 [get_bd_pins blue] [get_bd_pins video_data_1/In1]
  connect_bd_net -net green_1 [get_bd_pins green] [get_bd_pins video_data_1/In2]
  
  # Restore current instance
  current_bd_instance $oldCurInst
}

# Hierarchical cell: microblaze_subsystem
proc create_hier_cell_microblaze_subsystem { parentCell nameHier } {

  if { $parentCell eq "" || $nameHier eq "" } {
     puts "ERROR: create_hier_cell_microblaze_subsystem() - Empty argument(s)!"
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
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 lcd_gpio
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 rotary_gpio
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:iic_rtl:1.0 iic
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_gp
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 m_axi_hp
  create_bd_intf_pin -mode Master -vlnv xilinx.com:interface:gmii_rtl:1.0 gmii

  # Create pins
  create_bd_pin -dir I microblaze_clk
  create_bd_pin -dir I -type clk ACLK
  create_bd_pin -dir I -from 0 -to 0 -type rst ARESETN
  create_bd_pin -dir I -from 0 -to 0 -type rst s_aresetn
  create_bd_pin -dir I -type rst processor_rst
  create_bd_pin -dir O Debug_SYS_Rst
  create_bd_pin -dir I -from 0 -to 0 LMB_Rst
  create_bd_pin -dir O phy_rst_n
  create_bd_pin -dir I -type clk ethernet_gtx_clk
  create_bd_pin -dir I -type clk ethernet_ref_clk
  create_bd_pin -dir O mdc
  create_bd_pin -dir IO mdio
  create_bd_pin -dir I -from 4 -to 0 push_button_gpio
  create_bd_pin -dir I -from 3 -to 0 dip_sw_gpio
  create_bd_pin -dir O -from 7 -to 0 led_gpio
  create_bd_pin -dir O -from 0 -to 0 iic_gpo
  create_bd_pin -dir I vauxp0
  create_bd_pin -dir I vauxn0
  create_bd_pin -dir I vauxn8
  create_bd_pin -dir I vauxp8
  create_bd_pin -dir O -from 11 -to 0 temp_out
  create_bd_pin -dir I sin
  create_bd_pin -dir O sout
  create_bd_pin -dir O -from 0 -to 0 flash_cen
  create_bd_pin -dir O flash_wen
  create_bd_pin -dir O -from 0 -to 0 flash_oen
  create_bd_pin -dir O -from 15 -to 0 flash_dq_o
  create_bd_pin -dir I -from 15 -to 0 flash_dq_i
  create_bd_pin -dir O -from 15 -to 0 flash_dq_t
  create_bd_pin -dir O flash_adv_ldn
  create_bd_pin -dir O sd_clk
  create_bd_pin -dir I sd_base_lock
  create_bd_pin -dir O -from 3 -to 0 sd_dat_o
  create_bd_pin -dir I -from 3 -to 0 sd_dat_i
  create_bd_pin -dir O -from 3 -to 0 sd_dat_t
  create_bd_pin -dir O sd_cmd_o
  create_bd_pin -dir I sd_cmd_i
  create_bd_pin -dir O sd_cmd_t
  create_bd_pin -dir I sd_cd_n
  create_bd_pin -dir I sd_wp
  create_bd_pin -dir I -from 0 -to 0 Interrupt_8
  create_bd_pin -dir I -from 0 -to 0 Interrupt_9
  create_bd_pin -dir I -from 0 -to 0 Interrupt_6
  create_bd_pin -dir I -from 0 -to 0 Interrupt_10
  create_bd_pin -dir I -from 0 -to 0 Interrupt_2
  create_bd_pin -dir I -from 0 -to 0 Interrupt_7
  create_bd_pin -dir I -from 0 -to 0 Interrupt_5
  create_bd_pin -dir I -from 0 -to 0 Interrupt_1
  create_bd_pin -dir I -from 0 -to 0 Interrupt_3
  create_bd_pin -dir I -from 0 -to 0 Interrupt_4
  create_bd_pin -dir O -from 0 -to 0 const
  create_bd_pin -dir O -from 25 -to 0 Flash_address

  # Create instance: microblaze_0, and set properties
  set microblaze_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.1 microblaze_0 ]
  set_property -dict [ list CONFIG.C_FAULT_TOLERANT {0} CONFIG.C_D_AXI {1} CONFIG.C_D_LMB {1} CONFIG.C_I_LMB {1} CONFIG.C_DEBUG_ENABLED {1} CONFIG.C_ICACHE_BASEADDR {0x80000000} CONFIG.C_ICACHE_HIGHADDR {0xFFFFFFFF} CONFIG.C_USE_ICACHE {1} CONFIG.C_ICACHE_LINE_LEN {8} CONFIG.C_ICACHE_ALWAYS_USED {1} CONFIG.C_ICACHE_FORCE_TAG_LUTRAM {1} CONFIG.C_DCACHE_BASEADDR {0x80000000} CONFIG.C_DCACHE_HIGHADDR {0xFFFFFFFF} CONFIG.C_USE_DCACHE {1} CONFIG.C_DCACHE_LINE_LEN {8} CONFIG.C_DCACHE_ALWAYS_USED {1} CONFIG.C_DCACHE_FORCE_TAG_LUTRAM {1}  ] $microblaze_0

  # Create instance: debug_module, and set properties
  set debug_module [ create_bd_cell -type ip -vlnv xilinx.com:ip:mdm:3.0 debug_module ]
  set_property -dict [ list CONFIG.C_USE_UART {1}  ] $debug_module

  # Create instance: LEDs_8Bits, and set properties
  set LEDs_8Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 LEDs_8Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {8} CONFIG.C_ALL_OUTPUTS {1}  ] $LEDs_8Bits

  # Create instance: RS232_Uart_1, and set properties
  set RS232_Uart_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uart16550:2.0 RS232_Uart_1 ]

  # Create instance: Dual_Timer_Counter, and set properties
  set Dual_Timer_Counter [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 Dual_Timer_Counter ]

  # Create instance: Push_Buttons_5Bits, and set properties
  set Push_Buttons_5Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 Push_Buttons_5Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {5} CONFIG.C_ALL_INPUTS {1}  ] $Push_Buttons_5Bits

  # Create instance: DIP_Switches_4Bits, and set properties
  set DIP_Switches_4Bits [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 DIP_Switches_4Bits ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {4} CONFIG.C_ALL_INPUTS {1}  ] $DIP_Switches_4Bits

  # Create instance: LCD_GPIO, and set properties
  set LCD_GPIO [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 LCD_GPIO ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {7}  ] $LCD_GPIO

  # Create instance: ROTARY_GPIO, and set properties
  set ROTARY_GPIO [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 ROTARY_GPIO ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {3}  ] $ROTARY_GPIO

  # Create instance: Linear_Flash, and set properties
  set Linear_Flash [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_emc:2.0 Linear_Flash ]
  set_property -dict [ list CONFIG.C_WR_REC_TIME_MEM_0 {100000} CONFIG.C_TLZWE_PS_MEM_0 {35000} CONFIG.C_TWPH_PS_MEM_0 {12000} CONFIG.C_TWP_PS_MEM_0 {70000} CONFIG.C_TWC_PS_MEM_0 {70000} CONFIG.C_THZOE_PS_MEM_0 {7000} CONFIG.C_THZCE_PS_MEM_0 {35000} CONFIG.C_TAVDV_PS_MEM_0 {130000} CONFIG.C_TCEDV_PS_MEM_0 {130000} CONFIG.C_INCLUDE_DATAWIDTH_MATCHING_0 {1} CONFIG.C_MAX_MEM_WIDTH {16} CONFIG.C_MEM0_WIDTH {16} CONFIG.C_MEM0_TYPE {2}  ] $Linear_Flash

  # Create instance: IIC_EEPROM, and set properties
  set IIC_EEPROM [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_iic:2.0 IIC_EEPROM ]

  # Create instance: Interrupt_Cntlr, and set properties
  set Interrupt_Cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:3.1 Interrupt_Cntlr ]
  set_property -dict [ list CONFIG.C_HAS_FAST {0}  ] $Interrupt_Cntlr

  # Create instance: Internal_BRAM, and set properties
  set Internal_BRAM [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:3.0 Internal_BRAM ]

  # Create instance: Internal_BRAM_block, and set properties
  set Internal_BRAM_block [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.0 Internal_BRAM_block ]
  set_property -dict [ list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Enable_32bit_Address {true} CONFIG.use_bram_block {BRAM_Controller}  ] $Internal_BRAM_block

  # Create instance: AXI_DMA_Ethernet, and set properties
  set AXI_DMA_Ethernet [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.0 AXI_DMA_Ethernet ]
  set_property -dict [ list CONFIG.c_sg_length_width {16} CONFIG.c_include_mm2s_dre {1} CONFIG.c_sg_use_stsapp_length {1} CONFIG.c_include_s2mm_dre {1}  ] $AXI_DMA_Ethernet

  # Create instance: logisdhc_0, and set properties
  set logisdhc_0 [ create_bd_cell -type ip -vlnv xylon:logicbricks:logisdhc:1.07.a logisdhc_0 ]
  set_property -dict [ list CONFIG.C_USE_DMA {0}  ] $logisdhc_0

  # Create instance: axi_xadc_0, and set properties
  set axi_xadc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xadc_wiz:3.0 axi_xadc_0 ]
  set_property -dict [ list CONFIG.INTERFACE_SELECTION {Enable_AXI} CONFIG.XADC_STARUP_SELECTION {channel_sequencer} CONFIG.CHANNEL_ENABLE_VAUXP0_VAUXN0 {true} CONFIG.CHANNEL_ENABLE_VAUXP8_VAUXN8 {true} CONFIG.AVERAGE_ENABLE_VAUXP0_VAUXN0 {false} CONFIG.ENABLE_TEMP_BUS {true}  ] $axi_xadc_0

  # Create instance: microblaze_0_local_memory
  create_hier_cell_microblaze_0_local_memory $hier_obj microblaze_0_local_memory

  # Create instance: axi4lite_0, and set properties
  set axi4lite_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi4lite_0 ]
  set_property -dict [ list CONFIG.NUM_MI {16}  ] $axi4lite_0

  # Create instance: axi_mm_mb, and set properties
  set axi_mm_mb [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi_mm_mb ]
  set_property -dict [ list CONFIG.NUM_SI {5} CONFIG.NUM_MI {2} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {32} CONFIG.M00_HAS_REGSLICE {1} CONFIG.M01_HAS_REGSLICE {1} CONFIG.S01_HAS_REGSLICE {1} CONFIG.S02_HAS_REGSLICE {1} CONFIG.S03_HAS_REGSLICE {1} CONFIG.S04_HAS_REGSLICE {1}  ] $axi_mm_mb

  # Create instance: Soft_Ethernet_MAC, and set properties
  set Soft_Ethernet_MAC [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_ethernet:5.0 Soft_Ethernet_MAC ]
  set_property -dict [ list CONFIG.Statistics_Counters {false}  ] $Soft_Ethernet_MAC

  # Create instance: intr_concat_1, and set properties
  set intr_concat_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 intr_concat_1 ]
  set_property -dict [ list CONFIG.NUM_PORTS {5}  ] $intr_concat_1

  # Create instance: int_ctrl_concat, and set properties
  set int_ctrl_concat [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:1.0 int_ctrl_concat ]
  set_property -dict [ list CONFIG.NUM_PORTS {14} CONFIG.IN15_WIDTH {1}  ] $int_ctrl_concat

  # Create instance: logic_high, and set properties
  set logic_high [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.0 logic_high ]

  # Create instance: flash_addr_slice, and set properties
  set flash_addr_slice [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 flash_addr_slice ]
  set_property -dict [ list CONFIG.DIN_TO {1} CONFIG.DIN_FROM {26}  ] $flash_addr_slice

  # Create interface connections
  connect_bd_intf_net -intf_net axi_intc_1_interrupt [get_bd_intf_pins Interrupt_Cntlr/interrupt] [get_bd_intf_pins microblaze_0/INTERRUPT]
  connect_bd_intf_net -intf_net microblaze_0_dlmb [get_bd_intf_pins microblaze_0/DLMB] [get_bd_intf_pins microblaze_0_local_memory/DLMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb [get_bd_intf_pins microblaze_0/ILMB] [get_bd_intf_pins microblaze_0_local_memory/ILMB]
  connect_bd_intf_net -intf_net axi4lite_0_s00 [get_bd_intf_pins axi4lite_0/S00_AXI] [get_bd_intf_pins microblaze_0/M_AXI_DP]
  connect_bd_intf_net -intf_net axi_mm_mb_dc [get_bd_intf_pins microblaze_0/M_AXI_DC] [get_bd_intf_pins axi_mm_mb/S01_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_ic [get_bd_intf_pins microblaze_0/M_AXI_IC] [get_bd_intf_pins axi_mm_mb/S00_AXI]
  connect_bd_intf_net -intf_net microblaze_0_debug [get_bd_intf_pins debug_module/MBDEBUG_0] [get_bd_intf_pins microblaze_0/DEBUG]
  connect_bd_intf_net -intf_net axi4lite_0_m00 [get_bd_intf_pins axi4lite_0/M00_AXI] [get_bd_intf_pins debug_module/S_AXI]
  connect_bd_intf_net -intf_net axi4lite_0_m01 [get_bd_intf_pins axi4lite_0/M01_AXI] [get_bd_intf_pins LEDs_8Bits/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m02 [get_bd_intf_pins axi4lite_0/M02_AXI] [get_bd_intf_pins RS232_Uart_1/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m03 [get_bd_intf_pins axi4lite_0/M03_AXI] [get_bd_intf_pins Dual_Timer_Counter/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m04 [get_bd_intf_pins axi4lite_0/M04_AXI] [get_bd_intf_pins Push_Buttons_5Bits/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m05 [get_bd_intf_pins axi4lite_0/M05_AXI] [get_bd_intf_pins DIP_Switches_4Bits/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m06 [get_bd_intf_pins axi4lite_0/M06_AXI] [get_bd_intf_pins LCD_GPIO/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m07 [get_bd_intf_pins axi4lite_0/M07_AXI] [get_bd_intf_pins ROTARY_GPIO/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m08 [get_bd_intf_pins axi4lite_0/M08_AXI] [get_bd_intf_pins Linear_Flash/s_axi_mem]
  connect_bd_intf_net -intf_net axi4lite_0_m09 [get_bd_intf_pins axi4lite_0/M09_AXI] [get_bd_intf_pins IIC_EEPROM/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m10 [get_bd_intf_pins axi4lite_0/M10_AXI] [get_bd_intf_pins Interrupt_Cntlr/s_axi]
  connect_bd_intf_net -intf_net axi_mm_mb_bram [get_bd_intf_pins axi_mm_mb/M00_AXI] [get_bd_intf_pins Internal_BRAM/S_AXI]
  connect_bd_intf_net -intf_net internal_bram_bram_porta [get_bd_intf_pins Internal_BRAM_block/BRAM_PORTA] [get_bd_intf_pins Internal_BRAM/BRAM_PORTA]
  connect_bd_intf_net -intf_net internal_bram_bram_portb [get_bd_intf_pins Internal_BRAM_block/BRAM_PORTB] [get_bd_intf_pins Internal_BRAM/BRAM_PORTB]
  connect_bd_intf_net -intf_net axi4lite_0_m11 [get_bd_intf_pins axi4lite_0/M11_AXI] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXI_LITE]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_sg [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_SG] [get_bd_intf_pins axi_mm_mb/S02_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_mm2s [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_MM2S] [get_bd_intf_pins axi_mm_mb/S03_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_dma_s2mm [get_bd_intf_pins AXI_DMA_Ethernet/M_AXI_S2MM] [get_bd_intf_pins axi_mm_mb/S04_AXI]
  connect_bd_intf_net -intf_net ethernet_txd [get_bd_intf_pins AXI_DMA_Ethernet/M_AXIS_MM2S] [get_bd_intf_pins Soft_Ethernet_MAC/axi_str_txd]
  connect_bd_intf_net -intf_net ethernet_rxd [get_bd_intf_pins Soft_Ethernet_MAC/axi_str_rxd] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net ethernet_cntrl [get_bd_intf_pins AXI_DMA_Ethernet/M_AXIS_CNTRL] [get_bd_intf_pins Soft_Ethernet_MAC/axi_str_txc]
  connect_bd_intf_net -intf_net ethernet_sts [get_bd_intf_pins Soft_Ethernet_MAC/axi_str_rxs] [get_bd_intf_pins AXI_DMA_Ethernet/S_AXIS_STS]
  connect_bd_intf_net -intf_net axi4lite_0_m13 [get_bd_intf_pins axi4lite_0/M13_AXI] [get_bd_intf_pins logisdhc_0/s_axi]
  connect_bd_intf_net -intf_net axi4lite_0_m14 [get_bd_intf_pins axi4lite_0/M14_AXI] [get_bd_intf_pins axi_xadc_0/s_axi_lite]
  connect_bd_intf_net -intf_net axi4lite_0_m12 [get_bd_intf_pins axi4lite_0/M12_AXI] [get_bd_intf_pins Soft_Ethernet_MAC/s_axi]
  connect_bd_intf_net -intf_net lcd_gpio_port [get_bd_intf_pins lcd_gpio] [get_bd_intf_pins LCD_GPIO/gpio]
  connect_bd_intf_net -intf_net rotary_gpio_port [get_bd_intf_pins rotary_gpio] [get_bd_intf_pins ROTARY_GPIO/gpio]
  connect_bd_intf_net -intf_net eeprom_port [get_bd_intf_pins iic] [get_bd_intf_pins IIC_EEPROM/iic]
  connect_bd_intf_net -intf_net axi4lite_0_m15 [get_bd_intf_pins m_axi_gp] [get_bd_intf_pins axi4lite_0/M15_AXI]
  connect_bd_intf_net -intf_net axi_mm_mb_m01_axi [get_bd_intf_pins m_axi_hp] [get_bd_intf_pins axi_mm_mb/M01_AXI]
  connect_bd_intf_net -intf_net soft_ethernet_mac_gmii [get_bd_intf_pins gmii] [get_bd_intf_pins Soft_Ethernet_MAC/gmii]

  # Create port connections
  connect_bd_net -net AXI_STR_TXD_ARESETN [get_bd_pins AXI_DMA_Ethernet/mm2s_prmry_reset_out_n] [get_bd_pins Soft_Ethernet_MAC/axi_txd_arstn]
  connect_bd_net -net AXI_STR_TXC_ARESETN [get_bd_pins AXI_DMA_Ethernet/mm2s_cntrl_reset_out_n] [get_bd_pins Soft_Ethernet_MAC/axi_txc_arstn]
  connect_bd_net -net AXI_STR_RXD_ARESETN [get_bd_pins AXI_DMA_Ethernet/s2mm_prmry_reset_out_n] [get_bd_pins Soft_Ethernet_MAC/axi_rxd_arstn]
  connect_bd_net -net AXI_STR_RXS_ARESETN [get_bd_pins AXI_DMA_Ethernet/s2mm_sts_reset_out_n] [get_bd_pins Soft_Ethernet_MAC/axi_rxs_arstn]
  connect_bd_net -net soft_ethernet_mac_phy_rst_n [get_bd_pins phy_rst_n] [get_bd_pins Soft_Ethernet_MAC/phy_rst_n]
  connect_bd_net -net sd_wp_1 [get_bd_pins sd_wp] [get_bd_pins logisdhc_0/sd_wp]
  connect_bd_net -net mem_dq_i_1 [get_bd_pins flash_dq_i] [get_bd_pins Linear_Flash/mem_dq_i]
  connect_bd_net -net axi_emc_1_mem_dq_t [get_bd_pins flash_dq_t] [get_bd_pins Linear_Flash/mem_dq_t]
  connect_bd_net -net vauxp0_1 [get_bd_pins vauxp0] [get_bd_pins axi_xadc_0/vauxp0]
  connect_bd_net -net axi_uart16550_1_sout [get_bd_pins sout] [get_bd_pins RS232_Uart_1/sout]
  connect_bd_net -net vauxp8_1 [get_bd_pins vauxp8] [get_bd_pins axi_xadc_0/vauxp8]
  connect_bd_net -net axi_emc_1_mem_dq_o [get_bd_pins flash_dq_o] [get_bd_pins Linear_Flash/mem_dq_o]
  connect_bd_net -net axi_iic_1_gpo [get_bd_pins iic_gpo] [get_bd_pins IIC_EEPROM/gpo]
  connect_bd_net -net clk_150Mhz [get_bd_pins microblaze_clk] [get_bd_pins microblaze_0_local_memory/LMB_Clk] [get_bd_pins axi4lite_0/S00_ACLK] [get_bd_pins axi_mm_mb/ACLK] [get_bd_pins axi_mm_mb/S00_ACLK] [get_bd_pins axi_mm_mb/S01_ACLK] [get_bd_pins Interrupt_Cntlr/processor_clk] [get_bd_pins axi_mm_mb/S02_ACLK] [get_bd_pins axi_mm_mb/S03_ACLK] [get_bd_pins axi_mm_mb/S04_ACLK] [get_bd_pins AXI_DMA_Ethernet/m_axi_sg_aclk] [get_bd_pins AXI_DMA_Ethernet/m_axi_mm2s_aclk] [get_bd_pins AXI_DMA_Ethernet/m_axi_s2mm_aclk] [get_bd_pins Soft_Ethernet_MAC/axis_clk] [get_bd_pins microblaze_0/Clk] [get_bd_pins axi_mm_mb/M01_ACLK]
  connect_bd_net -net logisdhc_0_sd_cmd_t [get_bd_pins sd_cmd_t] [get_bd_pins logisdhc_0/sd_cmd_t]
  connect_bd_net -net ethernet_clk_125 [get_bd_pins ethernet_gtx_clk] [get_bd_pins Soft_Ethernet_MAC/gtx_clk]
  connect_bd_net -net push_buttons_5bits_tri_i_1 [get_bd_pins push_button_gpio] [get_bd_pins Push_Buttons_5Bits/gpio_io_i]
  connect_bd_net -net logisdhc_0_sd_clk [get_bd_pins sd_clk] [get_bd_pins logisdhc_0/sd_clk]
  connect_bd_net -net soft_ethernet_mac_mdc [get_bd_pins mdc] [get_bd_pins Soft_Ethernet_MAC/mdc]
  connect_bd_net -net proc_sys_reset_1_mb_reset [get_bd_pins processor_rst] [get_bd_pins Interrupt_Cntlr/processor_rst] [get_bd_pins microblaze_0/Reset]
  connect_bd_net -net xadc_wiz_1_temp_out [get_bd_pins temp_out] [get_bd_pins axi_xadc_0/temp_out]
  connect_bd_net -net mig_clk_200 [get_bd_pins ethernet_ref_clk] [get_bd_pins Soft_Ethernet_MAC/ref_clk]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins Debug_SYS_Rst] [get_bd_pins debug_module/Debug_SYS_Rst]
  connect_bd_net -net Net [get_bd_pins mdio] [get_bd_pins Soft_Ethernet_MAC/mdio]
  connect_bd_net -net axi_emc_1_mem_adv_ldn [get_bd_pins flash_adv_ldn] [get_bd_pins Linear_Flash/mem_adv_ldn]
  connect_bd_net -net clk_wiz_1_locked [get_bd_pins sd_base_lock] [get_bd_pins logisdhc_0/sd_base_lock]
  connect_bd_net -net dip_switches_4bits_tri_i_1 [get_bd_pins dip_sw_gpio] [get_bd_pins DIP_Switches_4Bits/gpio_io_i]
  connect_bd_net -net axi_emc_1_mem_cen [get_bd_pins flash_cen] [get_bd_pins Linear_Flash/mem_cen]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins LMB_Rst] [get_bd_pins microblaze_0_local_memory/LMB_Rst]
  connect_bd_net -net proc_sys_reset_1_interconnect_aresetn [get_bd_pins ARESETN] [get_bd_pins axi_mm_mb/ARESETN] [get_bd_pins axi4lite_0/ARESETN]
  connect_bd_net -net clk_100Mhz [get_bd_pins ACLK] [get_bd_pins axi4lite_0/ACLK] [get_bd_pins axi4lite_0/M00_ACLK] [get_bd_pins LEDs_8Bits/s_axi_aclk] [get_bd_pins axi4lite_0/M01_ACLK] [get_bd_pins axi4lite_0/M02_ACLK] [get_bd_pins RS232_Uart_1/s_axi_aclk] [get_bd_pins Dual_Timer_Counter/s_axi_aclk] [get_bd_pins axi4lite_0/M03_ACLK] [get_bd_pins Push_Buttons_5Bits/s_axi_aclk] [get_bd_pins axi4lite_0/M04_ACLK] [get_bd_pins DIP_Switches_4Bits/s_axi_aclk] [get_bd_pins axi4lite_0/M05_ACLK] [get_bd_pins LCD_GPIO/s_axi_aclk] [get_bd_pins axi4lite_0/M06_ACLK] [get_bd_pins ROTARY_GPIO/s_axi_aclk] [get_bd_pins axi4lite_0/M07_ACLK] [get_bd_pins axi4lite_0/M08_ACLK] [get_bd_pins Linear_Flash/s_axi_aclk] [get_bd_pins Linear_Flash/rdclk] [get_bd_pins IIC_EEPROM/s_axi_aclk] [get_bd_pins axi4lite_0/M09_ACLK] [get_bd_pins Interrupt_Cntlr/s_axi_aclk] [get_bd_pins axi4lite_0/M10_ACLK] [get_bd_pins Internal_BRAM/S_AXI_ACLK] [get_bd_pins axi_mm_mb/M00_ACLK] [get_bd_pins axi4lite_0/M11_ACLK] [get_bd_pins AXI_DMA_Ethernet/s_axi_lite_aclk] [get_bd_pins Soft_Ethernet_MAC/s_axi_lite_clk] [get_bd_pins axi4lite_0/M12_ACLK] [get_bd_pins axi4lite_0/M13_ACLK] [get_bd_pins logisdhc_0/S_AXI_ACLK] [get_bd_pins logisdhc_0/sd_base_clk] [get_bd_pins axi4lite_0/M14_ACLK] [get_bd_pins axi_xadc_0/s_axi_aclk] [get_bd_pins debug_module/S_AXI_ACLK] [get_bd_pins axi4lite_0/M15_ACLK]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins s_aresetn] [get_bd_pins axi4lite_0/S00_ARESETN] [get_bd_pins axi4lite_0/M00_ARESETN] [get_bd_pins LEDs_8Bits/s_axi_aresetn] [get_bd_pins axi4lite_0/M01_ARESETN] [get_bd_pins axi4lite_0/M02_ARESETN] [get_bd_pins RS232_Uart_1/s_axi_aresetn] [get_bd_pins Dual_Timer_Counter/s_axi_aresetn] [get_bd_pins axi4lite_0/M03_ARESETN] [get_bd_pins Push_Buttons_5Bits/s_axi_aresetn] [get_bd_pins axi4lite_0/M04_ARESETN] [get_bd_pins DIP_Switches_4Bits/s_axi_aresetn] [get_bd_pins axi4lite_0/M05_ARESETN] [get_bd_pins LCD_GPIO/s_axi_aresetn] [get_bd_pins axi4lite_0/M06_ARESETN] [get_bd_pins ROTARY_GPIO/s_axi_aresetn] [get_bd_pins axi4lite_0/M07_ARESETN] [get_bd_pins axi4lite_0/M08_ARESETN] [get_bd_pins Linear_Flash/s_axi_aresetn] [get_bd_pins IIC_EEPROM/s_axi_aresetn] [get_bd_pins axi4lite_0/M09_ARESETN] [get_bd_pins Interrupt_Cntlr/s_axi_aresetn] [get_bd_pins axi4lite_0/M10_ARESETN] [get_bd_pins Internal_BRAM/S_AXI_ARESETN] [get_bd_pins axi_mm_mb/S00_ARESETN] [get_bd_pins axi_mm_mb/S01_ARESETN] [get_bd_pins axi_mm_mb/M00_ARESETN] [get_bd_pins axi_mm_mb/M01_ARESETN] [get_bd_pins axi_mm_mb/S02_ARESETN] [get_bd_pins axi_mm_mb/S03_ARESETN] [get_bd_pins axi_mm_mb/S04_ARESETN] [get_bd_pins AXI_DMA_Ethernet/axi_resetn] [get_bd_pins axi4lite_0/M11_ARESETN] [get_bd_pins Soft_Ethernet_MAC/s_axi_lite_resetn] [get_bd_pins axi4lite_0/M12_ARESETN] [get_bd_pins axi4lite_0/M13_ARESETN] [get_bd_pins logisdhc_0/S_AXI_ARESETN] [get_bd_pins axi4lite_0/M14_ARESETN] [get_bd_pins axi_xadc_0/s_axi_aresetn] [get_bd_pins axi4lite_0/M15_ARESETN] [get_bd_pins debug_module/S_AXI_ARESETN]
  connect_bd_net -net leds_8bits_gpio_io_o [get_bd_pins led_gpio] [get_bd_pins LEDs_8Bits/gpio_io_o]
  connect_bd_net -net vauxn0_1 [get_bd_pins vauxn0] [get_bd_pins axi_xadc_0/vauxn0]
  connect_bd_net -net sin_1 [get_bd_pins sin] [get_bd_pins RS232_Uart_1/sin]
  connect_bd_net -net logisdhc_0_sd_dat_o [get_bd_pins sd_dat_o] [get_bd_pins logisdhc_0/sd_dat_o]
  connect_bd_net -net sd_dat_i_1 [get_bd_pins sd_dat_i] [get_bd_pins logisdhc_0/sd_dat_i]
  connect_bd_net -net axi_emc_1_mem_oen [get_bd_pins flash_oen] [get_bd_pins Linear_Flash/mem_oen]
  connect_bd_net -net logisdhc_0_sd_cmd_o [get_bd_pins sd_cmd_o] [get_bd_pins logisdhc_0/sd_cmd_o]
  connect_bd_net -net sd_cmd_i_1 [get_bd_pins sd_cmd_i] [get_bd_pins logisdhc_0/sd_cmd_i]
  connect_bd_net -net vauxn8_1 [get_bd_pins vauxn8] [get_bd_pins axi_xadc_0/vauxn8]
  connect_bd_net -net axi_emc_1_mem_wen [get_bd_pins flash_wen] [get_bd_pins Linear_Flash/mem_wen]
  connect_bd_net -net logisdhc_0_sd_dat_t [get_bd_pins sd_dat_t] [get_bd_pins logisdhc_0/sd_dat_t]
  connect_bd_net -net sd_cd_n_1 [get_bd_pins sd_cd_n] [get_bd_pins logisdhc_0/sd_cd_n]
  connect_bd_net -net xlconcat_2_dout [get_bd_pins intr_concat_1/dout] [get_bd_pins int_ctrl_concat/In13]
  connect_bd_net -net axi_xadc_0_ip2intc_irpt [get_bd_pins axi_xadc_0/ip2intc_irpt] [get_bd_pins int_ctrl_concat/In0]
  connect_bd_net -net logisdhc_0_sd_int [get_bd_pins logisdhc_0/sd_int] [get_bd_pins int_ctrl_concat/In11]
  connect_bd_net -net in8_1 [get_bd_pins Interrupt_8] [get_bd_pins int_ctrl_concat/In8]
  connect_bd_net -net in9_1 [get_bd_pins Interrupt_9] [get_bd_pins int_ctrl_concat/In9]
  connect_bd_net -net in6_1 [get_bd_pins Interrupt_6] [get_bd_pins int_ctrl_concat/In6]
  connect_bd_net -net in10_1 [get_bd_pins Interrupt_10] [get_bd_pins int_ctrl_concat/In10]
  connect_bd_net -net in2_1 [get_bd_pins Interrupt_2] [get_bd_pins int_ctrl_concat/In2]
  connect_bd_net -net in7_1 [get_bd_pins Interrupt_7] [get_bd_pins int_ctrl_concat/In7]
  connect_bd_net -net in5_1 [get_bd_pins Interrupt_5] [get_bd_pins int_ctrl_concat/In5]
  connect_bd_net -net in1_1 [get_bd_pins Interrupt_1] [get_bd_pins int_ctrl_concat/In1]
  connect_bd_net -net soft_ethernet_mac_interrupt [get_bd_pins Soft_Ethernet_MAC/interrupt] [get_bd_pins intr_concat_1/In2]
  connect_bd_net -net in3_1 [get_bd_pins Interrupt_3] [get_bd_pins int_ctrl_concat/In3]
  connect_bd_net -net in4_1 [get_bd_pins Interrupt_4] [get_bd_pins int_ctrl_concat/In4]
  connect_bd_net -net axi_dma_ethernet_mm2s_introut [get_bd_pins AXI_DMA_Ethernet/mm2s_introut] [get_bd_pins intr_concat_1/In4]
  connect_bd_net -net axi_dma_ethernet_s2mm_introut [get_bd_pins AXI_DMA_Ethernet/s2mm_introut] [get_bd_pins intr_concat_1/In3]
  connect_bd_net -net rs232_uart_1_ip2intc_irpt [get_bd_pins RS232_Uart_1/ip2intc_irpt] [get_bd_pins int_ctrl_concat/In12]
  connect_bd_net -net int_ctrl_concat_dout [get_bd_pins int_ctrl_concat/dout] [get_bd_pins Interrupt_Cntlr/intr]
  connect_bd_net -net dual_timer_counter_interrupt [get_bd_pins Dual_Timer_Counter/interrupt] [get_bd_pins intr_concat_1/In1]
  connect_bd_net -net iic_eeprom_iic2intc_irpt [get_bd_pins IIC_EEPROM/iic2intc_irpt] [get_bd_pins intr_concat_1/In0]
  connect_bd_net -net logic_high_const [get_bd_pins const] [get_bd_pins logic_high/const]
  connect_bd_net -net flash_addr_slice_dout [get_bd_pins Flash_address] [get_bd_pins flash_addr_slice/Dout]
  connect_bd_net -net linear_flash_mem_a [get_bd_pins Linear_Flash/mem_a] [get_bd_pins flash_addr_slice/Din]
  
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

  # Create ports
  set DVI_IN_0_fmc_hpc_dvidp_dvii_clk [ create_bd_port -dir I -from 0 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_clk ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_hsync [ create_bd_port -dir I -from 0 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_hsync ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_vsync [ create_bd_port -dir I -from 0 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_vsync ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_de [ create_bd_port -dir I -from 0 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_de ]
  set RESET [ create_bd_port -dir I -type rst RESET ]
  set_property -dict [ list CONFIG.POLARITY {ACTIVE_HIGH}  ] $RESET
  set RS232_UART_1_sout [ create_bd_port -dir O RS232_UART_1_sout ]
  set RS232_UART_1_sin [ create_bd_port -dir I RS232_UART_1_sin ]
  set IIC_MUX_RESET_B [ create_bd_port -dir O -from 0 -to 0 IIC_MUX_RESET_B ]
  set Push_Buttons_5Bits_TRI_I [ create_bd_port -dir I -from 4 -to 0 Push_Buttons_5Bits_TRI_I ]
  set DIP_Switches_TRI_I [ create_bd_port -dir I -from 3 -to 0 DIP_Switches_TRI_I ]
  set Linear_Flash_we_n [ create_bd_port -dir O Linear_Flash_we_n ]
  set Linear_Flash_oe_n [ create_bd_port -dir O -from 0 -to 0 Linear_Flash_oe_n ]
  set Linear_Flash_ce_n [ create_bd_port -dir O -from 0 -to 0 Linear_Flash_ce_n ]
  set Linear_Flash_Data_T [ create_bd_port -dir O -from 15 -to 0 Linear_Flash_Data_T ]
  set Linear_Flash_Data_O [ create_bd_port -dir O -from 15 -to 0 Linear_Flash_Data_O ]
  set Linear_Flash_Data_I [ create_bd_port -dir I -from 15 -to 0 Linear_Flash_Data_I ]
  set Linear_Flash_adv_ldn [ create_bd_port -dir O Linear_Flash_adv_ldn ]
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
  set Soft_Ethernet_MAC_RST_n_pin [ create_bd_port -dir O Soft_Ethernet_MAC_RST_n_pin ]
  set init_calib_complete [ create_bd_port -dir O init_calib_complete ]
  set ddr3_1_n [ create_bd_port -dir O -from 2 -to 0 ddr3_1_n ]
  set ddr3_1_p [ create_bd_port -dir O -from 1 -to 0 ddr3_1_p ]
  set axi_xadc_0_VAUXP [ create_bd_port -dir I axi_xadc_0_VAUXP ]
  set axi_xadc_0_VAUXN [ create_bd_port -dir I axi_xadc_0_VAUXN ]
  set axi_xadc_8_VAUXP [ create_bd_port -dir I axi_xadc_8_VAUXP ]
  set axi_xadc_8_VAUXN [ create_bd_port -dir I axi_xadc_8_VAUXN ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_green [ create_bd_port -dir I -from 7 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_green ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_red [ create_bd_port -dir I -from 7 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_red ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_clk [ create_bd_port -dir I -from 0 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_clk ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_hsync [ create_bd_port -dir I -from 0 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_hsync ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_vsync [ create_bd_port -dir I -from 0 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_vsync ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_de [ create_bd_port -dir I -from 0 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_de ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_green [ create_bd_port -dir I -from 7 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_green ]
  set LEDs_8Bits_TRI_O [ create_bd_port -dir O -from 7 -to 0 LEDs_8Bits_TRI_O ]
  set CLK_N [ create_bd_port -dir I CLK_N ]
  set CLK_P [ create_bd_port -dir I CLK_P ]
  set dvi_24_to_16bit_hdmi_clk [ create_bd_port -dir O dvi_24_to_16bit_hdmi_clk ]
  set dvi_24_to_16bit_hdmi_data [ create_bd_port -dir O -from 15 -to 0 -type data dvi_24_to_16bit_hdmi_data ]
  set dvi_24_to_16bit_hsync [ create_bd_port -dir O dvi_24_to_16bit_hsync ]
  set dvi_24_to_16bit_vsync [ create_bd_port -dir O dvi_24_to_16bit_vsync ]
  set dvi_24_to_16bit_de [ create_bd_port -dir O dvi_24_to_16bit_de ]
  set DVI_IN_0_fmc_hpc_dvidp_dvii_blue [ create_bd_port -dir I -from 7 -to 0 DVI_IN_0_fmc_hpc_dvidp_dvii_blue ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_red [ create_bd_port -dir I -from 7 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_red ]
  set DVI_IN_1_fmc_lpc_dvidp_dvii_blue [ create_bd_port -dir I -from 7 -to 0 DVI_IN_1_fmc_lpc_dvidp_dvii_blue ]
  set sm_fan_pwm [ create_bd_port -dir O -from 0 -to 0 sm_fan_pwm ]
  set Linear_Flash_address [ create_bd_port -dir O -from 0 -to 25 Linear_Flash_address ]

  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]

  # Create instance: clock_generator_1, and set properties
  set clock_generator_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.0 clock_generator_1 ]
  set_property -dict [ list CONFIG.PRIM_IN_FREQ {200.000} CONFIG.CLKOUT2_USED {true} CONFIG.CLKOUT3_USED {false} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {150.000} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {125.000} CONFIG.PRIM_SOURCE {No_buffer} CONFIG.CLKOUT2_DRIVES {BUFG} CONFIG.USE_RESET {false}  ] $clock_generator_1

  # Create instance: microblaze_subsystem
  create_hier_cell_microblaze_subsystem [current_bd_instance .] microblaze_subsystem

  # Create instance: axi4lite_1, and set properties
  set axi4lite_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi4lite_1 ]
  set_property -dict [ list CONFIG.NUM_MI {14}  ] $axi4lite_1

  # Create instance: axi_mm_video01, and set properties
  set axi_mm_video01 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi_mm_video01 ]
  set_property -dict [ list CONFIG.NUM_SI {4} CONFIG.NUM_MI {1} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {512} CONFIG.M00_HAS_REGSLICE {1} CONFIG.M00_HAS_DATA_FIFO {2} CONFIG.S00_HAS_REGSLICE {1} CONFIG.S01_HAS_REGSLICE {1} CONFIG.S02_HAS_REGSLICE {1} CONFIG.S03_HAS_REGSLICE {1} CONFIG.S00_HAS_DATA_FIFO {2} CONFIG.S01_HAS_DATA_FIFO {2} CONFIG.S02_HAS_DATA_FIFO {2} CONFIG.S03_HAS_DATA_FIFO {2}  ] $axi_mm_video01

  # Create instance: axi_mm_video23, and set properties
  set axi_mm_video23 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi_mm_video23 ]
  set_property -dict [ list CONFIG.NUM_SI {4} CONFIG.NUM_MI {1} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {512} CONFIG.M00_HAS_REGSLICE {1} CONFIG.M00_HAS_DATA_FIFO {2} CONFIG.S00_HAS_REGSLICE {1} CONFIG.S01_HAS_REGSLICE {1} CONFIG.S02_HAS_REGSLICE {1} CONFIG.S03_HAS_REGSLICE {1} CONFIG.S00_HAS_DATA_FIFO {2} CONFIG.S01_HAS_DATA_FIFO {2} CONFIG.S02_HAS_DATA_FIFO {2} CONFIG.S03_HAS_DATA_FIFO {2}  ] $axi_mm_video23

  # Create instance: axi4_0, and set properties
  set axi4_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.0 axi4_0 ]
  set_property -dict [ list CONFIG.NUM_SI {4} CONFIG.NUM_MI {1} CONFIG.ENABLE_ADVANCED_OPTIONS {1} CONFIG.XBAR_DATA_WIDTH {512} CONFIG.M00_HAS_REGSLICE {1} CONFIG.M00_HAS_DATA_FIFO {2} CONFIG.S00_HAS_REGSLICE {1} CONFIG.S01_HAS_REGSLICE {1} CONFIG.S02_HAS_REGSLICE {1} CONFIG.S03_HAS_REGSLICE {1} CONFIG.S00_HAS_DATA_FIFO {0} CONFIG.S01_HAS_DATA_FIFO {2} CONFIG.S02_HAS_DATA_FIFO {2} CONFIG.S03_HAS_DATA_FIFO {2}  ] $axi4_0

  # Create instance: v_in_pipe_1
  create_hier_cell_v_in_pipe_1 [current_bd_instance .] v_in_pipe_1

  # Create instance: mem_scaler_1
  create_hier_cell_mem_scaler_1 [current_bd_instance .] mem_scaler_1

  # Create instance: inline_scaler_1
  create_hier_cell_inline_scaler_1 [current_bd_instance .] inline_scaler_1

  # Create instance: v_in_pipe_2
  create_hier_cell_v_in_pipe_2 [current_bd_instance .] v_in_pipe_2

  # Create instance: mem_scaler_2
  create_hier_cell_mem_scaler_2 [current_bd_instance .] mem_scaler_2

  # Create instance: inline_scaler_2
  create_hier_cell_inline_scaler_2 [current_bd_instance .] inline_scaler_2

  # Create instance: Display_Controller
  create_hier_cell_Display_Controller [current_bd_instance .] Display_Controller

  # Create instance: DDR_Interface
  create_hier_cell_DDR_Interface [current_bd_instance .] DDR_Interface

  # Create instance: timebase_0, and set properties
  set timebase_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tc:6.0 timebase_0 ]
  set_property -dict [ list CONFIG.VIDEO_MODE {1080p}  ] $timebase_0

  # Create interface connections
  connect_bd_intf_net -intf_net axi4lite_0_m15 [get_bd_intf_pins microblaze_subsystem/m_axi_gp] [get_bd_intf_pins axi4lite_1/S00_AXI]
  connect_bd_intf_net -intf_net axi4lite_1_m01 [get_bd_intf_pins axi4lite_1/M01_AXI] [get_bd_intf_pins v_in_pipe_1/vdma_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m02 [get_bd_intf_pins axi4lite_1/M02_AXI] [get_bd_intf_pins mem_scaler_1/vdma_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m04 [get_bd_intf_pins axi4lite_1/M04_AXI] [get_bd_intf_pins timebase_0/ctrl]
  connect_bd_intf_net -intf_net axi4lite_1_m05 [get_bd_intf_pins axi4lite_1/M05_AXI] [get_bd_intf_pins v_in_pipe_1/tpg_s_axi]
  connect_bd_intf_net -intf_net axi_mm_video01_s00 [get_bd_intf_pins v_in_pipe_1/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video01/S00_AXI]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi_mm_video01_s00]
  connect_bd_intf_net -intf_net axi_mm_video01_s01 [get_bd_intf_pins mem_scaler_1/M_AXI_MM2S] [get_bd_intf_pins axi_mm_video01/S01_AXI]
  connect_bd_intf_net -intf_net axi_mm_video01_s02 [get_bd_intf_pins mem_scaler_1/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video01/S02_AXI]
  connect_bd_intf_net -intf_net axi4_0_s00 [get_bd_intf_pins microblaze_subsystem/m_axi_hp] [get_bd_intf_pins axi4_0/S00_AXI]
  connect_bd_intf_net -intf_net axi4_0_s01 [get_bd_intf_pins axi_mm_video01/M00_AXI] [get_bd_intf_pins axi4_0/S01_AXI]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi4_0_s01]
  connect_bd_intf_net -intf_net axi4_0_m00 [get_bd_intf_pins axi4_0/M00_AXI] [get_bd_intf_pins DDR_Interface/S_AXI1]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi4_0_m00]
  connect_bd_intf_net -intf_net tpg_0_vdma_m_axi_s2mm [get_bd_intf_pins inline_scaler_1/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video01/S03_AXI]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets tpg_0_vdma_m_axi_s2mm]
  connect_bd_intf_net -intf_net axi4lite_1_m00_axi [get_bd_intf_pins axi4lite_1/M00_AXI] [get_bd_intf_pins Display_Controller/s_axi1]
  connect_bd_intf_net -intf_net axi4lite_1_m06_axi [get_bd_intf_pins axi4lite_1/M06_AXI] [get_bd_intf_pins inline_scaler_1/vdma_s_axi]
  connect_bd_intf_net -intf_net axi_mm_video23_s00 [get_bd_intf_pins v_in_pipe_2/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video23/S00_AXI]
  set_property -dict [ list HDL_ATTRIBUTE.MARK_DEBUG {true}  ] [get_bd_intf_nets axi_mm_video23_s00]
  connect_bd_intf_net -intf_net axi_mm_video23_s01 [get_bd_intf_pins mem_scaler_2/M_AXI_MM2S] [get_bd_intf_pins axi_mm_video23/S01_AXI]
  connect_bd_intf_net -intf_net axi_mm_video23_s02 [get_bd_intf_pins mem_scaler_2/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video23/S02_AXI]
  connect_bd_intf_net -intf_net axi4_0_s03 [get_bd_intf_pins axi_mm_video23/M00_AXI] [get_bd_intf_pins axi4_0/S03_AXI]
  connect_bd_intf_net -intf_net tpg_2_vdma_m_axi_s2mm [get_bd_intf_pins inline_scaler_2/M_AXI_S2MM] [get_bd_intf_pins axi_mm_video23/S03_AXI]
  connect_bd_intf_net -intf_net axi4lite_1_m07_axi [get_bd_intf_pins axi4lite_1/M07_AXI] [get_bd_intf_pins v_in_pipe_2/tpg_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m08_axi [get_bd_intf_pins axi4lite_1/M08_AXI] [get_bd_intf_pins v_in_pipe_2/vdma_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m09_axi [get_bd_intf_pins axi4lite_1/M09_AXI] [get_bd_intf_pins mem_scaler_2/vdma_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m10_axi [get_bd_intf_pins axi4lite_1/M10_AXI] [get_bd_intf_pins inline_scaler_2/vdma_s_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m12_axi [get_bd_intf_pins axi4lite_1/M12_AXI] [get_bd_intf_pins inline_scaler_2/ctrl]
  connect_bd_intf_net -intf_net axi4lite_1_m11_axi [get_bd_intf_pins axi4lite_1/M11_AXI] [get_bd_intf_pins inline_scaler_1/ctrl]
  connect_bd_intf_net -intf_net axi4lite_1_m13_axi [get_bd_intf_pins axi4lite_1/M13_AXI] [get_bd_intf_pins DDR_Interface/s_axi]
  connect_bd_intf_net -intf_net s02_axi_1 [get_bd_intf_pins axi4_0/S02_AXI] [get_bd_intf_pins Display_Controller/m_axi]
  connect_bd_intf_net -intf_net axi4lite_1_m03_axi [get_bd_intf_pins axi4lite_1/M03_AXI] [get_bd_intf_pins Display_Controller/s_axi]
  connect_bd_intf_net -intf_net lcd_gpio_port [get_bd_intf_ports LCD_GPIO_IO] [get_bd_intf_pins microblaze_subsystem/lcd_gpio]
  connect_bd_intf_net -intf_net rotary_gpio_port [get_bd_intf_ports ROTARY_GPIO_IO] [get_bd_intf_pins microblaze_subsystem/rotary_gpio]
  connect_bd_intf_net -intf_net eeprom_port [get_bd_intf_ports IIC_MAIN] [get_bd_intf_pins microblaze_subsystem/iic]
  connect_bd_intf_net -intf_net ddr3_port [get_bd_intf_ports DDR3] [get_bd_intf_pins DDR_Interface/DDR3]
  connect_bd_intf_net -intf_net soft_ethernet_mac_gmii [get_bd_intf_ports Soft_Ethernet_MAC] [get_bd_intf_pins microblaze_subsystem/gmii]

  # Create port connections
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_clk_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_clk] [get_bd_pins v_in_pipe_1/video_clk_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_hsync_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_hsync] [get_bd_pins v_in_pipe_1/hsync_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_vsync_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_vsync] [get_bd_pins v_in_pipe_1/vsync_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_de_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_de] [get_bd_pins v_in_pipe_1/de_1]
  connect_bd_net -net v_tc_1_vsync_out [get_bd_pins timebase_0/vsync_out] [get_bd_pins v_in_pipe_1/vsync_0] [get_bd_pins v_in_pipe_2/vsync_0] [get_bd_pins inline_scaler_2/vid_vsync] [get_bd_pins inline_scaler_1/vid_vsync]
  connect_bd_net -net v_tc_1_hsync_out [get_bd_pins timebase_0/hsync_out] [get_bd_pins v_in_pipe_1/hsync_0] [get_bd_pins v_in_pipe_2/hsync_0] [get_bd_pins inline_scaler_2/vid_hsync] [get_bd_pins inline_scaler_1/vid_hsync]
  connect_bd_net -net v_tc_1_active_video_out [get_bd_pins timebase_0/active_video_out] [get_bd_pins v_in_pipe_1/de_0] [get_bd_pins v_in_pipe_2/de_0] [get_bd_pins inline_scaler_2/vid_active_video] [get_bd_pins inline_scaler_1/vid_active_video]
  connect_bd_net -net proc_sys_reset_1_bus_struct_reset [get_bd_pins proc_sys_reset_0/bus_struct_reset] [get_bd_pins v_in_pipe_1/rst] [get_bd_pins microblaze_subsystem/LMB_Rst] [get_bd_pins v_in_pipe_2/rst] [get_bd_pins inline_scaler_1/rst] [get_bd_pins inline_scaler_2/rst] [get_bd_pins Display_Controller/rst]
  connect_bd_net -net clk_100Mhz [get_bd_pins DDR_Interface/ui_addn_clk_0] [get_bd_pins proc_sys_reset_0/slowest_sync_clk] [get_bd_pins axi4lite_1/ACLK] [get_bd_pins axi4lite_1/S00_ACLK] [get_bd_pins axi4lite_1/M00_ACLK] [get_bd_pins axi4lite_1/M01_ACLK] [get_bd_pins axi4lite_1/M03_ACLK] [get_bd_pins axi4lite_1/M02_ACLK] [get_bd_pins axi4lite_1/M04_ACLK] [get_bd_pins axi4lite_1/M05_ACLK] [get_bd_pins axi4lite_1/M06_ACLK] [get_bd_pins v_in_pipe_1/s_axi_lite_aclk] [get_bd_pins mem_scaler_1/s_axi_aclk] [get_bd_pins inline_scaler_1/s_axi_lite_aclk] [get_bd_pins microblaze_subsystem/ACLK] [get_bd_pins inline_scaler_2/s_axi_lite_aclk] [get_bd_pins v_in_pipe_2/s_axi_lite_aclk] [get_bd_pins mem_scaler_2/s_axi_aclk] [get_bd_pins axi4lite_1/M07_ACLK] [get_bd_pins axi4lite_1/M08_ACLK] [get_bd_pins axi4lite_1/M09_ACLK] [get_bd_pins axi4lite_1/M10_ACLK] [get_bd_pins axi4lite_1/M11_ACLK] [get_bd_pins axi4lite_1/M12_ACLK] [get_bd_pins axi4lite_1/M13_ACLK] [get_bd_pins Display_Controller/s_axi_aclk] [get_bd_pins timebase_0/s_axi_aclk]
  connect_bd_net -net proc_sys_reset_1_interconnect_aresetn [get_bd_pins proc_sys_reset_0/interconnect_aresetn] [get_bd_pins axi4_0/ARESETN] [get_bd_pins axi_mm_video01/ARESETN] [get_bd_pins axi_mm_video23/ARESETN] [get_bd_pins axi4lite_1/ARESETN] [get_bd_pins microblaze_subsystem/ARESETN] [get_bd_pins DDR_Interface/core_aresetn]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins microblaze_subsystem/Debug_SYS_Rst] [get_bd_pins proc_sys_reset_0/mb_debug_sys_rst]
  connect_bd_net -net proc_sys_reset_1_peripheral_aresetn [get_bd_pins proc_sys_reset_0/peripheral_aresetn] [get_bd_pins axi4_0/M00_ARESETN] [get_bd_pins axi4_0/S00_ARESETN] [get_bd_pins axi4_0/S02_ARESETN] [get_bd_pins axi4_0/S01_ARESETN] [get_bd_pins axi4lite_1/M05_ARESETN] [get_bd_pins axi4lite_1/S00_ARESETN] [get_bd_pins axi4lite_1/M00_ARESETN] [get_bd_pins axi4lite_1/M01_ARESETN] [get_bd_pins axi4lite_1/M02_ARESETN] [get_bd_pins axi4lite_1/M04_ARESETN] [get_bd_pins axi4lite_1/M03_ARESETN] [get_bd_pins axi_mm_video01/S02_ARESETN] [get_bd_pins axi_mm_video01/S01_ARESETN] [get_bd_pins axi_mm_video01/M00_ARESETN] [get_bd_pins axi_mm_video01/S00_ARESETN] [get_bd_pins axi_mm_video01/S03_ARESETN] [get_bd_pins axi4lite_1/M06_ARESETN] [get_bd_pins v_in_pipe_1/aresetn] [get_bd_pins mem_scaler_1/s_axi_aresetn] [get_bd_pins inline_scaler_1/axi_resetn] [get_bd_pins microblaze_subsystem/s_aresetn] [get_bd_pins inline_scaler_2/axi_resetn] [get_bd_pins v_in_pipe_2/aresetn] [get_bd_pins mem_scaler_2/s_axi_aresetn] [get_bd_pins axi_mm_video23/S01_ARESETN] [get_bd_pins axi_mm_video23/M00_ARESETN] [get_bd_pins axi_mm_video23/S00_ARESETN] [get_bd_pins axi_mm_video23/S02_ARESETN] [get_bd_pins axi4_0/S03_ARESETN] [get_bd_pins axi4lite_1/M07_ARESETN] [get_bd_pins axi4lite_1/M08_ARESETN] [get_bd_pins axi4lite_1/M09_ARESETN] [get_bd_pins axi4lite_1/M10_ARESETN] [get_bd_pins axi_mm_video23/S03_ARESETN] [get_bd_pins axi4lite_1/M12_ARESETN] [get_bd_pins axi4lite_1/M11_ARESETN] [get_bd_pins axi4lite_1/M13_ARESETN] [get_bd_pins Display_Controller/s_axi_aresetn] [get_bd_pins DDR_Interface/aresetn] [get_bd_pins timebase_0/resetn] [get_bd_pins timebase_0/s_axi_aresetn]
  connect_bd_net -net proc_sys_reset_1_mb_reset [get_bd_pins proc_sys_reset_0/mb_reset] [get_bd_pins microblaze_subsystem/processor_rst]
  connect_bd_net -net ext_reset_in_1 [get_bd_ports RESET] [get_bd_pins proc_sys_reset_0/ext_reset_in] [get_bd_pins DDR_Interface/sys_rst]
  connect_bd_net -net axi_uart16550_1_sout [get_bd_ports RS232_UART_1_sout] [get_bd_pins microblaze_subsystem/sout]
  connect_bd_net -net sin_1 [get_bd_ports RS232_UART_1_sin] [get_bd_pins microblaze_subsystem/sin]
  connect_bd_net -net clk_150Mhz [get_bd_pins clock_generator_1/clk_out1] [get_bd_pins axi_mm_video01/ACLK] [get_bd_pins axi_mm_video01/S00_ACLK] [get_bd_pins axi_mm_video01/M00_ACLK] [get_bd_pins axi_mm_video01/S01_ACLK] [get_bd_pins axi_mm_video01/S02_ACLK] [get_bd_pins axi_mm_video01/S03_ACLK] [get_bd_pins v_in_pipe_1/video_clk_0] [get_bd_pins mem_scaler_1/m_axis_mm2s_aclk] [get_bd_pins inline_scaler_1/m_axi_s2mm_aclk] [get_bd_pins microblaze_subsystem/microblaze_clk] [get_bd_pins inline_scaler_2/m_axi_s2mm_aclk] [get_bd_pins v_in_pipe_2/video_clk_0] [get_bd_pins mem_scaler_2/m_axis_mm2s_aclk] [get_bd_pins axi_mm_video23/ACLK] [get_bd_pins axi_mm_video23/S00_ACLK] [get_bd_pins axi_mm_video23/M00_ACLK] [get_bd_pins axi_mm_video23/S01_ACLK] [get_bd_pins axi_mm_video23/S02_ACLK] [get_bd_pins axi_mm_video23/S03_ACLK] [get_bd_pins axi4_0/S03_ACLK] [get_bd_pins axi4_0/S02_ACLK] [get_bd_pins axi4_0/S01_ACLK] [get_bd_pins axi4_0/S00_ACLK] [get_bd_pins Display_Controller/M_AXI_ACLK] [get_bd_pins timebase_0/clk]
  connect_bd_net -net axi_iic_1_gpo [get_bd_ports IIC_MUX_RESET_B] [get_bd_pins microblaze_subsystem/iic_gpo]
  connect_bd_net -net push_buttons_5bits_tri_i_1 [get_bd_ports Push_Buttons_5Bits_TRI_I] [get_bd_pins microblaze_subsystem/push_button_gpio]
  connect_bd_net -net dip_switches_4bits_tri_i_1 [get_bd_ports DIP_Switches_TRI_I] [get_bd_pins microblaze_subsystem/dip_sw_gpio]
  connect_bd_net -net axi_emc_1_mem_wen [get_bd_ports Linear_Flash_we_n] [get_bd_pins microblaze_subsystem/flash_wen]
  connect_bd_net -net axi_emc_1_mem_oen [get_bd_ports Linear_Flash_oe_n] [get_bd_pins microblaze_subsystem/flash_oen]
  connect_bd_net -net axi_emc_1_mem_cen [get_bd_ports Linear_Flash_ce_n] [get_bd_pins microblaze_subsystem/flash_cen]
  connect_bd_net -net axi_emc_1_mem_dq_t [get_bd_ports Linear_Flash_Data_T] [get_bd_pins microblaze_subsystem/flash_dq_t]
  connect_bd_net -net axi_emc_1_mem_dq_o [get_bd_ports Linear_Flash_Data_O] [get_bd_pins microblaze_subsystem/flash_dq_o]
  connect_bd_net -net mem_dq_i_1 [get_bd_ports Linear_Flash_Data_I] [get_bd_pins microblaze_subsystem/flash_dq_i]
  connect_bd_net -net axi_emc_1_mem_adv_ldn [get_bd_ports Linear_Flash_adv_ldn] [get_bd_pins microblaze_subsystem/flash_adv_ldn]
  connect_bd_net -net Net [get_bd_ports Soft_Ethernet_MAC_MDIO_pin] [get_bd_pins microblaze_subsystem/mdio]
  connect_bd_net -net soft_ethernet_mac_mdc [get_bd_ports Soft_Ethernet_MAC_MDC_pin] [get_bd_pins microblaze_subsystem/mdc]
  connect_bd_net -net ethernet_clk_125 [get_bd_pins clock_generator_1/clk_out2] [get_bd_pins microblaze_subsystem/ethernet_gtx_clk]
  connect_bd_net -net logisdhc_0_sd_clk [get_bd_ports sd_clk] [get_bd_pins microblaze_subsystem/sd_clk]
  connect_bd_net -net logisdhc_0_sd_dat_o [get_bd_ports sd_dat_o] [get_bd_pins microblaze_subsystem/sd_dat_o]
  connect_bd_net -net logisdhc_0_sd_dat_t [get_bd_ports sd_dat_t] [get_bd_pins microblaze_subsystem/sd_dat_t]
  connect_bd_net -net sd_dat_i_1 [get_bd_ports sd_dat_i] [get_bd_pins microblaze_subsystem/sd_dat_i]
  connect_bd_net -net sd_wp_1 [get_bd_ports sd_wp] [get_bd_pins microblaze_subsystem/sd_wp]
  connect_bd_net -net sd_cd_n_1 [get_bd_ports sd_cd_n] [get_bd_pins microblaze_subsystem/sd_cd_n]
  connect_bd_net -net sd_cmd_i_1 [get_bd_ports sd_cmd_i] [get_bd_pins microblaze_subsystem/sd_cmd_i]
  connect_bd_net -net logisdhc_0_sd_cmd_o [get_bd_ports sd_cmd_o] [get_bd_pins microblaze_subsystem/sd_cmd_o]
  connect_bd_net -net logisdhc_0_sd_cmd_t [get_bd_ports sd_cmd_t] [get_bd_pins microblaze_subsystem/sd_cmd_t]
  connect_bd_net -net xadc_wiz_1_temp_out [get_bd_pins microblaze_subsystem/temp_out] [get_bd_pins DDR_Interface/device_temp_i]
  connect_bd_net -net soft_ethernet_mac_phy_rst_n [get_bd_ports Soft_Ethernet_MAC_RST_n_pin] [get_bd_pins microblaze_subsystem/phy_rst_n]
  connect_bd_net -net mig_clk_200 [get_bd_pins DDR_Interface/ui_clk] [get_bd_pins clock_generator_1/clk_in1] [get_bd_pins axi4_0/ACLK] [get_bd_pins axi4_0/M00_ACLK] [get_bd_pins microblaze_subsystem/ethernet_ref_clk]
  connect_bd_net -net mig_7series_1_init_calib_complete [get_bd_ports init_calib_complete] [get_bd_pins DDR_Interface/init_calib_complete]
  connect_bd_net -net xlconstant_3_const [get_bd_ports ddr3_1_n] [get_bd_pins DDR_Interface/ddr3_1_n]
  connect_bd_net -net xlconstant_4_const [get_bd_ports ddr3_1_p] [get_bd_pins DDR_Interface/ddr3_1_p]
  connect_bd_net -net vauxp0_1 [get_bd_ports axi_xadc_0_VAUXP] [get_bd_pins microblaze_subsystem/vauxp0]
  connect_bd_net -net vauxn0_1 [get_bd_ports axi_xadc_0_VAUXN] [get_bd_pins microblaze_subsystem/vauxn0]
  connect_bd_net -net vauxp8_1 [get_bd_ports axi_xadc_8_VAUXP] [get_bd_pins microblaze_subsystem/vauxp8]
  connect_bd_net -net vauxn8_1 [get_bd_ports axi_xadc_8_VAUXN] [get_bd_pins microblaze_subsystem/vauxn8]
  connect_bd_net -net dvi_in_0_vdma_s2mm_fsync_out [get_bd_pins v_in_pipe_1/s2mm_fsync_out] [get_bd_pins mem_scaler_1/fsync] [get_bd_pins Display_Controller/e_sw_vbuff_4]
  connect_bd_net -net scaler_1_vdma_s2mm_fsync_in [get_bd_pins timebase_0/fsync_out]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_green_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_green] [get_bd_pins v_in_pipe_1/green]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_red_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_red] [get_bd_pins v_in_pipe_1/red]
  connect_bd_net -net video_mux_sel_1 [get_bd_pins Display_Controller/video_mux_sel_1] [get_bd_pins v_in_pipe_1/video_sel]
  connect_bd_net -net clock_generator_1_locked [get_bd_pins clock_generator_1/locked] [get_bd_pins proc_sys_reset_0/dcm_locked] [get_bd_pins microblaze_subsystem/sd_base_lock]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_clk_2 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_clk] [get_bd_pins v_in_pipe_2/video_clk_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_hsync_2 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_hsync] [get_bd_pins v_in_pipe_2/hsync_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_vsync_2 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_vsync] [get_bd_pins v_in_pipe_2/vsync_1]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_de_2 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_de] [get_bd_pins v_in_pipe_2/de_1]
  connect_bd_net -net dvi_in_1_vdma_s2mm_fsync_out [get_bd_pins v_in_pipe_2/s2mm_fsync_out] [get_bd_pins mem_scaler_2/fsync] [get_bd_pins Display_Controller/e_sw_vbuff_2]
  connect_bd_net -net dvi_in_1_fmc_lpc_dvidp_dvii_green_1 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_green] [get_bd_pins v_in_pipe_2/green]
  connect_bd_net -net video_mux_sel_2 [get_bd_pins Display_Controller/video_mux_sel_2] [get_bd_pins v_in_pipe_2/video_sel]
  connect_bd_net -net inline_scaler_1_reset_n [get_bd_pins Display_Controller/inline_scaler_1_reset_n] [get_bd_pins inline_scaler_1/scaler_reset_n]
  connect_bd_net -net inline_scaler_2_reset_n [get_bd_pins Display_Controller/inline_scaler_2_reset_n] [get_bd_pins inline_scaler_2/scaler_reset_n]
  connect_bd_net -net microblaze_subsystem_led_gpio [get_bd_ports LEDs_8Bits_TRI_O] [get_bd_pins microblaze_subsystem/led_gpio]
  connect_bd_net -net clk_n_1 [get_bd_ports CLK_N] [get_bd_pins DDR_Interface/sys_clk_n]
  connect_bd_net -net clk_p_1 [get_bd_ports CLK_P] [get_bd_pins DDR_Interface/sys_clk_p]
  connect_bd_net -net logicvc_2_pix_clk_o [get_bd_ports dvi_24_to_16bit_hdmi_clk] [get_bd_pins Display_Controller/pix_clk_o]
  connect_bd_net -net logicvc_2_d_pix_o [get_bd_ports dvi_24_to_16bit_hdmi_data] [get_bd_pins Display_Controller/d_pix_o]
  connect_bd_net -net logicvc_2_hsync_o [get_bd_ports dvi_24_to_16bit_hsync] [get_bd_pins Display_Controller/hsync_o]
  connect_bd_net -net logicvc_2_vsync_o [get_bd_ports dvi_24_to_16bit_vsync] [get_bd_pins Display_Controller/vsync_o]
  connect_bd_net -net logicvc_2_blank_o [get_bd_ports dvi_24_to_16bit_de] [get_bd_pins Display_Controller/blank_o]
  connect_bd_net -net dvi_in_0_fmc_hpc_dvidp_dvii_blue_1 [get_bd_ports DVI_IN_0_fmc_hpc_dvidp_dvii_blue] [get_bd_pins v_in_pipe_1/blue]
  connect_bd_net -net dvi_in_1_fmc_lpc_dvidp_dvii_red_1 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_red] [get_bd_pins v_in_pipe_2/red]
  connect_bd_net -net dvi_in_1_fmc_lpc_dvidp_dvii_blue_1 [get_bd_ports DVI_IN_1_fmc_lpc_dvidp_dvii_blue] [get_bd_pins v_in_pipe_2/blue]
  connect_bd_net -net mem_scaler_1_s2mm_introut [get_bd_pins mem_scaler_1/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_8]
  connect_bd_net -net mem_scaler_1_mm2s_introut [get_bd_pins mem_scaler_1/mm2s_introut] [get_bd_pins microblaze_subsystem/Interrupt_9]
  connect_bd_net -net inline_scaler_1_s2mm_introut [get_bd_pins inline_scaler_1/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_6]
  connect_bd_net -net v_in_pipe_1_s2mm_introut [get_bd_pins v_in_pipe_1/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_10]
  connect_bd_net -net inline_scaler_2_s2mm_introut [get_bd_pins inline_scaler_2/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_2]
  connect_bd_net -net timebase_0_irq [get_bd_pins timebase_0/irq] [get_bd_pins microblaze_subsystem/Interrupt_7]
  connect_bd_net -net v_in_pipe_2_s2mm_introut [get_bd_pins v_in_pipe_2/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_5]
  connect_bd_net -net logicvc_1_interrupt [get_bd_pins Display_Controller/cvc_interrupt] [get_bd_pins microblaze_subsystem/Interrupt_1]
  connect_bd_net -net mem_scaler_2_s2mm_introut [get_bd_pins mem_scaler_2/s2mm_introut] [get_bd_pins microblaze_subsystem/Interrupt_3]
  connect_bd_net -net mem_scaler_2_mm2s_introut [get_bd_pins mem_scaler_2/mm2s_introut] [get_bd_pins microblaze_subsystem/Interrupt_4]
  connect_bd_net -net microblaze_subsystem_const [get_bd_ports sm_fan_pwm] [get_bd_pins microblaze_subsystem/const]
  connect_bd_net -net microblaze_subsystem_dout [get_bd_ports Linear_Flash_address] [get_bd_pins microblaze_subsystem/Flash_address]
  connect_bd_net -net inline_scaler_1_s2mm_fsync_out [get_bd_pins inline_scaler_1/s2mm_fsync_out] [get_bd_pins Display_Controller/e_sw_vbuff_3]
  connect_bd_net -net inline_scaler_2_s2mm_fsync_out [get_bd_pins inline_scaler_2/s2mm_fsync_out] [get_bd_pins Display_Controller/e_sw_vbuff_1]
  connect_bd_net -net display_controller_mem_scaler_1_reset_n [get_bd_pins Display_Controller/mem_scaler_1_reset_n] [get_bd_pins mem_scaler_1/scaler_reset_n]
  connect_bd_net -net display_controller_mem_scaler_2_reset_n [get_bd_pins Display_Controller/mem_scaler_2_reset_n] [get_bd_pins mem_scaler_2/scaler_reset_n]

  # Create address segments
  create_bd_addr_seg -range 0x2000 -offset 0x0 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/microblaze_0_local_memory/LocalMemory_Cntlr_D/SLMB/Mem] SEG2
  create_bd_addr_seg -range 0x10000 -offset 0x40200000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/debug_module/S_AXI/Reg] SEG3
  create_bd_addr_seg -range 0x10000 -offset 0x40600000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/LEDs_8Bits/s_axi/Reg] SEG1
  create_bd_addr_seg -range 0x10000 -offset 0x40400000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/RS232_Uart_1/s_axi/Reg] SEG4
  create_bd_addr_seg -range 0x10000 -offset 0x40300000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Dual_Timer_Counter/s_axi/Reg] SEG5
  create_bd_addr_seg -range 0x10000 -offset 0x40500000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Push_Buttons_5Bits/s_axi/Reg] SEG6
  create_bd_addr_seg -range 0x10000 -offset 0x40700000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/DIP_Switches_4Bits/s_axi/Reg] SEG7
  create_bd_addr_seg -range 0x10000 -offset 0x40800000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/LCD_GPIO/s_axi/Reg] SEG8
  create_bd_addr_seg -range 0x10000 -offset 0x40900000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/ROTARY_GPIO/s_axi/Reg] SEG9
  create_bd_addr_seg -range 0x8000000 -offset 0x48000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Linear_Flash/S_AXI_MEM/MEM0] SEG10
  create_bd_addr_seg -range 0x10000 -offset 0x40A00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/IIC_EEPROM/s_axi/Reg] SEG11
  create_bd_addr_seg -range 0x10000 -offset 0x40100000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Interrupt_Cntlr/s_axi/Reg] SEG12
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Internal_BRAM/S_AXI/Mem0] SEG13
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG14
  create_bd_addr_seg -range 0x10000 -offset 0x40B00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/logisdhc_0/s_axi/Reg] SEG17
  create_bd_addr_seg -range 0x10000 -offset 0x40D00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/axi_xadc_0/s_axi_lite/Reg] SEG18
  create_bd_addr_seg -range 0x10000 -offset 0x50300000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs v_in_pipe_1/DVI_IN_0_VDMA/S_AXI_LITE/Reg] SEG19
  create_bd_addr_seg -range 0x10000 -offset 0x50400000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs mem_scaler_1/SCALER_0_VDMA/S_AXI_LITE/Reg] SEG20
  create_bd_addr_seg -range 0x10000 -offset 0x50500000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs timebase_0/ctrl/Reg] SEG21
  create_bd_addr_seg -range 0x10000 -offset 0x50200000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs v_in_pipe_1/vid_source_0/ctrl/Reg] SEG22
  create_bd_addr_seg -range 0x10000 -offset 0x50700000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs inline_scaler_1/SCALER_1_VDMA/S_AXI_LITE/Reg] SEG23
  create_bd_addr_seg -range 0x10000 -offset 0x50900000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs v_in_pipe_2/DVI_IN_1_VDMA/S_AXI_LITE/Reg] SEG26
  create_bd_addr_seg -range 0x10000 -offset 0x50A00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs mem_scaler_2/SCALER_2_VDMA/S_AXI_LITE/Reg] SEG27
  create_bd_addr_seg -range 0x10000 -offset 0x50C00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs inline_scaler_2/SCALER_3_VDMA/S_AXI_LITE/Reg] SEG28
  create_bd_addr_seg -range 0x10000 -offset 0x50800000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs v_in_pipe_2/vid_source_2/ctrl/Reg] SEG29
  create_bd_addr_seg -range 0x10000 -offset 0x50600000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs inline_scaler_1/vid_source_1/ctrl/Reg] SEG30
  create_bd_addr_seg -range 0x10000 -offset 0x50B00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs inline_scaler_2/vid_source_3/ctrl/Reg] SEG31
  create_bd_addr_seg -range 0x40000 -offset 0x50100000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/Soft_Ethernet_MAC/axi_ethernet_buffer/S_AXI_Ethernet/Axi_Ethernet_Mem0] SEG16
  create_bd_addr_seg -range 0x10000 -offset 0x50000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs microblaze_subsystem/AXI_DMA_Ethernet/S_AXI_LITE/Reg] SEG15
  create_bd_addr_seg -range 0x10000 -offset 0x40C00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs Display_Controller/gpio_rst_mux/s_axi/Reg] SEG25
  create_bd_addr_seg -range 0x10000 -offset 0x40000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs DDR_Interface/axi_perf_mon_1/s_axi/Reg] SEG32
  create_bd_addr_seg -range 0x10000 -offset 0x50D00000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Data] [get_bd_addr_segs Display_Controller/logicvc_1/s_axi/Reg] SEG24
  create_bd_addr_seg -range 0x2000 -offset 0x0 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Instruction] [get_bd_addr_segs microblaze_subsystem/microblaze_0_local_memory/LocalMemory_Cntlr_I/SLMB/Mem] SEG1
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Instruction] [get_bd_addr_segs microblaze_subsystem/Internal_BRAM/S_AXI/Mem0] SEG2
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_subsystem/microblaze_0/Instruction] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG3
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_SG] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG10
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_SG] [get_bd_addr_segs microblaze_subsystem/Internal_BRAM/S_AXI/Mem0] SEG1
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_MM2S] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG11
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_MM2S] [get_bd_addr_segs microblaze_subsystem/Internal_BRAM/S_AXI/Mem0] SEG2
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG12
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces microblaze_subsystem/AXI_DMA_Ethernet/Data_S2MM] [get_bd_addr_segs microblaze_subsystem/Internal_BRAM/S_AXI/Mem0] SEG3
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces v_in_pipe_1/DVI_IN_0_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG1
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces mem_scaler_1/SCALER_0_VDMA/Data_MM2S] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG2
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces mem_scaler_1/SCALER_0_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG3
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces inline_scaler_1/SCALER_1_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG4
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces v_in_pipe_2/DVI_IN_1_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG5
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces mem_scaler_2/SCALER_2_VDMA/Data_MM2S] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG6
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces mem_scaler_2/SCALER_2_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG7
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces inline_scaler_2/SCALER_3_VDMA/Data_S2MM] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG8
  create_bd_addr_seg -range 0x40000000 -offset 0x80000000 [get_bd_addr_spaces Display_Controller/logicvc_1/videoData] [get_bd_addr_segs DDR_Interface/DDR3_SDRAM/memmap/memaddr] SEG25
  

  # Restore current instance
  current_bd_instance $oldCurInst
}


#####################################################
# Main Flow
#####################################################

create_root_design ""
