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
	
	subtype t_freq is std_logic_vector(4 downto 0);
	type t_freq_array is array(natural range <>) of t_freq;
	
	type t_sym_and_freq is record
		sym  : t_sym;
		freq : t_freq;
	end record;
	type t_sym_and_freq_array is array(natural range <>) of t_sym_and_freq;
	
	subtype t_node is std_logic_vector(4 downto 0);
	type t_node_array is array(natural range <>) of t_node;

	subtype t_dep is std_logic_vector(2 downto 0);
	type t_dep_array is array(natural range <>) of t_dep;
	
	type t_node_and_freq is record
		node : t_node;
		freq : t_freq;
	end record;
	type t_node_and_freq_array is array(natural range <>) of t_node_and_freq;
	
end package global;
