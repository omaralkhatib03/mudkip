from mudkip_pytest import paralell_runner
import logging, random

def get_config(seed = None) -> dict[str, str]:
    """
    Get the configuration for the build.
    """

    if seed is not None:
        # Set the seed for reproducibility
        random.seed(seed)

    config = {}
    config["IN_WIDTH"] = random.randint(1, 64)
    config["ID_WIDTH"] = random.randint(1, 16)
    config["LOCATION"] = random.randint(1, 64)
    config["PARALLELISM"] = random.randint(1, 64)
    config["SEED"] = seed

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

    # Run the parallel runner
    paralell_runner(
        get_config=get_config,
        jobs=jobs,
        num_configs=num_configs,
        target="spmv_network_op_tb",
    )

if __name__ == "__main__":
    main()
