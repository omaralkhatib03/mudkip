import mps_to_mtx
import benchmark
import os
import sys
import numpy as np
import scipy.io
import scipy.sparse
from pathlib import Path
from typing import List
from benchmark import logger 
from mps_to_mtx import convert_mps_to_mtx

def pad_csr_matrix(csr, target_rows=4096):
    if csr.shape[0] >= target_rows:
        return csr[:target_rows, :]
    else:
        diff = target_rows - csr.shape[0]
        extra = scipy.sparse.csr_matrix((diff, csr.shape[1]))
        return scipy.sparse.vstack([csr, extra]).tocsr()

def write_vector(filename, arr):
    with open(filename, 'w') as f:
        for v in arr:
            f.write(str(v) + '\n')

def convert_problem(problems: List[str], out_dir: str):
    logger.debug(f'Obtaining AIE Data for: {problem}')

def main(problem):
    os.makedirs(f'{Path.cwd()}/data/', exist_ok=True)
    mtx_file = f'{Path.cwd()}/mtx/{problem}.mtx'
    mat = scipy.io.mmread(mtx_file).tocsr()

    mat = pad_csr_matrix(mat, target_rows=1024)

    r_ptr   = mat.indptr
    c_idx   = mat.indices
    vals    = mat.data
    

    write_vector(f'{Path.cwd()}/data/r_ptr.txt', r_ptr)
    write_vector(f'{Path.cwd()}/data/c_idx.txt', c_idx)
    write_vector(f'{Path.cwd()}/data/vals.txt', vals)

    logger.info(f"Files written: data/r_ptr.txt, data/c_idx.txt, data/vals.txt")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        logger.error("Usage: python generate_spmv_inputs.py <problem>")
        exit(1)
    main(sys.argv[1])


