import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily as ctx
import numpy as np
from shapely.geometry import Point,MultiPoint
from sklearn.cluster import DBSCAN

def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame) -> geopandas.GeoDataFrame:
    """
    Combines accident data and location data to GeoDataFrame
    """
    # merge the two DataFrames
    df = pd.merge(df_accidents, df_locations, on="p1", how="inner")

    # filter out rows with zero coordinates
    df = df[(df['d'] != 0) & (df['e'] != 0)]

    # identify rows where coordinates d are smaller than e and correct the swapped coordinates
    mask = df['d'] < df['e']
    df.loc[mask, ['d', 'e']] = df.loc[mask, ['e', 'd']].values

    # create geometry points
    geometry = [Point(xy) for xy in zip(df['d'], df['e'])]

    # convert the DataFrame into a GeoDataFrame
    gdf = geopandas.GeoDataFrame(df, geometry=geometry)

    # set the CRS to S-JTSK
    gdf.set_crs(epsg=5514, inplace=True)

    return gdf


def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Plots a graph showing accident locations under the influence of alcohol
    """
    # filter accidents caused under the influence of alcohol
    gdf_alcohol = gdf[gdf['p11'] >= 4]

    # select region
    region = "JHM"
    gdf_region = gdf_alcohol[gdf_alcohol['region'] == region]
    gdf_region = gdf_region.to_crs(epsg=3857)

    # extract accidents from specific months
    gdf_region['month'] = gdf_region['date'].dt.month
    gdf_month1 = gdf_region[gdf_region['month'] == 1] 
    gdf_month7 = gdf_region[gdf_region['month'] == 7]

    # determine map boundaries
    minx, miny, maxx, maxy = gdf_region.total_bounds

    # create a figure with two subplots
    fig, axs = plt.subplots(1, 2, figsize=(15, 8), constrained_layout=True)

    # plot for January
    gdf_month1.plot(ax=axs[0], color="red", markersize=5, alpha=0.6)
    ctx.add_basemap(axs[0], crs=gdf_region.crs, source=ctx.providers.OpenStreetMap.Mapnik)
    axs[0].set_title("JHM kraj - pod vlivem alkoholu (Leden)")
    axs[0].set_xlim(minx, maxx)
    axs[0].set_ylim(miny, maxy)
    axs[0].set_axis_off()

    # plot for July
    gdf_month7.plot(ax=axs[1], color="red", markersize=5, alpha=0.6)
    ctx.add_basemap(axs[1], crs=gdf_region.crs, source=ctx.providers.OpenStreetMap.Mapnik)
    axs[1].set_title("JHM kraj - pod vlivem alkoholu (Červenec)")
    axs[1].set_xlim(minx, maxx)
    axs[1].set_ylim(miny, maxy)
    axs[1].set_axis_off()

    # save the figure if requested
    if fig_location:
        plt.savefig(fig_location, dpi=300, bbox_inches="tight")

    # show the figure if requested
    if show_figure:
        plt.show()


def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Plots clusters of accidents caused by wild animals
    """
    # filter accidents caused by wild animals
    gdf_animal = gdf[gdf['p10'] == 4] 

    # select region LBK
    region = "LBK"
    gdf_region = gdf_animal[gdf_animal['region'] == region]
    gdf_region = gdf_region.to_crs(epsg=3857)  # convert mercator for basemap 

    # perform DBSCAN clustering
    coords = np.array(list(zip(gdf_region.geometry.x, gdf_region.geometry.y)))
    db = DBSCAN(eps=1500, min_samples=3).fit(coords)  # adjusted parameters for better clustering
    gdf_region['cluster'] = db.labels_

    # create convex hulls for each cluster
    cluster_polygons = []
    for cluster_id in set(db.labels_):
        if cluster_id == -1: 
            continue
        cluster_points = gdf_region[gdf_region['cluster'] == cluster_id]
        if len(cluster_points) >= 3:
            hull = MultiPoint(cluster_points.geometry.tolist()).convex_hull
            cluster_polygons.append((hull, len(cluster_points)))

    # create a GeoDataFrame for clusters
    cluster_gdf = geopandas.GeoDataFrame(
        {'geometry': [hull for hull, _ in cluster_polygons],
         'count': [count for _, count in cluster_polygons]},
        crs="EPSG:3857"
    )

    # lot clusters and points
    fig, ax = plt.subplots(1, 1, figsize=(15, 10))
    cluster_gdf.plot(ax=ax, column='count', cmap='viridis', legend=True, alpha=0.5, edgecolor="black")
    gdf_region.plot(ax=ax, color='red', markersize=5, alpha=0.6, label="Nehody zaviněné lesní zvěří")

    # add basemap
    ctx.add_basemap(ax, crs=gdf_region.crs, source=ctx.providers.OpenStreetMap.Mapnik)

    # configure the plot
    ax.set_title("Nehody zaviněné lesní zvěří v LBK kraji")
    ax.legend()
    ax.set_axis_off()

    # save the figure
    if fig_location:
        plt.savefig(fig_location, dpi=300, bbox_inches="tight")

    # show the figure
    if show_figure:
        plt.show()
