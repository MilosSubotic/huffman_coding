------------------------------------------------------------------------------
-- @license MIT
-- @brief Sorting symbols by count.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use work.global.all;

entity sort_syms_by_freq is
	port(
		i_clk            : in  std_logic;
		in_rst           : in  std_logic;
		i_stage          : in  t_stage;
		i_pipe_en        : in  std_logic;
		
		i_hist           : in  t_freq_array(0 to 15);

		o_sorted_by_freq : out t_sym_and_freq_array(0 to 15)
	);
end entity sort_syms_by_freq;

architecture arch_sort_syms_by_freq_v1 of sort_syms_by_freq is
	
	-- TODO Rename stuff.
	signal sym : t_sym_array(0 to 15);
	signal freq : t_freq_array(0 to 15);
	
	signal next_freq      : t_freq_array(0 to 15);
	signal en_freq        : t_freq_array(0 to 15);
	signal stage_16_freq  : t_freq_array(0 to 15);
	signal sort_freq      : t_freq_array(0 to 15);
	signal sort_freq_even : t_freq_array(0 to 15);
	signal sort_freq_odd  : t_freq_array(0 to 15);
	signal next_sym      : t_sym_array(0 to 15);
	signal stage_16_sym  : t_sym_array(0 to 15);
	signal sort_sym      : t_sym_array(0 to 15);
	signal sort_sym_even : t_sym_array(0 to 15);
	signal sort_sym_odd  : t_sym_array(0 to 15);
	
	signal swap_even_01  : std_logic;
	signal swap_odd_12   : std_logic;
	signal swap_even_23  : std_logic;
	signal swap_odd_34   : std_logic;
	signal swap_even_45  : std_logic;
	signal swap_odd_56   : std_logic;
	signal swap_even_67  : std_logic;
	signal swap_odd_78   : std_logic;
	signal swap_even_89  : std_logic;
	signal swap_odd_9a   : std_logic;
	signal swap_even_ab  : std_logic;
	signal swap_odd_bc   : std_logic;
	signal swap_even_cd  : std_logic;
	signal swap_odd_de   : std_logic;
	signal swap_even_ef  : std_logic;
	
	
