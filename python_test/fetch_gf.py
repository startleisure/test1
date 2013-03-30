#!/usr/bin/python

### fetch google finance historical data ## 
### author: Ken Lin ###
## example:
##
import httplib
import urllib2
import re

print 'Hello World'

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

#debug_print(get_site(2485, 1, 31, 1986, 0, 200))
#debug_print(get_site())


httplib.HTTPConnection.debuglevel = 1
stock_ids = ( 2485, 2330)
#stock_ids = ( 2485, 2324, 8078, 2311, 2330, 2891, 8926)

opener = urllib2.build_opener()
opener.addheaders = [('User-agent', 'Mozilla/5.0')]


# For finding stock price
iRE_price = re.compile( r".*nowrap><b>([\d.]+)<.*", \
						re.I | re.U | re.M | re.S )

# pattern: >2330</a><br><a href="/pf/pfsel?stocklist= 
# For finding stock name
# synonym e = r'.*>\d+(.+)</a><br><a href="/pf/pfsel\?stocklist=.*'
e = ".*>\d+" + r'(.+)</a><br><a href="/pf/pfsel\?stocklist=.*'
debug_print( e, "for name" )
iRE_name = re.compile( e, re.I | re.U | re.M | re.S )



for stock_id in stock_ids:
    # Get web page content
    content = opener.open( 'http://tw.stock.yahoo.com/q/q?s=' + \
                           str( stock_id ) ).read()

    # Print the whole content for debugging
    #print content

    stock_price = iRE_price.match( content ).groups()[ 0 ]
    stock_name =  unicode( iRE_name.match( content ).groups()[ 0 ], "BIG5" )

    # Print result
    print "%d\t%.2f" % ( int( stock_id ), \
                             float( stock_price ) )
    #print "%d\t%s\t%.2f" % ( int( stock_id ), stock_name, \
    #                         float( stock_price ) )

#raw_input( "Press any key..." )

