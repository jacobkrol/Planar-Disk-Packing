#include <iostream> //cin, cout
#include <ctime> //time (rand)
#include <cstdlib> //rand
#include <math.h> //pow, sqrt
#include <cmath> //abs
#include <fstream> //doc
#include <stdlib.h> //malloc, abs
#include <string> //file naming
#include <sstream> //string streams
#include <algorithm> //std::max
using namespace std;

//****CGAL includes + typedefs***********************
#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> Rep;

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>
#include <CGAL/Apollonius_graph_adaptation_policies_2.h>
#include <CGAL/Apollonius_graph_adaptation_traits_2.h>
#include <CGAL/Voronoi_diagram_2.h>

typedef CGAL::Apollonius_graph_filtered_traits_2<Rep>           Traits;
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>      VertBase;
typedef CGAL::Triangulation_face_base_2<Traits>                 FaceBase;
typedef CGAL::Triangulation_data_structure_2<VertBase,FaceBase> TDS;
typedef CGAL::Apollonius_graph_2<Traits,TDS>                    Apollonius_graph;
typedef Apollonius_graph::Site_2                                Site;
typedef Apollonius_graph::Point_2                               SitePoint;

typedef CGAL::Apollonius_graph_adaptation_traits_2<Apollonius_graph>                   AdTraits;
typedef CGAL::Apollonius_graph_caching_degeneracy_removal_policy_2<Apollonius_graph>   AdPolicy;
typedef CGAL::Voronoi_diagram_2<Apollonius_graph,AdTraits,AdPolicy>                    AWVD;
//****CGAL includes + typedefs***********************

// Linked list for disks in palette
struct node {
	float x;
	float y;
	int r;
	float dx;
	float dy;
    int cell;
	struct node * next;
};
struct node * head = (struct node*)malloc(sizeof(struct node));

// Linked list for vertices given from AWVD
struct vertex {
	float x;
	float y;
	struct vertex * next;
};
struct vertex * start = (struct vertex*)malloc(sizeof(struct vertex));

//allocate globals
struct scaleVals {
	float air;
	float nudge;
} scale;

int numCells;
int grid_length;
char updateStyle = 'i';
int palsize = 775;
const float PI = 3.14159265359;
int numCircles, r1, r2, numVertices(0);

bool to_check(int c, int o, int n){
	// c = current cell, o = other cell (or the cell-to-be-checked)
	// Corner Cases
	if(((c==n-1) && !(o==c+n || o==c-1 || o==c+n-1)) || ((c==0)  &&  !( o==c+n || o==c+1 || o==c+n+1)) || ((c==(n*n)-n) && !(o==c+1 || o==c-n || o==c-n+1)) || ((c==(n*n)-1) && !(o==c-1 || o==c-n || o==c-n-1))){
//		cout << "Huh.\n";
		return false;
	}
	// Edge
	if (((0==c%n) && !(o==c-n || o==c+n || o==c+1 || o==c-n+1 || o==c+n+1)) || ((n-1==c%n) && !(o==c+n || o==c-n || o==c-1 || o==c+n-1 || o==c-n-1)) || ((c<n-1) && !(o==c+1 || o==c-1 || o==c+n || o==c+n+1 || o==c+n-1)) || ((c>((n*n)-n)) && !(o==c+1 || o==c-1 || o==c-n+1 || o==c-n-1 || o==c-n)) ){
		return false;
	}
	return true;
	// Rest of cells (completely inside)
}

bool inside(node * check1, node * check2) {
	if(check1 == check2){
		return false;
	}
	double dist = pow( pow(check1->x - check2->x,2) + pow(check1->y - check2->y,2) ,0.5);
	return (dist < check1->r + check2->r);
}

float calcDensity() {
	struct node * moving = head->next;
	float circleArea = 0;
	while(moving != NULL) {
		circleArea += PI * pow(moving->r,2);
		moving = moving->next;
	}
	return circleArea/pow(palsize,2);
}

