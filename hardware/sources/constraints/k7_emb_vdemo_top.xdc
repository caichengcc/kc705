#
# pin constraints
#
########################## CLK ###################################
set_property IOSTANDARD DIFF_SSTL15 [get_ports CLK_N]

set_property PACKAGE_PIN AD12 [get_ports CLK_P]
set_property PACKAGE_PIN AD11 [get_ports CLK_N]
set_property IOSTANDARD DIFF_SSTL15 [get_ports CLK_P]


create_clock -period 5.000 -name sys_clk_pin [get_ports CLK_P]

########################## RESET ###################################

set_property PACKAGE_PIN AB7 [get_ports RESET]
set_property IOSTANDARD LVCMOS15 [get_ports RESET]

########################## DDR3 ###################################

set_property PACKAGE_PIN F16 [get_ports init_calib_complete]
set_property IOSTANDARD LVCMOS25 [get_ports init_calib_complete]

set_property PACKAGE_PIN AF11 [get_ports {ddr3_1_p[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_1_p[0]}]

set_property PACKAGE_PIN AE8 [get_ports {ddr3_1_p[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_1_p[1]}]

set_property PACKAGE_PIN AE11 [get_ports {ddr3_1_n[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_1_n[0]}]

set_property PACKAGE_PIN AE10 [get_ports {ddr3_1_n[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_1_n[1]}]

set_property PACKAGE_PIN AC10 [get_ports {ddr3_1_n[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_1_n[2]}]


set_property DCI_CASCADE {32 34} [get_iobanks 33]

############################ DIP Switches ########################## 

set_property PACKAGE_PIN Y29 [get_ports {DIP_Switches_TRI_I[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DIP_Switches_TRI_I[0]}]

set_property PACKAGE_PIN W29 [get_ports {DIP_Switches_TRI_I[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DIP_Switches_TRI_I[1]}]

set_property PACKAGE_PIN AA28 [get_ports {DIP_Switches_TRI_I[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DIP_Switches_TRI_I[2]}]

set_property PACKAGE_PIN Y28 [get_ports {DIP_Switches_TRI_I[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DIP_Switches_TRI_I[3]}]

########################## LEDs ###################################

set_property PACKAGE_PIN AB8 [get_ports {LEDs_8Bits_TRI_O[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LEDs_8Bits_TRI_O[0]}]

set_property PACKAGE_PIN AA8 [get_ports {LEDs_8Bits_TRI_O[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LEDs_8Bits_TRI_O[1]}]

set_property PACKAGE_PIN AC9 [get_ports {LEDs_8Bits_TRI_O[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LEDs_8Bits_TRI_O[2]}]

set_property PACKAGE_PIN AB9 [get_ports {LEDs_8Bits_TRI_O[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LEDs_8Bits_TRI_O[3]}]

set_property PACKAGE_PIN AE26 [get_ports {LEDs_8Bits_TRI_O[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {LEDs_8Bits_TRI_O[4]}]

set_property PACKAGE_PIN G19 [get_ports {LEDs_8Bits_TRI_O[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {LEDs_8Bits_TRI_O[5]}]

set_property PACKAGE_PIN E18 [get_ports {LEDs_8Bits_TRI_O[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {LEDs_8Bits_TRI_O[6]}]

##set_property LOC F16 [ get_ports LEDs_8Bits_TRI_O[7]]
##set_property IOSTANDARD LVCMOS25 [ get_ports LEDs_8Bits_TRI_O[7]]

########################## Push Button ########################## 

set_property PACKAGE_PIN G12 [get_ports {Push_Buttons_5Bits_TRI_I[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Push_Buttons_5Bits_TRI_I[0]}]

set_property PACKAGE_PIN AC6 [get_ports {Push_Buttons_5Bits_TRI_I[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {Push_Buttons_5Bits_TRI_I[1]}]

set_property PACKAGE_PIN AB12 [get_ports {Push_Buttons_5Bits_TRI_I[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {Push_Buttons_5Bits_TRI_I[2]}]

set_property PACKAGE_PIN AG5 [get_ports {Push_Buttons_5Bits_TRI_I[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {Push_Buttons_5Bits_TRI_I[3]}]

set_property PACKAGE_PIN AA12 [get_ports {Push_Buttons_5Bits_TRI_I[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {Push_Buttons_5Bits_TRI_I[4]}]

########################## Fan Control ########################## 

set_property PACKAGE_PIN L26 [get_ports {sm_fan_pwm[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sm_fan_pwm[0]}]

########################## Rotary Switch ########################## 

set_property PACKAGE_PIN Y26 [get_ports {ROTARY_GPIO_IO_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ROTARY_GPIO_IO_tri_io[0]}]

set_property PACKAGE_PIN Y25 [get_ports {ROTARY_GPIO_IO_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ROTARY_GPIO_IO_tri_io[1]}]

set_property PACKAGE_PIN AA26 [get_ports {ROTARY_GPIO_IO_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ROTARY_GPIO_IO_tri_io[2]}]

########################## LCD ########################## 

set_property PACKAGE_PIN AB10 [get_ports {LCD_GPIO_IO_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[0]}]

set_property PACKAGE_PIN Y11 [get_ports {LCD_GPIO_IO_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[1]}]

set_property PACKAGE_PIN AB13 [get_ports {LCD_GPIO_IO_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[2]}]

set_property PACKAGE_PIN Y10 [get_ports {LCD_GPIO_IO_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[3]}]

set_property PACKAGE_PIN AA11 [get_ports {LCD_GPIO_IO_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[4]}]

set_property PACKAGE_PIN AA10 [get_ports {LCD_GPIO_IO_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[5]}]

set_property PACKAGE_PIN AA13 [get_ports {LCD_GPIO_IO_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LCD_GPIO_IO_tri_io[6]}]


########################## IIC Main ########################## 

set_property PACKAGE_PIN K21 [get_ports IIC_MAIN_scl_io]
set_property DRIVE 8 [get_ports IIC_MAIN_scl_io]
set_property IOSTANDARD LVCMOS25 [get_ports IIC_MAIN_scl_io]
set_property SLEW SLOW [get_ports IIC_MAIN_scl_io]

set_property PACKAGE_PIN L21 [get_ports IIC_MAIN_sda_io]
set_property DRIVE 8 [get_ports IIC_MAIN_sda_io]
set_property IOSTANDARD LVCMOS25 [get_ports IIC_MAIN_sda_io]
set_property SLEW SLOW [get_ports IIC_MAIN_sda_io]

set_property PACKAGE_PIN P23 [get_ports {IIC_MUX_RESET_B[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {IIC_MUX_RESET_B[0]}]

########################## Flash ########################## 

set_property PACKAGE_PIN M22 [get_ports {Linear_Flash_address[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[0]}]

set_property PACKAGE_PIN M23 [get_ports {Linear_Flash_address[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[1]}]

set_property PACKAGE_PIN N26 [get_ports {Linear_Flash_address[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[2]}]

set_property PACKAGE_PIN N19 [get_ports {Linear_Flash_address[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[3]}]

set_property PACKAGE_PIN N20 [get_ports {Linear_Flash_address[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[4]}]

set_property PACKAGE_PIN N21 [get_ports {Linear_Flash_address[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[5]}]

set_property PACKAGE_PIN N22 [get_ports {Linear_Flash_address[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[6]}]

set_property PACKAGE_PIN N24 [get_ports {Linear_Flash_address[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[7]}]

set_property PACKAGE_PIN P21 [get_ports {Linear_Flash_address[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[8]}]

set_property PACKAGE_PIN P22 [get_ports {Linear_Flash_address[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[9]}]

set_property PACKAGE_PIN V27 [get_ports {Linear_Flash_address[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[10]}]

set_property PACKAGE_PIN V29 [get_ports {Linear_Flash_address[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[11]}]

set_property PACKAGE_PIN V30 [get_ports {Linear_Flash_address[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[12]}]

set_property PACKAGE_PIN V25 [get_ports {Linear_Flash_address[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[13]}]

set_property PACKAGE_PIN W26 [get_ports {Linear_Flash_address[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[14]}]

set_property PACKAGE_PIN V19 [get_ports {Linear_Flash_address[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[15]}]

set_property PACKAGE_PIN V20 [get_ports {Linear_Flash_address[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[16]}]

set_property PACKAGE_PIN W23 [get_ports {Linear_Flash_address[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[17]}]

set_property PACKAGE_PIN W24 [get_ports {Linear_Flash_address[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[18]}]

set_property PACKAGE_PIN U23 [get_ports {Linear_Flash_address[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[19]}]

set_property PACKAGE_PIN V21 [get_ports {Linear_Flash_address[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[20]}]

set_property PACKAGE_PIN V22 [get_ports {Linear_Flash_address[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[21]}]

set_property PACKAGE_PIN U24 [get_ports {Linear_Flash_address[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[22]}]

set_property PACKAGE_PIN V24 [get_ports {Linear_Flash_address[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[23]}]

set_property PACKAGE_PIN W21 [get_ports {Linear_Flash_address[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[24]}]

set_property PACKAGE_PIN W22 [get_ports {Linear_Flash_address[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_address[25]}]

set_property PACKAGE_PIN U19 [get_ports {Linear_Flash_ce_n[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_ce_n[0]}]

set_property PACKAGE_PIN R26 [get_ports {Linear_Flash_Data[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[0]}]

set_property PACKAGE_PIN P26 [get_ports {Linear_Flash_Data[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[1]}]

set_property PACKAGE_PIN T30 [get_ports {Linear_Flash_Data[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[2]}]

set_property PACKAGE_PIN P28 [get_ports {Linear_Flash_Data[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[3]}]

set_property PACKAGE_PIN P27 [get_ports {Linear_Flash_Data[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[4]}]

set_property PACKAGE_PIN R29 [get_ports {Linear_Flash_Data[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[5]}]

set_property PACKAGE_PIN P29 [get_ports {Linear_Flash_Data[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[6]}]

set_property PACKAGE_PIN U20 [get_ports {Linear_Flash_Data[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[7]}]

set_property PACKAGE_PIN T23 [get_ports {Linear_Flash_Data[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[8]}]

set_property PACKAGE_PIN T22 [get_ports {Linear_Flash_Data[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[9]}]

set_property PACKAGE_PIN T21 [get_ports {Linear_Flash_Data[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[10]}]

set_property PACKAGE_PIN T20 [get_ports {Linear_Flash_Data[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[11]}]

set_property PACKAGE_PIN R21 [get_ports {Linear_Flash_Data[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[12]}]

set_property PACKAGE_PIN R20 [get_ports {Linear_Flash_Data[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[13]}]

set_property PACKAGE_PIN R25 [get_ports {Linear_Flash_Data[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[14]}]

set_property PACKAGE_PIN P24 [get_ports {Linear_Flash_Data[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_Data[15]}]

set_property PACKAGE_PIN M24 [get_ports {Linear_Flash_oe_n[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Linear_Flash_oe_n[0]}]

set_property PACKAGE_PIN M25 [get_ports Linear_Flash_we_n]
set_property IOSTANDARD LVCMOS25 [get_ports Linear_Flash_we_n]

set_property PACKAGE_PIN M30 [get_ports Linear_Flash_adv_ldn]
set_property IOSTANDARD LVCMOS25 [get_ports Linear_Flash_adv_ldn]


########################## UART ########################## 

set_property PACKAGE_PIN M19 [get_ports RS232_UART_1_sin]
set_property IOSTANDARD LVCMOS25 [get_ports RS232_UART_1_sin]

set_property PACKAGE_PIN K24 [get_ports RS232_UART_1_sout]
set_property IOSTANDARD LVCMOS25 [get_ports RS232_UART_1_sout]


########################## SDHC (Bank 12) ########################## 

set_property PACKAGE_PIN AB23 [get_ports sd_clk]
set_property IOSTANDARD LVCMOS25 [get_ports sd_clk]

set_property PACKAGE_PIN AC20 [get_ports {sd_dat_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sd_dat_io[0]}]

set_property PACKAGE_PIN AA23 [get_ports {sd_dat_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sd_dat_io[1]}]

set_property PACKAGE_PIN AA22 [get_ports {sd_dat_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sd_dat_io[2]}]

set_property PACKAGE_PIN AC21 [get_ports {sd_dat_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sd_dat_io[3]}]

set_property PACKAGE_PIN AB22 [get_ports sd_cmd_io]
set_property IOSTANDARD LVCMOS25 [get_ports sd_cmd_io]

set_property PACKAGE_PIN AA21 [get_ports sd_cd_n]
set_property IOSTANDARD LVCMOS25 [get_ports sd_cd_n]

set_property PACKAGE_PIN Y21 [get_ports sd_wp]
set_property IOSTANDARD LVCMOS25 [get_ports sd_wp]


########################## XADC ########################## 

set_property IOSTANDARD LVCMOS25 [get_ports axi_xadc_0_VAUXP]

set_property IOSTANDARD LVCMOS25 [get_ports axi_xadc_0_VAUXN]

set_property IOSTANDARD LVCMOS25 [get_ports axi_xadc_8_VAUXP]

set_property LOC XADC_X0Y0 [get_cells k7_emb_vdemo_i/microblaze_subsystem/axi_xadc_0/inst/AXI_XADC_CORE_I/XADC_INST]
set_property PACKAGE_PIN L23 [get_ports axi_xadc_8_VAUXN]
set_property PACKAGE_PIN J24 [get_ports axi_xadc_0_VAUXN]
set_property PACKAGE_PIN L22 [get_ports axi_xadc_8_VAUXP]
set_property PACKAGE_PIN J23 [get_ports axi_xadc_0_VAUXP]
set_property IOSTANDARD LVCMOS25 [get_ports axi_xadc_8_VAUXN]

########################## Ethernet ########################## 

set_property PACKAGE_PIN R23 [get_ports Soft_Ethernet_MAC_MDC_pin]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_MDC_pin]

set_property PACKAGE_PIN J21 [get_ports Soft_Ethernet_MAC_MDIO_pin]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_MDIO_pin]

set_property PACKAGE_PIN L20 [get_ports Soft_Ethernet_MAC_RST_n_pin]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_RST_n_pin]
set_false_path -through [get_ports Soft_Ethernet_MAC_RST_n_pin]

set_property PACKAGE_PIN M28 [get_ports Soft_Ethernet_MAC_tx_clk]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_tx_clk]
create_clock -period 40.000 -name Soft_Ethernet_MAC_tx_clk [get_ports Soft_Ethernet_MAC_tx_clk]

set_property PACKAGE_PIN U30 [get_ports {Soft_Ethernet_MAC_rxd[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[0]}]

set_property PACKAGE_PIN U25 [get_ports {Soft_Ethernet_MAC_rxd[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[1]}]

set_property PACKAGE_PIN T25 [get_ports {Soft_Ethernet_MAC_rxd[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[2]}]

set_property PACKAGE_PIN U28 [get_ports {Soft_Ethernet_MAC_rxd[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[3]}]

set_property PACKAGE_PIN R19 [get_ports {Soft_Ethernet_MAC_rxd[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[4]}]

set_property PACKAGE_PIN T27 [get_ports {Soft_Ethernet_MAC_rxd[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[5]}]

set_property PACKAGE_PIN T26 [get_ports {Soft_Ethernet_MAC_rxd[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[6]}]

set_property PACKAGE_PIN T28 [get_ports {Soft_Ethernet_MAC_rxd[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_rxd[7]}]

set_property PACKAGE_PIN U27 [get_ports Soft_Ethernet_MAC_rx_clk]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_clk]

set_property PACKAGE_PIN R28 [get_ports Soft_Ethernet_MAC_rx_dv]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_dv]

set_property PACKAGE_PIN V26 [get_ports Soft_Ethernet_MAC_rx_er]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_er]

set_property PACKAGE_PIN N27 [get_ports {Soft_Ethernet_MAC_txd[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[0]}]

set_property PACKAGE_PIN N25 [get_ports {Soft_Ethernet_MAC_txd[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[1]}]

set_property PACKAGE_PIN M29 [get_ports {Soft_Ethernet_MAC_txd[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[2]}]

set_property PACKAGE_PIN L28 [get_ports {Soft_Ethernet_MAC_txd[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[3]}]

set_property PACKAGE_PIN J26 [get_ports {Soft_Ethernet_MAC_txd[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[4]}]

set_property PACKAGE_PIN K26 [get_ports {Soft_Ethernet_MAC_txd[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[5]}]

set_property PACKAGE_PIN L30 [get_ports {Soft_Ethernet_MAC_txd[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[6]}]

set_property PACKAGE_PIN J28 [get_ports {Soft_Ethernet_MAC_txd[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {Soft_Ethernet_MAC_txd[7]}]

set_property PACKAGE_PIN K30 [get_ports Soft_Ethernet_MAC_gtx_clk]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_gtx_clk]

set_property PACKAGE_PIN M27 [get_ports Soft_Ethernet_MAC_tx_en]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_tx_en]

set_property PACKAGE_PIN N29 [get_ports Soft_Ethernet_MAC_tx_er]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_tx_er]

set_property SLEW FAST [get_ports Soft_Ethernet_MAC_txd*]
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_tx_en]
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_tx_er]
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_gtx_clk]

create_clock -period 8.000 -name rx_clk -add [get_nets Soft_Ethernet_MAC_rx_clk]

# Due to the early nature of speed files for this family this constraint has been relaxed
# to give a setup time of 2.7ns and a hold of 0.7 (should be 2ns an 0ns respectively)
set_input_delay -clock [get_clocks rx_clk] -max -add_delay 4.800 [get_ports [get_ports Soft_Ethernet_MAC_rxd*]]
set_input_delay -clock [get_clocks rx_clk] -min -add_delay 0.700 [get_ports [get_ports Soft_Ethernet_MAC_rxd*]]

set_input_delay -clock [get_clocks rx_clk] -max -add_delay 4.800 [get_ports [get_ports Soft_Ethernet_MAC_rx_dv]]
set_input_delay -clock [get_clocks rx_clk] -min -add_delay 0.700 [get_ports [get_ports Soft_Ethernet_MAC_rx_dv]]

set_input_delay -clock [get_clocks rx_clk] -max -add_delay 4.800 [get_ports [get_ports Soft_Ethernet_MAC_rx_er]]
set_input_delay -clock [get_clocks rx_clk] -min -add_delay 0.700 [get_ports [get_ports Soft_Ethernet_MAC_rx_er]]


##--------------------------------------------------------------------------------------------------------
## DVI IN - 0, FMC HPC Constraints
##--------------------------------------------------------------------------------------------------------

set_property PACKAGE_PIN F20 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_clk[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_clk[0]}]

set_property PACKAGE_PIN C30 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_de[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_de[0]}]

set_property PACKAGE_PIN B29 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_vsync[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_vsync[0]}]

set_property PACKAGE_PIN G27 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_hsync[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_hsync[0]}]

set_property PACKAGE_PIN G17 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[0]}]

set_property PACKAGE_PIN A21 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[1]}]

set_property PACKAGE_PIN B18 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[2]}]

set_property PACKAGE_PIN C19 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[3]}]

set_property PACKAGE_PIN A22 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[4]}]

set_property PACKAGE_PIN B20 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[5]}]

set_property PACKAGE_PIN C20 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[6]}]

set_property PACKAGE_PIN A20 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[7]}]

set_property PACKAGE_PIN F18 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[0]}]

set_property PACKAGE_PIN B22 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[1]}]

set_property PACKAGE_PIN F21 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[2]}]

set_property PACKAGE_PIN E20 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[3]}]

set_property PACKAGE_PIN D19 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[4]}]

set_property PACKAGE_PIN E19 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[5]}]

set_property PACKAGE_PIN G18 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[6]}]

set_property PACKAGE_PIN B24 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_green[7]}]

set_property PACKAGE_PIN C24 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[0]}]

set_property PACKAGE_PIN A27 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[1]}]

set_property PACKAGE_PIN A28 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[2]}]

set_property PACKAGE_PIN B28 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[3]}]

set_property PACKAGE_PIN A26 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[4]}]

set_property PACKAGE_PIN B27 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[5]}]

set_property PACKAGE_PIN F27 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[6]}]

set_property PACKAGE_PIN A25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_red[7]}]


#
##--------------------------------------------------------------------------------------------------------
## DVI IN - 1, FMC LPC Constraints
##--------------------------------------------------------------------------------------------------------
# LPC dvi in pixels clock                                                                                                                                                           

set_property PACKAGE_PIN AB27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_clk[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_clk[0]}]

set_property PACKAGE_PIN AK25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_de[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_de[0]}]

set_property PACKAGE_PIN AB20 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_vsync[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_vsync[0]}]

set_property PACKAGE_PIN AE25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_hsync[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_hsync[0]}]

set_property PACKAGE_PIN AC26 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[0]}]

set_property PACKAGE_PIN AG28 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[1]}]

set_property PACKAGE_PIN AK29 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[2]}]

set_property PACKAGE_PIN AJ28 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[3]}]

set_property PACKAGE_PIN AH27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[4]}]

set_property PACKAGE_PIN AK28 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[5]}]

set_property PACKAGE_PIN AJ27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[6]}]

set_property PACKAGE_PIN AG27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[7]}]

set_property PACKAGE_PIN AK26 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[0]}]

set_property PACKAGE_PIN AH26 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[1]}]

set_property PACKAGE_PIN AD27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[2]}]

set_property PACKAGE_PIN AC27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[3]}]

