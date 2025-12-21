import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def load_data(filepath):
    """
    Loads data from a .pkl.gz file
    """
    try:
        df = pd.read_pickle(filepath)
        return df
    except FileNotFoundError:
        print(f"File {filepath} not found.")
        return None

def analyze_alcohol_impact(df):
    """
    Analyzes the impact of alcohol levels on accidents
    """
    alcohol_levels = {
        1: 'do 0,24‰',
        2: '0,24-0,5‰',
        3: '0,5-0,8‰',
        4: '0,8-1,0‰',
        5: '1,0-1,5‰',
        6: 'nad 1,5‰',
        7: 'odmítl',
        8: 'nezjišťováno',
        0: 'ne'
    }
    df['alcohol'] = df['p11'].map(alcohol_levels)  # map the alcohol levels
    alcohol_summary = df['alcohol'].value_counts()  # count occurrences of each level
    return alcohol_summary

def analyze_time_intervals(df):
    """
    Analyzes accidents distribution across time intervals
    """
    df['hour'] = pd.to_datetime(df['p2b'], format='%H%M', errors='coerce').dt.hour
    time_bins = [0, 6, 12, 18, 24]
    labels = ['00:00-06:00', '06:00-12:00', '12:00-18:00', '18:00-24:00']
    df['time_interval'] = pd.cut(df['hour'], bins=time_bins, labels=labels, include_lowest=True)
    time_summary = df['time_interval'].value_counts().sort_index()
    return time_summary

def generate_graph_alcohol(alcohol_summary, output_file):
    """
    Generates a bar plot for accidents based on alcohol levels
    """
    fig, ax = plt.subplots(figsize=(10, 6))
    sns.barplot(x=alcohol_summary.values, y=alcohol_summary.index, palette='coolwarm', ax=ax)
    ax.set_title('Number of Accidents by Alcohol Level', fontsize=14)
    ax.set_xlabel('Number of Accidents', fontsize=12)
    ax.set_ylabel('Alcohol Level', fontsize=12)
    plt.tight_layout()
    plt.savefig(output_file, format='png')
    plt.close()

def generate_graph_time(time_summary, output_file):
    """
    Generates a bar plot for accidents by time intervals
    """
    fig, ax = plt.subplots(figsize=(10, 6))
    sns.barplot(x=time_summary.index.astype(str), y=time_summary.values, palette='viridis', ax=ax)
    ax.set_title('Number of Accidents by Time Interval', fontsize=14)
    ax.set_xlabel('Time Interval', fontsize=12)
    ax.set_ylabel('Number of Accidents', fontsize=12)
    plt.tight_layout()
    plt.savefig(output_file, format='png')
    plt.close()

def main():
    """
    Main script function
    """
    # path to the data file
    filepath = 'accidents.pkl.gz'

    # load data
    df = load_data(filepath)
    if df is None:
        return

    # analyze alcohol impact
    alcohol_summary = analyze_alcohol_impact(df)
    print("Number of accidents by alcohol level:")
    print(alcohol_summary)

    # analyze time intervals
    time_summary = analyze_time_intervals(df)
    print("\nNumber of accidents by time interval:")
    print(time_summary)

    # generate graphs
    generate_graph_alcohol(alcohol_summary, 'alcohol_graph.png')
    print("Graph for alcohol impact saved to alcohol_graph.png.")

    generate_graph_time(time_summary, 'time_graph.png')
    print("Graph for time intervals saved to time_graph.png.")

    # calculate and display statistics
    total_accidents = len(df)
    alcohol_related_accidents = total_accidents - alcohol_summary.loc['ne']
    alcohol_ratio = (alcohol_related_accidents / total_accidents) * 100
    most_common_alcohol = alcohol_summary.idxmax()
    most_common_alcohol_count = alcohol_summary.max()
    most_common_time_interval = time_summary.idxmax()
    most_common_time_count = time_summary.max()

    print("\nStatistics:")
    print(f"Total number of accidents: {total_accidents}")
    print(f"Number of alcohol-related accidents: {alcohol_related_accidents}")
    print(f"Percentage of alcohol-related accidents: {alcohol_ratio:.2f}%")
    print(f"Most common alcohol category: {most_common_alcohol} ({most_common_alcohol_count} accidents)")
    print(f"Most accidents occurred in the time interval {most_common_time_interval} ({most_common_time_count} accidents).")

if __name__ == "__main__":
    main()
