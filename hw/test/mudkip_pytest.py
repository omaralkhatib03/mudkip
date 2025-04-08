import os
import logging
from typing import Callable, Any
from concurrent.futures import ThreadPoolExecutor, as_completed
import subprocess, random

# Set up logging
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)


def add_verilator_args(build_command: str, config: dict[str, Any]) -> str:
    """
    Add verilator arguments to the build command.
    """
    # Simulate adding verilator arguments
    build_command += " -DOVERRIDES='"
    for param, value in config.items():
        build_command += f"-G{param}={value};"
    build_command += "'"
    return build_command

def print_overrides(config: dict[str, Any]) -> None:
    """
    Print the configuration overrides.
    """
    logger.info("Configuration Overrides:")
    for param, value in config.items():
        logger.info(f"  {param}: {value}")

def configure_mudkip(build_dir: str, config: dict[str, Any]) -> int:
    """
    Build a mudkip based on the provided configuration.
    """

    mudkip_command = f"cmake -S . -B ./{build_dir} -DCMAKE_BUILD_TYPE=Release"
    mudkip_command = add_verilator_args(mudkip_command, config)
    ret = os.system(mudkip_command)

    if ret != 0:
        logger.error("Build failed with error code %d", ret)
        return ret

    logger.info("Build succeeded with command: %s", mudkip_command)

    print_overrides(config)

    return 0

def run_build(work_dir: str, build_dir: str, target: str, config: dict[str, Any]) -> int:
    os.makedirs(work_dir + "/" + build_dir, exist_ok=True)
    ret = configure_mudkip(work_dir + "/" + build_dir, config)

    mudkip_binary_build = f"cmake --build {work_dir}/{build_dir} --target {target} --parallel"
    mudkip_binary = f"{work_dir}/{build_dir}/bin/{target}"

    logger.info("Running build command: %s", mudkip_binary_build)
    ret = os.system(mudkip_binary_build)

    logger.info("Running target: %s", mudkip_binary)
    ret = os.system(mudkip_binary)

    if ret != 0:
        logger.error("Build failed with error code %d", ret)
        return -1

    return 0

def paralell_runner(
    get_config: Callable[[int], dict[str, str]], jobs: int, num_configs: int, target: str, seed=None
) -> int:
    """
    Run the build in parallel with the given number of jobs.
    Returns the a list of passing and failing seeds.
    """
    work_dir = "work"
    os.makedirs(work_dir, exist_ok=True)

    rand_seed = random.randint(0, 2**32 - 1)

    passing_seeds = []
    failing_seeds = []

    if seed is not None:
        configs = [get_config(seed=seed) for _ in range(num_configs)]
    else:
        configs = [get_config(seed=rand_seed + i) for i in range(num_configs)]

    config_ids = [i for i in range(num_configs)]
    work_list = zip(configs, config_ids)

    with ThreadPoolExecutor(max_workers=jobs) as executor:
        futures = [
            executor.submit(run_build, work_dir, f"build_{config_id}", target, config)
            for config, config_id in work_list
        ]

        # if the future is done, check result
        # success means ret = 0, and append seed to passing_seeds
        # failure means ret != 0, and append seed to failing_seeds
        for future in as_completed(futures):
            ret = future.result()
            cfg = configs[futures.index(future)]
            if ret == 0:
                passing_seeds.append(cfg["SEED"])
            else:
                failing_seeds.append(cfg["SEED"])
                logger.error("Build failed for config ID %d", config_id)

    logger.info("Total seeds: %d", len(passing_seeds) + len(failing_seeds))
    logger.info("Passing seeds: %s", passing_seeds)
    logger.info("Failing seeds: %s", failing_seeds)

    return len(failing_seeds), len(passing_seeds)
