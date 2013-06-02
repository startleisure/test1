#ifndef STOCK_H
#define STOCK_H

#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <cmath>

using namespace std;
// design interface , 2013,0601, ChiHsien //

//int get_suggest_stock_id()
//void report() 
// data format: date, open, high, low, close, volume
// ID 1101 SZ 3526
// LEN 200
// ('Mar 29, 2013', '37.50', '37.55', '36.90', '37.50', '3,548,000')
//  ...
// ID ...
class stock_t;
class cell_t;

class stock_t {
  public:
  stock_t() {};
  ~stock_t() {};
    struct entity_t {
		entity_t() {
			open = high = low = close = g = revenue = 0.0;
			volume = 0;
			hly = vector<double> (3, 0.0);
		}
        double open, high, low, close;
		double g; // (v/hly[0] -1 )*100 
		double revenue;
        int volume;
		map<int, double> ma; // ma[30] , ma[72] , ....
		vector<double> hly; // gravity line hly[0], velocity: hly[1], acceleration: hly[2]
    };
	

	void get_token(ifstream &ifs);
    void parse_stock_data_from_file(ifstream &ifs, int target = 0);
	void parse_date(ifstream &ifs, int &date);
	void parse_value(ifstream &ifs, entity_t &entity);
	void compute_gravity_for(int id);
	void compute_gravity_all();
	void compute_ma_for(int id, int t);
	void compute_velocity();
	void compute_revenue_all(); // revenue at +t days when buy at (id, date)
	void compute_revenue_for(int id, int date, int t); // revenue at +t days when buy at (id, date)
	
	void print_data();
	void box_training();
	void box_testing();

  private:
    typedef map<int, entity_t> entity_map; // map int date, entity e 
    typedef map<int , entity_map> data_map; // map stock_id, stock_data 

    data_map data; // map stock_id, stock_data 
	string token;
	vector<cell_t> boxsys;
};


class cell_t {
  public:
	static const double min_radius_g  = 0.5;  // radius of gravity
	static const double min_radius_h1 = 0.05; // radius of hly derivative
	static const double min_radius_h2 = 0.005; // radius of hly derivative
	static const double radius_ratio = 0.1;   // 10% of the center absolute value
	enum dim_type { G = 0, H1, H2, DIM };
	typedef map<dim_type, double> coord;
	cell_t(coord &point) { 
		center = point;
	}
	cell_t(double g, double h1, double h2) {
		double tmp = 0.0; 
		force = 0.0;
		center[G]  = g;
		center[H1] = h1;
		center[H2] = h2;
		
		tmp = abs(radius_ratio*g); 
		if (tmp > min_radius_g) radius[G] = tmp;
		else					radius[G] = min_radius_g;

		tmp = abs(radius_ratio*h1); 
		if (tmp > min_radius_h1) radius[H1] = tmp;
		else		 			 radius[H1] = min_radius_h1;

		tmp = abs(radius_ratio*h2);
		if (tmp > min_radius_h2) radius[H2] = tmp;
		else 				 	 radius[H2] = min_radius_h2;
	}
	double get_excite_value(double g, double h1, double h2);

  private:
	coord center;
	coord radius;
	double force;
};


#endif
