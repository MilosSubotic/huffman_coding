------------------------------------------------------------------------------
-- @license MIT
-- @brief Global definitions.
------------------------------------------------------------------------------

library  ieee;
use ieee.std_logic_1164.all;

package global is

	subtype t_stage is std_logic_vector(4 downto 0);

	subtype t_sym is std_logic_vector(3 downto 0);
	type t_sym_array is array(natural range <>) of t_sym;
	
	subtype t_cnt is std_logic_vector(4 downto 0);
	type t_cnt_array is array(natural range <>) of t_cnt;

end package global;
