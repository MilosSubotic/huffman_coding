------------------------------------------------------------------------------
-- @license MIT
-- @brief Huffman coding example.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity huffman_coding is
	port(
		i_sw                   : in  std_logic_vector(7 downto 0);
		o_leds                 : out std_logic_vector(7 downto 0);
		on_7_segm_led_disp_a   : out std_logic;
		on_7_segm_led_disp_b   : out std_logic;
		on_7_segm_led_disp_c   : out std_logic;
		on_7_segm_led_disp_d   : out std_logic;
		on_7_segm_led_disp_e   : out std_logic;
		on_7_segm_led_disp_f   : out std_logic;
		on_7_segm_led_disp_g   : out std_logic
	);
end entity huffman_coding;

architecture arch_huffman_coding_v1 of huffman_coding is


begin

	on_7_segm_led_disp_a   <= i_sw(0);
	on_7_segm_led_disp_b   <= i_sw(1);
	on_7_segm_led_disp_c   <= i_sw(2);
	on_7_segm_led_disp_d   <= i_sw(3);
	on_7_segm_led_disp_e   <= i_sw(4);
	on_7_segm_led_disp_f   <= i_sw(5);
	on_7_segm_led_disp_g   <= i_sw(6);

	o_leds <= i_sw;
	
end architecture arch_huffman_coding_v1;