float findPropInc(int points) {
	return 1/(float)(points-1);
}

float calcProportion() {
	struct node * moving = head->next;
	int numR1 = 0;
	while(moving != NULL) {
		if(moving->r == r1) {
			numR1++;
		}
		moving = moving->next;
	}
	return (float)numR1/(float)numCircles;
}

void calcRational(float val, int * small, int * large) {
	float error = 0.5; //percent error allowed
	float num, den = 0;
	
	//loop until less than error
	do{
		den++;
		num = 0;
		//loop until closest fraction found
		while(num/den < val) {
			num++;
		}
		
		if( (num/den) - val > val - (num-1)/den ) {
			*small = num-1;
			*large = den;
		} else {
			*small = num;
			*large = den;
		}
		
	} while( abs( 100*( ( (float) *small / (float) *large - val ) / val ) ) > error );
}

int selectRadius(float val, int loop) {
	int small, large;
	calcRational(val, &small, &large);
	if(loop % large < small) {
		return r1;
	} else {
		return r2;
	}
}

void printNodeData() {
	struct node * moving = head->next;
	while(moving != NULL) {
		cout << "x:" << moving->x << " y:" << moving->y << " r:" << moving->r << endl;
		moving = moving->next;
	}
	cout << endl;
}

void addNode(float propVal, int loop) {
	struct node * moving = head;
	while(moving->next != NULL) {
		moving = moving->next;
	}
	struct node * add = (struct node*)malloc(sizeof(struct node));
	moving->next = add;
	int r = selectRadius(propVal, loop);
	float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;
	float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;
	if((x > (palsize/grid_length)*(grid_length-1)) && (y > (palsize/grid_length)*(grid_length-1))){
		add->cell = pow(grid_length,2) - 1;
	} else if (x > (palsize/grid_length)*(grid_length-1)) {
		add->cell = (grid_length-1) + grid_length*floor(y/(palsize/grid_length));
	} else if (y > (palsize/grid_length)*(grid_length-1)){
		add->cell = floor(x/(palsize/grid_length)) + grid_length*(grid_length-1);
	} else {
		add-> cell = floor(x/(palsize/grid_length)) +grid_length*floor(y/(palsize/grid_length));
	}
	add->r = r;
	add->x = x;
	add->y = y;
	add->dx = 0;
	add->dy = 0;
	add->next = NULL;
	numCircles++;
}

struct node * add_specific_node(float propVal, int loop,float x, float y) {
	struct node * moving = head;
	while(moving->next != NULL) {
		moving = moving->next;
	}
	struct node * add = (struct node*)malloc(sizeof(struct node));
	moving->next = add;
	add->x = x;
	add->y = y;
	if((x > (palsize/grid_length)*(grid_length-1)) && (y > (palsize/grid_length)*(grid_length-1))){
		add->cell = pow(grid_length,2) - 1;
	} else if (x > (palsize/grid_length)*(grid_length-1)) {
		add->cell = (grid_length-1) + grid_length*floor(y/(palsize/grid_length));
	} else if (y > (palsize/grid_length)*(grid_length-1)){
		add->cell = floor(x/(palsize/grid_length)) + grid_length*(grid_length-1);
	} else {
		add-> cell = floor(x/(palsize/grid_length)) +grid_length*floor(y/(palsize/grid_length));
	}
	add->r = selectRadius(propVal,loop);
	add->dx = 0;
	add->dy = 0;
	add->next = NULL;
	numCircles++;
	return add;
}

struct vertex * add_vertex(float x, float y){
	struct vertex * moving = start;
	while(moving->next != NULL){
		moving = moving->next;
	}
	struct vertex * add = (struct vertex*)malloc(sizeof(struct vertex));
	moving->next = add;
	add->x = x;
	add->y = y;
	add->next = NULL;
	numVertices++;
	return add;
}

