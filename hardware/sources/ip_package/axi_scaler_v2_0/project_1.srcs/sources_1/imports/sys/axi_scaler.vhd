--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- axi_scaler.vhd
-- keshava gopal
--------------------------------------------------------------------------------
-- history of changes:
--    12 April 2011 keshav: created
--------------------------------------------------------------------------------
-- Scaling block for video data, it drops alternative pixel and alternative line of the frame.
--  Input and output interface is AXI streaming
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

library ieee;
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

entity axi_scaler is
  generic (C_AXIS_TDATA_WIDTH : integer        := 32); --;
       --    SIMU               : boolean        := '0' ); 
  port (
        -- axi stream input interface towards video dma ip                                                                                                                         
        mm2s_prmry_reset_out_n       : in  std_logic;                                                -- asynchronous reset from video dma ip                                 
        m_axi_mm2s_aclk              : in  std_logic;                                                -- axi streaming clock                                                                                                                          
        m_axis_mm2s_tdata            : in  std_logic_vector(C_AXIS_TDATA_WIDTH-1   downto 0);        -- axi streaming data                                                   
        m_axis_mm2s_tkeep            : in  std_logic_vector(C_AXIS_TDATA_WIDTH/8-1 downto 0);        -- axi keep signals representing byte valid                             
        m_axis_mm2s_tuser            : in  std_logic;                                                -- data valid signal                                                    
        m_axis_mm2s_tvalid           : in  std_logic;                                                -- data valid signal                                                    
        m_axis_mm2s_tready           : out std_logic;                                                -- ready signal to video dma indicates IP is ready to recieve data from VDMA
        m_axis_mm2s_tlast            : in  std_logic;                                                -- indicates last data beat of stream data, end of packet              
        
        -- axi stream output interface towards video dma ip                                                                                                                         
        s_axis_s2mm_aresetn          : in  std_logic;                                                -- asynchronous reset from video dma ip                                 
        m_axi_s2mm_aclk              : in  std_logic;                                                -- axi streaming clock                                                                                                                          
        s_axis_s2mm_tdata            : out std_logic_vector(C_AXIS_TDATA_WIDTH-1   downto 0);        -- axi streaming data                                                   
        s_axis_s2mm_tkeep            : out std_logic_vector(C_AXIS_TDATA_WIDTH/8-1 downto 0);        -- axi keep signals representing byte valid                             
        s_axis_s2mm_tvalid           : out std_logic;                                                -- data valid signal                                                    
        s_axis_s2mm_tuser            : out std_logic;                                                -- data valid signal                                                    
        s_axis_s2mm_tready           : in  std_logic;                                                -- ready signal from video dma indicates video dma is ready recieve data
        s_axis_s2mm_tlast            : out std_logic                                                 -- indicates last data beat of stream data, end of packet               
         
);                                                                                                   
end axi_scaler;

architecture vhdl_rtl of axi_scaler is


  --------------------------------------------------------------------------------
  -- component declaration
  --------------------------------------------------------------------------------
  
  --------------------------------------------------------------------------------
  -- axi fifo declaration
  --------------------------------------------------------------------------------
  component fifo_generator_v9_3
  port (
        m_aclk             : in  std_logic;
        s_aclk             : in  std_logic;
        s_aresetn          : in  std_logic;
        s_axis_tuser      : in  std_logic_vector(0 downto 0);
        s_axis_tvalid      : in  std_logic;
        s_axis_tready      : out std_logic;
        s_axis_tdata       : in  std_logic_vector(31 downto 0);
        s_axis_tkeep       : in  std_logic_vector(3 downto 0);
        s_axis_tlast       : in  std_logic;
        m_axis_tuser      : out std_logic_vector(0 downto 0);
        m_axis_tvalid      : out std_logic;
        m_axis_tready      : in  std_logic;
        m_axis_tdata       : out std_logic_vector(31 downto 0);
        m_axis_tkeep       : out std_logic_vector(3 downto 0);
        m_axis_tlast       : out std_logic );
  end component;
      
  --------------------------------------------------------------------------------
  -- signal declaration
  --------------------------------------------------------------------------------
  -- Internal FIFO interface signals
  signal reset_n               : std_logic;
  signal fifo_tvalid_int       : std_logic;
  signal fifo_tlast_int        : std_logic;
  signal fifo_tvalid_out       : std_logic;
  signal drop_pix_n            : std_logic;
  signal drop_line_n           : std_logic;
