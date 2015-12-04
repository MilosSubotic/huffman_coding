...
begin
	
	process(i_clk, in_rst)
	begin
		if in_rst = '0' then
			state <= NEW_CHAR;
		elsif rising_edge(i_clk) then
			state <= next_state;
		end if;
	end process;
	...