int updateCircles() {
	struct node * i = head->next;
	int numCircles = 0;
	int numIntersections = 0;
	while(i != NULL) {
		
		//dampen dx and dy
		i->dx *= scale.air;
		i->dy *= scale.air;
		
		//boundary cond.
		if( i->x > palsize - i->r || i->x < i->r ) {
			i->dx *= -1;
		}
		if( i->y > palsize - i->r || i->y < i->r ) {
			i->dy *= -1;
		}
		
		struct node * j = head->next;
		while(j != NULL) {
			/*
			if( i == j ) {
				j = j->next;
			}*/
			
			if((((i->cell == j->cell) || (j->cell == i->cell+1) || (j->cell == i->cell-1) || (j->cell == i->cell+grid_length) || (j->cell == i->cell-grid_length) || (j->cell == i->cell+grid_length+1) || (j->cell == i->cell+grid_length-1) || (j->cell == i->cell-grid_length+1) || (j->cell == i->cell-grid_length-1))) && (i != j)) {
				if(to_check(i->cell,j->cell,grid_length)){

                    //update numIntersections
                    numIntersections++;
                    //if left/right
                    if(i->x < j->x) {
                        i->dx += -scale.nudge;
                        j->dx += scale.nudge;
                    } else {
                        i->dx += scale.nudge;
                        j->dx += -scale.nudge;
                    }

                    //if over/under
                    if(i->y < j->y) {
                        i->dy += -scale.nudge;
                        j->dy += scale.nudge;
                    } else {
                        i->dy += scale.nudge;
                        j->dy += -scale.nudge;
                    }

                    //hard boundary condition
                    if( i->x > palsize - i->r ) {
                        i->x = palsize - i->r;
                    }
                    if( i->x < i->r ) {
                        i->x = i->r;
                    }
                    if( i->y > palsize - i->r ) {
                        i->y = palsize - i->r;
                    }
                    if( i->y < i->r ) {
                        i->y = i->r;
                    }

                }
            }
			j = j->next;
		}
		
		//update x and y
		if(updateStyle == 'a' || (updateStyle == 'i' && numIntersections) ) {
			i->x += i->dx;
			i->y += i->dy;
		}
		if((i->x > (palsize/grid_length)*(grid_length-1)) && (i->y > (palsize/grid_length)*(grid_length-1))){
			i->cell = pow(grid_length,2) - 1;
		} else if (i->x > (palsize/grid_length)*(grid_length-1)) {
			i->cell = (grid_length-1) + grid_length*floor(i->y/(palsize/grid_length));
		} else if (i->y > (palsize/grid_length)*(grid_length-1)){
			i->cell = floor(i->x/(palsize/grid_length)) + grid_length*(grid_length-1);
		} else {
			i-> cell = floor(i->x/(palsize/grid_length)) +grid_length*floor(i->y/(palsize/grid_length));
		}

		//move to next
		i = i->next;
		
	}
	
	return numIntersections;
}

void deleteCircle(){
	struct node * moving = head;
	while(moving->next->next != NULL){
		moving = moving-> next;
	}
	struct node * temp = moving->next;
	moving->next = NULL;
	free(temp);
	numCircles--;
}

void fill() {
	cout << "Inside fill().\n";
	int numIntersections = updateCircles();
	int scale = numIntersections;
	while( numIntersections > 0){
//		cout << "Inside while loop.\n";
		if(numIntersections > 400){
			for(int n = 0; n < (10000/numIntersections); n++){
//				cout << "Inside for loop with " << numIntersections << " intersections.\n";
				numIntersections = updateCircles();
				if(numIntersections == 0){break;}
			}
		}else{
			for(int n = 0; n < 26; n++){
				numIntersections = updateCircles();
				if(numIntersections == 0){break;}
			}
		}
		for(int quick = 0; quick < 6; quick++){
			deleteCircle();
		}
		numIntersections = updateCircles();
	}
}

