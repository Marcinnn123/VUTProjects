-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Marcin Sochacki (xsocha03)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       DATA_VALID : out std_logic;
       STARTBIT_EN : out std_logic;
       DATA_EN : out std_logic;
       CNT : in std_logic_vector(3 downto 0);
       CNT2 : in std_logic_vector(3 downto 0);
       IN_IDLE : out  std_logic 
    );
end entity;

architecture behavioral of UART_RX_FSM is
    type states is (IDLE, START, DATA_READ, ENDD, VALID);
    signal state : states := IDLE;
    signal flw_state : states := IDLE;
begin
    process_states : process(CLK)
    begin
	-- pokud je reset tak se vracime do stavu idle a nulujeme countrzy
        if RST = '1' then
            state <= IDLE;
	--pokud ne tak podle podminky projdeme do dalsiho stavu 
        elsif rising_edge(CLK) then
            state <= flw_state;
        end if;
    end process;
    
    state_choose : process(DIN,CNT,CNT2, state)
    begin
        flw_state <= state;
        case state is
            when IDLE =>
		--pokud je din 0 muveme projit do dalsiho stavu
                if DIN = '0' then
                    flw_state <= START;
                end if;
                
            when START => 
		-- pokud nacteme 8 cyklu a je platny startbit muzeme dal
                if CNT = "0111" then
                    if DIN = '0' then
			flw_state <= DATA_READ;
		    else
			--pokud neni tak spadky od idle
		       flw_state <= IDLE;
		    end if;	 	 
                end if;
                
            when DATA_READ =>
		-- kdyz nacteme 8 bitu muzeme dal
                if CNT2 = "1000" then
                    flw_state <= ENDD;
                end if;
                
            when ENDD =>
		-- kdyz jsme v 16 cyklu a je platny stopbit muzeme do valid
                if CNT = "1111" then
                    if DIN = '0' then
		       flw_state <= IDLE;
		    else
			flw_state <= VALID;
		    end if;
                end if;
                            
            when VALID =>
                flw_state <= IDLE;

            when others => 
                null;   
        end case;
    end process;
	--nstaveni moorovych vystupu podle navrhu
    moorovy_vystupy : process(state) begin
        case (state) is
	    -- pokud jsme v stavu IDLE	
            when IDLE =>
		-- IN_IDLE = 1 aby se v uart_rx zresetovaly country 
                IN_IDLE <= '1';
                STARTBIT_EN <= '0';
                DATA_EN <= '0';
                DATA_VALID <= '0';

            when START =>
                IN_IDLE <= '0';
		-- STARTBIT_EN = 1 aby jsme zjistili kdy se nachazime v midbitu od startbitu
                STARTBIT_EN <= '1';
                DATA_EN <= '0';
                DATA_VALID <= '0';
        
            when DATA_READ =>
                IN_IDLE <= '0';
            	STARTBIT_EN <= '0';
		-- DATA_EN = 1 aby jsme zjistili jak dlouho mame pocitat bity
            	DATA_EN <= '1';
            	DATA_VALID <= '0';
        
            when ENDD =>
                IN_IDLE <= '0';
            	STARTBIT_EN <= '0';
            	DATA_EN <= '0';
            	DATA_VALID <= '0';	
        
            when VALID =>
                IN_IDLE <= '0';
                STARTBIT_EN <= '0';
                DATA_EN <= '0';
		-- DATA_VALID = 1 aby jsme nastavili v stavu DOUT_VLD na 1
                DATA_VALID <= '1';
    
        end case;
     end process;
end architecture;
