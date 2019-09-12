# Voronoi JavaScript Environment

This was created using [Kcnarf's JavaScript environment](https://github.com/Kcnarf/d3-weighted-voronoi) as a basis. It relies on weighted Voronoi decompositions to create a network of connected line segments between vertices in a plane which lie equidistant from their nearest neighbors.

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