void saveNodeData(float propVal) {
	int small, large;
	ofstream doc;
	ostringstream r1s, r2s, prop1, prop2;
	r1s << r1;
	r2s << r2;
 	calcRational(propVal, &small, &large);
 	prop1 << 100*(float)small/(float)large;
    	prop2 << 100*(1-(float)small/(float)large);
	//100*(float)smallProp/(float)largeProp << "% : " << 100*(1-(float)smallProp/(float)largeProp) << "%"
	string fileTitle = "data/circle data r = " + r1s.str() + ", " + r2s.str() + " at " + prop1.str() + "-" + prop2.str() + ".csv";
	doc.open(fileTitle.c_str());
	struct node * moving = head->next;
	//doc << "x:     y:     r:\n";
	while(moving != NULL) {
		doc << moving->x/20 << "," << moving->y/20 << "," << (float) (moving->r)/20 << "\n";
		moving = moving->next;
	}
	doc.close();
}

void saveRunResults(int dataPoints, float propVal) {
	ofstream doc;
	ostringstream r1s, r2s, steps;
	r1s << r1;
	r2s << r2;
	steps << dataPoints;
	string fileTitle = "data/run results r = " + r1s.str() + ", " + r2s.str() + " with " + steps.str() + " points.csv";
	doc.open(fileTitle.c_str(), ios_base::app);
	int small, large;
	calcRational(propVal, &small, &large);
	doc << 100*(float)small/(float)large << "," << calcProportion() << "," << calcDensity() << "\n";
	doc.close();
}

void deleteCircles() {
	struct node * moving = head->next;
	struct node * last = head->next;
	while(moving != NULL) {
		moving = moving->next;
		free(last);
		last = moving;
	}
	head->next = NULL;
	numCircles = 0;
}

void deleteVertices(){
	struct vertex * moving = start->next;
	struct vertex * last = start->next;
	while(moving != NULL) {
		moving = moving->next;
		free(last);
		last = moving;
	}
	start->next = NULL;
}

bool voronoi_saturate(float inc, int loops) {
	// This boolean represents whether or not the program was able to add a node
	bool success = false;
	// Write disk data to file in form x y r for Additvely Weigted Voronoi Diagram to read
	ofstream file;
	file.open("final/disks.txt", ofstream::out | ofstream::trunc);
	struct node * reading = head->next;
	while(reading != NULL){
		file << reading->x << "\t" << reading->y << "\t" << reading->r << "\n";
		reading = reading->next;
	}
	file.close();
	cout << "Disks have been written to file.\n";
	// Initialize AWVD to compute vertices for disk placement
	Site site;
	AWVD vor;
	ifstream ifs("final/disks.txt");
	assert(ifs);
	while ( ifs >> site ) {
		vor.insert(site);
	}
	AWVD::Vertex_iterator vi;
	// Retrieving veritces and constructing linked list of vertices
	for (vi = vor.vertices_begin(); vi != vor.vertices_end(); vi++) {
		if(vi->point().x() >= (palsize-max(r1,r2)) || vi->point().y() >= (palsize-max(r1,r2)) || vi->point().x() <= max(r1,r2) || vi->point().y() <= max(r1,r2)){
			continue;
		}
		add_vertex(vi->point().x(),vi->point().y());
	}
	cout << "Voronoi list has been made.\n";
	// The for loop runs through the two cases which first check for possible nodes of the larger radius on each vertex
	// and then all possible nodes of the smaller radius
//	cout << "Beginning of for loops.\n";
	// This for loop runs twice (once for each radius) with the larger radius first
//	for(int radius = max(r1,r2); radius >= min(r1,r2);radius = radius-(max(r1,r2)-min(r1,r2))){
	struct node * temp = add_specific_node(inc,++loops,0,0);
	struct vertex * moving2 = start->next;
	struct node * moving;
	int intersecting;
	for(int nice = 0; nice < 2; nice++){
		moving2 = start->next;
		while(moving2 != NULL){
			if (moving2->x >= palsize || moving2->y >= palsize || moving2->x <= 0 || moving2->y <= 0){
				moving2 = moving2->next;
				continue;
			}
//			cout << "Back at top of vertex traversal.\n";
			temp->x = moving2->x;
			temp->y = moving2->y;
//			cout << "Assigned vertex coordinates to temp node: "<< moving2->x << " " << moving2->y << endl;
			moving = head->next;
			intersecting = 0;
			while(moving != NULL){
				if(inside(moving,temp)){intersecting++;}
				moving = moving->next;
			}
			if(intersecting ==  0){
				success = true;
				cout << "Adding a circle of radius " << selectRadius(inc,loops)  << " at coordinates (" << temp->x << "," << temp->y << ") from voronoi vertices for a density of " << calcDensity() << " and " << updateCircles()  <<" intersections.\n";
				cout << "This circle is intersecting " << intersecting << " other circles.\n";
				temp->next = add_specific_node(inc,++loops,0,0);
				temp = temp->next;
				numCircles++;
				moving2 = moving2->next;
			}else {moving2 = moving2->next;}
		}
	deleteCircle();
	deleteVertices();
	return success;
	}
}