set_property PACKAGE_PIN AF27 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[4]}]

set_property PACKAGE_PIN AF26 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[5]}]

set_property PACKAGE_PIN AJ26 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[6]}]

set_property PACKAGE_PIN AD24 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_green[7]}]

set_property PACKAGE_PIN AC24 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[0]}]

set_property PACKAGE_PIN AD22 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[1]}]

set_property PACKAGE_PIN AE21 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[2]}]

set_property PACKAGE_PIN AD21 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[3]}]

set_property PACKAGE_PIN AC25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[4]}]

set_property PACKAGE_PIN AC22 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[5]}]

set_property PACKAGE_PIN AF25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[6]}]

set_property PACKAGE_PIN AB24 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_red[7]}]


#####################################################################################################################################################                           
# Local HDMI Transmitter                                                                                                                                                              
# ADV7511, Input ID type-6, left justified, Style-3                                                                                                                                                                        
#####################################################################################################################################################                           
set_property PACKAGE_PIN B23 [get_ports {dvi_24_to_16bit_hdmi_data[0]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[0]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[0]}]

set_property PACKAGE_PIN A23 [get_ports {dvi_24_to_16bit_hdmi_data[1]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[1]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[1]}]

set_property PACKAGE_PIN E23 [get_ports {dvi_24_to_16bit_hdmi_data[2]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[2]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[2]}]

