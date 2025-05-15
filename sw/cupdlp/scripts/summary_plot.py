
# summary_plot.py
import pandas as pd
import matplotlib.pyplot as plt

def plot_summary_csv(file_path='summary.csv'):
    df = pd.read_csv(file_path)
    print("Loaded summary.csv with columns:", df.columns.tolist())

    # Example plot: Execution time of each kernel/API
    if 'Name' in df.columns and 'Duration (ns)' in df.columns:
        df_sorted = df.sort_values(by='Duration (ns)', ascending=False)
        plt.figure(figsize=(12, 6))
        plt.barh(df_sorted['Name'], df_sorted['Duration (ns)'], color='skyblue')
        plt.xlabel("Duration (ns)")
        plt.ylabel("Kernel/API Name")
        plt.title("Execution Time per Kernel/API")
        plt.tight_layout()
        plt.show()
    else:
        print("Expected columns not found in summary.csv")
