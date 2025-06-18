import argparse
import numpy as np
import logging
from scipy.io import mmwrite
from scipy.sparse import csr_matrix
import utils
import benchmark
from benchmark import logger
from pathlib import Path
from heatmap import load_mps_as_sparse_matrix

def sort_rows_by_sum(matrix):
    logger.debug("Computing row sums and sorting...")
    row_sums = matrix.sum(axis=1).A1
    sorted_indices = np.argsort(row_sums)
    logger.debug("Sorting complete.")
    return matrix[sorted_indices]

def save_matrix_to_mtx(matrix, output_path):
    logger.debug(f"Writing matrix to {output_path}")
    mmwrite(output_path, matrix)
    logger.info(f"Matrix written to {output_path}")

def convert_mps_to_mtx(problem: str, out_dir: str):
    problem_path = Path.cwd() / 'problems' / f'{problem}.mps'
    matrix = load_mps_as_sparse_matrix(problem_path)
    sorted_matrix = sort_rows_by_sum(matrix)
    save_matrix_to_mtx(sorted_matrix, f'{out_dir}{problem}.mtx')

def main():
    parser = argparse.ArgumentParser(
        description="Convert MPS to MTX, sorting rows of matrix A by row sum."
    )
    parser.add_argument("input_mps", help="Input .mps file")
    parser.add_argument("output_mtx", help="Output .mtx file")
    args = parser.parse_args()

    logger.info(f"Starting conversion: {args.input_mps} → {args.output_mtx}")
    A = load_mps_as_sparse_matrix(args.input_mps)
    sorted_A = sort_rows_by_sum(A)
    save_matrix_to_mtx(sorted_A, args.output_mtx)
    logger.info("Conversion completed successfully.")

if __name__ == "__main__":
    main()