set_property PACKAGE_PIN D23 [get_ports {dvi_24_to_16bit_hdmi_data[3]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[3]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[3]}]

set_property PACKAGE_PIN F25 [get_ports {dvi_24_to_16bit_hdmi_data[4]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[4]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[4]}]

set_property PACKAGE_PIN E25 [get_ports {dvi_24_to_16bit_hdmi_data[5]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[5]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[5]}]

set_property PACKAGE_PIN E24 [get_ports {dvi_24_to_16bit_hdmi_data[6]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[6]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[6]}]

set_property PACKAGE_PIN D24 [get_ports {dvi_24_to_16bit_hdmi_data[7]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[7]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[7]}]

set_property PACKAGE_PIN F26 [get_ports {dvi_24_to_16bit_hdmi_data[8]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[8]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[8]}]

set_property PACKAGE_PIN E26 [get_ports {dvi_24_to_16bit_hdmi_data[9]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[9]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[9]}]

set_property PACKAGE_PIN G23 [get_ports {dvi_24_to_16bit_hdmi_data[10]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[10]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[10]}]

set_property PACKAGE_PIN G24 [get_ports {dvi_24_to_16bit_hdmi_data[11]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[11]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[11]}]

set_property PACKAGE_PIN J19 [get_ports {dvi_24_to_16bit_hdmi_data[12]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[12]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[12]}]

