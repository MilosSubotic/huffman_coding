------------------------------------------------------------------------------
-- @license MIT
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use std.textio.all;

use work.global.all;

use work.sort_syms_by_freq;

entity sort_syms_by_freq_tb is
end entity sort_syms_by_freq_tb;
 
architecture arch_sort_syms_by_freq_tb of sort_syms_by_freq_tb is   
	
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
	signal i_hist : t_freq_array(0 to 15) := (others => (others => '0'));
		
	--Outputs
	signal o_sorted_by_freq : t_sym_and_freq_array(0 to 15);

	-- Clock period definitions
	constant i_clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: entity sort_syms_by_freq
	port map (	
		i_clk => i_clk,
		in_rst => in_rst,
		i_stage => i_stage,
		i_pipe_en => i_pipe_en,
		
		i_hist => i_hist,
		
		o_sorted_by_freq => o_sorted_by_freq
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
			i_hist(i) <= conv_std_logic_vector(20-i, t_freq'length);
		end loop;
		i_hist(4) <= conv_std_logic_vector(0, 5);

		i_pipe_en <= '1';
		wait for i_clk_period;
		for s in 0 to 15 loop
			i_stage <= conv_std_logic_vector(s, t_stage'length);
			wait for i_clk_period;
		end loop;
		i_pipe_en <= '0';
		
		assert o_sorted_by_freq(0).sym = 15
			report "o_sorted_by_freq(0).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(1).sym = 14
			report "o_sorted_by_freq(1).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(2).sym = 13
			report "o_sorted_by_freq(2).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(3).sym = 12
			report "o_sorted_by_freq(3).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(4).sym = 11
			report "o_sorted_by_freq(4).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(5).sym = 10
			report "o_sorted_by_freq(5).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(6).sym = 9
			report "o_sorted_by_freq(6).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(7).sym = 8
			report "o_sorted_by_freq(7).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(8).sym = 7
			report "o_sorted_by_freq(8).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(9).sym = 6
			report "o_sorted_by_freq(9).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(10).sym = 5
			report "o_sorted_by_freq(10).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(11).sym = 3
			report "o_sorted_by_freq(11).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(12).sym = 2
			report "o_sorted_by_freq(12).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(13).sym = 1
			report "o_sorted_by_freq(13).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(14).sym = 0
			report "o_sorted_by_freq(14).sym wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(15).sym = 4
			report "o_sorted_by_freq(15).sym wrong!"
			severity assert_severity;

		assert o_sorted_by_freq(0).freq = 5
			report "o_sorted_by_freq(0).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(1).freq = 6
			report "o_sorted_by_freq(1).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(2).freq = 7
			report "o_sorted_by_freq(2).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(3).freq = 8
			report "o_sorted_by_freq(3).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(4).freq = 9
			report "o_sorted_by_freq(4).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(5).freq = 10
			report "o_sorted_by_freq(5).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(6).freq = 11
			report "o_sorted_by_freq(6).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(7).freq = 12
			report "o_sorted_by_freq(7).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(8).freq = 13
			report "o_sorted_by_freq(8).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(9).freq = 14
			report "o_sorted_by_freq(9).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(10).freq = 15
			report "o_sorted_by_freq(10).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(11).freq = 17
			report "o_sorted_by_freq(11).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(12).freq = 18
			report "o_sorted_by_freq(12).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(13).freq = 19
			report "o_sorted_by_freq(13).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(14).freq = 20
			report "o_sorted_by_freq(14).freq wrong!"
			severity assert_severity;
		assert o_sorted_by_freq(15).freq = 31
			report "o_sorted_by_freq(15).freq wrong!"
			severity assert_severity;


		println("--------------------------------------");
		println("Testbench done!");
		println("--------------------------------------");

		wait;
	end process;

end architecture arch_sort_syms_by_freq_tb;
