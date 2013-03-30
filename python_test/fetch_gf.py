#!/usr/bin/python

### fetch google finance historical data ## 
### author: Ken Lin ###
## example:
##
import httplib
import urllib2
import re

stock_ids = ( 1111, 2485, 2330, 0)

print 'Hello World'
def displaymatch(match):
    if match is None:
        return None
    return '<Match: %r, groups=%r>' % (match.group(), match.groups())

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
#stock_ids = ( 2485, 2324, 8078, 2311, 2330, 2891, 8926)

opener = urllib2.build_opener()
opener.addheaders = [('User-agent', 'Mozilla/5.0')]


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

reg_price = re.compile( pattern)



for stock_id in stock_ids:
    # Get web page content
    urlsite = get_site(stock_id, 1, 31, 1986, 0, 30)
    debug_print(urlsite)
    content = opener.open( urlsite ).read()
    f = open( str(stock_id)+'.log', 'w')
    stock_data = reg_price.findall( content )
    length = len(stock_data)
    if (length is 0 ):
        print "stock_id: "+ str(stock_id) + " not found! "
        continue
    
    print length 
    for i in range(length):
        print str(stock_data[i])
        f.write(str(stock_data[i]))
        f.write("\n")

    print "here"

    #f.write(stock_data)
    # Print the whole content for debugging
    #print content
    
    #stock_name =  unicode( iRE_name.match( content ).groups()[ 0 ], "BIG5" )
    
    # Print result
    #print "%d\t%.2f" % ( int( stock_id ), \
    #                         float( stock_price ) )
    #print "%d\t%s\t%.2f" % ( int( stock_id ), stock_name, \
    #                         float( stock_price ) )

#raw_input( "Press any key..." )

