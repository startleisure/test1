#ifndef STOCK_H
#define STOCK_H

#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <queue>

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

class stock_t {
  public:
  stock_t() {};
  ~stock_t() {};
    struct entity_t {
		entity_t() {
			hly = vector<double> (3, 0.0);
		}
        double open, high, low, close;
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
	
	void print_data();

  private:
    typedef map<int, entity_t> entity_map; // map int date, entity e 
    typedef map<int , entity_map> data_map; // map stock_id, stock_data 

    data_map data; // map stock_id, stock_data 
	string token;
};


#endif
