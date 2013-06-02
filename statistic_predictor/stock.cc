#include "stock.h"
#include <iomanip>



namespace stock {
	enum data_state { NORMAL, ID, SZ, DATE, VALUE, OPEN, HIGH, LOW, CLOSE, VOLUME, SKIP};
}
using namespace stock;

void stock_t::get_token(ifstream &ifs) {
	ifs >> token;  // get one token
	//cout << "TOKEN: <" << token << " >" << endl; // debug print
}

void stock_t::parse_stock_data_from_file(ifstream &ifs, int target) {
	data_state state = NORMAL;
	int id = 0, sz = 0, date;

	int runstop = 0;

    while (ifs.good() && runstop <2) {
		
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

				if (target != 0) {
					if (id != target) {
						state = SKIP;
						break;
					}
				}
				//cout << "set ID = " << id << endl;
				state = NORMAL;
				break; 
			case SKIP:
				get_token(ifs);
				if (token.compare("*ID") == 0) 
					state = ID;
				
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
		entity_map &refmap = it->second;
		for (entity_map::iterator it = refmap.begin(); it != refmap.end(); ++it) {
			entity_t &e = it->second;
		//	cout << it->first << " " << e.open << " " << e.high << " " << e.low << " " << e.close << " " << e.volume << endl;
			//cout << it->first << " " << e.close << " " << e.ma[30] << " " << e.ma[72] << " " << e.hly[0] << " " << e.hly[1] << " " << e.hly[2]<< endl;
			cout << it->first << setw(5) << " C[" 
				 << e.close  << "]\tg[" 
			     << e.g 	 << "]\th[" 
				 << e.hly[0] << "]\th1[" 
				 << e.hly[1] << "]\th2[" 
				 << e.hly[2] << "]\tR["
				 << e.revenue <<"]" << endl;
		}
	}
}
void stock_t::compute_gravity_all() {
	for (data_map::iterator it = data.begin(); it != data.end(); ++it) {
		int id = it->first;
		compute_gravity_for(id);
	}
}

inline double calculate_delta_value(double p1, double p2, double p3, double p4) { // PS. delta value of distance is velocity
	return -0.3*p1 + -0.1*p2 + 0.1*p3 + 0.3*p4;
}
void stock_t::compute_gravity_for(int id) { // HLY formula
	entity_map &stock_map = data.at(id);
	int t1 = 30;
	int t2 = 72;
	compute_ma_for(id, t1);
	compute_ma_for(id, t2);

	for (entity_map::iterator it = stock_map.begin(); it != stock_map.end(); ++it) {
		entity_t &e = it->second;
		e.hly[0] = (e.ma[t1] + e.ma[t2])/2;
		// compute g
		e.g = (e.close/e.hly[0] - 1)*100;
	}

	//compute velocity:  4 points estimation: -0.3, -0.1, 0.1, 0.3
	// unit %:  percentange change 
	vector<double> points(4, 0.0);
	for (entity_map::iterator it = stock_map.begin(); it != stock_map.end(); ++it) {
		entity_t &e = it->second;
		points[0] = points[1];
		points[1] = points[2];
		points[2] = points[3];
		points[3] = e.hly[0];
		if (points[3] == 0.0 ) {
			e.hly[1] = 100*calculate_delta_value(points[0], points[1], points[2], points[3])/10;
		} else { 
			e.hly[1] = 100*calculate_delta_value(points[0], points[1], points[2], points[3])/e.hly[0];
		}
	}

	//compute acceleration:  4 points estimation: -0.3, -0.1, 0.1, 0.3
	vector<double> vpoints(4, 0.0);
	for (entity_map::iterator it = stock_map.begin(); it != stock_map.end(); ++it) {
		entity_t &e = it->second;
		vpoints[0] = vpoints[1];
		vpoints[1] = vpoints[2];
		vpoints[2] = vpoints[3];
		vpoints[3] = e.hly[1];
		if (vpoints[3] == 0.0 ) {
			e.hly[2] = calculate_delta_value(vpoints[0], vpoints[1], vpoints[2], vpoints[3])/10;
		} else { 
			e.hly[2] = calculate_delta_value(vpoints[0], vpoints[1], vpoints[2], vpoints[3])/e.hly[1];
		}
	}
	
}

void stock_t::compute_ma_for(int id, int t) {
	if (t <= 1) { return; }

	entity_map &stock_map = data.at(id);
	double sum = 0;
	queue<double> qued;

	for (entity_map::iterator it = stock_map.begin(); it != stock_map.end(); ++it) {
		entity_t &e = it->second;
		if (qued.size() >= t) {
			sum -= qued.front();
			qued.pop();
		} 
		sum += e.close;
		qued.push(e.close);
		e.ma[t] = sum/qued.size();
	}

} 

void stock_t::compute_revenue_all() {
	for (data_map::iterator it = data.begin(); it != data.end(); ++it) {
		int id = it->first;
		entity_map &stock_map = data.at(id);
		for (entity_map::iterator it2 = stock_map.begin(); it2 != stock_map.end(); ++it2) {
			compute_revenue_for(id, it2->first, 5);
		}
	}
}

void stock_t::compute_revenue_for(int id, int date, int t) {
	int n = 0;
	double cost = 0;
	double sold = 0;
	vector<double> v1;

	entity_map &stock_map = data.at(id);
	entity_map::iterator start = stock_map.find(date);

	if (start == stock_map.end()) return;

	entity_t &e_start = start->second;
	cost = e_start.close;
		
	for (entity_map::iterator it = start; n != t && it != stock_map.end(); ++it, ++n) {
		if (n == (t-1)) {
			entity_t &e = it->second;
			sold = e.close;
		}
	}

	e_start.revenue = 100*(sold-cost)/cost;
	

}



