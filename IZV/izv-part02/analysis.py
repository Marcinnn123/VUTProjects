from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile


def load_data(filename: str, ds: str) -> pd.DataFrame:
    """
    Load data from a ZIP archive containing HTML files encoded in cp1250

    Returns:
        pd.DataFrame: Combined data from all matching files
    """
    # open the ZIP file
    with zipfile.ZipFile(filename, 'r') as z:
        # find all matching files with the format 'I{ds}.xls'
        target_files = [name for name in z.namelist() if name.endswith(f"I{ds}.xls")]
        if not target_files:
            raise FileNotFoundError(f"No 'I{ds}.xls' files")
        
        # load data from all matching files
        dfs = []
        for target_file in target_files:
            with z.open(target_file) as f:
                # read data as a DataFrame with cp1250 encoding
                df = pd.read_html(f, encoding='cp1250')[0]
                
                df = df.dropna(axis=1, how='all')
                dfs.append(df)
        
        # combine all DataFrames into one
        if len(dfs) > 1:
            combined_df = pd.concat(dfs, ignore_index=True)
        else:
            combined_df = dfs[0]
    
    
    return combined_df

def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    """
    Clean and format data from a DataFrame

    Returns:
        pd.DataFrame: Cleaned and formatted DataFrame
    """
    cleaned_df = df.copy()
    
    # convert column p2a to a datetime
    cleaned_df['date'] = pd.to_datetime(cleaned_df['p2a'], format='%d.%m.%Y', errors='coerce')
    
    region_mapping = {
        0: "PHA", 1: "STC", 2: "JHC", 3: "PLK", 4: "ULK", 5: "HKK",
        6: "JHM", 7: "MSK", 14: "OLK", 15: "ZLK", 16: "VYS", 17: "PAK",
        18: "LBK", 19: "KVK"
    }
    cleaned_df['region'] = cleaned_df['p4a'].map(region_mapping)
    
    # remove duplicates
    cleaned_df = cleaned_df.drop_duplicates(subset='p1', keep='first')
    
    # calculate and print the size of the DataFrame
    if verbose:
        total_size_bytes = cleaned_df.memory_usage(deep=True).sum()
        total_size_mb = total_size_bytes / 10**6
        print(f"new_size={total_size_mb:.1f} MB")
    
    return cleaned_df


