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

	vector<double> v1(3,0.0);
	cell obj(v1, v1);
	exit(0);

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

	cout << "save data" << endl;
	stkobj.save_boxsys("boxsys2.data");

	cout << "testing" << endl;
	stkobj.box_testing() ;
//	cout << "print data" << endl;
//	stkobj.print_boxsys();
//	stkobj.print_data();

    return 0;
}
