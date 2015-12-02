------------------------------------------------------------------------------
-- @license MIT
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

use work.global.all;

use work.sort_symbols_by_count;

entity sort_symbols_by_count_tb is
end entity sort_symbols_by_count_tb;
 
architecture arch_sort_symbols_by_count_tb of sort_symbols_by_count_tb is   

	--Inputs
	signal i_clk : std_logic := '0';
	signal in_rst : std_logic := '0';
	signal i_stage : t_stage := (others => '0');
	signal i_pipe_en : std_logic := '0';
	signal i_hist : t_cnt_array(0 to 15) := (others => (others => '0'));
		
	--Outputs
	signal o_sorted_sym : t_sym_array(0 to 15) := (others => (others => '0'));
	signal o_sorted_cnt : t_cnt_array(0 to 15) := (others => (others => '0'));

	-- Clock period definitions
	constant i_clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: entity sort_symbols_by_count
	port map (	
		i_clk => i_clk,
		in_rst => in_rst,
		i_stage => i_stage,
		i_pipe_en => i_pipe_en,
		
		i_hist => i_hist,
		
		o_sorted_sym => o_sorted_sym,
		o_sorted_cnt => o_sorted_cnt
	);

   -- Clock process definitions
   i_clk_proc: process
   begin
		i_clk <= '0';
		wait for i_clk_period/2;
		i_clk <= '1';
		wait for i_clk_period/2;
   end process;
 
	stim_proc: process
	begin
		i_stage <= conv_std_logic_vector(16, i_stage'length);
		wait for i_clk_period*2;
		in_rst <= '1';
		wait for i_clk_period;

		i_hist <= (
			conv_std_logic_vector(4, 5),
			conv_std_logic_vector(3, 5),
			conv_std_logic_vector(2, 5),
			conv_std_logic_vector(1, 5),
			others => "00000"
		);

		i_pipe_en <= '1';
		wait for i_clk_period;
		for s in 0 to 16 loop
			i_stage <= conv_std_logic_vector(s, i_stage'length);
			wait for i_clk_period;
		end loop;
		i_pipe_en <= '0';

		wait;
	end process;

end architecture arch_sort_symbols_by_count_tb;
