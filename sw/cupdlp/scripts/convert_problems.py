import mps_to_mtx
from pathlib import Path
import benchmark
from typing import List

def convert_problems(problems: List[str], out_dir: str):
    for problem in problems:
        logger.debug(f'Converting: {problem}')
        decompressProblem(problem)
        moveProblem(problem)
        convert_mps_to_mtx(problem, out_dir)
        deleteProblem(problem)

def main():

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
    output_dir = f"{Path.cwd()}/mtx/";

    if not output_dir.exists():
        os.mkdir(output_dir)

    logger.info(f'Read {len(problems)} problems')
    logger.info(f'Solved Problems: {len(solved)} ')

    logger.info(f'Applying filters ...')

    problems = removeFileExtension(problems)
    solved = removeFileExtension(solved)
    problems = filterDisabled(problems, DISABLED_PROBLEMS)
    problems = filterDisabled(problems, solved)

    logger.info(f'Post Filter Problems: {len(problems)}')

    logger.info('Converting Problems ...')

    convert_problems(problems, out_dir)
    
    logger.info('Converted All Problems to MTX !')


if __name__ == "__main__":
    main()
