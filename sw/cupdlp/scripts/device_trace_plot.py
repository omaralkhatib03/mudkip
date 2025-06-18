# device_trace_plot.py
import matplotlib.pyplot as plt
import pandas as pd
import re

def parse_vtf_events(file_path='device_trace_0'):
    with open(file_path, 'r') as f:
        content = f.read()

    # Extract EVENTS section
    events_section = content.split("EVENTS")[-1].strip()
    lines = [line.strip() for line in events_section.splitlines() if line.strip()]
    
    # Parse lines into DataFrame
    event_data = []
    for line in lines:
        parts = line.split(',')
        if len(parts) < 6:
            continue
        event_id, parent_id, timestamp, _, event_type, *rest = parts
        resource_id = rest[-1]
        event_data.append({
            'EventID': int(event_id),
            'ParentID': int(parent_id),
            'Timestamp': float(timestamp),
            'EventType': event_type.strip(),
            'ResourceID': int(resource_id),
        })

    df = pd.DataFrame(event_data)
    return df

def plot_vtf_trace(df):
    plt.figure(figsize=(12, 6))
    colors = {
        'KERNEL': 'skyblue',
        'KERNEL_READ': 'orange',
        'KERNEL_WRITE': 'lightgreen',
        'DMA_READ': 'red',
        'DMA_WRITE': 'purple',
    }

    y_labels = []
    y_ticks = []
    y = 0
    for etype in df['EventType'].unique():
        subset = df[df['EventType'] == etype]
        plt.scatter(subset['Timestamp'], [y]*len(subset), label=etype, color=colors.get(etype, 'gray'))
        y_labels.append(etype)
        y_ticks.append(y)
        y += 1

    plt.yticks(y_ticks, y_labels)
    plt.xlabel("Time (ms)")
    plt.title("VTF Device Trace Timeline")
    plt.legend()
    plt.tight_layout()
    plt.show()

def main():
    df = parse_vtf_events('device_trace_0')
    print(f"Parsed {len(df)} events.")
    plot_vtf_trace(df)

if __name__ == '__main__':
    main()
