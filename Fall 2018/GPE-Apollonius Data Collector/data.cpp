#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(){
	ifstream file ("final/specific/15,10/'circle data r = 15, 7 at 100-0.csv'");
	float x,y,r;
	while(file >> x >> y >> r){
		cout << x << "\t" << y << "\t" << r << endl;
	}
	return 0;
}
