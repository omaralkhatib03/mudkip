import os
import logging
from typing import Callable, Any
from concurrent.futures import ThreadPoolExecutor, as_completed
import subprocess

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

    mudkip_binary_build = f"cmake --build {work_dir}/{build_dir} --target {target} -- -j 12"
    mudkip_binary = f"{work_dir}/{build_dir}/bin/{target}"

    logger.info("Running build command: %s", mudkip_binary_build)
    os.system(mudkip_binary_build)

    logger.info("Running target: %s", mudkip_binary)
    ret = os.system(mudkip_binary)

    if ret != 0:
        logger.error("Build failed with error code %d", ret)
        return ret
    return 0

def paralell_runner(
    get_config: Callable[[int], dict[str, str]], jobs: int, num_configs: int, target: str, seed=None
) -> int:
    """
    Run the build in parallel with the given number of jobs.
    Returns the results of each build in a list
    """
    work_dir = "work"
    os.makedirs(work_dir, exist_ok=True)

    if seed is not None:
        configs = [get_config() for _ in range(num_configs)]
    else:
        configs = [get_config(seed=i) for i in range(num_configs)]

    config_ids = [i for i in range(num_configs)]
    work_list = zip(configs, config_ids)

    # run_build(work_dir, f'build_{9}', target, configs[9])

    with ThreadPoolExecutor(max_workers=jobs) as executor:
        futures = [
            executor.submit(run_build, work_dir, f"build_{config_id}", target, config)
            for config, config_id in work_list
        ]

        for future in as_completed(futures):
            try:
                result = future.result()
                if result != 0:
                    logger.error("Build failed with error code %d", result)
                    return result
            except Exception as e:
                logger.error("Build raised an exception: %s", str(e))
                return -1

    logger.info("All builds completed successfully.")

    return 0

