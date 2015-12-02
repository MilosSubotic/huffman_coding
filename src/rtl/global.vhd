------------------------------------------------------------------------------
-- @license MIT
-- @brief Global definitions.
------------------------------------------------------------------------------

library  ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package global is

	subtype t_stage is std_logic_vector(4 downto 0);

	subtype t_sym is std_logic_vector(3 downto 0);
--	type t_char_array is array(0 to 255) of t_char;
	
--	subtype t_char_cnt is unsigned(9 downto 0);
--	type t_char_cnt_array is array(0 to 255) of t_char_cnt;

end package global;
