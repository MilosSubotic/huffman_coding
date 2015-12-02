------------------------------------------------------------------------------
-- @license MIT
-- @brief Huffman encoder 8-bit symbols, max 16 symbols in group.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

use work.global.all;

use work.text2sym_conv_and_stage_cnt;
use work.histogram;

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

architecture arch_huffman_encoder of huffman_encoder is
	
	signal clk       : std_logic;
	signal n_rst     : std_logic;
	signal stage     : t_stage;
	signal pipe_en   : std_logic;
	
	signal sym       : t_sym;
	
	signal hist      : t_cnt_array(0 to 15);

begin
	clk <= aclk;
	n_rst <= axi_resetn;

	text2sym_conv_and_stage_cnt_i: entity text2sym_conv_and_stage_cnt
	port map(
		i_clk         => clk,
      in_rst        => n_rst,
		
		s_axis_tdata  => s_axis_tdata,
		s_axis_tvalid => s_axis_tvalid,
		s_axis_tready => s_axis_tready,
		s_axis_tlast  => s_axis_tlast,
		
		o_stage       => stage,
		o_pipe_en     => pipe_en,
		o_sym         => sym
	);
	
	histogram_i: entity histogram
	port map(
		i_clk       => clk,
      in_rst      => n_rst,
		i_stage     => stage,
		i_pipe_en   => pipe_en,
		
		i_sym       => sym,

		o_hist      => hist
	);
	


end architecture arch_huffman_encoder;
