#include <map>

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

class astock_t {
  public:
    astock_t (int id) : ID(id) {};
    ~astock_t();

  private:
    struct stockdata {
        double open;
        double high;
        double low;
        double close;
        int volume;
    };
    int ID;
    map<int, stockdata> data;
};

