import argparse
import numpy as np
from scipy.io import mmread
from scipy.sparse import csr_matrix
from pathlib import Path
from pulp import LpProblem, LpVariable, lpSum, LpMinimize, value

def load_vector(path):
    vec = mmread(path)
    if hasattr(vec, 'tocsc'):
        vec = vec.tocsc().todense()
    return np.squeeze(np.asarray(vec))

def sort_rows_by_rowsum(A):
    row_sums = A.sum(axis=1).A1
    sorted_indices = np.argsort(-row_sums)
    return A[sorted_indices], sorted_indices

def main():
    parser = argparse.ArgumentParser(description="Convert LP .mtx files to .mps format using PuLP")
    parser.add_argument("problem", type=str, help="Name of the problem directory")
    args = parser.parse_args()

    base = Path(args.problem)
    name = base.name

    mat_A = csr_matrix(mmread(base / f"{name}.mtx"))
    vec_lo = load_vector(base / f"{name}_lo.mtx")
    vec_hi = load_vector(base / f"{name}_hi.mtx")
    vec_b  = load_vector(base / f"{name}_b.mtx")
    vec_c  = load_vector(base / f"{name}_c.mtx")

    # mat_A, row_perm = sort_rows_by_rowsum(mat_A)
    # vec_b = vec_b[row_perm]

    m, n = mat_A.shape
    prob = LpProblem(name, LpMinimize)

    x = [LpVariable(f"x{i}", lowBound=vec_lo[i], upBound=vec_hi[i]) for i in range(n)]

    # Objective
    prob += lpSum(vec_c[i] * x[i] for i in range(n)), "Objective"

    # Constraints
    for i in range(m):
        row = mat_A.getrow(i)
        indices = row.indices
        data = row.data
        prob += lpSum(data[k] * x[indices[k]] for k in range(len(indices))) == vec_b[i], f"c{i}"

    # Write to MPS
    prob.writeMPS(f"{name}.mps")
    print(f"Wrote MPS file to: {name}.mps")

if __name__ == "__main__":
    main()

