#include <iostream> //cin, cout

#include <ctime> //time (rand)

#include <cstdlib> //rand

#include <math.h> //pow, sqrt

#include <cmath> //abs

#include <fstream> //doc

#include <stdlib.h> //malloc, abs

#include <string> //file naming

#include <sstream> //string streams



using namespace std;]

/*

****CGAL includes + typedefs***********************

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

****CGAL includes + typedefs***********************

*/



// Linked list for disks in palette

struct node {

	float x;

	float y;

	int r;

	float dx;

	float dy;

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

char updateStyle = 'i';

int palsize = 775;

const float PI = 3.14159265359;

int numCircles, r1, r2;



bool inside(node * check1, node * check2) {

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

	add->x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;

	add->y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;

	add->r = r;

	add->dx = 0;

	add->dy = 0;

	add->inter = 0;

	add->next = NULL;

	numCircles++;

}



void add_specifc_node(float x, float y, int r) {

	struct node * moving = head;

	while(moving->next != NULL) {

		moving = moving->next;

	}

	struct node * add = (struct node*)malloc(sizeof(struct node));

	moving->next = add;

	add->x = x;

	add->y = y;

	add->r = r;

	add->dx = 0;

	add->dy = 0;

	add->inter = 0;

	add->next = NULL;

	numCircles++;

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

			

			if( inside(i,j) && i != j ) {

				

				//update numIntersections

				numIntersections++;

				

				//update 'inter' attribute

				j->inter = 1;

				

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

			j = j->next;

		}

		

		//update x and y

		if(updateStyle == 'a' || (updateStyle == 'i' && numIntersections) ) {

			i->x += i->dx;

			i->y += i->dy;

		}

		

		//move to next

		i = i->next;

		

	}

	

	return numIntersections;

}



void deleteCircle(){

	struct node * temp = head->next;

	head->next = head->next->next;

	free(temp);

}



void fill() {

	int numIntersections = 1;

	int print_int = 0;

	while( numIntersections > 0){

		cout << "Loop " << print_int << " of the while loop." << endl;

		print_int++;

		for(int n = 0; n < (10000/numIntersections); n++){

			cout << "updating... (" << numIntersections << " intersections)" << endl;

			numIntersections = updateCircles();

			if(numIntersections == 0) { break; }

		}

		if (print_int%50 == 0){cout << "Intersections: " << numIntersections << endl;}

		deleteCircle();

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

    // 100*(float)smallProp/(float)largeProp << "% : " << 100*(1-(float)smallProp/(float)largeProp) << "%"

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

}



void voronoi_saturate() {

	// Write disk data to file in form x y r for Additvely Weigted Voronoi Diagram to read

	ofstream file;

	file.open("data/disks.txt");

	struct node * reading = head->next;

	while(reading->next != NULL){

		file << temp->x << " " << temp->y << " " << temp->r << "\n";

	}

	file.close();

	// Initialize AWVD to compute vertices for disk placement

	Site site;

	AWVD vor;

	ifstream ifs("data/disks.txt");

	assert(ifs);

	while ( ifs >> site ) {

		vor.insert(site);

	}

	AWVD::Vertex_iterator vi;

	string AWVD_output;

	// Retrieving veritces and splitting strings

	for (vi = vor.vertices_begin(); vi != vor.vertices_end(); vi++) {

		AWVD = vi->point();

	}

	add_specifc_node(x,y,r)

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

		do {

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

			cout << "updating circles..." << endl;

			t0 = clock();

			loops = 0;

			overflow = 5000;

			int numIntersections = 1;

/*			while( numIntersections && loops < overflow) {

				scale.nudge = (float) (numIntersections) / 5000.0;

				numIntersections = updateCircles();

				if(++loops%50 == 0) {

					cout << "intersections: " << numIntersections << " scale.nudge: " << scale.nudge << endl;

				}

			}*/

			

			//loop if not accomplished before overflow

		} while(loops == overflow);

		t1 = clock();

		cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds and " << loops << " loops" << endl;

		cout << "Filling palette..." << endl;

		t0 = clock();

		fill();

		t1 = clock();

		//Enter Voronoi loop here

		cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;

		cout << "Final Results:" << endl;

		cout << "Circles: " << numCircles << " Density: " << calcDensity() << endl;

		saveNodeData(prop*incVal);

		saveRunResults(numDataPoints, prop*incVal);

	}

	return 0;

}