set_property PACKAGE_PIN H19 [get_ports {dvi_24_to_16bit_hdmi_data[13]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[13]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[13]}]

set_property PACKAGE_PIN L17 [get_ports {dvi_24_to_16bit_hdmi_data[14]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[14]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[14]}]

set_property PACKAGE_PIN L18 [get_ports {dvi_24_to_16bit_hdmi_data[15]}]
set_property DRIVE 8 [get_ports {dvi_24_to_16bit_hdmi_data[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {dvi_24_to_16bit_hdmi_data[15]}]
set_property SLEW FAST [get_ports {dvi_24_to_16bit_hdmi_data[15]}]

set_property PACKAGE_PIN H17 [get_ports dvi_24_to_16bit_de]
set_property DRIVE 8 [get_ports dvi_24_to_16bit_de]
set_property IOSTANDARD LVCMOS25 [get_ports dvi_24_to_16bit_de]
set_property SLEW FAST [get_ports dvi_24_to_16bit_de]

set_property PACKAGE_PIN J18 [get_ports dvi_24_to_16bit_hsync]
set_property DRIVE 8 [get_ports dvi_24_to_16bit_hsync]
set_property IOSTANDARD LVCMOS25 [get_ports dvi_24_to_16bit_hsync]
set_property SLEW FAST [get_ports dvi_24_to_16bit_hsync]

set_property PACKAGE_PIN H20 [get_ports dvi_24_to_16bit_vsync]
set_property DRIVE 8 [get_ports dvi_24_to_16bit_vsync]
set_property IOSTANDARD LVCMOS25 [get_ports dvi_24_to_16bit_vsync]
set_property SLEW FAST [get_ports dvi_24_to_16bit_vsync]

set_property PACKAGE_PIN K18 [get_ports dvi_24_to_16bit_hdmi_clk]
set_property DRIVE 8 [get_ports dvi_24_to_16bit_hdmi_clk]
set_property IOSTANDARD LVCMOS25 [get_ports dvi_24_to_16bit_hdmi_clk]
set_property SLEW FAST [get_ports dvi_24_to_16bit_hdmi_clk]



##=====================================================================================================================================================
##DVI_IN_0 clock
##=====================================================================================================================================================
create_clock -period 6.667 -name DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_clk[0]}]

