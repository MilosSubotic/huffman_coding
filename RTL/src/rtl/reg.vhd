-- @license MIT
-- @brief D flip-flop based register.

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity reg is
	generic(
		WIDTH    : positive := 1;
		RST_INIT : integer := 0
	);
	port(
		i_clk  : in  std_logic;
		in_rst : in  std_logic;
		i_d    : in  std_logic_vector(WIDTH-1 downto 0);
		o_q    : out std_logic_vector(WIDTH-1 downto 0)
	);
end entity reg;

architecture arch_reg of reg is
	
	signal r_q : std_logic_vector(WIDTH-1 downto 0);
	
begin
	
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			r_q <= conv_std_logic_vector(RST_INIT, WIDTH);
		elsif rising_edge(i_clk) then
			r_q <= i_d;
		end if;
	end process;
	
	o_q <= r_q;
	
end architecture arch_reg;
