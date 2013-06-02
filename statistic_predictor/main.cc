#include <iostream> 
#include <climits>
#include <fstream>
#include <string>

#include "stock.h"

int main(int argc, char *argv[])
{

    cout << "Stock Statistic Predictor v1.0: " << endl;

    ifstream ifs;
    stock_t stkobj;

	// open file
    ifs.open("stock.data"); 
	// parse data
    stkobj.parse_stock_data_from_file(ifs, 3034);
//    stkobj.parse_stock_data_from_file(ifs);
    ifs.close();

//	stkobj.compute_gravity_for(1101);
	stkobj.compute_gravity_all();
	stkobj.print_data();

    return 0;
}