# Constraint to override the clock rule
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {DVI_IN_0_fmc_hpc_dvidp_dvii_clk[0]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets k7_emb_vdemo_i/clock_generator_1/inst/clk_out2]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets k7_emb_vdemo_i/clock_generator_1/inst/clk_out1]

# OFFSET_IN for dvi interface signals w.r.t dvi in clock , setup requirement is 1ns and Hold is 1ns ######                                                                                                           
set_input_delay -clock [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin] -max -add_delay 3.000 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[0] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[1] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[2] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[3] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[4] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[5] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[6] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[7] DVI_IN_0_fmc_hpc_dvidp_dvii_de[0] DVI_IN_0_fmc_hpc_dvidp_dvii_green[0] DVI_IN_0_fmc_hpc_dvidp_dvii_green[1] DVI_IN_0_fmc_hpc_dvidp_dvii_green[2] DVI_IN_0_fmc_hpc_dvidp_dvii_green[3] DVI_IN_0_fmc_hpc_dvidp_dvii_green[4] DVI_IN_0_fmc_hpc_dvidp_dvii_green[5] DVI_IN_0_fmc_hpc_dvidp_dvii_green[6] DVI_IN_0_fmc_hpc_dvidp_dvii_green[7] DVI_IN_0_fmc_hpc_dvidp_dvii_hsync[0] DVI_IN_0_fmc_hpc_dvidp_dvii_red[0] DVI_IN_0_fmc_hpc_dvidp_dvii_red[1] DVI_IN_0_fmc_hpc_dvidp_dvii_red[2] DVI_IN_0_fmc_hpc_dvidp_dvii_red[3] DVI_IN_0_fmc_hpc_dvidp_dvii_red[4] DVI_IN_0_fmc_hpc_dvidp_dvii_red[5] DVI_IN_0_fmc_hpc_dvidp_dvii_red[6] DVI_IN_0_fmc_hpc_dvidp_dvii_red[7] DVI_IN_0_fmc_hpc_dvidp_dvii_vsync[0]}]
set_input_delay -clock [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin] -min -add_delay 1.000 [get_ports {DVI_IN_0_fmc_hpc_dvidp_dvii_blue[0] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[1] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[2] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[3] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[4] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[5] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[6] DVI_IN_0_fmc_hpc_dvidp_dvii_blue[7] DVI_IN_0_fmc_hpc_dvidp_dvii_de[0] DVI_IN_0_fmc_hpc_dvidp_dvii_green[0] DVI_IN_0_fmc_hpc_dvidp_dvii_green[1] DVI_IN_0_fmc_hpc_dvidp_dvii_green[2] DVI_IN_0_fmc_hpc_dvidp_dvii_green[3] DVI_IN_0_fmc_hpc_dvidp_dvii_green[4] DVI_IN_0_fmc_hpc_dvidp_dvii_green[5] DVI_IN_0_fmc_hpc_dvidp_dvii_green[6] DVI_IN_0_fmc_hpc_dvidp_dvii_green[7] DVI_IN_0_fmc_hpc_dvidp_dvii_hsync[0] DVI_IN_0_fmc_hpc_dvidp_dvii_red[0] DVI_IN_0_fmc_hpc_dvidp_dvii_red[1] DVI_IN_0_fmc_hpc_dvidp_dvii_red[2] DVI_IN_0_fmc_hpc_dvidp_dvii_red[3] DVI_IN_0_fmc_hpc_dvidp_dvii_red[4] DVI_IN_0_fmc_hpc_dvidp_dvii_red[5] DVI_IN_0_fmc_hpc_dvidp_dvii_red[6] DVI_IN_0_fmc_hpc_dvidp_dvii_red[7] DVI_IN_0_fmc_hpc_dvidp_dvii_vsync[0]}]

