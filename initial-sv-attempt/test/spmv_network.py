from mudkip_pytest import paralell_runner
import logging, random, os
import numpy as np

def get_config(seed = None) -> dict[str, str]:
    """
    Get the configuration for the build.
    """

    if seed is not None:
        random.seed(seed)

    config = {}
    config["IN_WIDTH"]          = random.randint(2, 32)
    config["ID_WIDTH"]          = random.randint(2, 16)
    config["NETWORK_WIDTH"]     = 2*random.randint(1, 16)
    config["FIFO_DEPTH"]        = 2**random.randint(2, 6)
    config["SEED"]              = seed

    return config

def main():
    '''
    Main function to run the build process.
    Target: fproduct
    '''

    # Set up logging
    logging.basicConfig(level=logging.DEBUG)
    logger = logging.getLogger(__name__)

    # Set the number of jobs and configurations
    jobs = 4
    num_configs = 10
    
    seed = os.getenv('SEED');

    # Run the parallel runner
    paralell_runner(
        get_config=get_config,
        jobs=jobs,
        num_configs=num_configs,
        target="spmv_network_tb",
        seed=seed if seed is not None else None
    )

if __name__ == "__main__":
    main()
