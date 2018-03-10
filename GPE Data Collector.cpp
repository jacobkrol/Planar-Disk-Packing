#include <iostream> //cin, cout
#include <ctime> //time (rand)
#include <cstdlib> //rand
#include <math.h> //pow, sqrt
#include <fstream> //doc
#include <stdlib.h> //malloc
#include <string> //file naming
#include <sstream> //string streams
using namespace std;

struct node {
	float x;
	float y;
	int r;
	float dx;
	float dy;
	struct node * next;
};
struct node * head = (struct node*)malloc(sizeof(struct node));

//allocate globals
struct scaleVals {
	float air;
	float nudge;
} scale;
char updateStyle = 'i';
int palsize = 775;
const float PI = 3.14159265359;
int numCircles, r1, r2, prop, propmax;

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

int selectRadius(int i) {
	if(++i % (prop+1) == 0) {
		return r2;
	}
	return r1;
}

void printNodeData() {
	struct node * moving = head->next;
	while(moving != NULL) {
		cout << "x:" << moving->x << " y:" << moving->y << " r:" << moving->r << endl;
		moving = moving->next;
	}
	cout << endl;
}

void addNode(int loop) {
	struct node * moving = head;
	while(moving->next != NULL) {
		moving = moving->next;
	}
	struct node * add = new node;
	moving->next = add;
	int r = selectRadius(loop);
	add->x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;
	add->y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (palsize-2*r) + r;
	add->r = r;
	add->dx = 0;
	add->dy = 0;
	add->next = NULL;
	numCircles++;
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

void saveNodeData() {
	ofstream doc;
	ostringstream r1s, r2s, props;
    r1s << r1;
    r2s << r2;
    props << prop;
	string fileTitle = "data/circle data r = " + r1s.str() + ", " + r2s.str() + " at 1-" + props.str() + ".csv";
	doc.open(fileTitle.c_str());
	struct node * moving = head->next;
	//doc << "x:     y:     r:\n";
	while(moving != NULL) {
		doc << moving->x/20 << "," << moving->y/20 << "," << (float) (moving->r)/20 << "\n";
		moving = moving->next;
	}
	doc.close();
}

void saveRunResults() {
	//save all of the results from all of the proportions into one file
}

int main() {
	
	//initialize random seed
	srand( static_cast <unsigned> (time(0)));
	
	//initialize var's
	scale.air = 0.95;
	scale.nudge = 0.05;
	clock_t t0, t1;
	
	//get values from user
	cout << "Range [1,40] -- Enter RADIUS 1: ";
	cin >> r1;
	cout << "Range [1,40] -- Enter RADIUS 2: ";
	cin >> r2;
	cout << "Range [1,inf] -- Enter Max Proportion: ";
	cin >> propmax;
	cout << endl;
	
	//loop through every prop
	for(prop = 1; prop <= propmax; prop++) {
		cout << "Proportion - 1:" << prop << endl;
		//start timer
		int loops, overflow;
		do {
			//reset list of circles
			head->next = NULL;
			numCircles = 0;
			
			//begin adding
			cout << "adding circles..." << endl;
			loops = 0;
			t0 = clock();
			while(calcDensity() < 0.7) {
				addNode(++loops);
			}
			t1 = clock();
			cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
			
			//begin updating
			cout << "updating circles..." << endl;
			t0 = clock();
			loops = 0;
			overflow = 1000;
			int numIntersections = 1;
			while( numIntersections && loops < overflow) {
				scale.nudge = (float) (numIntersections) / 5000.0;
				numIntersections = updateCircles();
				if(++loops%50 == 0) {
					cout << "intersections: " << numIntersections << " scale.nudge: " << scale.nudge << endl;
				}
			}
			
			//loop if not accomplished before overflow
		} while(loops == overflow);
		t1 = clock();
		cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds and " << loops << " loops" << endl;
		cout << "Final Results:" << endl;
		cout << "Circles: " << numCircles << " Density: " << calcDensity() << endl;
		saveNodeData();
	}
	
	return 0;
}
