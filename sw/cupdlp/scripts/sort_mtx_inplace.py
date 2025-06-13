import argparse
import numpy as np
import logging
from pathlib import Path
from scipy.io import mmread, mmwrite
from scipy.sparse import csr_matrix

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger("mtx_sort")

def load_mtx_as_sparse_matrix(mtx_path):
    logger.info(f"Loading MTX: {mtx_path}")
    A = mmread(mtx_path).tocsr()
    logger.info(f"Loaded. Shape: {A.shape}, NNZ: {A.nnz}")
    return A

def sort_rows(A: csr_matrix, method: str):
    logger.info(f"Sorting method: {method}")
    if method == "index":
        sorted_indices = np.arange(A.shape[0])
    elif method == "nnz":
        row_nnz = np.diff(A.indptr)
        sorted_indices = np.argsort(row_nnz)[::-1]
    elif method == "rowsum":
        row_sums = A.sum(axis=1).A1
        sorted_indices = np.argsort(row_sums)[::-1]
    else:
        raise ValueError("Invalid method. Use one of: index, nnz, rowsum")

    return A[sorted_indices]

def main():
    parser = argparse.ArgumentParser(description="Sort a Matrix Market (.mtx) file in-place by row properties.")
    parser.add_argument("mtx_path", type=str, help="Path to the .mtx file")
    parser.add_argument("--method", choices=["index", "nnz", "rowsum"], default="rowsum",
                        help="Sorting method: index (default), nnz (non-zero count), rowsum")
    args = parser.parse_args()

    mtx_file = Path(args.mtx_path)
    A = load_mtx_as_sparse_matrix(mtx_file)
    A_sorted = sort_rows(A, args.method)
    mmwrite(mtx_file, A_sorted)
    logger.info("Overwrite complete.")

if __name__ == "__main__":
    main()
