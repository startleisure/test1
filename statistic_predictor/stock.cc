#include "stock.h"
#include <iostream>
#include <cstdlib>
#include <vector>



namespace stock {
	enum data_state { NORMAL, ID, SZ, DATE0, DATE1, DATE2 , OPEN, HIGH, LOW, CLOSE, VOL};
}
using namespace stock;

void stock_t::get_token(ifstream &ifs) {
	ifs >> token;  // get one token
	cout << "TOKEN: <" << token << " >" << endl; // debug print
}

void stock_t::parse_stock_data_from_file(ifstream &ifs) {
	data_state state = NORMAL;
	int id = 0, sz = 0, date;

    while (ifs.good()) {
		get_token(ifs);
		
		switch(state) {
			case NORMAL:
				if (token.compare("*ID") == 0) {
					state = ID;
				} else if (token.compare("*SZ") == 0) {
					state = SZ;
				} else {
					state = DATE0;
				}
				break;
			case ID:
				id = atoi(token.c_str());
				cout << "set ID = " << id << endl;
				state = NORMAL;

				break; 
			case SZ:
				sz = atoi(token.c_str());
				cout << "set SZ = " << sz << endl;
				state = DATE0;

				break;
			case DATE0:
				parse_date(ifs, date);
				state = OPEN;
				
				break;
			case OPEN:
				state = HIGH;

				break;
			case HIGH:
				state = LOW;

				break;
			case LOW:
				state = CLOSE;

				break;
			case CLOSE:
				state = VOL;

				break;
			case VOL:
				state = NORMAL;

				break;
				


			default:
				cout << "ERROR in parse stock data" << endl;
				break;
		}
        if (token.find(")") != string::npos) { break;}

    }
}
int mtoi(const string &month) {
	// Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
	// return 1 ~ 12 * 100;
	// 2013, Jan , 18 => 20130118
	static const char *monthStr[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	for (int i = 0; i != 12; ++i) {
		if (month.compare(monthStr[i]) == 0) 
			return i+1;
	}
	return 0;
}
void stock_t::parse_date(ifstream &ifs, int &date) {
	data_state state = DATE0;

	int month = 0, day = 0, year = 0;
	string tmp; 

	// month
	tmp = token.substr(token.find("'")+1);
	month = mtoi(tmp);

	// day
	tmp.clear();
	get_token(ifs);
	tmp =  token.substr(0, token.find(",")).c_str()  ;
	day = atoi (tmp.c_str());

	// year
	tmp.clear();
	get_token(ifs);
	tmp =  token.substr(0, token.find("'")).c_str()  ;
	year = atoi (tmp.c_str());

	date = year*10000 + month*100 + day;
	cout << "set DATE = " << date << endl;
	if (month ==0 || day == 0 || year == 0) { 
		cout << "parse_date error" << endl; 
		exit(0);
	}
}












