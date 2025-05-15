
# user_events_plot.py
import pandas as pd
import matplotlib.pyplot as plt

def plot_user_events(file_path='user_events.csv'):
    df = pd.read_csv(file_path)
    print("Loaded user_events.csv with columns:", df.columns.tolist())

    if 'Name' in df.columns and 'Duration (ns)' in df.columns:
        plt.figure(figsize=(12, 6))
        plt.barh(df['Name'], df['Duration (ns)'], color='orange')
        plt.xlabel("Duration (ns)")
        plt.ylabel("User Event")
        plt.title("User Event Timing")
        plt.tight_layout()
        plt.show()
    else:
        print("Expected columns not found in user_events.csv")
