------------------------------------------------------------------------------
-- @license MIT
-- @brief
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

use work.global.all;
use work.reg;

entity text2sym_conv_and_stage_freq is
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
		o_pipe_flush  : out std_logic;
		o_sym         : out t_sym
	);
end entity text2sym_conv_and_stage_freq;

architecture arch_text2sym_conv_and_stage_freq of 
		text2sym_conv_and_stage_freq is

	type t_states is (
		NEW_CHAR,
		UPPER_NIBBLE,
		LAST_STAGE,
		UPPER_NIBBLE_LAST,
		FLUSH
	);
	signal state             : t_states;

	signal sym               : t_sym;
	signal en                : std_logic;
	
	signal up_nib            : t_sym;
	signal next_up_nib       : t_sym;
	signal up_nib_en         : std_logic;

	signal stage             : t_stage;
	signal next_stage        : t_stage;
	signal wrap_stage        : t_stage;
	signal stage_en          : std_logic;
	
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
						state <= FLUSH;
				when FLUSH =>
					-- TODO Implement.
					state <= NEW_CHAR;
			end case;
		end if;
	end process;


	with state select en <=
		s_axis_tvalid when NEW_CHAR,
		'1' when others;
	o_pipe_en <= en;
	
	with state select s_axis_tready <=
		'1' when NEW_CHAR,
		'0' when others;
	
	with state select o_pipe_flush <=
		'1' when FLUSH,
		'0' when others;
		
	with state select sym <=
		s_axis_tdata(3 downto 0) when NEW_CHAR,
		up_nib when others;
	o_sym <= sym;

	-- Upper nibble.
	with state select up_nib_en <=
		s_axis_tvalid when NEW_CHAR,
		'0' when others;		
	next_up_nib <= s_axis_tdata(7 downto 4) when up_nib_en = '1' else up_nib;
	up_nib_reg: entity reg
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
	stage_en <= en;
	wrap_stage <= "00000" when stage = 16 else stage + 1;
	next_stage <=  wrap_stage when stage_en = '1' else stage;
	stage_reg: entity reg
	generic map(
		WIDTH    => 5
	)
	port map(
		i_clk  => i_clk,
		in_rst => in_rst,
		i_d    => next_stage,
		o_q    => stage
	);
	o_stage <= stage;
	

end architecture arch_text2sym_conv_and_stage_freq;
