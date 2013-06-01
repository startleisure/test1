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
    stkobj.parse_stock_data_from_file(ifs);
    ifs.close();

    return 0;
}
