# Planar-Disk-Packing

MCL Project: Spring 2018, University of Illinois at Chicago

Contributors: Rohit Banerjee, Ivan Cruz, Jacob Krol

Advisor: Ali Mohajer

Outcomes: We initially attempted to develop an understanding of the bounding density curves of disk packings by creating an interactive visualization where the user could add various sized disks and manipulate them under a simulation of almost gaseous physics. We then turned to determining a thorough method for evaluating the saturation of a given palette of disks. Though we were able to make great strides in a rigorous testing method, using additively-weighted Voronoi decompositions, our project concluded without substantial data using this method. Instead, our final data was drawn with an experimental method of adding and deleting disks until a satisfyingly saturated palette was achieved. In addition to the relatively large size of the disks and possible inaccuracies in our data collection, our methods should be sharpened in the future to evaluate the validity of our final results.

Presentations:
* Mathematical Computing Laboratory Poster Session, Chicago, IL, May 2018

- - - -

MCL Project: Fall 2018, University of Illinois at Chicago

Contributors: Rohit Banerjee, Jacob Krol

Advisor: Ali Mohajer

Outcomes: As a continuation of our previous work on this subject, we were able to utilize systems that we had already developed while implementing more mathematically-rigorous methods to further our research. Using our gaseous physics engine, we placed it in the context of a larger data collector which used the physics algorithms in tandem with Voronoi; decompositions to automate the process of creating and saturating a palette with disks of given radii and proportion. Additively-weighted Voronoi decompositions allowed us to decompose the plane into cells, drawn with their edges and vertices defined by being equidistant from the nearest neighboring disks. Testing this finite list of vertices, therefore, could exhaustively check the possibility of adding a new disk at any point in our finite plane. With this implementation, we could consistently draw densities for various initial conditions for two-species packings. Obtaining curves that were slightly different from expected in shape and y-intercept suggests a need for further investigation into these patterns. A necessary next step for this research would involve decreasing the complexity of our physics algorithms to improve run-time and, in turn, allow for larger samples that better approximate an infinite space.

Presentations:
* Mathematical Computing Laboratory (MCL) Poster Session, Chicago, IL, December 2018
* Joint Mathematics Meetings (JMM), Baltimore, MD, January 2019
