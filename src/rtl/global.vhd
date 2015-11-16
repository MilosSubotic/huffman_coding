------------------------------------------------------------------------------
-- @license MIT
-- @brief Global definitions.
------------------------------------------------------------------------------

library  ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package global is

	-- 257 steps, 256 for operations and last to send result to next stage.
	subtype t_step is unsigned(8 downto 0);

	subtype t_char is std_logic_vector(7 downto 0);
	type t_char_array is array(0 to 255) of t_char;
	
	subtype t_char_cnt is unsigned(9 downto 0);
	type t_char_cnt_array is array(0 to 255) of t_char_cnt;

end package global;
