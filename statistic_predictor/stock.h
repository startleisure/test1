#ifndef STOCK_H
#define STOCK_H

#include <map>
#include <set>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <list>
#include <cmath>
#include <sstream>

#include "cell.h"

extern double gStart;
extern double gDelta;
extern double hly1Start;
extern double hly1Delta;
extern double hly2Start;
extern double hly2Delta;

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
        double k, d; //, rsv, high9, low9; // KD test
		double revenue;
        int volume;
		double rsi;
		map<int, double> ma; // ma[30] , ma[72] , ....
		vector<double> hly; // gravity line hly[0], velocity: hly[1], acceleration: hly[2]
    };
	struct Trade_t {
		int id;
		int date_in;
		int date_out;
		double price_in;
		double price_out;
		double revenue;
	};
	typedef vector<Trade_t> result_t;
	
    typedef map<int, entity_t> entity_map; // map int date, entity e 
    typedef map<int , entity_map> data_map; // map stock_id, stock_data 
	typedef vector<cell_t> cell_vector;
	typedef vector< pair< double, cell_vector::iterator> > useCell_t;
	

	void get_token(ifstream &ifs);
    void parse_stock_data_from_file(ifstream &ifs, int target = 0);
	void parse_date(ifstream &ifs, int &date);
	void parse_value(ifstream &ifs, entity_t &entity);
	void compute_rsi_for(int id, int n);
	void compute_rsi_all(int n);
	void rsi_buy_simulation();
	void trade_simulation_for_id(int id);
	void trade_id_date(int id, entity_map::iterator curr);
	bool TradeModel(entity_map::iterator start, entity_map::iterator curr);
	void report_result(double &theRatio, double &theRevenue);
	void clear_result();

	void compute_gravity_for(int id);
	void compute_gravity_all();
	void compute_ma_for(int id, int t);
	void compute_velocity();
	void compute_revenue_all(); // revenue at +t days when buy at (id, date)
	void compute_revenue_for(int id, int date, int t); // revenue at +t days when buy at (id, date)

    // KDJ test
	void compute_KDJ_all();
    void compute_KDJ_for(int id, int t);
	
	void print_data();
	void create_box_system();
	void box_training();
	void error_correct(useCell_t &useCell, double err, double ratio);

	void box_testing(int id);
	void print_boxsys();
	void save_boxsys(const char *fileName = NULL);
	bool restore_boxsys(const char *fileName = NULL);

  private:

    data_map data; // map stock_id, stock_data 
	string token;
	cell_vector boxsys;
	result_t result_bag;
};


class cell_t {
  public:
	static const double radius_ratio = 0.08;   // % of the center to be min radius value
	// tune this from 0.1 ~ 0.4 to control the box size, larger ratio smaller size of box created

	static const double min_radius_g  = 0.8;  // radius of gravity
	static const double min_radius_h1 = 0.08; // radius of hly derivative
	static const double min_radius_h2 = 0.008; // radius of hly derivative
	static const double dratio = 0.9; // define if distance >  dratio% of radius  imply outside the cell
	enum dim_type { G = 0, H1, H2, DIM };
	typedef map<dim_type, double> coord;
	cell_t(coord &point) { 
		center = point;
	}
	cell_t(double w, double g, double h1, double h2, double rg, double rh1, double rh2) { // direct construct the cell 
		weight = w;
		center[G]  = g;
		center[H1] = h1;
		center[H2] = h2;
		radius[G]  = rg;
		radius[H1] = rh1;
		radius[H2] = rh2;
	}
	cell_t(double g, double h1, double h2) {
		double tmp = 0.0; 
		weight = 0.0;
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
	bool is_near(double g, double h1, double h2);

	double &get_weight() { return weight;}
	coord &get_center() { return center;}
	coord &get_radius() { return radius;}

  private:
	double weight; 
	coord center;
	coord radius;
};

class ghlySet_t {
	public:
		double g;
		double hly1;
		double hly2;
		double val;
		ghlySet_t() {
			g = -99;
			hly1 = -99;
			hly2 = -99;
			val = -99 ;
		}
		void set(double ig, double ihly1, double ihly2, double ival) {
			g = ig; hly1 = ihly1; hly2 = ihly2; val  =  ival;
		}
		void report(){
			cout << "(" << g << ", " << hly1 << ", " << hly2 << ") : val = " << val << endl;
		}
};


#endif
