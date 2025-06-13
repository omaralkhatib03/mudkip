import mps_to_mtx
import benchmark
import os
import scipy.io
import scipy.sparse

from pathlib import Path
from typing import List
from benchmark import * 
from mps_to_mtx import convert_mps_to_mtx
from typing import List, Tuple

def run_mtx(mtcs: List[str], ft:str = ''):
    for mtx in mtcs:
        logger.debug(f'Running Matrix: {mtx}')
        cmd = f'./scripts/run_mtx.sh {mtx}.mtx {ft}'
        subprocess.run(['bash', '-c', cmd])


def fetch_file_from_versal(remote_path: str, local_path: str):
    scp_cmd = f"scp petalinux@Versal:{remote_path} {local_path}"
    result = subprocess.run(scp_cmd, shell=True, check=True)
    return result

def hw_viable(mtx_file, max_size):
    logger.info(f'Checking: {mtx_file}') 
    mat = scipy.io.mmread(mtx_file).tocsr()
    m, n = mat.shape
    logger.info(f'm x n: {m} x {n}') 
    return (m <= max_size) and (n <= max_size)

def unsolvable_problems(problems: List[str], max_size: int, ft: str = ''):
    return [problem for problem in problems if not hw_viable(f'{Path.cwd()}/mtx{ft}/{problem}.mtx', max_size)]

def main():

    # For now err on the side of caution
    DISABLED_PROBLEMS = [
        # 'cont11',
        # 'cont1',
        # 'Dual2_5000',
        'lp_tuff', 
        'lp_ship08s', 
        'stormg2-27', 
        'lp_greenbeb', 
        'stat96v5', 
        'lp_d6cube', 
        'lp_cre_c', 
        'lp_modszk1', 
        'lp_cre_a', 
        'nemswrld', 
        'model9', 
        'lp_cre_d', 
        'lp_25fv47', 
        'lp_cycle', 
        'lp_cre_b', 
        'lp_ship04s', 
        'stormg2-8'
    ]

    # DISABLED_PROBLEMS += ["Primal2_1000", "cont11", "fhnw-binschedule1", "neos"]
    # DISABLED_PROBLEMS += ["physiciansched3-3", "s82", "s100", "shs1023", "square41"]
    # DISABLED_PROBLEMS += ["thk_48", "thk_63", "tpl-tub-ws1617", "set-cover-model"]
    # STRESSFUL_PROBLEMS = ["rail02", "rail4284"]  # Run these last
    
    logger.info('Reading Matrices ...')
    ft=''
    max_size = 200000 

    problems = os.listdir(f"{Path.cwd()}/mtx{ft}")
    output_dir = f"{Path.cwd()}/solutions/";

    os.makedirs(output_dir, exist_ok=True)

    logger.info(f'Read {len(problems)} Matrices')
    logger.info(f'Applying filters ...')

    problems = removeFileExtension(problems)

    unsolvable = unsolvable_problems(problems, max_size, ft)
    logger.info(f'Len Unsolvable: {len(unsolvable)}')

    problems = filterDisabled(problems, DISABLED_PROBLEMS)
    problems = filterDisabled(problems, unsolvable)

    logger.info(f'Post Filter Problems: {len(problems)}')

    logger.info('Running Problems ...')

    run_mtx(problems, ft)
    
    logger.info('Ran All Problems to MTX !')

    fetch_file_from_versal('/home/petalinux/cltb/solved.csv', f'{Path.cwd()}/solutions/')
    fetch_file_from_versal('/home/petalinux/cltb/unsolved.csv', f'{Path.cwd()}/solutions/')

    logger.info('Read Back CSV files')



if __name__ == '__main__':
    main()
