#!/usr/bin/python

### fetch google finance historical data ## 

import httplib
import urllib2
import re


def debug_print( s, msg = None):
	print "DEBUG", msg, s
	pass

#  get url 
#  https://www.google.com/finance/historical?q=TPE:2330&startdate=1/31/1986&start=30&num=30
def get_site( stkID = 2330, month = 1, day = 1, year = 1985, start = 0, num = 30):
	return 'https://www.google.com/finance/historical?q=TPE:' + \
				str( stkID ) + '&startdate=' + \
				str(day) + '/' + str(month) + '/' + str(year) + \
				'&start=' + str(start) + '&num=' + str(num)
# testing
#debug_print(get_site(2485, 1, 31, 1986, 0, 200))
#debug_print(get_site())

# target source:
# date, open, high, low, close, volume
#<td class="lm">Jun 8, 2012
#<td class="rgt">77.00
#<td class="rgt">78.70
#<td class="rgt">77.00
#<td class="rgt">77.90
#<td class="rgt rm">34,628,000

pattern = r'<td class="lm">(.+)' +  \
           '\s<td class="rgt">([\d\.]+)' + \
           '\s<td class="rgt">([\d\.]+)' + \
           '\s<td class="rgt">([\d\.]+)' + \
           '\s<td class="rgt">([\d\.]+)' + \
           '\s<td class="rgt rm">([\d\,]+)'
reg_price = re.compile( pattern )

pattern_total_size = r'google\.finance\.applyPagination\(\s+\d+,\s+\d+,\s+(\d+),\s+'
reg_row_size =  re.compile( pattern_total_size )

def write_fetch_data(outf, stock_id = 0, month = 1, day = 31, year = 1986):
    if stock_id < 1000 :
        return None
    # Get web page content
    urlsite = get_site(stock_id, month, day, year, 0, 200)
    content = opener.open( urlsite ).read()

    match_r_sz = reg_row_size.search( content )
    if (match_r_sz is None):
        print "stock_id: "+ str(stock_id) + " not found! "
        return None

    ## write ID SZ line
    row_size = int (match_r_sz.groups()[0])
    id_line = "*ID " + str(stock_id) + " *SZ " + str(row_size) + "\n"
    print id_line
    #outf.write(id_line)
    
    ## page number , 200rows/page
    page_num = row_size/200 + 1;
    print "pages: " + str(page_num)
    page_range = range(page_num)

    for i in page_range:
        start_pos = i*200
        site = get_site(stock_id, month, day, year, start_pos, start_pos+200)
        print "Process page:" + str(i) + " " + site
        cnt = opener.open( site ).read()
        stock_data = reg_price.findall( cnt )
        length = len(stock_data)
        if (length is 0 ):
            print "stock_id: "+ str(stock_id) + " not found! "
            return None
        else:
            print length
            #outf.write(' *LEN ' + str(length))
            #outf.write("\n")

            for i in range(length):
                outf.write(str(stock_data[i]))
                outf.write("\n")

def check_id_from(line):
	reg_id = re.compile(r"\*ID (\d+)")
	match = reg_id.search(line)
	if (match is None):
		return None
	return match.groups()[0]

def check_date_from(line):
	reg_date = re.compile(r"\(\'([\da-zA-Z, ]+)\', ")
	match = reg_date.search(line)
	if (match is None):
		print "Wrong Process!"
		exit("Exit!")

	return match.groups()[0]

def mmStr_to_num(month):
	mmlist = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]

	cnt = 0 
	for ref in mmlist:
		cnt+=1
		if (ref == month):
			return str(cnt)

def write_today_to(outf, idx, date):
	reg_mmddyy = re.compile(r"([a-zA-Z]+) (\d+), (\d+)")
	match = reg_mmddyy.search(date)
	month = mmStr_to_num( match.groups()[0] )
	day = match.groups()[1]
	year = match.groups()[2]
	#msg1 = "write from today to "+ month + "/"+ day + "/" + year + " for ID: "+ idx + "\n"

	write_fetch_data(outf, idx, 5, 31, 2013)
	#outf.write(msg1)
        
    
####################
#### 	Main 	####
####################

# http init
httplib.HTTPConnection.debuglevel = 1
opener = urllib2.build_opener()
opener.addheaders = [('User-agent', 'Mozilla/5.0')]

inFileName = 'stock.old.data'
inf = open(inFileName, 'r')
outFileName = 'stock.data'
outf = open(outFileName, 'w')

line  = inf.readline()

while line:
	idx = check_id_from(line)
	if (idx is not None):
		outf.write(line)

		line = inf.readline()
		date = check_date_from(line)
		write_today_to(outf, idx, date)
		line = inf.readline()
	
	outf.write(line)
	line = inf.readline()

inf.close() 
outf.close


exit("exit ")

#stock_ids = range(9999)
##stock_ids = (1111, 2330, 2485)
#
#print 'Start Fetch!'
#for stock_id in stock_ids:  # fetch all
#
#    write_fetch_data(fileName, stock_id)
#
#    #print "here"


