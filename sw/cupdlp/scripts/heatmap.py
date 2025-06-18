import pulp
import numpy as np
import matplotlib.pyplot as plt
import utils
import logging 
import sys
import seaborn as sns
import scienceplots

from scipy.sparse import csr_matrix, issparse
from benchmark import decompressProblem, moveProblem, deleteProblem, logger
from pathlib import Path
import matplotlib.patches as patches

# plt.style.use('science')
plt.style.use(['science', 'no-latex'])

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

    rows = []
    cols = []
    data = []

    for c in constraints:
        for v, coef in c.items():
            rows.append(con_index[c.name])
            cols.append(var_index[v.name])
            data.append(coef)

    A = csr_matrix((data, (rows, cols)), shape=(len(constraints), len(variables)))
    logger.info(f'Number of Non-Zero Elements: {A.nnz}')
    return A


def generate_ideal_sparse(m, n, nnz):
    assert nnz <= m * n

    avg_nnz_per_row = nnz // m
    extra = nnz % m
    row_counts = np.full(m, avg_nnz_per_row, dtype=np.int32)
    row_counts[:extra] += 1     
    rows = np.repeat(np.arange(m), row_counts)
    
    max_row_nnz = row_counts.max()
    col_template = (np.arange(max_row_nnz) * n) // max_row_nnz

    col_idx = np.concatenate([
        col_template[:count] for count in row_counts
    ])

    data = np.ones(nnz, dtype=np.float32)

    return csr_matrix((data, (rows, col_idx)), shape=(m, n))

def plot_sparse_heatmap(sparse_matrix):
    plt.figure(figsize=(10, 6))
    plt.spy(sparse_matrix, markersize=1)
    plt.title('Nonzero Pattern of LP Constraint Matrix')
    plt.xlabel('Variables')
    plt.ylabel('Constraints')
    plt.tight_layout()

def main():
    problem = sys.argv[1] if len(sys.argv) > 1 else 'afiro'
    decompressProblem(problem)
    moveProblem(problem)
    logger.info(f'Problem Name: {problem}')
    mps_file = f'{Path.cwd()}/problems/{problem}.mps'
    A = load_mps_as_sparse_matrix(mps_file)
    plot_sparse_heatmap(A)
    A = generate_ideal_sparse(A.shape[0], A.shape[1], A.nnz) 
    plot_sparse_heatmap(A)
    plt.show()
    deleteProblem(problem)

if __name__ == '__main__':
    main()