int main() {
	
	//initialize random seed
	srand( static_cast <unsigned> (time(0)));
	
	//initialize var's
	scale.air = 0.95;
	scale.nudge = 0.05;
	clock_t t0, t1;
	int numDataPoints;
	
	//get values from user
	cout << "Range [1,40] -- Enter RADIUS 1: ";
	cin >> r1;
	cout << "Range [1,40] -- Enter RADIUS 2: ";
	cin >> r2;
    cout << "Side-Length of spatial grid (Enter 3 for 9 cells, n for n^2): ";
    cin >> grid_length;
    numCells = grid_length*grid_length;
	cout << "Range [2,inf] -- Enter Number of Data Points: ";
	cin >> numDataPoints;
	cout << endl;
	
	float incVal = findPropInc(numDataPoints);
	
	//loop through every prop
	for(int prop = 0; prop < numDataPoints; prop++) {
		int smallProp, largeProp;
		calcRational(prop*incVal, &smallProp,&largeProp);
		cout << "Proportion - " << 100*(float)smallProp/(float)largeProp << "% : " << 100*(1-(float)smallProp/(float)largeProp) << "%" << endl;
		//start timer
		int loops, overflow;
			//reset list of circles
			head->next = NULL;
			deleteCircles();
			numCircles = 0;
			
			//begin adding
			cout << "adding circles..." << endl;
			loops = 0;
			t0 = clock();
			while(calcDensity() < 0.9) {
				addNode(prop*incVal, ++loops);
			}
			t1 = clock();
			cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
			
			//begin updating
			loops = 0;
			overflow = 5000;
			int numIntersections = 1;
			//loop if not accomplished before overflow
		cout << "Reducing palette..." << endl;
		t0 = clock();
		fill();
		t1 = clock();
		//Enter Voronoi loop here
		cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
		cout << "Saturating palette using voronoi vertices..." << endl;
		t0 = clock();
		while(voronoi_saturate(prop*incVal,loops)){
			struct node * moving = head;
			cout << "Randomizing dx and dy values\n";
			while(moving != NULL){
				moving->dx = 2*((static_cast <float> (rand()) / static_cast <float> (RAND_MAX) )-0.5);
				moving->dy = 2*((static_cast <float> (rand()) / static_cast <float> (RAND_MAX) )-0.5);
				moving = moving->next;
			}
			cout << "Shaking palette.\n";
			for(int m = 0; m < 100; m++){
//				cout << numCircles << " circles.\n" << "Density of "  << calcDensity() << "\n";
				updateCircles();
			}
			if(calcDensity() >= 1){cout << "Density is impossibly high -- stopping.\n";break;}
			
		}
		t1 = clock();
//		cout << "Voronoi saturation finished in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
		cout << "Final Results:" << endl;
		cout << "Circles: " << numCircles << " Density: " << calcDensity() << endl;
//		saveOverallDensities();
		saveNodeData(prop*incVal);
		saveRunResults(numDataPoints, prop*incVal);
//		printNodeData();
	}
	return 0;
}
