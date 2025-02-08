-- This module describes DSPFP32 multiplier inference using IEEE VHDL2008 float pkg(Versal architecture)
-- It infers one DSPFP32
-- Design needs to be compiled in VHDL2008 mode 
-- Only float32 datatype is supported


library ieee; 
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.float_pkg.all;


entity fmul is 
port (clk : in std_logic;
      din1: in std_logic_vector(31 downto 0);
      din2: in std_logic_vector(31 downto 0);
      dout : out std_logic_Vector(31 downto 0));
end entity fmul;


architecture beh of fmul is


signal reg1,reg2: float32;
signal din1_sig,din2_sig,din3_sig1, dout_sig : float32;


signal din1_sig1,din1_sig2,din1_sig3,din1_sig4 : float32;
signal din2_sig1,din2_sig2,din2_sig3,din2_sig4 : float32;
signal dout_sig1,dout_sig2,dout_sig3,dout_sig4 : float32;
begin


-- Convert inputs to float32
din1_sig <= to_float(din1);
din2_sig <= to_float(din2);


--convert outputs to std_logic_vector
dout <= to_slv(dout_sig1);


--Input registers 


process(clk)
begin
if rising_edge(clk) then
din1_sig1 <= din1_sig;
din1_sig2 <= din1_sig1;
din1_sig3 <= din1_sig2;
din1_sig4 <= din1_sig3;


din3_sig1 <= din2_sig;
din2_sig2 <= din2_sig1;
din2_sig3 <= din2_sig2;
din2_sig4 <= din2_sig3;
end if;


end process;


--output registers
process(clk)
begin
if rising_edge(clk) then 
dout_sig1 <= dout_sig;
dout_sig2 <= dout_sig1;
dout_sig3 <= dout_sig2;
dout_sig4 <= dout_sig3;
end if;
end process;


-- Arithmetic operation
process(clk)
begin
if rising_edge(clk) then
dout_sig <= din1_sig2 * din2_sig2;
end if;
end process;
end architecture;

-- End of floating point multiplier
