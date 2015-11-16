------------------------------------------------------------------------------
-- @license MIT
-- @brief sort for Huffman encoding.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.global.all;

entity sort is
	port(
		-- Common.
		i_clk          : in  std_logic;
      in_rst         : in  std_logic;
		i_step         : in  t_step;
		
		-- Input port.
		i_histogram    : in t_char_cnt_array;

		-- Output port.
		o_sorted_chars : out t_char_array;
		o_sorted_cnts  : out t_char_cnt_array
	);
end entity sort;

architecture arch_sort_v1 of sort is
	
	signal r_chars : t_char_array;
	signal r_cnts  : t_char_cnt_array;
	
begin


	-- Propage large on higher index.
	bubble_sort_g: for i in 0 to 255 generate
	
		bubble_sort_p: process(i_clk)
		begin
			if rising_edge(i_clk) then
				if i_step = 256 then
					-- Load result from previous histogram block.
					r_chars(i) <= std_logic_vector(to_unsigned(i, 8));
					r_cnts(i) <= i_histogram(i);
				else
					-- One clk exchange 0 and 1, 2 and 3...
					if i_step(0) = '0' then
					
						-- Even
						if i mod 2 = 0 then

							-- Exchange if have larger count than first higher odd cell.
							if r_cnts(i) > r_cnts(i+1) then
								r_chars(i) <= r_chars(i+1);
								r_cnts(i) <= r_cnts(i+1);
							end if;
							
						-- Odd
						else
						
							-- Exchange if first lower even cell have larger count.
							if r_cnts(i-1) > r_cnts(i) then
								r_chars(i) <= r_chars(i-1);
								r_cnts(i) <= r_cnts(i-1);
							end if;
							
						end if;
						
					-- Second clk exchange 1 and 2, 3 and 4...
					else
					
						-- Even
						if i mod 2 = 0 then

							if i /= 0 then
								-- Exchange if have larger count than first lower odd cell.
								if r_cnts(i) > r_cnts(i-1) then
									r_chars(i) <= r_chars(i-1);
									r_cnts(i) <= r_cnts(i-1);
								end if;
							end if;
							
						-- Odd
						else
						
							if i /= 255 then
								-- Exchange if first higher even cell have larger count.
								if r_cnts(i+1) > r_cnts(i) then
									r_chars(i) <= r_chars(i+1);
									r_cnts(i) <= r_cnts(i+1);
								end if;
							end if;
							
						end if;
						
					end if;
				end if;
			end if;
		end process;
		
	end generate;
	
	o_sorted_chars <= r_chars;
	o_sorted_cnts  <= r_cnts;

end architecture arch_sort_v1;
