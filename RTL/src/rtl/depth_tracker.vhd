------------------------------------------------------------------------------
-- @license MIT
-- @brief Tracking depth of leaves on quasi-tree.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use work.global.all;

entity depth_tracker is
	port(
		i_clk       : in  std_logic;
		in_rst      : in  std_logic;
		i_stage     : in  t_stage;
		i_pipe_en   : in  std_logic;
		
		i_sort_sym  : in t_sym_array(0 to 15);
		i_sort_cnt  : in t_cnt_array(0 to 15)

	);
end entity depth_tracker;

architecture arch_depth_tracker_v1 of depth_tracker is
	
	signal leaves_node     : t_node_array(0 to 15);
	signal leaves_cnt      : t_cnt_array(0 to 15);
	
	signal parents_node    : t_node_array(0 to 15);
	signal parents_cnt     : t_cnt_array(0 to 15);
	
	signal parents_end     : std_logic_vector(3 downto 0);
	signal new_parent_node : t_node;
	
	
	
begin


end architecture arch_depth_tracker_v1;
