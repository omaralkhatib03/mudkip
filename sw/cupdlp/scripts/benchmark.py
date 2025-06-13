import os
import sys
import bz2
import logging
import shutil
import subprocess
import logging
import utils

from typing import List, Tuple
from pathlib import Path

logger = utils.setup_logger(logging.DEBUG)

def decompressProblem(problem: str) -> None:
    logger.debug(f'Decompressing {problem}')
    source = Path.cwd() / 'mittelman' / f'{problem}.mps.bz2'
    target = Path.cwd() / 'mittelman' / f'{problem}.mps'
    
    with bz2.open(source, 'rb') as f_in, open(target, 'wb') as f_out:
        shutil.copyfileobj(f_in, f_out)

def moveProblem(problem: str) -> None:
    logger.debug(f'Moving {problem}')
    source = Path.cwd() / 'mittelman' / f'{problem}.mps'
    destination = Path.cwd() / 'problems' / f'{problem}.mps'
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.move(str(source), str(destination))

def runProblem(problem: str) -> None: 
    logger.debug(f'Running {problem}')
    problem_path = f'{problem}.mps'
    cmd = f'./scripts/run.sh {problem_path}'
    subprocess.run(['bash', '-c', cmd])

def deleteProblem(problem: str) -> None:
    logger.debug(f'Deleting {problem}')
    path = Path.cwd() / 'problems' / f'{problem}.mps'
    if path.exists():
        path.unlink()   

def removeFileExtension(problems: List[str]) -> List[str]:
    return [problem.split(".")[0] for problem in problems]

def filterDisabled(problems: List[str], disabled: List[str]) -> List[str]:
    return [p for p in problems if p not in disabled]

def splitProblems(problems: List[str], stressful: List[str]) -> Tuple[List[str], List[str]]:
    normal = [p for p in problems if p not in stressful]
    return normal, stressful

def runProblems(problems: List[str]) -> None:

    for problem in problems: 
        decompressProblem(problem)
        moveProblem(problem)
        runProblem(problem)
        deleteProblem(problem)

def checkEnviroment():
    cxx = os.environ.get('CXX')
    ld_library_path = os.environ.get('LD_LIBRARY_PATH')

    if not cxx:
        logger.error("CXX environment variable is not set.")
        sys.exit(1)

    if ld_library_path:
        logger.warning("LD_LIBRARY_PATH is set, but it should be unset for this run.")
    else:
        logger.debug("LD_LIBRARY_PATH is correctly unset.")

def cleanBuild():
    try:
        logger.info("Cleaning build...")
        subprocess.run(['make', 'clean'], check=True)

        logger.info("Clean completed successfully.")
    except subprocess.CalledProcessError as e:
        logger.error(f"Clean failed: {e}")
        raise

def buildBinary():
    
    try:
        logger.info("Building binary...")
        subprocess.run(['make', '-j', '8'], check=True)

        logger.info("Build completed successfully.")
    except subprocess.CalledProcessError as e:
        logger.error(f"Build failed: {e}")
        raise

def main():
    # checkEnviroment()
    # cleanBuild()
    # buildBinary()

    # For now err on the side of caution
    DISABLED_PROBLEMS = [
        "dlr2",
        "dlr1",
        "Dual2_5000",
        "L2CTA3D",
        "bdry2",
        "L1_sixm1000obs",
    ]

    DISABLED_PROBLEMS += ["Primal2_1000", "cont11", "fhnw-binschedule1", "neos"]
    DISABLED_PROBLEMS += ["physiciansched3-3", "s82", "s100", "shs1023", "square41"]
    DISABLED_PROBLEMS += ["thk_48", "thk_63", "tpl-tub-ws1617", "set-cover-model"]
    STRESSFUL_PROBLEMS = ["rail02", "rail4284"]  # Run these last

    logger.info('Reading Problems ...')

    problems = os.listdir(f"{Path.cwd()}/mittelman")
    solved = os.listdir(f"{Path.cwd()}/solutions")

    logger.info(f'Read {len(problems)} problems')
    logger.info(f'Solved Problems: {len(solved)} ')

    logger.info(f'Applying filters ...')

    problems = removeFileExtension(problems)
    solved = removeFileExtension(solved)

    problems = filterDisabled(problems, DISABLED_PROBLEMS)
    problems = filterDisabled(problems, solved)

    logger.info(f'Post Filter Problems: {len(problems)}')

    normal_problems, stress_problems = splitProblems(problems, STRESSFUL_PROBLEMS)

    logger.info(f'Total Normal Problems: {len(normal_problems)}')
    logger.info(f'Stress Problems: {stress_problems}')

    logger.info('Running Normal Problems ...')
    runProblems(normal_problems)

    logger.info('Running Stress Problems ...')
    runProblems(stress_problems)

    logger.info('Completed All Problems !')

if __name__ == "__main__":
    main()
