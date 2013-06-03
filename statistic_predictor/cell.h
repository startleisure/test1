#ifndef CELL_H
#define CELL_H

#include <vector>

using namespace std;

class cell { // general cell 
	public:
	// construct with c and r
	cell(vector<double> &c, vector<double> &r) : weight(0.0), out(0.0), error(0.0) {
		if (c.size() != r.size() || c.size() == 0 || r.size() == 0) {
			cout << "wrong arguments to construct cell\n";
			exit(0);
		} 
		center = c;
		radius = r;
	}
	// construct with c
	cell(vector<double> &c) : weight(0.0), out(0.0), error(0.0){
		if (c.size() == 0) {
			cout << "wrong arguments to construct cell\n";
			exit(0);
		} 
		center = c;
	}

	private:
		double weight, out, error;
		vector<double> center;
		vector<double> radius;
};

#endif
