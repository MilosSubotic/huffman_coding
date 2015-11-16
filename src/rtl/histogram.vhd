------------------------------------------------------------------------------
-- @license MIT
-- @brief Histogram for Huffman encoding.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.global.all;

entity histogram is
	port(
		-- Common.
		i_clk       : in  std_logic;
      in_rst      : in  std_logic;
		i_step      : in  t_step;
		
		-- Input port.
		i_char      : in  t_char;
		i_en        : in  std_logic;

		-- Output port.
		o_histogram : out t_char_cnt_array
	);
end entity histogram;

architecture arch_histogram_v1 of histogram is
	
	signal r_histogram : t_char_cnt_array;
	
begin


	histogram_p: process(i_clk)
	begin
		if rising_edge(i_clk) then
			if in_rst = '0' or i_step = 256 then
				r_histogram <= (others => (others => '0'));
			elsif i_en = '1' then
				r_histogram(to_integer(unsigned(i_char))) <=
					r_histogram(to_integer(unsigned(i_char))) + 1;
			end if;
		end if;
	end process;
--	histogram_g: for i in 0 to 255 generate
--		histogram_p: process(i_clk)
--		begin
--			if rising_edge(i_clk) then
--				if in_rst = '0' then
--					r_histogram(i) <= (others => '0');
--				elsif i_en = '1' then
--					if unsigned(i_char) = i then
--						r_histogram(i) <= r_histogram(i) + 1;
--					end if;
--				end if;
--			end if;
--		end process;
--	end generate;
	
	o_histogram <= r_histogram;


end architecture arch_histogram_v1;