##=====================================================================================================================================================
##DVI_IN_1 clock
##=====================================================================================================================================================
create_clock -period 6.667 -name DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_clk[0]}]

#set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets DVI_IN_1_fmc_lpc_dvidp_dvii_clk[0]]

set_input_delay -clock [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin] -max -add_delay 3.000 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[0] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[1] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[2] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[3] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[4] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[5] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[6] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[7] DVI_IN_1_fmc_lpc_dvidp_dvii_de[0] DVI_IN_1_fmc_lpc_dvidp_dvii_green[0] DVI_IN_1_fmc_lpc_dvidp_dvii_green[1] DVI_IN_1_fmc_lpc_dvidp_dvii_green[2] DVI_IN_1_fmc_lpc_dvidp_dvii_green[3] DVI_IN_1_fmc_lpc_dvidp_dvii_green[4] DVI_IN_1_fmc_lpc_dvidp_dvii_green[5] DVI_IN_1_fmc_lpc_dvidp_dvii_green[6] DVI_IN_1_fmc_lpc_dvidp_dvii_green[7] DVI_IN_1_fmc_lpc_dvidp_dvii_hsync[0] DVI_IN_1_fmc_lpc_dvidp_dvii_red[0] DVI_IN_1_fmc_lpc_dvidp_dvii_red[1] DVI_IN_1_fmc_lpc_dvidp_dvii_red[2] DVI_IN_1_fmc_lpc_dvidp_dvii_red[3] DVI_IN_1_fmc_lpc_dvidp_dvii_red[4] DVI_IN_1_fmc_lpc_dvidp_dvii_red[5] DVI_IN_1_fmc_lpc_dvidp_dvii_red[6] DVI_IN_1_fmc_lpc_dvidp_dvii_red[7] DVI_IN_1_fmc_lpc_dvidp_dvii_vsync[0]}]
set_input_delay -clock [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin] -min -add_delay 1.000 [get_ports {DVI_IN_1_fmc_lpc_dvidp_dvii_blue[0] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[1] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[2] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[3] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[4] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[5] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[6] DVI_IN_1_fmc_lpc_dvidp_dvii_blue[7] DVI_IN_1_fmc_lpc_dvidp_dvii_de[0] DVI_IN_1_fmc_lpc_dvidp_dvii_green[0] DVI_IN_1_fmc_lpc_dvidp_dvii_green[1] DVI_IN_1_fmc_lpc_dvidp_dvii_green[2] DVI_IN_1_fmc_lpc_dvidp_dvii_green[3] DVI_IN_1_fmc_lpc_dvidp_dvii_green[4] DVI_IN_1_fmc_lpc_dvidp_dvii_green[5] DVI_IN_1_fmc_lpc_dvidp_dvii_green[6] DVI_IN_1_fmc_lpc_dvidp_dvii_green[7] DVI_IN_1_fmc_lpc_dvidp_dvii_hsync[0] DVI_IN_1_fmc_lpc_dvidp_dvii_red[0] DVI_IN_1_fmc_lpc_dvidp_dvii_red[1] DVI_IN_1_fmc_lpc_dvidp_dvii_red[2] DVI_IN_1_fmc_lpc_dvidp_dvii_red[3] DVI_IN_1_fmc_lpc_dvidp_dvii_red[4] DVI_IN_1_fmc_lpc_dvidp_dvii_red[5] DVI_IN_1_fmc_lpc_dvidp_dvii_red[6] DVI_IN_1_fmc_lpc_dvidp_dvii_red[7] DVI_IN_1_fmc_lpc_dvidp_dvii_vsync[0]}]

