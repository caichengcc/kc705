`timescale 1 ps / 1 ps
// lib IP_Integrator_Lib
module k7_emb_bist_top
   (CLK_N,
    CLK_P,
    DDR3_addr,
    DDR3_ba,
    DDR3_cas_n,
    DDR3_ck_n,
    DDR3_ck_p,
    DDR3_cke,
    DDR3_cs_n,
    DDR3_dm,
    DDR3_dq,
    DDR3_dqs_n,
    DDR3_dqs_p,
    DDR3_odt,
    DDR3_ras_n,
    DDR3_reset_n,
    DDR3_we_n,
    DIP_Switches_TRI_I,
    IIC_MAIN_scl_io,
    IIC_MAIN_sda_io,
    IIC_MUX_RESET_B,
    LCD_GPIO_IO_tri_io,
    LEDs_8Bits_TRI_O,
    Linear_Flash_Data,
    //Linear_Flash_Data_O,
    //Linear_Flash_Data_T,
    Linear_Flash_address,
    Linear_Flash_adv_ldn,
    Linear_Flash_ce_n,
    Linear_Flash_oe_n,
    Linear_Flash_we_n,
    Push_Buttons_5Bits_TRI_I,
    RESET,
    ROTARY_GPIO_IO_tri_io,
    RS232_UART_1_sin,
    RS232_UART_1_sout,
    Soft_Ethernet_MAC_MDC_pin,
    Soft_Ethernet_MAC_MDIO_pin,
    Soft_Ethernet_MAC_RST_n_pin,
    Soft_Ethernet_MAC_gtx_clk,
    Soft_Ethernet_MAC_rx_clk,
    Soft_Ethernet_MAC_rx_dv,
    Soft_Ethernet_MAC_rx_er,
    Soft_Ethernet_MAC_rxd,
    Soft_Ethernet_MAC_tx_clk,
    Soft_Ethernet_MAC_tx_en,
    Soft_Ethernet_MAC_tx_er,
    Soft_Ethernet_MAC_txd,
    axi_xadc_0_VAUXN,
    axi_xadc_0_VAUXP,
    axi_xadc_8_VAUXN,
    axi_xadc_8_VAUXP,
    ddr3_1_n,
    ddr3_1_p,
    sd_cd_n,
    sd_clk,
    sd_cmd_io,
    sd_dat_io,
    sd_wp,
    sm_fan_pwm);
  input CLK_N;
  input CLK_P;
  output [13:0]DDR3_addr;
  output [2:0]DDR3_ba;
  output DDR3_cas_n;
  output [0:0]DDR3_ck_n;
  output [0:0]DDR3_ck_p;
  output [0:0]DDR3_cke;
  output [0:0]DDR3_cs_n;
  output [7:0]DDR3_dm;
  inout [63:0]DDR3_dq;
  inout [7:0]DDR3_dqs_n;
  inout [7:0]DDR3_dqs_p;
  output [0:0]DDR3_odt;
  output DDR3_ras_n;
  output DDR3_reset_n;
  output DDR3_we_n;
  input [3:0]DIP_Switches_TRI_I;
  inout IIC_MAIN_scl_io;
  inout IIC_MAIN_sda_io;
  output [0:0]IIC_MUX_RESET_B;
  inout [6:0]LCD_GPIO_IO_tri_io;
  output [7:0]LEDs_8Bits_TRI_O;
  inout [15:0]Linear_Flash_Data;
  //output [15:0]Linear_Flash_Data_O;
  //output [15:0]Linear_Flash_Data_T;
  output [0:25]Linear_Flash_address;
  output Linear_Flash_adv_ldn;
  output [0:0]Linear_Flash_ce_n;
  output [0:0]Linear_Flash_oe_n;
  output Linear_Flash_we_n;
  input [4:0]Push_Buttons_5Bits_TRI_I;
  input RESET;
  inout [2:0]ROTARY_GPIO_IO_tri_io;
  input RS232_UART_1_sin;
  output RS232_UART_1_sout;
  output Soft_Ethernet_MAC_MDC_pin;
  inout Soft_Ethernet_MAC_MDIO_pin;
  output Soft_Ethernet_MAC_RST_n_pin;
  output Soft_Ethernet_MAC_gtx_clk;
  input Soft_Ethernet_MAC_rx_clk;
  input Soft_Ethernet_MAC_rx_dv;
  input Soft_Ethernet_MAC_rx_er;
  input [7:0]Soft_Ethernet_MAC_rxd;
  input Soft_Ethernet_MAC_tx_clk;
  output Soft_Ethernet_MAC_tx_en;
  output Soft_Ethernet_MAC_tx_er;
  output [7:0]Soft_Ethernet_MAC_txd;
  input axi_xadc_0_VAUXN;
  input axi_xadc_0_VAUXP;
  input axi_xadc_8_VAUXN;
  input axi_xadc_8_VAUXP;
  output [2:0]ddr3_1_n;
  output [1:0]ddr3_1_p;
  input sd_cd_n;
  output sd_clk;
  inout sd_cmd_io;
  inout [3:0]sd_dat_io;
  input sd_wp;
  output [0:0]sm_fan_pwm;

  wire CLK_N;
  wire CLK_P;
  wire [13:0]DDR3_addr;
  wire [2:0]DDR3_ba;
  wire DDR3_cas_n;
  wire \^DDR3_ck_n ;
  wire \^DDR3_ck_p ;
  wire \^DDR3_cke ;
  wire \^DDR3_cs_n ;
  wire [7:0]DDR3_dm;
  wire [63:0]DDR3_dq;
  wire [7:0]DDR3_dqs_n;
  wire [7:0]DDR3_dqs_p;
  wire \^DDR3_odt ;
  wire DDR3_ras_n;
  wire DDR3_reset_n;
  wire DDR3_we_n;
  wire [3:0]DIP_Switches_TRI_I;
  wire IIC_MAIN_scl_i;
  wire IIC_MAIN_scl_io;
  wire IIC_MAIN_scl_o;
  wire IIC_MAIN_scl_t;
  wire IIC_MAIN_sda_i;
  wire IIC_MAIN_sda_io;
  wire IIC_MAIN_sda_o;
  wire IIC_MAIN_sda_t;
  wire \^IIC_MUX_RESET_B ;
  wire LCD_GPIO_IO_tri_i_2;
  wire LCD_GPIO_IO_tri_i_3;
  wire LCD_GPIO_IO_tri_i_4;
  wire LCD_GPIO_IO_tri_i_5;
  wire LCD_GPIO_IO_tri_i_6;
  wire LCD_GPIO_IO_tri_i_7;
  wire LCD_GPIO_IO_tri_i_8;
  wire LCD_GPIO_IO_tri_io_2;
  wire LCD_GPIO_IO_tri_io_3;
  wire LCD_GPIO_IO_tri_io_4;
  wire LCD_GPIO_IO_tri_io_5;
  wire LCD_GPIO_IO_tri_io_6;
  wire LCD_GPIO_IO_tri_io_7;
  wire LCD_GPIO_IO_tri_io_8;
  wire LCD_GPIO_IO_tri_o_2;
  wire LCD_GPIO_IO_tri_o_3;
  wire LCD_GPIO_IO_tri_o_4;
  wire LCD_GPIO_IO_tri_o_5;
  wire LCD_GPIO_IO_tri_o_6;
  wire LCD_GPIO_IO_tri_o_7;
  wire LCD_GPIO_IO_tri_o_8;
  wire LCD_GPIO_IO_tri_t_2;
  wire LCD_GPIO_IO_tri_t_3;
  wire LCD_GPIO_IO_tri_t_4;
  wire LCD_GPIO_IO_tri_t_5;
  wire LCD_GPIO_IO_tri_t_6;
  wire LCD_GPIO_IO_tri_t_7;
  wire LCD_GPIO_IO_tri_t_8;
  wire [7:0]LEDs_8Bits_TRI_O;
  wire [15:0]Linear_Flash_Data_I;
  wire [15:0]Linear_Flash_Data_O;
  wire [15:0]Linear_Flash_Data_T;
  wire [0:25]Linear_Flash_address;
  wire Linear_Flash_adv_ldn;
  wire \^Linear_Flash_ce_n ;
  wire \^Linear_Flash_oe_n ;
  wire Linear_Flash_we_n;
  wire [4:0]Push_Buttons_5Bits_TRI_I;
  wire RESET;
  wire ROTARY_GPIO_IO_tri_i_10;
  wire ROTARY_GPIO_IO_tri_i_11;
  wire ROTARY_GPIO_IO_tri_i_9;
  wire ROTARY_GPIO_IO_tri_io_10;
  wire ROTARY_GPIO_IO_tri_io_11;
  wire ROTARY_GPIO_IO_tri_io_9;
  wire ROTARY_GPIO_IO_tri_o_10;
  wire ROTARY_GPIO_IO_tri_o_11;
  wire ROTARY_GPIO_IO_tri_o_9;
  wire ROTARY_GPIO_IO_tri_t_10;
  wire ROTARY_GPIO_IO_tri_t_11;
  wire ROTARY_GPIO_IO_tri_t_9;
  wire RS232_UART_1_sin;
  wire RS232_UART_1_sout;
  wire Soft_Ethernet_MAC_MDC_pin;
  wire Soft_Ethernet_MAC_MDIO_pin;
  wire Soft_Ethernet_MAC_RST_n_pin;
  wire Soft_Ethernet_MAC_gtx_clk;
  wire Soft_Ethernet_MAC_rx_clk;
  wire Soft_Ethernet_MAC_rx_dv;
  wire Soft_Ethernet_MAC_rx_er;
  wire [7:0]Soft_Ethernet_MAC_rxd;
  wire Soft_Ethernet_MAC_tx_clk;
  wire Soft_Ethernet_MAC_tx_en;
  wire Soft_Ethernet_MAC_tx_er;
  wire [7:0]Soft_Ethernet_MAC_txd;
  wire axi_xadc_0_VAUXN;
  wire axi_xadc_0_VAUXP;
  wire axi_xadc_8_VAUXN;
  wire axi_xadc_8_VAUXP;
  wire [2:0]ddr3_1_n;
  wire [1:0]ddr3_1_p;
  wire init_calib_complete;
  wire sd_cd_n;
  wire sd_clk;
  wire sd_cmd_i;
  wire sd_cmd_io;
  wire sd_cmd_o;
  wire sd_cmd_t;
  wire sd_dat_i_13;
  wire sd_dat_i_14;
  wire sd_dat_i_15;
  wire sd_dat_i_16;
  wire sd_dat_io_13;
  wire sd_dat_io_14;
  wire sd_dat_io_15;
  wire sd_dat_io_16;
  wire sd_dat_o_13;
  wire sd_dat_o_14;
  wire sd_dat_o_15;
  wire sd_dat_o_16;
  wire sd_dat_t_13;
  wire sd_dat_t_14;
  wire sd_dat_t_15;
  wire sd_dat_t_16;
  wire sd_wp;
  wire \^sm_fan_pwm ;

  assign DDR3_ck_n[0] = \^DDR3_ck_n ;
  assign DDR3_ck_p[0] = \^DDR3_ck_p ;
  assign DDR3_cke[0] = \^DDR3_cke ;
  assign DDR3_cs_n[0] = \^DDR3_cs_n ;
  assign DDR3_odt[0] = \^DDR3_odt ;
  assign IIC_MUX_RESET_B[0] = \^IIC_MUX_RESET_B ;
  assign Linear_Flash_ce_n[0] = \^Linear_Flash_ce_n ;
  assign Linear_Flash_oe_n[0] = \^Linear_Flash_oe_n ;
  assign sm_fan_pwm[0] = \^sm_fan_pwm ;
  xVIA LCD_GPIO_IO_tri_io_0via (LCD_GPIO_IO_tri_io[0], LCD_GPIO_IO_tri_io_2);
  xVIA LCD_GPIO_IO_tri_io_1via (LCD_GPIO_IO_tri_io[1], LCD_GPIO_IO_tri_io_3);
  xVIA LCD_GPIO_IO_tri_io_2via (LCD_GPIO_IO_tri_io[2], LCD_GPIO_IO_tri_io_4);
  xVIA LCD_GPIO_IO_tri_io_3via (LCD_GPIO_IO_tri_io[3], LCD_GPIO_IO_tri_io_5);
  xVIA LCD_GPIO_IO_tri_io_4via (LCD_GPIO_IO_tri_io[4], LCD_GPIO_IO_tri_io_6);
  xVIA LCD_GPIO_IO_tri_io_5via (LCD_GPIO_IO_tri_io[5], LCD_GPIO_IO_tri_io_7);
  xVIA LCD_GPIO_IO_tri_io_6via (LCD_GPIO_IO_tri_io[6], LCD_GPIO_IO_tri_io_8);
  xVIA ROTARY_GPIO_IO_tri_io_0via (ROTARY_GPIO_IO_tri_io[0], ROTARY_GPIO_IO_tri_io_9);
  xVIA ROTARY_GPIO_IO_tri_io_1via (ROTARY_GPIO_IO_tri_io[1], ROTARY_GPIO_IO_tri_io_10);
  xVIA ROTARY_GPIO_IO_tri_io_2via (ROTARY_GPIO_IO_tri_io[2], ROTARY_GPIO_IO_tri_io_11);
  xVIA sd_dat_io_0via (sd_dat_io[0], sd_dat_io_13);
  xVIA sd_dat_io_1via (sd_dat_io[1], sd_dat_io_14);
  xVIA sd_dat_io_2via (sd_dat_io[2], sd_dat_io_15);
  xVIA sd_dat_io_3via (sd_dat_io[3], sd_dat_io_16);

  genvar n;
   generate
      for (n=0; n<=15; n=n+1) begin : mem_bus
        IOBUF u1   
        (.I(Linear_Flash_Data_O[n]),
        .IO(Linear_Flash_Data[15-n]),
        .O(Linear_Flash_Data_I[n]),
        .T(Linear_Flash_Data_T[n]));
      end
    endgenerate


IOBUF IIC_MAIN_scl_iobuf_0
       (.I(IIC_MAIN_scl_o),
        .IO(IIC_MAIN_scl_io),
        .O(IIC_MAIN_scl_i),
        .T(IIC_MAIN_scl_t));
IOBUF IIC_MAIN_sda_iobuf_1
       (.I(IIC_MAIN_sda_o),
        .IO(IIC_MAIN_sda_io),
        .O(IIC_MAIN_sda_i),
        .T(IIC_MAIN_sda_t));
IOBUF LCD_GPIO_IO_tri_iobuf_2
       (.I(LCD_GPIO_IO_tri_o_2),
        .IO(LCD_GPIO_IO_tri_io_2),
        .O(LCD_GPIO_IO_tri_i_2),
        .T(LCD_GPIO_IO_tri_t_2));
IOBUF LCD_GPIO_IO_tri_iobuf_3
       (.I(LCD_GPIO_IO_tri_o_3),
        .IO(LCD_GPIO_IO_tri_io_3),
        .O(LCD_GPIO_IO_tri_i_3),
        .T(LCD_GPIO_IO_tri_t_3));
IOBUF LCD_GPIO_IO_tri_iobuf_4
       (.I(LCD_GPIO_IO_tri_o_4),
        .IO(LCD_GPIO_IO_tri_io_4),
        .O(LCD_GPIO_IO_tri_i_4),
        .T(LCD_GPIO_IO_tri_t_4));
IOBUF LCD_GPIO_IO_tri_iobuf_5
       (.I(LCD_GPIO_IO_tri_o_5),
        .IO(LCD_GPIO_IO_tri_io_5),
        .O(LCD_GPIO_IO_tri_i_5),
        .T(LCD_GPIO_IO_tri_t_5));
IOBUF LCD_GPIO_IO_tri_iobuf_6
       (.I(LCD_GPIO_IO_tri_o_6),
        .IO(LCD_GPIO_IO_tri_io_6),
        .O(LCD_GPIO_IO_tri_i_6),
        .T(LCD_GPIO_IO_tri_t_6));
IOBUF LCD_GPIO_IO_tri_iobuf_7
       (.I(LCD_GPIO_IO_tri_o_7),
        .IO(LCD_GPIO_IO_tri_io_7),
        .O(LCD_GPIO_IO_tri_i_7),
        .T(LCD_GPIO_IO_tri_t_7));
IOBUF LCD_GPIO_IO_tri_iobuf_8
       (.I(LCD_GPIO_IO_tri_o_8),
        .IO(LCD_GPIO_IO_tri_io_8),
        .O(LCD_GPIO_IO_tri_i_8),
        .T(LCD_GPIO_IO_tri_t_8));
IOBUF ROTARY_GPIO_IO_tri_iobuf_10
       (.I(ROTARY_GPIO_IO_tri_o_10),
        .IO(ROTARY_GPIO_IO_tri_io_10),
        .O(ROTARY_GPIO_IO_tri_i_10),
        .T(ROTARY_GPIO_IO_tri_t_10));
IOBUF ROTARY_GPIO_IO_tri_iobuf_11
       (.I(ROTARY_GPIO_IO_tri_o_11),
        .IO(ROTARY_GPIO_IO_tri_io_11),
        .O(ROTARY_GPIO_IO_tri_i_11),
        .T(ROTARY_GPIO_IO_tri_t_11));
IOBUF ROTARY_GPIO_IO_tri_iobuf_9
       (.I(ROTARY_GPIO_IO_tri_o_9),
        .IO(ROTARY_GPIO_IO_tri_io_9),
        .O(ROTARY_GPIO_IO_tri_i_9),
        .T(ROTARY_GPIO_IO_tri_t_9));
k7_emb_bist k7_emb_bist_i
       (.CLK_N(CLK_N),
        .CLK_P(CLK_P),
        .DDR3_addr(DDR3_addr),
        .DDR3_ba(DDR3_ba),
        .DDR3_cas_n(DDR3_cas_n),
        .DDR3_ck_n(\^DDR3_ck_n ),
        .DDR3_ck_p(\^DDR3_ck_p ),
        .DDR3_cke(\^DDR3_cke ),
        .DDR3_cs_n(\^DDR3_cs_n ),
        .DDR3_dm(DDR3_dm),
        .DDR3_dq(DDR3_dq),
        .DDR3_dqs_n(DDR3_dqs_n),
        .DDR3_dqs_p(DDR3_dqs_p),
        .DDR3_odt(\^DDR3_odt ),
        .DDR3_ras_n(DDR3_ras_n),
        .DDR3_reset_n(DDR3_reset_n),
        .DDR3_we_n(DDR3_we_n),
        .DIP_Switches_TRI_I_tri_i(DIP_Switches_TRI_I),
        .IIC_MAIN_scl_i(IIC_MAIN_scl_i),
        .IIC_MAIN_scl_o(IIC_MAIN_scl_o),
        .IIC_MAIN_scl_t(IIC_MAIN_scl_t),
        .IIC_MAIN_sda_i(IIC_MAIN_sda_i),
        .IIC_MAIN_sda_o(IIC_MAIN_sda_o),
        .IIC_MAIN_sda_t(IIC_MAIN_sda_t),
        .IIC_MUX_RESET_B(\^IIC_MUX_RESET_B ),
        .LCD_GPIO_IO_tri_i({LCD_GPIO_IO_tri_i_8,LCD_GPIO_IO_tri_i_7,LCD_GPIO_IO_tri_i_6,LCD_GPIO_IO_tri_i_5,LCD_GPIO_IO_tri_i_4,LCD_GPIO_IO_tri_i_3,LCD_GPIO_IO_tri_i_2}),
        .LCD_GPIO_IO_tri_o({LCD_GPIO_IO_tri_o_8,LCD_GPIO_IO_tri_o_7,LCD_GPIO_IO_tri_o_6,LCD_GPIO_IO_tri_o_5,LCD_GPIO_IO_tri_o_4,LCD_GPIO_IO_tri_o_3,LCD_GPIO_IO_tri_o_2}),
        .LCD_GPIO_IO_tri_t({LCD_GPIO_IO_tri_t_8,LCD_GPIO_IO_tri_t_7,LCD_GPIO_IO_tri_t_6,LCD_GPIO_IO_tri_t_5,LCD_GPIO_IO_tri_t_4,LCD_GPIO_IO_tri_t_3,LCD_GPIO_IO_tri_t_2}),
        .LEDs_8Bits_TRI_O_tri_o(LEDs_8Bits_TRI_O),
        .Linear_Flash_Data_I(Linear_Flash_Data_I),
        .Linear_Flash_Data_O(Linear_Flash_Data_O),
        .Linear_Flash_Data_T(Linear_Flash_Data_T),
        .Linear_Flash_address(Linear_Flash_address),
        .Linear_Flash_adv_ldn(Linear_Flash_adv_ldn),
        .Linear_Flash_ce_n(\^Linear_Flash_ce_n ),
        .Linear_Flash_oe_n(\^Linear_Flash_oe_n ),
        .Linear_Flash_we_n(Linear_Flash_we_n),
        .Push_Buttons_5Bits_TRI_I_tri_i(Push_Buttons_5Bits_TRI_I),
        .RESET(RESET),
        .ROTARY_GPIO_IO_tri_i({ROTARY_GPIO_IO_tri_i_11,ROTARY_GPIO_IO_tri_i_10,ROTARY_GPIO_IO_tri_i_9}),
        .ROTARY_GPIO_IO_tri_o({ROTARY_GPIO_IO_tri_o_11,ROTARY_GPIO_IO_tri_o_10,ROTARY_GPIO_IO_tri_o_9}),
        .ROTARY_GPIO_IO_tri_t({ROTARY_GPIO_IO_tri_t_11,ROTARY_GPIO_IO_tri_t_10,ROTARY_GPIO_IO_tri_t_9}),
        .RS232_UART_1_sin(RS232_UART_1_sin),
        .RS232_UART_1_sout(RS232_UART_1_sout),
        .Soft_Ethernet_MAC_MDC_pin(Soft_Ethernet_MAC_MDC_pin),
        .Soft_Ethernet_MAC_MDIO_pin(Soft_Ethernet_MAC_MDIO_pin),
        .Soft_Ethernet_MAC_RST_n_pin(Soft_Ethernet_MAC_RST_n_pin),
        .Soft_Ethernet_MAC_gtx_clk(Soft_Ethernet_MAC_gtx_clk),
        .Soft_Ethernet_MAC_rx_clk(Soft_Ethernet_MAC_rx_clk),
        .Soft_Ethernet_MAC_rx_dv(Soft_Ethernet_MAC_rx_dv),
        .Soft_Ethernet_MAC_rx_er(Soft_Ethernet_MAC_rx_er),
        .Soft_Ethernet_MAC_rxd(Soft_Ethernet_MAC_rxd),
        .Soft_Ethernet_MAC_tx_clk(Soft_Ethernet_MAC_tx_clk),
        .Soft_Ethernet_MAC_tx_en(Soft_Ethernet_MAC_tx_en),
        .Soft_Ethernet_MAC_tx_er(Soft_Ethernet_MAC_tx_er),
        .Soft_Ethernet_MAC_txd(Soft_Ethernet_MAC_txd),
        .axi_xadc_0_VAUXN(axi_xadc_0_VAUXN),
        .axi_xadc_0_VAUXP(axi_xadc_0_VAUXP),
        .axi_xadc_8_VAUXN(axi_xadc_8_VAUXN),
        .axi_xadc_8_VAUXP(axi_xadc_8_VAUXP),
        .ddr3_1_n(ddr3_1_n),
        .ddr3_1_p(ddr3_1_p),
        .init_calib_complete(init_calib_complete),
        .sd_cd_n(sd_cd_n),
        .sd_clk(sd_clk),
        .sd_cmd_i(sd_cmd_i),
        .sd_cmd_o(sd_cmd_o),
        .sd_cmd_t(sd_cmd_t),
        .sd_dat_i({sd_dat_i_16,sd_dat_i_15,sd_dat_i_14,sd_dat_i_13}),
        .sd_dat_o({sd_dat_o_16,sd_dat_o_15,sd_dat_o_14,sd_dat_o_13}),
        .sd_dat_t({sd_dat_t_16,sd_dat_t_15,sd_dat_t_14,sd_dat_t_13}),
        .sd_wp(sd_wp),
        .sm_fan_pwm(\^sm_fan_pwm ));
IOBUF sd_cmd_iobuf_12
       (.I(sd_cmd_o),
        .IO(sd_cmd_io),
        .O(sd_cmd_i),
        .T(sd_cmd_t));
IOBUF sd_dat_iobuf_13
       (.I(sd_dat_o_13),
        .IO(sd_dat_io_13),
        .O(sd_dat_i_13),
        .T(sd_dat_t_13));
IOBUF sd_dat_iobuf_14
       (.I(sd_dat_o_14),
        .IO(sd_dat_io_14),
        .O(sd_dat_i_14),
        .T(sd_dat_t_14));
IOBUF sd_dat_iobuf_15
       (.I(sd_dat_o_15),
        .IO(sd_dat_io_15),
        .O(sd_dat_i_15),
        .T(sd_dat_t_15));
IOBUF sd_dat_iobuf_16
       (.I(sd_dat_o_16),
        .IO(sd_dat_io_16),
        .O(sd_dat_i_16),
        .T(sd_dat_t_16));
endmodule
module xVIA(.a(w),.b(w));
inout w;
endmodule
