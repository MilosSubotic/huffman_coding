------------------------------------------------------------------------------
-- @license MIT
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use std.textio.all;

use work.global.all;

use work.sort_symbols_by_count;

entity sort_symbols_by_count_tb is
end entity sort_symbols_by_count_tb;
 
architecture arch_sort_symbols_by_count_tb of sort_symbols_by_count_tb is   
	
	-- Possible values: note, warning, error, failure;
	constant assert_severity : severity_level := error; 
	
	file stdout: text open write_mode is "STD_OUTPUT";
	procedure println(s: string) is
		variable l: line;
	begin
		write(l, s);
		writeline(stdout, l);
	end procedure println;
	
	
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
		i_stage <= conv_std_logic_vector(16, t_stage'length);
		wait for i_clk_period*2;
		in_rst <= '1';
		wait for i_clk_period;

		for i in 0 to 15 loop
			i_hist(i) <= conv_std_logic_vector(20-i, t_cnt'length);
		end loop;
		i_hist(4) <= conv_std_logic_vector(0, 5);

		i_pipe_en <= '1';
		wait for i_clk_period;
		for s in 0 to 15 loop
			i_stage <= conv_std_logic_vector(s, t_stage'length);
			wait for i_clk_period;
		end loop;
		i_pipe_en <= '0';
		
		assert o_sorted_sym = (
				conv_std_logic_vector(15, t_sym'length),
				conv_std_logic_vector(14, t_sym'length),
				conv_std_logic_vector(13, t_sym'length),
				conv_std_logic_vector(12, t_sym'length),
				conv_std_logic_vector(11, t_sym'length),
				conv_std_logic_vector(10, t_sym'length),
				conv_std_logic_vector(9, t_sym'length),
				conv_std_logic_vector(8, t_sym'length),
				conv_std_logic_vector(7, t_sym'length),
				conv_std_logic_vector(6, t_sym'length),
				conv_std_logic_vector(5, t_sym'length),
				conv_std_logic_vector(3, t_sym'length),
				conv_std_logic_vector(2, t_sym'length),
				conv_std_logic_vector(1, t_sym'length),
				conv_std_logic_vector(0, t_sym'length),
				conv_std_logic_vector(4, t_sym'length)
			)
			report "o_sorted_sym wrong!"
			severity assert_severity;
		assert o_sorted_cnt = (
				conv_std_logic_vector(5, t_cnt'length),
				conv_std_logic_vector(6, t_cnt'length),
				conv_std_logic_vector(7, t_cnt'length),
				conv_std_logic_vector(8, t_cnt'length),
				conv_std_logic_vector(9, t_cnt'length),
				conv_std_logic_vector(10, t_cnt'length),
				conv_std_logic_vector(11, t_cnt'length),
				conv_std_logic_vector(12, t_cnt'length),
				conv_std_logic_vector(13, t_cnt'length),
				conv_std_logic_vector(14, t_cnt'length),
				conv_std_logic_vector(15, t_cnt'length),
				conv_std_logic_vector(17, t_cnt'length),
				conv_std_logic_vector(18, t_cnt'length),
				conv_std_logic_vector(19, t_cnt'length),
				conv_std_logic_vector(20, t_cnt'length),
				conv_std_logic_vector(31, t_cnt'length)
			)
			report "o_sorted_cnt wrong!"
			severity assert_severity;
--		assert o_sorted_cnt = ()
--			report " wrong!"
--			severity assert_severity;


		println("--------------------------------------");
		println("Testbench done!");
		println("--------------------------------------");

		wait;
	end process;

end architecture arch_sort_symbols_by_count_tb;
