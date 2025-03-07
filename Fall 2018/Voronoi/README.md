# Voronoi JavaScript Environment

This was created using [Kcnarf's JavaScript environment](https://github.com/Kcnarf/d3-weighted-voronoi) as a basis. It relies on weighted Voronoi decompositions to create a network of connected line segments between vertices in a plane which lie equidistant from their nearest neighboring disks.

### Added Features:

We built upon this framework to add specialized functionality. Notably:

- Added non-zero radius capabilities on the vertices
- Updated the control panel to better fit our project's goals
- Reshaped environment field from circle to square
- Implemented ability to upload, visualize, and analyze custom disk data files
- Eliminated vertex-hide feature for one species of disk
- Established sample set to ease presentations and debugging
- Added vertex analysis which lists every line segment point of intersection
- Filter control, which adds markers to intersections where another disk could fit
- Previously mentioned markers can distinguish which species of disk has enough space

### How to download:

The .zip file in this directory will include the environment as well as all JavaScript dependencies.

### Control Panel Index:

| Name      | Action  |
| :--       | :--     |
| Show      | Marks all disks as visible in the plane |
| Hide      | Marks all disks as invisible in the plane |
| Sample    | Sets disks in plane to simple sample data |
| Import    | Opens finder to import custom data\* |
| Random    | Randomly places disks on plane until arbitrary saturation reached |
| Calculate | Analyzes and prints intersection vertices of the Voronoi decomposition |
| Filter    | Places markers at vertices indicating species of disk that would fit, centered there |

\* the imported data should match the guidelines given below.

### Import Data Formatting:

Imported files are simple, but should be formatted using the following guidelines.

#### File Layout

The imported data should be a .txt file with the following layout:

> \[ x<sub>1</sub> \] \[ y<sub>1</sub> \] \[ weight<sub>1</sub> \]
> 
> \[ x<sub>2</sub> \] \[ y<sub>2</sub> \] \[ weight<sub>2</sub> \]
> 
> ...
> 
> \[ x<sub>n</sub> \] \[ y<sub>n</sub> \] \[ weight<sub>n</sub> \]

Note: the x, y, and weight must be separated by a whitespace \( " " \) or a tab \( \\t \) and returned with a newline \( \\n \)

#### Data Ranges

The default boundary for \(x, y\) coordinates is 0&#8594;500 for each variable. Weights are best viewable as values 10&#8594;50 at the default 500x500 scale. If desired data file has different limits on the x and y, simply update the `origPal` variable on `line 21` of `read-in-data.js` to the desired maximum value and the program will adjust your variables to fit the display.

```javascript
var origPal = 500,  // update this variable with your data's limit for x and y values
    tarPal = 500,
    scale = tarPal / origPal;
```

#### Can I see?

Absolutely! Example File:

```
244.17 129.90 30
16.05 408.66 30
188.32 372.84 50
413.30 86.14 50
461.56 272.21 30
```

This file will process in the environment as the following. Please note that the origin (0,0) is oriented in the upper-left corner.

![Unable to load sample data image](https://github.com/jacobkrol/Planar-Disk-Packing/blob/master/Fall%202018/Voronoi/sample/sample-png.png?raw=true)
