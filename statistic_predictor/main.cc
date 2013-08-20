#include <iostream> 
#include <climits>
#include <fstream>
#include <string>
#include <cstring>

#include "stock.h"

void format_error() {
	cout << "error format" << endl;
	cout << "format : a.out -id num -box fileName" << endl;
	exit(0);
}

int main(int argc, char *argv[])
{


	int id = 0;
	string boxfileName;
	enum input_state { NORMAL, ID, BOX, ERROR};
	input_state state = NORMAL;
	for (int i = 1; i != argc; ++i) {
		switch(state) {
			case NORMAL:
				if (!strcmp(argv[i], "-id")) {
					state = ID;
					if (i == (argc-1))
						format_error();	
				} else if (!strcmp(argv[i], "-box")) {
					state = BOX;
					if (i == (argc-1))
						format_error();	
				} else {
					format_error();	
				}
				break;
			case ID:
				id = atoi(argv[i]);
				state = NORMAL;
				break;
			case BOX:
				boxfileName = string(argv[i]);
				state = NORMAL;
				break;
			default:
				break;
		}
	}

	cout << "com id = " << id << " box = " << boxfileName << endl;

    cout << "Stock Statistic Predictor v1.0: " << endl;
	cout << "Select specific ID: " << id << endl;

    ifstream ifs;
    stock_t stkobj;
	ostringstream convert;
	convert << "box" << id << ".data";

	// parse file
    ifs.open("stock.data"); 
	cout << "parse_stock_data" << endl;
    stkobj.parse_stock_data_from_file(ifs, id);
    ifs.close();

	// compute data
	cout << "comptute_data" << endl;
	stkobj.compute_gravity_all();
	stkobj.compute_revenue_all();

	// Create box system or restore
	if (boxfileName.size() != 0) {
		cout << "restore box system " << boxfileName << endl;
		if(stkobj.restore_boxsys(boxfileName.c_str())) {
			cout << "success!" << endl;
		} else {
			cout << "fail!" << endl;
		}
	} else {
		cout << "create_box_system" << endl;
		stkobj.create_box_system();
	}

	cout << "training" << endl;
	stkobj.box_training();

	cout << "save data: " << convert.str() << endl;
	stkobj.save_boxsys(convert.str().c_str());

	cout << "testing" << endl;
	stkobj.box_testing(id) ;
//	cout << "print data" << endl;
//	stkobj.print_boxsys();
//	stkobj.print_data();

    return 0;
}