--   signal fifo_tvalid_int_ff    : std_logic;
  signal m_axis_mm2s_tuser_int : std_logic_vector(0 downto 0);
  signal m_axis_mm2s_tuser_ff : std_logic;
  signal m_axis_mm2s_tvalid_ff : std_logic;
  signal m_axis_mm2s_tready_ff : std_logic;
  signal m_axis_mm2s_tdata_ff  : std_logic_vector(31 downto 0);
  signal m_axis_mm2s_tkeep_ff  : std_logic_vector(3 downto 0);
  signal m_axis_mm2s_tlast_ff  : std_logic;
  signal m_axis_mm2s_tuser_ff2 : std_logic;
  signal m_axis_mm2s_tuser_ff3 : std_logic;
  signal m_axis_mm2s_tready_ff2 : std_logic;
  signal m_axis_mm2s_tvalid_ff2 : std_logic;
  signal m_axis_mm2s_tdata_ff2  : std_logic_vector(31 downto 0);
  signal m_axis_mm2s_tkeep_ff2  : std_logic_vector(3 downto 0);
  signal m_axis_mm2s_tlast_ff2  : std_logic;


  signal m_axis_mm2s_tdata_ff_bit31  : std_logic;
  signal m_axis_mm2s_tuser_2pulse  : std_logic;
  signal m_axis_mm2s_tdata_loc  : std_logic_vector(31 downto 0);

  signal s_axis_s2mm_tuser_int : std_logic_vector(0 downto 0);
  signal s_axis_s2mm_tuser_int_ff : std_logic_vector(0 downto 0);

  
  signal dummy             : std_logic;

begin

  s_axis_s2mm_tuser <= s_axis_s2mm_tuser_int(0) and (not s_axis_s2mm_tuser_int_ff(0));
  -- Reset assignment
  reset_n <= mm2s_prmry_reset_out_n and s_axis_s2mm_aresetn;
  
 -- m_axis_mm2s_tready <= '1' when reset_n = '1' else '0';  -- Always accept data from VDMA0-MM2S
  
  --------------------------------------------------------------------------------
  -- Synchronization Flops
  --------------------------------------------------------------------------------
  sync_gen: process(m_axi_mm2s_aclk)
  begin
    if rising_edge(m_axi_mm2s_aclk) then 
      if (reset_n = '0') then   
--         fifo_tvalid_int_ff    <= '0';
        m_axis_mm2s_tvalid_ff <= '0';
        m_axis_mm2s_tuser_ff <= '0';
        m_axis_mm2s_tdata_ff  <= (others => '0');
        m_axis_mm2s_tkeep_ff  <= (others => '0');
        m_axis_mm2s_tlast_ff  <= '0';         
        m_axis_mm2s_tvalid_ff2 <= '0';
        m_axis_mm2s_tuser_ff2 <= '0';
        m_axis_mm2s_tdata_ff2  <= (others => '0');
        m_axis_mm2s_tkeep_ff2  <= (others => '0');
        m_axis_mm2s_tlast_ff2  <= '0';  
        m_axis_mm2s_tuser_ff3 <= '0'; 
        s_axis_s2mm_tuser_int_ff(0) <= '0';
      -- Synchronize FIFO inputs
      else
