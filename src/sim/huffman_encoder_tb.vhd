------------------------------------------------------------------------------
-- @license MIT
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
 
entity huffman_encoder_tb is
end entity huffman_encoder_tb;
 
architecture arch_huffman_encoder_tb of huffman_encoder_tb is
 
    -- Component Declaration for the Unit Under Test (UUT)
	component huffman_encoder is
		port(
			-- Clock.
			aclk          : in  std_logic;
			-- Reset.
			axi_resetn    : in  std_logic;
			
			-- Input port.
			s_axis_tdata  : in  std_logic_vector(7 downto 0);
			s_axis_tvalid : in  std_logic;
			s_axis_tready : out std_logic;
			s_axis_tlast  : in  std_logic;

			-- Output port.
			m_axis_tdata  : out std_logic_vector(7 downto 0);
			m_axis_tvalid : out std_logic;
			m_axis_tready : in  std_logic;
			m_axis_tlast  : out std_logic
		);
	end component huffman_encoder;
    

	--Inputs
	signal aclk : std_logic := '0';
	signal axi_resetn : std_logic := '0';
	signal s_axis_tdata : std_logic_vector(7 downto 0) := (others => '0');
	signal s_axis_tvalid : std_logic := '0';
	signal s_axis_tlast : std_logic := '0';
	signal m_axis_tready : std_logic := '0';

	--Outputs
	signal s_axis_tready : std_logic;
	signal m_axis_tdata : std_logic_vector(7 downto 0);
	signal m_axis_tvalid : std_logic;
	signal m_axis_tlast : std_logic;

	-- Clock period definitions
	constant aclk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: huffman_encoder
	port map (
		aclk => aclk,
		axi_resetn => axi_resetn,
		s_axis_tdata => s_axis_tdata,
		s_axis_tvalid => s_axis_tvalid,
		s_axis_tready => s_axis_tready,
		s_axis_tlast => s_axis_tlast,
		m_axis_tdata => m_axis_tdata,
		m_axis_tvalid => m_axis_tvalid,
		m_axis_tready => m_axis_tready,
		m_axis_tlast => m_axis_tlast
	);

   -- Clock process definitions
   aclk_proc: process
   begin
		aclk <= '0';
		wait for aclk_period/2;
		aclk <= '1';
		wait for aclk_period/2;
   end process;
 

	-- Stimulus process
	stim_proc: process
		variable symbol_idx : natural;
		type t_symbol_batch is array(0 to 15) of std_logic_vector(7 downto 0);
		constant symbol_batch : t_symbol_batch := (
			x"62",
			x"61",
			x"62",
			x"61",
			x"64",
			x"65",
			x"64",
			x"61",
			x"64",
			x"65",
			x"61",
			x"64",
			x"62",
			x"65",
			x"65",
			x"66"
		);
	begin
		wait for aclk_period*2;
		axi_resetn <= '1';
		wait for aclk_period;

		symbol_idx := 0;
		while symbol_idx <= 15 loop
			s_axis_tdata <= symbol_batch(symbol_idx);
			s_axis_tvalid <= '1';
			if symbol_idx = 15 then
				s_axis_tlast <= '1';
			else
				s_axis_tlast <= '0';
			end if;
		
			-- If slave is ready then symbols is considered sent,
			-- and we proceed to next symbol.
			if s_axis_tready = '1' then
				symbol_idx := symbol_idx + 1;
			end if;
			
			wait for aclk_period;
			
			-- Make pause for 3 clock periods.
			if symbol_idx mod 8 = 5 then
				s_axis_tvalid <= '0';
				wait for aclk_period*3;
			end if;
		end loop;
		s_axis_tvalid <= '0';
		s_axis_tlast <= '0';

		wait;
	end process;

end architecture arch_huffman_encoder_tb;
