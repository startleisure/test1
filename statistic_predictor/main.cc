#include <iostream> 
#include <climits>
#include <fstream>
#include <string>
#include <cstring>

#include "stock.h"

double gStart;
double gDelta;
double hly1Start;
double hly1Delta;
double hly2Start;
double hly2Delta;

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

	// compute more analysis data
	cout << "comptute_KDJ" << endl;
    stkobj.compute_KDJ_all();

    #if 0 
    stkobj.print_data();
    return 0;
    #endif

	//cout << "comptute_gravity" << endl;
	//stkobj.compute_gravity_all();
	//cout << "comptute_rsi" << endl;
	//stkobj.compute_rsi_all(10); // rsi n = 10

	// trade simulation 
    stkobj.KDJ_trade_simulation();
    
    // Report Result
	double tmpRatio = 0, tmpRevenue = -100;
    stkobj.report_result(tmpRatio, tmpRevenue);
	stkobj.clear_result();

	//gDelta = 1;
	//hly1Delta = 0.2;
	//hly2Delta = 0.02;

	//double tmpRatio = 0, tmpRevenue = -100;
	//ghlySet_t bestRevenue, bestRatio;

	//for (gStart = -10 ; gStart <10; gStart+=gDelta) {
	//	for (hly1Start = -1; hly1Start < 1; hly1Start+=hly1Delta){
	//		for (hly2Start = -0.1; hly2Start < 0.1; hly2Start+=hly2Delta){
	//			cout << "Simulation for (" << gStart << "," << hly1Start << "," << hly2Start << ")" << endl;
	//			stkobj.rsi_buy_simulation();
	//			stkobj.report_result(tmpRatio, tmpRevenue);
	//			stkobj.clear_result();

	//			if (tmpRatio > bestRatio.val) {
	//				bestRatio.set(gStart, hly1Start, hly2Start, tmpRatio);
	//			}
	//			if (tmpRevenue > bestRevenue.val) {
	//				bestRevenue.set(gStart, hly1Start, hly2Start, tmpRevenue);
	//			}
	//			cout << "#########################################\n" << endl;
	//		}
	//	}
	//}

	//cout << "report best Revenue set" << endl;
	//bestRevenue.report();
	//cout << "report best Ratio set" << endl;
	//bestRatio.report();

	//cout << "END!!" << endl;


	// report result
	//stkobj.report_result();

/////////////////////// obs //////////////////////
	// compute data
	//cout << "comptute_data" << endl;
	//stkobj.compute_gravity_all();
	//stkobj.compute_revenue_all();

	// Create box system or restore
	//if (boxfileName.size() != 0) {
	//	cout << "restore box system " << boxfileName << endl;
	//	if(stkobj.restore_boxsys(boxfileName.c_str())) {
	//		cout << "success!" << endl;
	//	} else {
	//		cout << "fail!" << endl;
	//	}
	//} else {
	//	cout << "create_box_system" << endl;
	//	stkobj.create_box_system();
	//}

	//cout << "training" << endl;
	//stkobj.box_training();

	//cout << "save data: " << convert.str() << endl;
	//stkobj.save_boxsys(convert.str().c_str());

	//cout << "testing" << endl;
	//stkobj.box_testing(id) ;

//	cout << "print data" << endl;
//	stkobj.print_boxsys();

//  stkobj.print_data();

    return 0;
}
