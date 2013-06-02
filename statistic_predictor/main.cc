#include <iostream> 
#include <climits>
#include <fstream>
#include <string>

#include "stock.h"

int main(int argc, char *argv[])
{
	int id = 0;
	if (argc == 2) {
		id = atoi(argv[1]);
	}

    cout << "Stock Statistic Predictor v1.0: " << endl;
	cout << "Select specific ID: " << id << endl;

    ifstream ifs;
    stock_t stkobj;

	// open file
    ifs.open("stock.data"); 
	// parse data
    stkobj.parse_stock_data_from_file(ifs, id);
//    stkobj.parse_stock_data_from_file(ifs);
    ifs.close();

//	stkobj.compute_gravity_for(1101);
	stkobj.compute_gravity_all();
	stkobj.compute_revenue_all();
	stkobj.print_data();

    return 0;
}
