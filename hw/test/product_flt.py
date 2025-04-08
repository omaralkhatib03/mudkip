from src.mudkip_pytest import paralell_runner
import logging, random

template = {
    "DATA_WIDTH" : 32,
    "E_WIDTH" : 8,
    "FRAC_WIDTH" : 23,
    "PARALLELISM" : 1,
    "DELAY" : 1,
}

def get_config(seed = None) -> dict[str, str]:
    """
    Get the configuration for the build.
    """

    if seed is not None:
        # Set the seed for reproducibility
        random.seed(seed)

    config = template.copy()
    config["DATA_WIDTH"] = random.randint(8, 64)
    config["E_WIDTH"] = random.randint(4, config["DATA_WIDTH"])
    config["FRAC_WIDTH"] = config["DATA_WIDTH"] - config["E_WIDTH"]
    config["PARALLELISM"] = random.randint(1, 64)
    config["DELAY"] = random.randint(1, 16)

    return config

def main():
    '''
    Main function to run the build process.
    Target: product
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
        target="product"
    )

if __name__ == "__main__":
    main()
