from pyworld3 import World3
import numpy as np 

def min_max_scaling(input):
    return (input - min(input)) /(max(input) - min(input))

def main():
    #instatiate world
    world3 = World3(dt=1)
    world3.init_world3_constants()
    world3.init_world3_variables()
    world3.set_world3_table_functions()
    world3.set_world3_delay_functions()
    world3.run_world3(fast=False)

    #get variables
    PPOLX = min_max_scaling(world3.ppolx)
    POP = min_max_scaling(world3.pop)
    FPC = min_max_scaling(world3.fpc)
    IOPC = min_max_scaling(world3.iopc)
    NRFR = min_max_scaling(world3.nrfr)
    variables = [PPOLX,POP,FPC,IOPC,NRFR]
    
    #names
    names = ["PPOLX","POP" ,"FPC" ,"IOPC","NRFR"]
    for i in range(len(variables)):
        np.savetxt(f"{names[i]}.csv",variables[i],fmt="%6.5f")

if __name__ == "__main__":
    main()