begin
	-- Bubble sort.
	-- On stage 16 pass histogram results, if 0 put it to all 1s.
	-- On even stage (check LSB of i_stage) 
	--	swap even and higher odd possitions if needed.
	-- On odd stage swap odd and higher even possitions if needed.
	-- Swap only if higher possition have lower frequency (low > high).

	swap_even_01 <= '1' when freq(0) > freq(1) else '0';
	swap_odd_12 <= '1' when freq(1) > freq(2) else '0';
	swap_even_23 <= '1' when freq(2) > freq(3) else '0';
	swap_odd_34 <= '1' when freq(3) > freq(4) else '0';
	swap_even_45 <= '1' when freq(4) > freq(5) else '0';
	swap_odd_56 <= '1' when freq(5) > freq(6) else '0';
	swap_even_67 <= '1' when freq(6) > freq(7) else '0';
	swap_odd_78 <= '1' when freq(7) > freq(8) else '0';
	swap_even_89 <= '1' when freq(8) > freq(9) else '0';
	swap_odd_9a <= '1' when freq(9) > freq(10) else '0';
	swap_even_ab <= '1' when freq(10) > freq(11) else '0';
	swap_odd_bc <= '1' when freq(11) > freq(12) else '0';
	swap_even_cd <= '1' when freq(12) > freq(13) else '0';
	swap_odd_de <= '1' when freq(13) > freq(14) else '0';
	swap_even_ef <= '1' when freq(14) > freq(15) else '0';


	next_freq(0) <= en_freq(0) when i_pipe_en = '1' else freq(0);
	en_freq(0) <= stage_16_freq(0) when i_stage = 16 else sort_freq(0);
	stage_16_freq(0) <= "11111" when i_hist(0) = 0 else i_hist(0);
	sort_freq(0) <= sort_freq_even(0) when i_stage(0) = '0' else sort_freq_odd(0);
	sort_freq_even(0) <= freq(1) when swap_even_01 = '1' else freq(0);
	sort_freq_odd(0) <= freq(0);
	next_sym(0) <= stage_16_sym(0) when i_stage = 16 else sort_sym(0);
	stage_16_sym(0) <= conv_std_logic_vector(0, t_sym'length);
	sort_sym(0) <= sort_sym_even(0) when i_stage(0) = '0' else sort_sym_odd(0);
	sort_sym_even(0) <= sym(1) when swap_even_01 = '1' else sym(0);
	sort_sym_odd(0) <= sym(0);
	
	next_freq(1) <= en_freq(1) when i_pipe_en = '1' else freq(1);
	en_freq(1) <= stage_16_freq(1) when i_stage = 16 else sort_freq(1);
	stage_16_freq(1) <= "11111" when i_hist(1) = 0 else i_hist(1);
	sort_freq(1) <= sort_freq_even(1) when i_stage(0) = '0' else sort_freq_odd(1);
	sort_freq_even(1) <= freq(0) when swap_even_01 = '1' else freq(1);
	sort_freq_odd(1) <= freq(2) when swap_odd_12 = '1' else freq(1);
	next_sym(1) <= stage_16_sym(1) when i_stage = 16 else sort_sym(1);
	stage_16_sym(1) <= conv_std_logic_vector(1, t_sym'length);
	sort_sym(1) <= sort_sym_even(1) when i_stage(0) = '0' else sort_sym_odd(1);
	sort_sym_even(1) <= sym(0) when swap_even_01 = '1' else sym(1);
	sort_sym_odd(1) <= sym(2) when swap_odd_12 = '1' else sym(1);

	next_freq(2) <= en_freq(2) when i_pipe_en = '1' else freq(2);
	en_freq(2) <= stage_16_freq(2) when i_stage = 16 else sort_freq(2);
	stage_16_freq(2) <= "11111" when i_hist(2) = 0 else i_hist(2);
	sort_freq(2) <= sort_freq_even(2) when i_stage(0) = '0' else sort_freq_odd(2);
	sort_freq_even(2) <= freq(3) when swap_even_23 = '1' else freq(2);
	sort_freq_odd(2) <= freq(1) when swap_odd_12 = '1' else freq(2);
	next_sym(2) <= stage_16_sym(2) when i_stage = 16 else sort_sym(2);
	stage_16_sym(2) <= conv_std_logic_vector(2, t_sym'length);
	sort_sym(2) <= sort_sym_even(2) when i_stage(0) = '0' else sort_sym_odd(2);
	sort_sym_even(2) <= sym(3) when swap_even_23 = '1' else sym(2);
	sort_sym_odd(2) <= sym(1) when swap_odd_12 = '1' else sym(2);
	
	next_freq(3) <= en_freq(3) when i_pipe_en = '1' else freq(3);
	en_freq(3) <= stage_16_freq(3) when i_stage = 16 else sort_freq(3);
	stage_16_freq(3) <= "11111" when i_hist(3) = 0 else i_hist(3);
	sort_freq(3) <= sort_freq_even(3) when i_stage(0) = '0' else sort_freq_odd(3);
	sort_freq_even(3) <= freq(2) when swap_even_23 = '1' else freq(3);
	sort_freq_odd(3) <= freq(4) when swap_odd_34 = '1' else freq(3);
	next_sym(3) <= stage_16_sym(3) when i_stage = 16 else sort_sym(3);
	stage_16_sym(3) <= conv_std_logic_vector(3, t_sym'length);
	sort_sym(3) <= sort_sym_even(3) when i_stage(0) = '0' else sort_sym_odd(3);
	sort_sym_even(3) <= sym(2) when swap_even_23 = '1' else sym(3);
	sort_sym_odd(3) <= sym(4) when swap_odd_34 = '1' else sym(3);
	
	next_freq(4) <= en_freq(4) when i_pipe_en = '1' else freq(4);
	en_freq(4) <= stage_16_freq(4) when i_stage = 16 else sort_freq(4);
	stage_16_freq(4) <= "11111" when i_hist(4) = 0 else i_hist(4);
	sort_freq(4) <= sort_freq_even(4) when i_stage(0) = '0' else sort_freq_odd(4);
	sort_freq_even(4) <= freq(5) when swap_even_45 = '1' else freq(4);
	sort_freq_odd(4) <= freq(3) when swap_odd_34 = '1' else freq(4);
	next_sym(4) <= stage_16_sym(4) when i_stage = 16 else sort_sym(4);
	stage_16_sym(4) <= conv_std_logic_vector(4, t_sym'length);
	sort_sym(4) <= sort_sym_even(4) when i_stage(0) = '0' else sort_sym_odd(4);
	sort_sym_even(4) <= sym(5) when swap_even_45 = '1' else sym(4);
	sort_sym_odd(4) <= sym(3) when swap_odd_34 = '1' else sym(4);
	
	next_freq(5) <= en_freq(5) when i_pipe_en = '1' else freq(5);
	en_freq(5) <= stage_16_freq(5) when i_stage = 16 else sort_freq(5);
	stage_16_freq(5) <= "11111" when i_hist(5) = 0 else i_hist(5);
	sort_freq(5) <= sort_freq_even(5) when i_stage(0) = '0' else sort_freq_odd(5);
	sort_freq_even(5) <= freq(4) when swap_even_45 = '1' else freq(5);
	sort_freq_odd(5) <= freq(6) when swap_odd_56 = '1' else freq(5);
	next_sym(5) <= stage_16_sym(5) when i_stage = 16 else sort_sym(5);
	stage_16_sym(5) <= conv_std_logic_vector(5, t_sym'length);
	sort_sym(5) <= sort_sym_even(5) when i_stage(0) = '0' else sort_sym_odd(5);
	sort_sym_even(5) <= sym(4) when swap_even_45 = '1' else sym(5);
	sort_sym_odd(5) <= sym(6) when swap_odd_56 = '1' else sym(5);
	
	next_freq(6) <= en_freq(6) when i_pipe_en = '1' else freq(6);
	en_freq(6) <= stage_16_freq(6) when i_stage = 16 else sort_freq(6);
	stage_16_freq(6) <= "11111" when i_hist(6) = 0 else i_hist(6);
	sort_freq(6) <= sort_freq_even(6) when i_stage(0) = '0' else sort_freq_odd(6);
	sort_freq_even(6) <= freq(7) when swap_even_67 = '1' else freq(6);
	sort_freq_odd(6) <= freq(5) when swap_odd_56 = '1' else freq(6);
	next_sym(6) <= stage_16_sym(6) when i_stage = 16 else sort_sym(6);
	stage_16_sym(6) <= conv_std_logic_vector(6, t_sym'length);
	sort_sym(6) <= sort_sym_even(6) when i_stage(0) = '0' else sort_sym_odd(6);
	sort_sym_even(6) <= sym(7) when swap_even_67 = '1' else sym(6);
	sort_sym_odd(6) <= sym(5) when swap_odd_56 = '1' else sym(6);
	
	next_freq(7) <= en_freq(7) when i_pipe_en = '1' else freq(7);
	en_freq(7) <= stage_16_freq(7) when i_stage = 16 else sort_freq(7);
	stage_16_freq(7) <= "11111" when i_hist(7) = 0 else i_hist(7);
	sort_freq(7) <= sort_freq_even(7) when i_stage(0) = '0' else sort_freq_odd(7);
	sort_freq_even(7) <= freq(6) when swap_even_67 = '1' else freq(7);
	sort_freq_odd(7) <= freq(8) when swap_odd_78 = '1' else freq(7);
	next_sym(7) <= stage_16_sym(7) when i_stage = 16 else sort_sym(7);
	stage_16_sym(7) <= conv_std_logic_vector(7, t_sym'length);
	sort_sym(7) <= sort_sym_even(7) when i_stage(0) = '0' else sort_sym_odd(7);
	sort_sym_even(7) <= sym(6) when swap_even_67 = '1' else sym(7);
	sort_sym_odd(7) <= sym(8) when swap_odd_78 = '1' else sym(7);
	
	next_freq(8) <= en_freq(8) when i_pipe_en = '1' else freq(8);
	en_freq(8) <= stage_16_freq(8) when i_stage = 16 else sort_freq(8);
	stage_16_freq(8) <= "11111" when i_hist(8) = 0 else i_hist(8);
	sort_freq(8) <= sort_freq_even(8) when i_stage(0) = '0' else sort_freq_odd(8);
	sort_freq_even(8) <= freq(9) when swap_even_89 = '1' else freq(8);
	sort_freq_odd(8) <= freq(7) when swap_odd_78 = '1' else freq(8);
	next_sym(8) <= stage_16_sym(8) when i_stage = 16 else sort_sym(8);
	stage_16_sym(8) <= conv_std_logic_vector(8, t_sym'length);
	sort_sym(8) <= sort_sym_even(8) when i_stage(0) = '0' else sort_sym_odd(8);
	sort_sym_even(8) <= sym(9) when swap_even_89 = '1' else sym(8);
	sort_sym_odd(8) <= sym(7) when swap_odd_78 = '1' else sym(8);
	
	next_freq(9) <= en_freq(9) when i_pipe_en = '1' else freq(9);
	en_freq(9) <= stage_16_freq(9) when i_stage = 16 else sort_freq(9);
	stage_16_freq(9) <= "11111" when i_hist(9) = 0 else i_hist(9);
	sort_freq(9) <= sort_freq_even(9) when i_stage(0) = '0' else sort_freq_odd(9);
	sort_freq_even(9) <= freq(8) when swap_even_89 = '1' else freq(9);
	sort_freq_odd(9) <= freq(10) when swap_odd_9a = '1' else freq(9);
	next_sym(9) <= stage_16_sym(9) when i_stage = 16 else sort_sym(9);
	stage_16_sym(9) <= conv_std_logic_vector(9, t_sym'length);
	sort_sym(9) <= sort_sym_even(9) when i_stage(0) = '0' else sort_sym_odd(9);
	sort_sym_even(9) <= sym(8) when swap_even_89 = '1' else sym(9);
	sort_sym_odd(9) <= sym(10) when swap_odd_9a = '1' else sym(9);
	
	next_freq(10) <= en_freq(10) when i_pipe_en = '1' else freq(10);
	en_freq(10) <= stage_16_freq(10) when i_stage = 16 else sort_freq(10);
	stage_16_freq(10) <= "11111" when i_hist(10) = 0 else i_hist(10);
	sort_freq(10) <= sort_freq_even(10) when i_stage(0) = '0' else sort_freq_odd(10);
	sort_freq_even(10) <= freq(11) when swap_even_ab = '1' else freq(10);
	sort_freq_odd(10) <= freq(9) when swap_odd_9a = '1' else freq(10);
	next_sym(10) <= stage_16_sym(10) when i_stage = 16 else sort_sym(10);
	stage_16_sym(10) <= conv_std_logic_vector(10, t_sym'length);
	sort_sym(10) <= sort_sym_even(10) when i_stage(0) = '0' else sort_sym_odd(10);
	sort_sym_even(10) <= sym(11) when swap_even_ab = '1' else sym(10);
	sort_sym_odd(10) <= sym(9) when swap_odd_9a = '1' else sym(10);
	
	next_freq(11) <= en_freq(11) when i_pipe_en = '1' else freq(11);
	en_freq(11) <= stage_16_freq(11) when i_stage = 16 else sort_freq(11);
	stage_16_freq(11) <= "11111" when i_hist(11) = 0 else i_hist(11);
	sort_freq(11) <= sort_freq_even(11) when i_stage(0) = '0' else sort_freq_odd(11);
	sort_freq_even(11) <= freq(10) when swap_even_ab = '1' else freq(11);
	sort_freq_odd(11) <= freq(12) when swap_odd_bc = '1' else freq(11);
	next_sym(11) <= stage_16_sym(11) when i_stage = 16 else sort_sym(11);
	stage_16_sym(11) <= conv_std_logic_vector(11, t_sym'length);
	sort_sym(11) <= sort_sym_even(11) when i_stage(0) = '0' else sort_sym_odd(11);
	sort_sym_even(11) <= sym(10) when swap_even_ab = '1' else sym(11);
	sort_sym_odd(11) <= sym(12) when swap_odd_bc = '1' else sym(11);
	
	next_freq(12) <= en_freq(12) when i_pipe_en = '1' else freq(12);
	en_freq(12) <= stage_16_freq(12) when i_stage = 16 else sort_freq(12);
	stage_16_freq(12) <= "11111" when i_hist(12) = 0 else i_hist(12);
	sort_freq(12) <= sort_freq_even(12) when i_stage(0) = '0' else sort_freq_odd(12);
	sort_freq_even(12) <= freq(13) when swap_even_cd = '1' else freq(12);
	sort_freq_odd(12) <= freq(11) when swap_odd_bc = '1' else freq(12);
	next_sym(12) <= stage_16_sym(12) when i_stage = 16 else sort_sym(12);
	stage_16_sym(12) <= conv_std_logic_vector(12, t_sym'length);
	sort_sym(12) <= sort_sym_even(12) when i_stage(0) = '0' else sort_sym_odd(12);
	sort_sym_even(12) <= sym(13) when swap_even_cd = '1' else sym(12);
	sort_sym_odd(12) <= sym(11) when swap_odd_bc = '1' else sym(12);
	
	next_freq(13) <= en_freq(13) when i_pipe_en = '1' else freq(13);
	en_freq(13) <= stage_16_freq(13) when i_stage = 16 else sort_freq(13);
	stage_16_freq(13) <= "11111" when i_hist(13) = 0 else i_hist(13);
	sort_freq(13) <= sort_freq_even(13) when i_stage(0) = '0' else sort_freq_odd(13);
	sort_freq_even(13) <= freq(12) when swap_even_cd = '1' else freq(13);
	sort_freq_odd(13) <= freq(14) when swap_odd_de = '1' else freq(13);
	next_sym(13) <= stage_16_sym(13) when i_stage = 16 else sort_sym(13);
	stage_16_sym(13) <= conv_std_logic_vector(13, t_sym'length);
	sort_sym(13) <= sort_sym_even(13) when i_stage(0) = '0' else sort_sym_odd(13);
	sort_sym_even(13) <= sym(12) when swap_even_cd = '1' else sym(13);
	sort_sym_odd(13) <= sym(14) when swap_odd_de = '1' else sym(13);
	
	next_freq(14) <= en_freq(14) when i_pipe_en = '1' else freq(14);
	en_freq(14) <= stage_16_freq(14) when i_stage = 16 else sort_freq(14);
	stage_16_freq(14) <= "11111" when i_hist(14) = 0 else i_hist(14);
	sort_freq(14) <= sort_freq_even(14) when i_stage(0) = '0' else sort_freq_odd(14);
	sort_freq_even(14) <= freq(15) when swap_even_ef = '1' else freq(14);
	sort_freq_odd(14) <= freq(13) when swap_odd_de = '1' else freq(14);
	next_sym(14) <= stage_16_sym(14) when i_stage = 16 else sort_sym(14);
	stage_16_sym(14) <= conv_std_logic_vector(14, t_sym'length);
	sort_sym(14) <= sort_sym_even(14) when i_stage(0) = '0' else sort_sym_odd(14);
	sort_sym_even(14) <= sym(15) when swap_even_ef = '1' else sym(14);
	sort_sym_odd(14) <= sym(13) when swap_odd_de = '1' else sym(14);
	
	next_freq(15) <= en_freq(15) when i_pipe_en = '1' else freq(15);
	en_freq(15) <= stage_16_freq(15) when i_stage = 16 else sort_freq(15);
	stage_16_freq(15) <= "11111" when i_hist(15) = 0 else i_hist(15);
	sort_freq(15) <= sort_freq_even(15) when i_stage(0) = '0' else sort_freq_odd(15);
	sort_freq_even(15) <= freq(14) when swap_even_ef = '1' else freq(15);
	sort_freq_odd(15) <= freq(15);
	next_sym(15) <= stage_16_sym(15) when i_stage = 16 else sort_sym(15);
	stage_16_sym(15) <= conv_std_logic_vector(15, t_sym'length);
	sort_sym(15) <= sort_sym_even(15) when i_stage(0) = '0' else sort_sym_odd(15);
	sort_sym_even(15) <= sym(14) when swap_even_ef = '1' else sym(15);
	sort_sym_odd(15) <= sym(15);
	
	
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			sym <= (others => (others => '0'));
			freq <= (others => (others => '0'));
		elsif rising_edge(i_clk) then
			sym <= next_sym;
			freq <= next_freq;
		end if;
	end process;

	output: for i in 0 to 15 generate
		o_sorted_by_freq(i).sym <= sym(i);
		o_sorted_by_freq(i).freq <= freq(i);
	end generate;

end architecture arch_sort_syms_by_freq_v1;
