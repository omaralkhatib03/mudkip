import json
import os
import utils
import logging
import matplotlib.pyplot as plt
import pandas as pd

from pathlib import Path
from typing import List
from benchmark import removeFileExtension, filterDisabled

logger = utils.setup_logger(logging.DEBUG)

def read_json(file: str):
    data = None
    try:
        with open(file, 'r') as f:
            data = json.load(f)
    except Exception as e:
        raise e
    return data

def get_solutions(path: str, problems: List[str]):
    return [read_json(f'{path}/{i}.json') for i in problems]

def filter_unsolved(json_problems: List):
    return [problem for problem in json_problems if problem.get("terminationCode") == "OPTIMAL"]

def problem_to_time(problems: List[str], json_problems: List):
    return {
        problems[i]: json_problems[i]["dSolvingTime"]
        for i in range(len(json_problems))
    }

def main():
    DISABLED_PROBLEMS = ['rail4284', 'rail02', 'ns1688926']

    path = f'{Path.cwd()}/solutions'
    problems_csv = f'{Path.cwd()}/data/problems.csv'

    filenames = os.listdir(path)
    names = removeFileExtension(filenames)
    names = filterDisabled(names, DISABLED_PROBLEMS) 
    data = get_solutions(path, names)
    data = filter_unsolved(data)
    
    name_to_time = problem_to_time(names, data)

    iterations = [entry['nIter'] for entry in data]
    times = [entry['dSolvingTime'] for entry in data]

    x = range(len(data))

    fig, ax1 = plt.subplots(figsize=(12, 6))

    ax1.bar(x, iterations, width=0.4, label='Iterations', align='center')
    ax2 = ax1.twinx()
    ax2.bar([i + 0.4 for i in x], times, width=0.4, color='orange', label='Solving Time (s)', align='center')
    
    df = pd.read_csv(problems_csv)
    df = df[df['problem_name'].isin(name_to_time.keys())]
    df['solving_time'] = df['problem_name'].map(name_to_time)

    ax1.set_xlabel('Problem')
    ax1.set_ylabel('Iterations')
    ax2.set_ylabel('Solving Time (s)')
    ax1.set_xticks([i + 0.2 for i in x])
    ax1.set_xticklabels(names, rotation=45, ha='right')
    ax1.legend(loc='upper left')
    ax2.legend(loc='upper right')

    plt.title('Iterations and Solving Time per Problem')
    plt.tight_layout()

    plt.figure(figsize=(10, 6))
    plt.scatter(df['K_col'], df['solving_time'], c='blue')

    for _, row in df.iterrows():
        plt.text(row['K_col'], row['solving_time'], row['problem_name'], fontsize=9, ha='right', va='bottom')

    plt.xlabel('Number of Non-Zeros (nnz)')
    plt.ylabel('Solving Time (s)')
    plt.title('NNZ vs. Solving Time for Solved Problems')
    plt.grid(True)
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    main()