def plot_state(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Plot the number of accidents by road surface condition and region

    Returns:
        None
    """
    
    surface_mapping = {
        1: "na vozovce je bláto", 2: "na vozovce je bláto",
        3: "na vozovce je námraza, ujetý sníh",
        4: "povrch mokrý",
        5: "povrch suchý", 6: "povrch suchý"
    }
    
    df['surface'] = df['p16'].map(surface_mapping)    
    df = df[df['p16'].isin(surface_mapping.keys())]
    
    # aggregate and pivot data
    grouped_data = df.groupby(['region', 'surface']).size().reset_index(name='count')    
    pivot_data = grouped_data.pivot(index='region', columns='surface', values='count').fillna(0)
    
    # create subplots
    fig, axes = plt.subplots(2, 2, figsize=(12, 8))
    fig.suptitle("Počet nehod dle povrchu vozovky a regionů", fontsize=16, weight='bold')
    colors = ['blue', 'brown', 'green', 'red']

    surfaces = ["na vozovce je bláto", "na vozovce je námraza, ujetý sníh",
                "povrch mokrý", "povrch suchý"]
    
    # titles and labels
    for ax, surface, color in zip(axes.flatten(), surfaces, colors):
        pivot_data[surface].plot(kind='bar', ax=ax, color=color)
        ax.set_title(f"Stav povrchu vozovky: {surface}", fontsize=12)
        ax.set_xlabel("Kraj")
        ax.set_ylabel("Počet nehod")
        ax.set_xticklabels(pivot_data.index, rotation=45)
        ax.set_facecolor('#e0f7fa')
    
    # layout
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    
    if show_figure:
        plt.show()
    else:
        plt.close()


def plot_alcohol(df: pd.DataFrame, df_consequences: pd.DataFrame,
                 fig_location: str = None, show_figure: bool = False):
    """
    Plot the consequences of alcohol accidends

    Returns:
        None
    """
    # merge dataframes on p1
    merged_df = pd.merge(df, df_consequences, on='p1', how='inner')
    
    #  filter for alcohol accidents
    filtered_df = merged_df[(merged_df['p11'] >= 3) & (~merged_df['p2b'].isna())].copy()
    
    # determine if the injured is the driver || passenger
    filtered_df['injured'] = filtered_df['p59a'].apply(lambda x: 'Řidič' if x == 1 else 'Spolujezdec')
    
    injury_levels = {
        1: "Bez zranění",
        2: "Lehké zranění",
        3: "Těžké zranění",
        4: "Usmrcení"
    }
    filtered_df['injury_level'] = filtered_df['p59g'].map(injury_levels)
    
    # aggregate data by region and injury level and injured party
    grouped_data = filtered_df.groupby(['region', 'injury_level', 'injured']).size().reset_index(name='count')
    
    # graph plot
    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    fig.suptitle("Následky nehod pod vlivem alkoholu v jednotlivých krajích", fontsize=16, weight='bold')

    injury_order = ["Bez zranění", "Lehké zranění", "Těžké zranění", "Usmrcení"]
    for ax, injury_level in zip(axes.flatten(), injury_order):
        injury_data = grouped_data[grouped_data['injury_level'] == injury_level]
        
        pivot_data = injury_data.pivot(index='region', columns='injured', values='count').fillna(0)
        
        pivot_data.plot(kind='bar', ax=ax, stacked=False, color=['blue', 'orange'])
        ax.set_title(f"Následky nehody: {injury_level}", fontsize=12)
        ax.set_xlabel("Kraj")
        ax.set_ylabel("Počet nehod pod vlivem")
        ax.set_xticklabels(pivot_data.index, rotation=45)
        ax.legend(title="Osoba")
        ax.set_facecolor('#f0f0f0')
    
    plt.tight_layout(rect=[0, 0, 1, 0.95])

  
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    
    if show_figure:
        plt.show()
    else:
        plt.close()


def plot_type(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Plot the number of collisions

    Returns:
        None
    """
    
    crash_mapping = {
        1: "s chodcem",
        2: "s domácím zvířetem",
        3: "s lesní zvěří",
        4: "s nekolejovým vozidlem",
        5: "s pevnou překážkou",
        6: "s tramvají",
        7: "s vlakem",
        8: "s vozidlem zaparkovaným"
    }
    df['crash_type'] = df['p6'].map(crash_mapping)
    df = df[df['p6'].isin(crash_mapping.keys())]

    # filter for selected regions
    selected_regions = ["JHM", "MSK", "OLK", "ZLK"]
    df = df[df['region'].isin(selected_regions)]

    df_pivot = df.pivot_table(
        index='date', columns=['region', 'crash_type'], values='p1', aggfunc='count'
    ).fillna(0)

    # resample to monthly data
    df_monthly = df_pivot.resample('M').sum()

    # date range
    df_monthly = df_monthly.loc['2023-01-01':'2024-10-01']

    # subplotz
    fig, axes = plt.subplots(2, 2, figsize=(16, 10), sharex=True, sharey=True)
    fig.suptitle("Počet nehod podle druhu srážky v čase", fontsize=16, weight='bold')

    for ax, region in zip(axes.flatten(), selected_regions):
        region_data = df_monthly.xs(region, axis=1, level='region')
        
        region_data.plot(ax=ax, legend=False)
        ax.set_title(f"Kraj: {region}", fontsize=12)
        ax.set_ylabel("Počet nehod")
        ax.set_xlabel("Datum")
        ax.grid(True)
        ax.set_xlim(df_monthly.index.min(), df_monthly.index.max())

    # shared legend
    handles, labels = ax.get_legend_handles_labels()
    fig.legend(handles, labels, loc='center right', title="Druh nehody")
    
    plt.tight_layout(rect=[0, 0, 0.85, 0.95])

    if fig_location:
        plt.savefig(fig_location, dpi=300)
    
    if show_figure:
        plt.show()
    else:
        plt.close()

