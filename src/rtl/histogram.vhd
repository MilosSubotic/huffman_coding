------------------------------------------------------------------------------
-- @license MIT
-- @brief Histogram for Huffman encoding.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use work.global.all;

entity histogram is
	port(
		-- Common.
		i_clk       : in  std_logic;
      in_rst      : in  std_logic;
		i_stage     : in  t_stage;
		i_pipe_en   : in  std_logic;
		
		-- Input port.
		i_sym       : in  t_sym;

		-- Output port.
		o_hist      : out t_cnt_array(0 to 15)
	);
end entity histogram;

architecture arch_histogram_v1 of histogram is
	
	signal r_hist : t_cnt_array(0 to 15);
	
begin

	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			r_hist <= (others => (others => '0'));
		elsif rising_edge(i_clk) then
			if i_pipe_en = '1' then
				if i_stage /= 16 then
					r_hist(conv_integer(i_sym)) <= r_hist(conv_integer(i_sym)) + 1;
				else
					r_hist <= (others => (others => '0'));
				end if;
			end if;
		end if;
	end process;
	o_hist <= r_hist;

end architecture arch_histogram_v1;