##=====================================================================================================================================================
## Internal clocks
##=====================================================================================================================================================
create_clock -period 6.667 -name sys_clk_s [get_pins k7_emb_vdemo_i/clock_generator_1/inst/mmcm_adv_inst/CLKOUT0]
create_clock -period 8.000 -name gtx_clk [get_pins k7_emb_vdemo_i/clock_generator_1/inst/mmcm_adv_inst/CLKOUT1]

set_clock_groups -name dvi_in_async -asynchronous -group [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin] -group [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin]
set_clock_groups -name exclusiv_vsrc0_clk -physically_exclusive -group [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin] -group [get_clocks sys_clk_s]
set_clock_groups -name exclusiv_vsrc1_clk -physically_exclusive -group [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin] -group [get_clocks sys_clk_s]
set_false_path -from [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin] -to [get_clocks sys_clk_s]
set_false_path -from [get_clocks u_ddr3_infrastructure/mmcm_clkout0] -to [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin]
set_false_path -from [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin] -to [get_clocks u_ddr3_infrastructure/mmcm_clkout0]
set_false_path -from [get_clocks u_ddr3_infrastructure/mmcm_clkout0] -to [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin]
set_false_path -from [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin] -to [get_clocks u_ddr3_infrastructure/mmcm_clkout0]
set_false_path -from [get_clocks u_ddr3_infrastructure/mmcm_clkout0] -to [get_clocks sys_clk_s]
set_false_path -from [get_clocks sys_clk_s] -to [get_clocks u_ddr3_infrastructure/mmcm_clkout0]
set_false_path -from [get_clocks sys_clk_s] -to [get_clocks DVI_IN_0_fmc_hpc_dvidp_dvii_clk_pin]
set_false_path -from [get_clocks sys_clk_s] -to [get_clocks DVI_IN_1_fmc_lpc_dvidp_dvii_clk_pin]