--         fifo_tvalid_int_ff    <= fifo_tvalid_int;
        m_axis_mm2s_tuser_ff <= m_axis_mm2s_tuser;
        m_axis_mm2s_tvalid_ff <= m_axis_mm2s_tvalid;
        m_axis_mm2s_tdata_ff  <= m_axis_mm2s_tdata;
        m_axis_mm2s_tkeep_ff  <= m_axis_mm2s_tkeep;
        m_axis_mm2s_tlast_ff  <= m_axis_mm2s_tlast;   
        m_axis_mm2s_tvalid_ff2 <= m_axis_mm2s_tvalid_ff;
        m_axis_mm2s_tuser_ff2 <= m_axis_mm2s_tuser_ff;
        m_axis_mm2s_tdata_ff2  <= m_axis_mm2s_tdata_ff;
        m_axis_mm2s_tkeep_ff2  <= m_axis_mm2s_tkeep_ff;
        m_axis_mm2s_tlast_ff2  <= m_axis_mm2s_tlast_ff; 
        m_axis_mm2s_tuser_ff3 <= m_axis_mm2s_tuser_ff2;
        m_axis_mm2s_tdata_ff_bit31 <=   m_axis_mm2s_tdata_ff2(31);

        s_axis_s2mm_tuser_int_ff(0) <= s_axis_s2mm_tuser_int(0) and s_axis_s2mm_tready;

      end if;  
    end if; -- clk
  end process sync_gen;
  
  m_axis_mm2s_tuser_2pulse <= m_axis_mm2s_tdata_ff_bit31 or m_axis_mm2s_tdata_ff2(31); --generating tuser for 2 pulse
  m_axis_mm2s_tdata_loc <= m_axis_mm2s_tuser_2pulse & m_axis_mm2s_tdata_ff2(30 downto 0); -- tuser in tdata(31)
  
  --------------------------------------------------------------------------------
  -- Drop pixel generation
  --------------------------------------------------------------------------------
  drop_pix_gen: process(m_axi_mm2s_aclk)
  begin
    if rising_edge(m_axi_mm2s_aclk) then 
      if (reset_n = '0') then   
        drop_pix_n <= '1';
      -- Drop every alternate pixel
      elsif (m_axis_mm2s_tuser_ff = '1') then
        drop_pix_n <= '0';
      elsif (m_axis_mm2s_tvalid_ff = '1') then
        drop_pix_n <= not drop_pix_n;
      elsif (m_axis_mm2s_tlast_ff2 = '1') then
        drop_pix_n <= '1';
      else
        drop_pix_n <= drop_pix_n;
      end if;  
    end if; -- clk
  end process drop_pix_gen;
      
  --------------------------------------------------------------------------------
  -- Drop line generation
  --------------------------------------------------------------------------------
  drop_line_gen: process(m_axi_mm2s_aclk)
  begin
    if rising_edge(m_axi_mm2s_aclk) then 
      if (reset_n = '0') then   
        drop_line_n <= '1';
      elsif (m_axis_mm2s_tuser_ff = '1') then
        drop_line_n <= '1';
      -- Drop every alternate line
      elsif ((m_axis_mm2s_tlast_ff2 = '1') and (m_axis_mm2s_tvalid_ff2 = '1'))then
        drop_line_n <= not drop_line_n;
      end if;  
    end if; -- clk
  end process drop_line_gen;

  --------------------------------------------------------------------------------
  -- Scale down input data by dropping alternate pixel and alternate line
  --------------------------------------------------------------------------------
  fifo_tvalid_int  <= drop_line_n and drop_pix_n and m_axis_mm2s_tvalid_ff2;
  
  m_axis_mm2s_tuser_int(0) <= (fifo_tvalid_int and m_axis_mm2s_tuser_ff3) or (fifo_tvalid_int and m_axis_mm2s_tuser_ff2);   
  --------------------------------------------------------------------------------
  -- axi Input fifo instantiation 
  --------------------------------------------------------------------------------
  inst_fifo_generator_v9_3: fifo_generator_v9_3
  port map (-- global signals
            s_aclk           => m_axi_mm2s_aclk,             -- Input interface from vdma 0
            s_aresetn        => reset_n,                     -- Input interface from vdma 0
            s_axis_tuser  => m_axis_mm2s_tuser_int,
            s_axis_tvalid    => fifo_tvalid_int,             -- Input interface from vdma 0     
            s_axis_tready    => m_axis_mm2s_tready,                       -- Input interface from vdma 0
            s_axis_tdata     => m_axis_mm2s_tdata_ff2,       -- Input interface from vdma 0  
            --s_axis_tdata     => m_axis_mm2s_tdata_loc,       
            s_axis_tkeep     => m_axis_mm2s_tkeep_ff2,       -- Input interface from vdma 0         
            s_axis_tlast     => m_axis_mm2s_tlast_ff2,       -- Input interface from vdma 0        
            m_aclk           => m_axi_s2mm_aclk,             -- Output interface to vdma 1    
            m_axis_tuser  => s_axis_s2mm_tuser_int,            
            m_axis_tvalid    => fifo_tvalid_out,             -- Output interface to vdma 1
            m_axis_tready    => s_axis_s2mm_tready,          -- Output interface to vdma 1
            m_axis_tdata     => s_axis_s2mm_tdata,           -- Output interface to vdma 1
            m_axis_tkeep     => s_axis_s2mm_tkeep,           -- Output interface to vdma 1
            m_axis_tlast     => fifo_tlast_int);             -- Output interface to vdma 1
 
  --------------------------------------------------------------------------------
  -- Tlast is asserted w.r.t Tvalid
  --------------------------------------------------------------------------------
  s_axis_s2mm_tlast   <= fifo_tlast_int and fifo_tvalid_out;
  s_axis_s2mm_tvalid  <= fifo_tvalid_out;
 
  
end vhdl_rtl;
