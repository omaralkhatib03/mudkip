

catch {close_sim -f} 

# Change to the directory where the .wdb file is located
cd /home/omar/Documents/year-4/FYP/cupdlp-cog/hardware/ 

# Launch the Vivado simulation with the .wdb file
open_wave_database xsim.wdb


add_wave {{/rw_2d_ram_v}}
