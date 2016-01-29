########################## CLK ###################################
set_property LOC AD11 [ get_ports CLK_N]
set_property IOSTANDARD DIFF_SSTL15 [ get_ports CLK_N]

set_property LOC AD12 [ get_ports CLK_P]
set_property IOSTANDARD DIFF_SSTL15 [ get_ports CLK_P]

create_clock -period 5.000 -name sys_clk_pin [get_ports CLK_P]

########################## RESET ###################################

set_property PACKAGE_PIN AB7 [get_ports RESET]
set_property IOSTANDARD LVCMOS15 [get_ports RESET]

########################## DDR3 ###################################

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

set_property slave_banks {32 34} [get_iobanks 33]

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

set_property LOC F16 [ get_ports LEDs_8Bits_TRI_O[7]]
set_property IOSTANDARD LVCMOS25 [ get_ports LEDs_8Bits_TRI_O[7]]

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

set_property LOC AB23 [ get_ports sd_clk]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_clk]

set_property LOC AC20 [ get_ports sd_dat_io[0]]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_dat_io[0]]

set_property LOC AA23 [ get_ports sd_dat_io[1]]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_dat_io[1]]

set_property LOC AA22 [ get_ports sd_dat_io[2]]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_dat_io[2]]

set_property LOC AC21 [ get_ports sd_dat_io[3]]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_dat_io[3]]

set_property LOC AB22 [ get_ports sd_cmd_io]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_cmd_io]

set_property LOC AA21 [ get_ports sd_cd_n]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_cd_n]

set_property LOC Y21 [ get_ports sd_wp]
set_property IOSTANDARD LVCMOS25 [ get_ports sd_wp]

########################## XADC ########################## 

set_property LOC J23 [ get_ports axi_xadc_0_VAUXP]
set_property IOSTANDARD LVCMOS25 [ get_ports axi_xadc_0_VAUXP]

set_property LOC J24 [ get_ports axi_xadc_0_VAUXN]
set_property IOSTANDARD LVCMOS25 [ get_ports axi_xadc_0_VAUXN]

set_property LOC L22 [ get_ports axi_xadc_8_VAUXP]
set_property IOSTANDARD LVCMOS25 [ get_ports axi_xadc_8_VAUXP]

set_property LOC L23 [ get_ports axi_xadc_8_VAUXN]
set_property IOSTANDARD LVCMOS25 [ get_ports axi_xadc_8_VAUXN]

########################## Ethernet ########################## 

##set_property LOC W19 [ get_ports Soft_Ethernet_MAC_GMII_COL_pin]
##set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_GMII_COL_pin]

##set_property LOC R30 [ get_ports Soft_Ethernet_MAC_GMII_CRS_pin]
##set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_GMII_CRS_pin]

set_property LOC R23 [ get_ports Soft_Ethernet_MAC_MDC_pin]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_MDC_pin]

set_property LOC J21 [ get_ports Soft_Ethernet_MAC_MDIO_pin]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_MDIO_pin]

set_property LOC L20 [ get_ports Soft_Ethernet_MAC_RST_n_pin]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_RST_n_pin]
set_false_path -through [ get_ports Soft_Ethernet_MAC_RST_n_pin]

set_property LOC M28 [ get_ports Soft_Ethernet_MAC_tx_clk]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_tx_clk]
create_clock -period 40.000 -name Soft_Ethernet_MAC_tx_clk [get_ports Soft_Ethernet_MAC_tx_clk]

set_property LOC U30 [ get_ports Soft_Ethernet_MAC_rxd[0]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[0]]

set_property LOC U25 [ get_ports Soft_Ethernet_MAC_rxd[1]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[1]]

set_property LOC T25 [ get_ports Soft_Ethernet_MAC_rxd[2]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[2]]

set_property LOC U28 [ get_ports Soft_Ethernet_MAC_rxd[3]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[3]]

set_property LOC R19 [ get_ports Soft_Ethernet_MAC_rxd[4]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[4]]

set_property LOC T27 [ get_ports Soft_Ethernet_MAC_rxd[5]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[5]]

set_property LOC T26 [ get_ports Soft_Ethernet_MAC_rxd[6]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[6]]

set_property LOC T28 [ get_ports Soft_Ethernet_MAC_rxd[7]]
set_property IOSTANDARD LVCMOS25 [ get_ports Soft_Ethernet_MAC_rxd[7]]

set_property LOC U27 [get_ports Soft_Ethernet_MAC_rx_clk]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_clk]

set_property LOC R28 [get_ports Soft_Ethernet_MAC_rx_dv]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_dv]

set_property LOC V26 [get_ports Soft_Ethernet_MAC_rx_er]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_rx_er]

set_property LOC N27 [get_ports Soft_Ethernet_MAC_txd[0]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[0]]

set_property LOC N25 [get_ports Soft_Ethernet_MAC_txd[1]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[1]]

set_property LOC M29 [get_ports Soft_Ethernet_MAC_txd[2]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[2]]

set_property LOC L28 [get_ports Soft_Ethernet_MAC_txd[3]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[3]]

set_property LOC J26 [get_ports Soft_Ethernet_MAC_txd[4]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[4]]

set_property LOC K26 [get_ports Soft_Ethernet_MAC_txd[5]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[5]]

set_property LOC L30 [get_ports Soft_Ethernet_MAC_txd[6]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[6]]

set_property LOC J28 [get_ports Soft_Ethernet_MAC_txd[7]]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_txd[7]]

set_property LOC K30 [get_ports Soft_Ethernet_MAC_gtx_clk]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_gtx_clk]

set_property LOC M27 [get_ports Soft_Ethernet_MAC_tx_en]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_tx_en]

set_property LOC N29 [get_ports Soft_Ethernet_MAC_tx_er]
set_property IOSTANDARD LVCMOS25 [get_ports Soft_Ethernet_MAC_tx_er]

set_property SLEW FAST [get_ports Soft_Ethernet_MAC_txd*]  
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_tx_en]
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_tx_er]
set_property SLEW FAST [get_ports Soft_Ethernet_MAC_gtx_clk]

create_clock -add -name rx_clk -period 8.000   [get_nets Soft_Ethernet_MAC_rx_clk]

# Due to the early nature of speed files for this family this constraint has been relaxed
# to give a setup time of 2.7ns and a hold of 0.7 (should be 2ns an 0ns respectively)
set_input_delay -clock [get_clocks rx_clk] -max 4.800 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rxd*]] 
set_input_delay -clock [get_clocks rx_clk] -min 0.700 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rxd*]]

set_input_delay -clock [get_clocks rx_clk] -max 4.800 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rx_dv]] 
set_input_delay -clock [get_clocks rx_clk] -min 0.700 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rx_dv]]

set_input_delay -clock [get_clocks rx_clk] -max 4.800 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rx_er]] 
set_input_delay -clock [get_clocks rx_clk] -min 0.700 -add_delay [get_ports [get_ports Soft_Ethernet_MAC_rx_er]]








