# Planar Disk Packing - Fall 2018

This was a continuation of the Spring 2018 MCL Research project under the same name.

### GPE-Apollonius Data Collector

<details><summary>Details</summary>

We created a data collector program in C++ to handle our large number of calculations and analysis. It was built using algorithms from *Efficiency of Planar Disk Packings, Spring 2018*; and CGAL, a library allowing us to compute weighted Voronoi decompositions.

#### Physics Engine

It was built upon a very similar Gaseous Physics Engine that the programs from the end of Spring 2018 were using. The algorithm was as follows:

![Flowchart failed to load](https://github.com/jacobkrol/Planar-Disk-Packing/blob/master/Fall%202018/images/Update%20Circles%20Flowchart.png?raw=true)


#### Voronoi Decompositions

The data collector would run this O(n<sup>2</sup>) complex process a set number of times determined by us. This would allow disks to fall into place and make room between themselves. The program would then pause to calculate a weighted Voronoi decomposition of the palette, finding the vertices where a space may be filled by a new disk without overlapping with neighboring disks. See a screenshot from our Voronoi Playground Environment below which visualizes the calculation completed here:

![Screenshot failed to load](https://github.com/jacobkrol/Planar-Disk-Packing/blob/master/Fall%202018/images/voronoi-example-crop.png?raw=true)

The data collector then adds disks where large enough spaces have been identified. The physics engine then runs again in an attempt to free up more space. This process repeats until a determined number of loops have completed without finding a single available vertex in the Voronoi decomposition for a disk of either species. This final, saturated palette information is outputted to a .csv file with x, y, and weight values stored for every disk.

#### Data Collection

The process within the C++ engine was:

> 1. Take input from user for
>     - Radius of Species 1
>     - Radius of Species 2
>     - Number of proportions to test
> > 2. Determine ratio for current run of Species 1 : Species 2 based on target number of points
> > 3. Fill a palette to sub-saturated levels with disks of each species according to current run proportion
> > > 4. Run Gaseous Physics Engine \(GPE\) for a set amount of time to allow disks to settle
> > > 5. Run Apollonius Graph analysis, finding the weighted Voronoi decomposition of the palette
> > > 6. Flag vertices of decomposition with enough space to fit another disk without overlapping neighbors
> > > 7. Add disks at such available vertices, attempting to maintain target proportion
> > 8. Repeat steps 4-7 until the program fails to find available vertices a pre-determined number of times
> > 9. Export .csv file of \( x, y, weight \) data on every disk in the palette \*
> > 10. Clear the palette
> 11. Repeat steps 2-10 for every proportion from 0:100 to 100:0 \( 100% Species 2 and 100% Species 1 \)

\* the actual proportion of the species present is modified here, so the exact value is calculated for macro-data analysis.

</details>

### Weighted Voronoi Decompositions

<details><summary>Details</summary>

Weighted Voronoi Decompositions are a way of drawing boundaries across a plane such that every point on a curve of the decomposition is equidistant from any neighboring disk edge. *The vertices at each curve intersection would therefore be a local maximum for this distance.* This was essential to our research as it allowed us to collect the coordinates of weighted Voronoi decomposition vertices, which generates a *finite* list of coordinates to check in the process of saturating a palette.

A detailed README.md file for this program can be found in the `/Voronoi` directory

</details>

### Poster Summary of Research

View the summary of our research on our poster, below:

![poster failed to load](https://github.com/jacobkrol/Planar-Disk-Packing/blob/master/Fall%202018/images/final-poster.png?raw=true)
