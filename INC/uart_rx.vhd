-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Marcin Sochacki (xsocha03)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
    signal cnt : std_logic_vector(3 downto 0) := "0000";
    signal cnt2 : std_logic_vector(3 downto 0) := "0000";
    signal data_en : std_logic;
    signal startbit_en :std_logic;
    signal in_idle : std_logic;
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        CNT => cnt,
        CNT2 => cnt2,
        DATA_VALID => DOUT_VLD,
	STARTBIT_EN => startbit_en, 
        DATA_EN => data_en,
	IN_IDLE=> in_idle
    );

    --DOUT <= (others => '0');
    --DOUT_VLD <= '0';

    reset_work: process (CLK)
    begin
        if rising_edge(CLK) then
		-- pokud je reset a jsme v idle nebo jsme v midbitu start bitu
	        if in_idle = '1' then
	        	cnt <= "0000";
	    	 	cnt2 <= "0000";
		elsif startbit_en = '1' and cnt = "0111" then
			cnt <= "0000";
			cnt2 <= "0000";
	        else
		-- pokud ne tak pripocitavan clk
		        cnt <= cnt + 1;
			--pokud jsme nacetli 16 hod cyklu muzeme pocitat bity
		        if cnt = "1111" then
               	   		 cnt2 <= cnt2 + 1;
                	end if;
	        end if;
        end if;
    end process;
	
    output_work : process (CLK)
    begin
	    if rising_edge(CLK) then
		-- pokud muzeme nacist data,  muzeme to zapsat na DOUT
		    if data_en = '1' and cnt = "1111" then
			   DOUT(conv_integer(cnt2)) <= DIN; 
		    end if;
	    end if;
    end process;
end architecture;
