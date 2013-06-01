#ifndef STOCK_H
#define STOCK_H

#include <map>
#include <fstream>

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
        double open, high, low, close;
        int volume;
    };
	

	void get_token(ifstream &ifs);
    void parse_stock_data_from_file(ifstream &ifs);
	void parse_date(ifstream &ifs, int &date);
	void parse_value(ifstream &ifs, entity_t &entity);
	
	void print_data();

  private:
    typedef map<int, entity_t> entity_map; // map int date, entity e 
    typedef map<int , entity_map> data_map; // map stock_id, stock_data 

    data_map data; // map stock_id, stock_data 
	string token;
};


#endif
