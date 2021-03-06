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
			cout << it->first << setw(5) 
				 << " C[" 	<< e.close  
				 << "]\th[" << e.hly[0] 
				 << "]\tg[" << e.g 	 
				 << "]\th1["<< e.hly[1] 
				 << "]\th2["<< e.hly[2] 
				 << "]\tR[" << e.revenue 
				 <<"]" << endl;
		}
	}
}
void stock_t::compute_gravity_all() {
//	stkobj.compute_gravity_for(1101);
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
			e.hly[2] = calculate_delta_value(vpoints[0], vpoints[1], vpoints[2], vpoints[3]);
		} else { 
			e.hly[2] = calculate_delta_value(vpoints[0], vpoints[1], vpoints[2], vpoints[3]);
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
void stock_t::create_box_system() {
	if (data.size() == 0) {
		cout << "no data to create box system" << endl;
		return;
	}
	//double gl , gr, h1l, h1r, h2l, h2r; // chtest to find range of data
	//gl = gr = h1l = h1r = h2l = h2r = 0.0;
	int datasize = 0;
	for (data_map::iterator it = data.begin(); it != data.end(); ++it) {
		entity_map &stock_map = it->second;
		datasize += stock_map.size();
		for (entity_map::iterator it2 = stock_map.begin(); it2 != stock_map.end(); ++it2) {
			entity_t &e = it2->second;
			double g = e.g, h1 = e.hly[1], h2 = e.hly[2];
			//double g = 0.0, h1 = 0.0, h2 = e.hly[2];
			bool has_near_cell = false;
			
			for (vector<cell_t>::iterator itc = boxsys.begin(); itc != boxsys.end(); ++itc) {
				if( has_near_cell = itc->is_near(g, h1, h2) ) 
					break;
			}
			if (!has_near_cell)  {
				boxsys.push_back( cell_t(g, h1, h2) );
			} else  {
				//cout << "alredy have excited "<< counter << " cell! " << it2->first <<" "<< e.g << " " << e.hly[1] << " " << e.hly[2] << " excite: " << sum_of_excite << endl;
			}
			//if (e.g < gl ) gl = e.g;  // chtest to find range of data
			//if (e.g > gr ) gr = e.g;
			//if (e.hly[1] < h1l) h1l = e.hly[1];
			//if (e.hly[1] > h1r) h1r = e.hly[1];
			//if (e.hly[2] < h2l) h2l = e.hly[2];
			//if (e.hly[2] > h2r) h2r = e.hly[2];
		}
		//cout << "data  "<< it->first<< "range: g[" << gl << " , " << gr << "] h1[" << h1l << " , " << h1r << "] h2[" << h2l << " , " << h2r << "]" << endl;
		cout << "create_box_system: sz= "<< boxsys.size() << " for data sz: " << datasize << endl;
	}
}

void stock_t::box_training() {
	set<double> errset;
	double tratio = 0.4;
	double ref = 0.0;
	int n = 0; 
	double error = 1.0;
	double err_min = 100.0;
	double err_max = 0.0;
	double err_sum = 0.0;
	double err_sum_d = 1.0;
	double err_sum_p = 0.0;
	double err_med = 1000.0;
	double err_95 = 1000.0;
	int err_num = 0;
	int accept_num = 0;
	if (data.size() == 0) {
		cout << "no data to train" << endl;
		return;
	}

	// training
	//while ( n < 1 && error > 0.01) {
	while ( n < 100  && err_95 > 0.1 && abs(err_sum_d) > 0.000001) {
		err_sum_p = err_sum;
		errset.clear();
		err_sum = err_max = 0.0;
		err_min = 100.0;
		err_num = accept_num = 0;
		++n;
		for (data_map::iterator it = data.begin(); it != data.end(); ++it) { // ID, entity_map
			entity_map &stock_map = it->second;
			entity_map::iterator t1 = stock_map.begin(); 
			entity_map::iterator t2 = stock_map.end(); 
			for (int i = 0; i!= 3; ++i) 
				t1++;
			for (int i = 0; i!= 5; ++i) 
				t2--;
			
			for (entity_map::iterator it2 = t1; it2 != t2; ++it2) { // DATE, entity
				entity_t &e = it2->second;
				entity_map::iterator next = it2; next++;
				if (next == stock_map.end()) { break;}
				double g = e.g, h1 = e.hly[1], h2 = e.hly[2];
				double ref = next->second.revenue;
				//if (ref <= -99.0 ) { break;}
				double boxout = 0.0;
				useCell_t useCell;
				for (vector<cell_t>::iterator itc = boxsys.begin(); itc != boxsys.end(); ++itc) { // traverse box system
					double excite = itc->get_excite_value(g, h1, h2);
					if (excite > 0.3) {
						boxout += (itc->get_weight() * excite);
						useCell.push_back(make_pair(excite, itc));
					}
				}
				error = boxout - ref;
				error_correct(useCell, error, tratio);
				//if (abs(error) < 0.1 && boxout > 2.0) {
				errset.insert(abs(error));
				if (abs(error) < 0.1 ) {
					accept_num ++;
					//cout << boxout << " -  " << ref << " = " << error <<endl; 
					//cout << "DATE: " << it2->first << " g[" << g << "] h1[" << h1 << "] h2[" << h2 << "]" << endl;
				}
				//if (abs(error) < 20 ) { 
				//	cout << boxout << " -  " << ref << " = " << error <<endl; 
				//	cout << "DATE: " << it2->first << " g[" << g << "] h1[" << h1 << "] h2[" << h2 << "]" << endl;
				//}
				if (abs(error) < abs(err_min)) { err_min  = error;}
				if (abs(error) > abs(err_max)) { err_max  = error;}
				++err_num;
				err_sum += abs(error);
			}
		}
		err_sum = err_sum/((double)err_num);
		err_sum_d = err_sum - err_sum_p;
		if (err_num != 0) { 
			cout << n << " average of error: " <<  err_sum << " N: "<< err_num << " accept " <<  accept_num  << endl;
			cout << "min max error " << err_min << " , " << err_max <<endl;
			vector<double> tmp;
			for (set<double>::const_iterator  it = errset.begin(); it != errset.end(); ++it) {
				tmp.push_back(*it);	
			}
			err_med = tmp[errset.size()/2];
			err_95 = tmp[errset.size()*0.95];
			cout << "50% error " << tmp[errset.size()/2] <<endl;
			cout << "80% error " << tmp[errset.size()*0.80] <<endl;
			cout << "95% error " << tmp[errset.size()*0.95] <<endl;
		}
	}
	cout <<"err_sum_d" << err_sum_d << endl; 

}
void stock_t::box_testing() {
	entity_t &e = data[4919][20130531];
	cout << e.g << " " << e.hly[1] << endl;
	double boxout = 0.0;
	for (vector<cell_t>::iterator itc = boxsys.begin(); itc != boxsys.end(); ++itc) { // traverse box system
		double excite = itc->get_excite_value(e.g, e.hly[1], e.hly[2]);
		boxout += (itc->get_weight() * excite);
	}

	cout << "output" << boxout << endl;
	

}

void stock_t::error_correct(useCell_t &useCell, double err, double ratio) {
	for (useCell_t::iterator it = useCell.begin(); it != useCell.end() ; ++it) {
		double excite = it->first;
		cell_t &cell = *(it->second);
		// error back propagation
		cell.get_weight() += (-excite * err * ratio);
	}
}

void stock_t::print_boxsys() {
	typedef map<cell_t::dim_type, set<double> > mapset;
	mapset mycenter;
	mapset myradius;
	for (vector<cell_t>::iterator it = boxsys.begin(); it != boxsys.end(); ++it) {
		cell_t::coord center = it->get_center();
		cell_t::coord radius = it->get_radius();
		for (cell_t::coord::iterator it2 = center.begin(); it2 != center.end(); ++it2)
			mycenter[it2->first].insert(it2->second);
		for (cell_t::coord::iterator it2 = radius.begin(); it2 != radius.end(); ++it2)
			myradius[it2->first].insert(it2->second);
	}

	cout << "cell center print: "<< endl;
	for(mapset::iterator it = mycenter.begin(); it != mycenter.end(); ++it) {
		if (it->first == cell_t::G) { cout << "G: " << endl;}
		if (it->first == cell_t::H1) { cout << "H1: " << endl;}
		if (it->first == cell_t::H2) { cout << "H2: " << endl;}
		for (mapset::mapped_type::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			cout << *it2 << " " ;
		}
		cout << endl;
	}
	cout << "cell radius print: "<< endl;
	for(mapset::iterator it = myradius.begin(); it != myradius.end(); ++it) {
		if (it->first == cell_t::G) { cout << "G: " << endl;}
		if (it->first == cell_t::H1) { cout << "H1: " << endl;}
		if (it->first == cell_t::H2) { cout << "H2: " << endl;}
		for (mapset::mapped_type::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			cout << *it2 << " " ;
		}
		cout << endl;
	}

}
bool stock_t::restore_boxsys(const char *fileName) {
	double w, g, h1, h2, rg, rh1, rh2;
	ifstream ifs;
	string token;
	ifs.open(fileName);
	if (!ifs.good()) { return false;}
	ifs >> token;
	while(ifs.good()) {
		w = atof(token.c_str());
		ifs >> token;
		g = atof(token.c_str());
		ifs >> token;
		h1 = atof(token.c_str());
		ifs >> token;
		h2 = atof(token.c_str());
		ifs >> token;
		rg = atof(token.c_str());
		ifs >> token;
		rh1 = atof(token.c_str());
		ifs >> token;
		rh2 = atof(token.c_str());
		ifs >> token;
		boxsys.push_back( cell_t(w, g, h1, h2, rg, rh1, rh2) );
	}
	ifs.close();

	return true;
}
void stock_t::save_boxsys(const char *fileName) {
	if (fileName == NULL) {
		cout << "Empty fileName !" << endl;
		return;
	}
	ofstream ofs; 
	ofs.open(fileName);

	typedef map<cell_t::dim_type, set<double> > mapset;
	for (vector<cell_t>::iterator it = boxsys.begin(); it != boxsys.end(); ++it) {
		ofs << it->get_weight() << endl;
		cell_t::coord center = it->get_center();
		cell_t::coord radius = it->get_radius();
		for (cell_t::coord::iterator it2 = center.begin(); it2 != center.end(); ++it2) {
			ofs << it2->second;
			ofs << " ";
		}
		for (cell_t::coord::iterator it2 = radius.begin(); it2 != radius.end(); ++it2) {
			ofs << it2->second;
			ofs << " ";
		}
		ofs << endl;
	}
	ofs.close();
}

bool cell_t::is_near(double g, double h1, double h2){
	// review completed
	double d1 = abs(g - center[G]);
	double d2 = abs(h1 - center[H1]);
	double d3 = abs(h2 - center[H2]);
	if (d1 > dratio*radius[G] || d2 > dratio*radius[H1] || d3 > dratio*radius[H2]) { // not near the center 
		return false; 
	}
	return true;
}
double cell_t::get_excite_value(double g, double h1, double h2){
	static const double exp = 1.0/3.0;
	double d1 = abs(g - center[G]);
	double d2 = abs(h1 - center[H1]);
	double d3 = abs(h2 - center[H2]);
	if (d1 > radius[G] || d2 > radius[H1] || d3 > radius[H2]) { // if outside the cell
		return 0.0;
	}
	double base =(1- d1/radius[G]) * (1- d2/radius[H1]) * (1- d3/radius[H2]);
	double result = pow(base, exp);

	return result;
}



