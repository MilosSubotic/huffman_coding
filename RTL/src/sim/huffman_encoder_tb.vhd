------------------------------------------------------------------------------
-- @license MIT
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use std.textio.all;

use work.global.all;

use work.huffman_encoder;

entity huffman_encoder_tb is
end entity huffman_encoder_tb;
 
architecture arch_huffman_encoder_tb of huffman_encoder_tb is

	-- Possible values: note, warning, error, failure;
	constant assert_severity : severity_level := error; 
		
	file stdout: text open write_mode is "STD_OUTPUT";
	procedure println(s: string) is
		variable l: line;
	begin
		write(l, s);
		writeline(stdout, l);
	end procedure println;
	
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
   uut: entity huffman_encoder
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
   clk_p: process
   begin
		aclk <= '0';
		wait for aclk_period/2;
		aclk <= '1';
		wait for aclk_period/2;
   end process;
 

	-- Stimulus process
	axi_master_p: process
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
	
		
		wait for aclk_period*10;

		println("--------------------------------------");
		println("Testbench done!");
		println("--------------------------------------");

		wait;
	end process;


end architecture arch_huffman_encoder_tb;
