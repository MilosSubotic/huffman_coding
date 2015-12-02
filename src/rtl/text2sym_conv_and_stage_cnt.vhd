------------------------------------------------------------------------------
-- @license MIT
-- @brief
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

use work.global.all;
use work.reg;

entity text2sym_conv_and_stage_cnt is
	port(
		i_clk         : in  std_logic;
      in_rst        : in  std_logic;
		
		-- Input port.
		s_axis_tdata  : in  std_logic_vector(7 downto 0);
		s_axis_tvalid : in  std_logic;
		s_axis_tready : out std_logic;
		s_axis_tlast  : in  std_logic;
		
		-- Output stuff.
		o_stage       : out t_stage;
		o_pipe_en     : out std_logic;
		o_sym         : out t_sym
	);
end entity text2sym_conv_and_stage_cnt;

architecture arch_text2sym_conv_and_stage_cnt of 
		text2sym_conv_and_stage_cnt is

	type t_states is (
		NEW_CHAR,
		UPPER_NIBBLE,
		LAST_STAGE,
		UPPER_NIBBLE_LAST,
		FILL_ZEROS
	);
	signal state             : t_states;

	signal sym               : t_sym;
	signal next_sym          : t_sym;
	signal fill_sym          : t_sym;
	signal nibble_sym        : t_sym;
	signal sym_en            : std_logic;
	signal sym_fill_zeros_en : std_logic;
	signal nibble_sel        : std_logic;
	
	signal up_nib            : t_sym;
	signal next_up_nib       : t_sym;
	signal up_nib_en         : std_logic;
	
	signal pipe_en           : std_logic;
	signal next_pipe_en      : std_logic;

	signal stage             : t_stage;
	signal next_stage        : t_stage;
	signal wrap_stage        : t_stage;
	signal stage_en          : std_logic;
	signal delayed_stage     : t_stage;
	
begin

	-- Regiser and transition function.
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			state <= NEW_CHAR;
		elsif rising_edge(i_clk) then
			case state is
				when NEW_CHAR =>
					if s_axis_tvalid = '1' then
						if s_axis_tlast = '1' then
							state <= UPPER_NIBBLE_LAST;
						else
							state <= UPPER_NIBBLE;
						end if;
					end if;
				when UPPER_NIBBLE =>
					if stage = 15 then
						state <= LAST_STAGE;
					else
						state <= NEW_CHAR;
					end if;
				when LAST_STAGE =>
					state <= NEW_CHAR;
				when UPPER_NIBBLE_LAST =>
					if stage = 15 then
						state <= LAST_STAGE;
					else
						state <= FILL_ZEROS;
					end if;
				when FILL_ZEROS =>
					if stage = 15 then
						state <= LAST_STAGE;
					end if;
			end case;
		end if;
	end process;

	-- Output functions.
	with state select s_axis_tready <=
		'1' when NEW_CHAR,
		'0' when others;
	with state select sym_en <=
		s_axis_tvalid when NEW_CHAR,
		'1' when UPPER_NIBBLE,
		'1' when UPPER_NIBBLE_LAST,
		'1' when FILL_ZEROS,
		'0' when others;
	with state select sym_fill_zeros_en <=
		'1' when FILL_ZEROS,
		'0' when others;
	with state select nibble_sel <=
		'0' when NEW_CHAR, -- Select lower nibble.
		'1' when others;
	with state select up_nib_en <=
		s_axis_tvalid when NEW_CHAR,
		'0' when others;
	with state select stage_en <= 
		s_axis_tvalid when NEW_CHAR,
		'1' when others;
		
	with state select next_pipe_en <=
		s_axis_tvalid when NEW_CHAR,
		'1' when others;
		
	-- Delay enable signal for 1 clk.
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			pipe_en <= '0';
		elsif rising_edge(i_clk) then
			pipe_en <= next_pipe_en;
		end if;
	end process;
	o_pipe_en <= pipe_en;

	-- Symbol handling.
	
	nibble_sym <= 
		s_axis_tdata(3 downto 0) when nibble_sel = '0' else up_nib;
	fill_sym <= x"0" when sym_fill_zeros_en = '1' else nibble_sym;
	next_sym <= fill_sym when sym_en = '1' else sym;

	r_sym: entity reg
	generic map(
		WIDTH => 4
	)
	port map(
		i_clk  => i_clk,
		in_rst => in_rst,
		i_d    => next_sym,
		o_q    => sym
	);

	o_sym <= sym;



	next_up_nib <= 
		s_axis_tdata(7 downto 4) when up_nib_en = '1' else up_nib;
		
	r_up_nib: entity reg
	generic map(
		WIDTH => 4
	)
	port map(
		i_clk  => i_clk,
		in_rst => in_rst,
		i_d    => next_up_nib,
		o_q    => up_nib
	);



	-- Stage counter.

	wrap_stage <= "00000" when stage = 16 else stage + 1;
	next_stage <=  wrap_stage when stage_en = '1' else stage;
	
	r_stage: entity reg
	generic map(
		WIDTH    => 5
	)
	port map(
		i_clk  => i_clk,
		in_rst => in_rst,
		i_d    => next_stage,
		o_q    => stage
	);

	r_delayed_stage: entity reg
	generic map(
		WIDTH    => 5
	)
	port map(
		i_clk  => i_clk,
		in_rst => in_rst,
		i_d    => stage,
		o_q    => delayed_stage
	);
	
	o_stage <= delayed_stage;
	

end architecture arch_text2sym_conv_and_stage_cnt;
