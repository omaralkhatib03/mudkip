import argparse
import pulp
import numpy as np
import logging
from scipy.sparse import csr_matrix, vstack
from pathlib import Path
from benchmark import decompressProblem, moveProblem, deleteProblem, logger

import matplotlib.pyplot as plt

def load_mps_as_sparse_matrix(mps_path):
    logger.debug("Loading MPS ....")
    _, model = pulp.LpProblem.fromMPS(mps_path, sense=pulp.LpMinimize)
    logger.debug("Loaded MPS Successfully")

    variables = model.variables()
    constraints = model.constraints.values()

    logger.info(f'Number of Variables (n): {len(variables)}')
    logger.info(f'Number of Constraints (m): {len(constraints)}')

    var_index = {v.name: i for i, v in enumerate(variables)}
    con_index = {c.name: i for i, c in enumerate(constraints)}

    rows, cols, data = [], [], []

    for c in constraints:
        for v, coef in c.items():
            rows.append(con_index[c.name])
            cols.append(var_index[v.name])
            data.append(coef)

    A = csr_matrix((data, (rows, cols)), shape=(len(constraints), len(variables)))
    logger.info(f'Number of Non-Zero Elements: {A.nnz}')
    return A

def sort_rows(A: csr_matrix, method: str):
    logger.info(f"Sorting method: {method}")
    if method == "index":
        sorted_indices = np.arange(A.shape[0])  # Default order
    elif method == "nnz":
        row_nnz = np.diff(A.indptr)
        sorted_indices = np.argsort(row_nnz)[::-1]
    elif method == "rowsum":
        row_sums = A.sum(axis=1).A1
        sorted_indices = np.argsort(row_sums)[::-1]
    else:
        raise ValueError("Invalid sort method. Choose from: index, nnz, rowsum")
    
    A_sorted = A[sorted_indices]
    return A_sorted

def plot_sparse_heatmap(sparse_matrix, title='Nonzero Pattern of LP Constraint Matrix'):
    plt.figure(figsize=(10, 6))
    plt.spy(sparse_matrix, markersize=1)
    plt.title(title)
    plt.xlabel('Variables')
    plt.ylabel('Constraints')
    plt.tight_layout()
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Sort LP constraint matrix rows from an MPS file.")
    parser.add_argument("problem", type=str, help="Problem name (without extension)")
    parser.add_argument("--plot", action="store_true", help="Show sparse matrix plot")
    parser.add_argument("--method", choices=["index", "nnz", "rowsum"], default="index",
                        help="Sorting method: index (default), nnz (non-zero count), rowsum")
    args = parser.parse_args()

    decompressProblem(args.problem)
    moveProblem(args.problem)
    logger.info(f'Problem Name: {args.problem}')
    
    mps_file = f'{Path.cwd()}/problems/{args.problem}.mps'
    A = load_mps_as_sparse_matrix(mps_file)

    A_sorted = sort_rows(A, method=args.method)

    if args.plot:
        plot_sparse_heatmap(A_sorted, title=f"Sparse Matrix (sorted by {args.method})")

    deleteProblem(args.problem)

if __name__ == "__main__":
    main()
