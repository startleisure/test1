#!/usr/bin/python
import re

pat='abc'
string='123 abc'
match = re.search(pat,string)
print match.group()
