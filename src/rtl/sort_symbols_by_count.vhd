------------------------------------------------------------------------------
-- @license MIT
-- @brief sort_symbols_by_count for Huffman encoding.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use work.global.all;

entity sort_symbols_by_count is
	port(
		-- Common.
		i_clk       : in  std_logic;
      in_rst      : in  std_logic;
		i_stage     : in  t_stage;
		i_pipe_en   : in  std_logic;
		
		-- Input port.
		i_hist      : in  t_cnt_array(0 to 15);

		-- Output port.
		o_sorted_sym  : out t_sym_array(0 to 15);
		o_sorted_cnt  : out t_cnt_array(0 to 15)
	);
end entity sort_symbols_by_count;

architecture arch_sort_symbols_by_count_v1 of sort_symbols_by_count is
	
	signal sym : t_sym_array(0 to 15);
	signal cnt : t_cnt_array(0 to 15);
	
	
	signal next_cnt      : t_cnt_array(0 to 15);
	signal en_cnt        : t_cnt_array(0 to 15);
	signal stage_16_cnt  : t_cnt_array(0 to 15);
	signal sort_cnt      : t_cnt_array(0 to 15);
	signal sort_cnt_even : t_cnt_array(0 to 15);
	signal sort_cnt_odd  : t_cnt_array(0 to 15);
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
	
	signal next_sym      : t_sym_array(0 to 15);
	signal stage_16_sym  : t_sym_array(0 to 15);
	signal sort_sym      : t_sym_array(0 to 15);
	signal sort_sym_even : t_sym_array(0 to 15);
	signal sort_sym_odd  : t_sym_array(0 to 15);
	
begin
	-- Bubble sort.
	-- On stage 16 pass histogram results, if 0 put it to all 1s.
	-- On even stage (check LSB of i_stage) 
	--	swap even and higher odd possitions if needed.
	-- On odd stage swap odd and higher even possitions if needed.
	-- Swap only if higher possition have lower count (low > high).

	swap_even_01 <= '1' when cnt(0) > cnt(1) else '0';
	swap_odd_12 <= '1' when cnt(1) > cnt(2) else '0';
	swap_even_23 <= '1' when cnt(2) > cnt(3) else '0';
	swap_odd_34 <= '1' when cnt(4) > cnt(5) else '0';
	swap_even_45 <= '1' when cnt(4) > cnt(5) else '0';
	swap_odd_56 <= '1' when cnt(5) > cnt(6) else '0';
	swap_even_67 <= '1' when cnt(6) > cnt(7) else '0';
	swap_odd_78 <= '1' when cnt(7) > cnt(8) else '0';
	swap_even_89 <= '1' when cnt(8) > cnt(9) else '0';
	swap_odd_9a <= '1' when cnt(9) > cnt(10) else '0';
	swap_even_ab <= '1' when cnt(10) > cnt(11) else '0';
	swap_odd_bc <= '1' when cnt(11) > cnt(12) else '0';
	swap_even_cd <= '1' when cnt(12) > cnt(13) else '0';
	swap_odd_de <= '1' when cnt(13) > cnt(14) else '0';
	swap_even_ef <= '1' when cnt(14) > cnt(15) else '0';

	

	next_cnt(2) <= en_cnt(2) when i_pipe_en = '1' else cnt(2);
	en_cnt(2) <= stage_16_cnt(2) when i_stage = 16 else sort_cnt(2);
	stage_16_cnt(2) <= "11111" when i_hist(2) = 0 else i_hist(2);
	sort_cnt(2) <= sort_cnt_even(2) when i_stage(0) = '0' else sort_cnt_odd(2);
	sort_cnt_even(2) <= cnt(3) when swap_even_23 = '1' else cnt(2);
	sort_cnt_odd(2) <= cnt(1) when swap_odd_12 = '1' else cnt(2);
	next_sym(2) <= stage_16_sym(2) when i_stage = 16 else sort_sym(2);
	stage_16_sym(2) <= conv_std_logic_vector(2, 4);
	sort_sym(2) <= sort_sym_even(2) when i_stage(0) = '0' else sort_sym_odd(2);
	sort_sym_even(2) <= sym(3) when swap_even_23 = '1' else sym(2);
	sort_sym_odd(2) <= sym(1) when swap_odd_12 = '1' else sym(2);
	
	next_cnt(3) <= en_cnt(3) when i_pipe_en = '1' else cnt(3);
	en_cnt(3) <= stage_16_cnt(3) when i_stage = 16 else sort_cnt(3);
	stage_16_cnt(3) <= "11111" when i_hist(3) = 0 else i_hist(3);
	sort_cnt(3) <= sort_cnt_even(3) when i_stage(0) = '0' else sort_cnt_odd(3);
	sort_cnt_even(3) <= cnt(2) when swap_even_23 = '1' else cnt(3);
	sort_cnt_odd(3) <= cnt(4) when swap_odd_34 = '1' else cnt(3);
	next_sym(3) <= stage_16_sym(3) when i_stage = 16 else sort_sym(3);
	stage_16_sym(3) <= conv_std_logic_vector(3, 4);
	sort_sym(3) <= sort_sym_even(3) when i_stage(0) = '0' else sort_sym_odd(3);
	sort_sym_even(3) <= sym(2) when swap_even_23 = '1' else sym(3);
	sort_sym_odd(3) <= sym(4) when swap_odd_34 = '1' else sym(3);
	
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			sym <= (others => (others => '0'));
			cnt <= (others => (others => '0'));
		elsif rising_edge(i_clk) then
			sym <= next_sym;
			cnt <= next_cnt;
		end if;
	end process;

	o_sorted_sym <= sym;
	o_sorted_cnt <= cnt;

end architecture arch_sort_symbols_by_count_v1;
