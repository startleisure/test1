#include "stock.h"
#include <iostream>
#include <cstdlib>
#include <vector>



namespace stock {
	enum data_state { NORMAL, ID, SZ, DATE, VALUE, OPEN, HIGH, LOW, CLOSE, VOLUME};
}
using namespace stock;

void stock_t::get_token(ifstream &ifs) {
	ifs >> token;  // get one token
	//cout << "TOKEN: <" << token << " >" << endl; // debug print
}

void stock_t::parse_stock_data_from_file(ifstream &ifs) {
	data_state state = NORMAL;
	int id = 0, sz = 0, date;

    while (ifs.good()) {
		
		switch(state) {
			case NORMAL:
				get_token(ifs);
				if (token.compare("*ID") == 0) {
					state = ID;
				} else if (token.compare("*SZ") == 0) {
					state = SZ;
				} else {
					state = DATE;
				}
				break;
			case ID:
				get_token(ifs);
				id = atoi(token.c_str());
				//cout << "set ID = " << id << endl;
				state = NORMAL;
				break; 
			case SZ:
				get_token(ifs);
				sz = atoi(token.c_str());
				//cout << "set SZ = " << sz << endl;
				state = NORMAL;

				break;
			case DATE:
				parse_date(ifs, date);
				state = VALUE;
				
				break;
			case VALUE:
				parse_value(ifs, data[id][date]);
				state = NORMAL;

				break;
			default:
				cout << "ERROR in parse stock data" << endl;
				break;
		}
//        if (token.find(")") != string::npos) { break;}

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
	//cout << "set DATE = " << date << endl;
	if (month ==0 || day == 0 || year == 0) { 
		cout << "parse_date error" << endl; 
		exit(0);
	}
}

void stock_t::parse_value(ifstream &ifs, entity_t &entity) {
	// parse: open high low close volume
	data_state state = OPEN;
	string tmp; 
	int pos =0, pos2, len =0;	

	while (ifs.good() && state != NORMAL) {
		get_token(ifs);
		switch(state) {
			case OPEN:
				pos = token.find("'")+1;
				len = token.rfind("'") - pos;
				tmp = token.substr(pos, len);
				entity.open = atof(tmp.c_str());
				state = HIGH;
				break;
			case HIGH:
				pos = token.find("'")+1;
				len = token.rfind("'") - pos;
				tmp = token.substr(pos, len);
				entity.high = atof(tmp.c_str());
				state = LOW;
				break;
			case LOW:
				pos = token.find("'")+1;
				len = token.rfind("'") - pos;
				tmp = token.substr(pos, len);
				entity.low = atof(tmp.c_str());
				state = CLOSE;
				break;
			case CLOSE:
				pos = token.find("'")+1;
				len = token.rfind("'") - pos;
				tmp = token.substr(pos, len);
				entity.close = atof(tmp.c_str());
				state = VOLUME;
				break;
			case VOLUME:	
				pos = token.find("'")+1;
				len = token.rfind("'") - pos;
				tmp = token.substr(pos, len);
				for (string::iterator it = tmp.begin(); it != tmp.end(); ++it) {
					if (*it == ',') 
						it = tmp.erase(it);
				}
				entity.volume = atoi(tmp.c_str());
				state = NORMAL;
			default:
				break;
			
		}
	}
	//cout << entity.open <<endl;
	//cout << entity.high <<endl;
	//cout << entity.low <<endl;
	//cout << entity.close <<endl;
	//cout << entity.volume <<endl;
}

void stock_t::print_data() {
	for (data_map::iterator it = data.begin(); it != data.end(); ++it) {
		cout << "ID " << it->first << endl;
		entity_map refmap = it ->second;
		for (entity_map::iterator it = refmap.begin(); it != refmap.end(); ++it) {
			entity_t e = it->second;
			cout << it->first << " " << e.open << " " << e.high << " " << e.low << " " << e.close << " " << e.volume << endl;
		}
	}
}









