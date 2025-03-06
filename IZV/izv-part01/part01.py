#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: Marcin Sochacki (xsocha03)

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Callable, Dict, Any

def distance(a: np.array, b: np.array) -> np.array:
    """
    Finds distance between two groups of points

    Returns:
        np.array: Distance for each pair of points
    """
    
    squared_diff = np.sum((a - b) ** 2, axis=1)
    
    # return square root of the result
    return np.sqrt(squared_diff)

def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    """
    Draws graphs for each value in 'a'
    
    Returns:
        None
    """
    
    # x values from 0 to 6π
    x = np.linspace(0, 6 * np.pi, 500)
    
    # reshape numpy array
    a_array = np.array(a).reshape(-1, 1)
    
    # calculate each point
    f_a_x = a_array**2 * np.sin(x)
    
    # plot figure
    plt.figure(figsize=(10, 5))
    
    # plot everything
    plt.plot(x, f_a_x.T)
    
    # fill under each curve
    for curve in f_a_x:
        plt.fill_between(x, curve, alpha=0.1)

    # label axes
    plt.xlabel(r'$x$', fontsize=14)
    plt.ylabel(r'$f_a(x)$', fontsize=14)
    
    # set x-axis points to fractions of π
    pi_ticks = [n * np.pi / 2 for n in range(13)]
    pi_labels = [r'$0$', r'$\frac{1}{2}\pi$', r'$\pi$', r'$\frac{3}{2}\pi$', r'$2\pi$', 
                 r'$\frac{5}{2}\pi$', r'$3\pi$', r'$\frac{7}{2}\pi$', r'$4\pi$', 
                 r'$\frac{9}{2}\pi$', r'$5\pi$', r'$\frac{11}{2}\pi$', r'$6\pi$']
    
    plt.xticks(ticks=pi_ticks, labels=pi_labels)
    
    # legend for each curve
    plt.legend([f'$\\gamma_{{{a_val}}}(x)$' for a_val in a], fontsize=12)
    
    # save or show the plot
    if save_path:
        plt.savefig(save_path)
    if show_figure:
        plt.show()

def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    """
    Generates a plot with three subplots showing two functions (f1 and f2) and their sum
    
    Returns:
        None
    """
    
    # time range
    t = np.linspace(0, 100, 1000)
    
    # define functions f1 and f2
    f1 = 0.5 * np.cos((1/50) * np.pi * t)
    f2 = 0.25 * (np.sin(np.pi * t) + np.sin((3/2) * np.pi * t))
    
    # sum f1 and f2
    f_sum = f1 + f2
    
    # create figure with subplots
    fig, axs = plt.subplots(3, 1, figsize=(8, 6), sharey=True)
    
    # plot f1
    axs[0].plot(t, f1, color='blue')
    axs[0].set_ylabel(r'$f_1(t)$')
    axs[0].set_xlim([0, 100])
    
    # plot f2
    axs[1].plot(t, f2, color='blue')
    axs[1].set_ylabel(r'$f_2(t)$')
    axs[1].set_xlim([0, 100])
    
    # plot f1(t) + f2(t) with conditional coloring
    for i in range(len(t) - 1):
        if f_sum[i] > f1[i]:
            axs[2].plot(t[i:i+2], f_sum[i:i+2], color='green')  # green if f_sum > f1
        elif t[i] < 50:
            axs[2].plot(t[i:i+2], f_sum[i:i+2], color='red')  # red if f_sum <= f1 and t < 50
        else:
            axs[2].plot(t[i:i+2], f_sum[i:i+2], color='orange')  # orange in else
    
    axs[2].set_ylabel(r'$f_1(t) + f_2(t)$')
    axs[2].set_xlim([0, 100])

    # set axis limits and ticks
    for ax in axs:
        ax.set_ylim(-0.8, 0.8)
        ax.set_yticks([0.8, 0.4, 0, -0.4, -0.8])
    
    axs[2].set_xticks([0, 25, 50, 75, 100])
    axs[0].set_xticks([])
    axs[1].set_xticks([])
    
    # save or show the figure
    if save_path:
        plt.savefig(save_path)
    if show_figure:
        plt.show()

from typing import List, Dict, Any
import requests
from bs4 import BeautifulSoup

def download_data() -> Dict[str, List[Any]]:
    """
    Downloads and parses geographic data of meteorological stations from a webpage

    Returns:
        Dict[str, List[Any]]
            A dictionary with four lists:
                - "positions": Names of the stations
                - "lats": Latitude values of each station
                - "longs": Longitude values of each station
                - "heights": Heights of each station
    """
    def extract_float(text: str) -> float:
        """
        Converts a string to a float by removing non-numeric characters
        
        Returns:
            The numeric value converted from the string
        """
        text = text.replace('°', '').replace('\'', '').replace('"', '').replace(',', '.').strip()
        return float(text)

    # URL to scrape
    url = "https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html"
    response = requests.get(url)

    # parse the HTML content
    soup = BeautifulSoup(response.content, 'html.parser')

    # find all tables on the page and select the second table
    tables = soup.find_all('table')
    stations_table = tables[1]

    # initialize dictionary to store scraped data
    data: Dict[str, List[Any]] = {
        "positions": [],
        "lats": [],
        "longs": [],
        "heights": []
    }

    # skip header row and parse each subsequent row
    rows = stations_table.find_all('tr')[1:]
    for row in rows:
        cols = row.find_all('td')
        if len(cols) == 7:  # check column count
            station_name = cols[0].text.strip()
            latitude = extract_float(cols[2].text.strip())
            longitude = extract_float(cols[4].text.strip())
            height = extract_float(cols[6].text.strip())

            data["positions"].append(station_name)
            data["lats"].append(latitude)
            data["longs"].append(longitude)
            data["heights"].append(height)

    return data
