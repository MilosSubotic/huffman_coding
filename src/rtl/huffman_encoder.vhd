------------------------------------------------------------------------------
-- @license MIT
-- @brief Huffman encoder 8-bit symbols, max 16 symbols in group.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity huffman_encoder is
	port(
		-- Clock.
		aclk          : in  std_logic;
		-- Reset.
      axi_resetn    : in  std_logic;
		
		-- Input port.
		s_axis_tdata  : in  std_logic_vector(7 downto 0);
		s_axis_tvalid : in  std_logic;
		s_axis_tready : out std_logic;
		s_axis_tlast  : in  std_logic;

		-- Output port.
		m_axis_tdata  : out std_logic_vector(7 downto 0);
		m_axis_tvalid : out std_logic;
		m_axis_tready : in  std_logic;
		m_axis_tlast  : out std_logic
	);
end entity huffman_encoder;

architecture arch_huffman_encoder_v1 of huffman_encoder is
	constant NUM_SYMBOLS_LOG_2 : natural := 4;
	
	-------
		
	signal in_en : std_logic;
	signal in_symbol : std_logic_vector(7 downto 0);
	
	constant NUM_SYMBOLS       : natural := 2**NUM_SYMBOLS_LOG_2;
	-- One bit larger to fit range [0, 2**NUM_SYMBOLS_LOG_2].
	subtype t_hyst_cnt is unsigned(NUM_SYMBOLS_LOG_2 downto 0);
	type t_hysteresis is array(0 to 255) of t_hyst_cnt;
	signal r_hysteresis : t_hysteresis;
begin

	s_axis_tready <= '1';
	in_en <= s_axis_tvalid;
	in_symbol <= s_axis_tdata;

	-- Hysteresis.
	hysteresis_p: process(aclk)
	begin
		if rising_edge(aclk) then
			if axi_resetn = '0' then
				r_hysteresis <= (others => (others => '0'));
			elsif in_en = '1' then
				r_hysteresis(to_integer(unsigned(in_symbol))) <=
					r_hysteresis(to_integer(unsigned(in_symbol))) + 1;
			end if;
		end if;
	end process;


end architecture arch_huffman_encoder_v1;
