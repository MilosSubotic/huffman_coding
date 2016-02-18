------------------------------------------------------------------------------
-- @license MIT
-- @brief Building quasi-tree.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use work.global.all;


entity quasi_tree is
	port(
		i_clk        : in  std_logic;
		in_rst       : in  std_logic;
		i_stage      : in  t_stage;
		i_pipe_en    : in  std_logic;
		
		i_sort_sym   : in t_sym_array(0 to 15);
		i_sort_cnt   : in t_cnt_array(0 to 15);
		
		o_child0     : out t_node_and_cnt;
		o_child1     : out t_node_and_cnt;
		o_new_parent : out t_node_and_cnt

	);
end entity quasi_tree;

architecture arch_quasi_tree_v1 of quasi_tree is
	
	signal leaves          : t_node_and_cnt_array(0 to 15);
	
	signal parents         : t_node_and_cnt_array(0 to 15);
	
	signal parents_end     : std_logic_vector(3 downto 0);
	signal new_parent_node : t_node;
	
	signal child0          : t_node_and_cnt;
	signal child1          : t_node_and_cnt;
	signal new_parent      : t_node_and_cnt;
	
	
	
	signal next_leaves     : t_node_and_cnt_array(0 to 15);
	
	signal next_parents    : t_node_and_cnt_array(0 to 15);
	
	signal leaves_shift    : std_logic_vector(1 downto 0);
	signal parents_shift   : std_logic_vector(1 downto 0);
	
	signal leaves_shifter  : t_node_and_cnt_array(0 to 15);
	
	signal parents_shifter : t_node_and_cnt_array(0 to 15);
	
	signal push_new_parent : std_logic;
	
	signal cmp0            : std_logic;
	signal cmp1            : std_logic;
	signal cmp2            : std_logic;
	
begin

	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			leaves_node <= (others => ("11111", "11111"));
			parents_node <= (others => ("11111", "11111"));
		elsif rising_edge(i_clk) then
			leaves <= next_leaves;
			parents <= next_parents;
		end if;
	end process;
	
	next_leaves <= i_sort_sym when stage_i = 16 else leaves_shifter;
	ls: for i in 0 to 13 generate
		with leaves_shift select leaves_shifter(i) <=
			leaves(i) when "00",
			leaves(i+1) when "01",
			leaves(i+2) when others;
	end generate;
	with leaves_shift select leaves_shifter(14) <=
		leaves(14) when "00",
		leaves(15) when "01",
		("11111", "11111") when others;
	with leaves_shift select leaves_shifter(15) <=
		leaves(15) when "00",
		("11111", "11111") when "01",
		("11111", "11111") when others;

	next_parents <= (others => "11111") when stage_i = 16 else parents_pusher;
	pp: for i in 0 to 15 generate
		parents_pusher(i) <= 
			new_parent when parents_end = i and push_new_parent = '1' else
			parents_shifter;
	end generate;
	ps: for i in 0 to 13 generate
		with parents_shift select parents_shifter(i) <=
			parents(i) when "00",
			parents(i+1) when "01",
			parents(i+2) when others;
	end generate;
	with parents_shift select parents_shifter(14) <=
		parents(14) when "00",
		parents(15) when "01",
		("11111", "11111") when others;
	with parents_shift select parents_shifter(15) <=
		parents(15) when "00",
		("11111", "11111") when "01",
		("11111", "11111") when others;


	cmp0 <= '1' when leaves(0).cnt < parents(0).cnt else '1';
	cmp1 <= '1' when leaves(1).cnt < parents(0).cnt else '1';
	cmp2 <= '1' when parents(1).cnt < leaves(0).cnt else '1';
	
	
	

end architecture arch_quasi_tree_v1;
