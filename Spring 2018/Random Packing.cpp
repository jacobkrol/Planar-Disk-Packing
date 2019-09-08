#include <iostream> //cin, cout
#include <ctime> //time (rand)
#include <cstdlib> //rand
#include <math.h> //pow, sqrt
#include <fstream> //doc
using namespace std;

struct node {
	float x;
	float y;
	int r;
	float dx;
	float dy;
	struct node * next;
};

struct node * head = new node;

bool inside(node * check, float x, float y, int r) {
	struct node * moving = check;
	while(moving != NULL) {
		double dist = pow( pow(moving->x - x,2) + pow(moving->y - y,2) ,0.5);
		if(dist < moving->r + r ) {
			return true;
		}
		moving = moving->next;
	}
	return false;
}

int selectRadius() {
	return 20;
}

void printNodeData() {
	struct node * moving = head->next;
	while(moving != NULL) {
		cout << "x:" << moving->x << " y:" << moving->y << " r:" << moving->r << endl;
		moving = moving->next;
	}
	cout << endl;
}

bool addNode(int max) {
	struct node * moving = head;
	while(moving->next != NULL) {
		moving = moving->next;
	}
	struct node * add = new node;
	float x;
	float y;
	int r;
	bool intersecting;
	int tries = 0;
	do {
		intersecting = false;
		r = selectRadius();
		x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (775-2*r) + r;
		y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ) * (775-2*r) + r;
		struct node * temp = head->next;
		while(temp != NULL && !intersecting) {
			intersecting = inside(temp, x, y, r);
			temp = temp->next;
		}
	} while (intersecting && ++tries <= max);
	
	if(tries > max) {
		return false;
	}
	
	add->x = x;
	add->y = y;
	add->r = r;
	add->dx = 0;
	add->dy = 0;
	moving->next = add;
	add->next = NULL;
	return true;
}

void saveNodeData() {
	ofstream doc;
	doc.open("circle data.txt");
	struct node * moving = head->next;
	//doc << "x:     y:     r:\n";
	while(moving != NULL) {
		doc << moving->x/20 << "," << moving->y/20 << "," << moving->r/20 << ",0,0\n";
		moving = moving->next;
	}
	doc.close();
}

int main() {
	
	//initialize random seed
	srand( static_cast <unsigned> (time(0)));
	
	head->next = NULL;
	
	int max = 1000;
	cout << "Objective: Fill palette until " << max << " missed attempts." << endl;
	clock_t t0 = clock();
	while(addNode(max)) { }
	clock_t t1 = clock();
	cout << "Operation performed in " << (double)(t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
	saveNodeData();
	
	return 0;
}
