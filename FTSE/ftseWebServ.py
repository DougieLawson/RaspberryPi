#!/usr/bin/python3
# 
'''
    Copyright 2014, Dougie Lawson, all rights reserved.

    This file is part of FTSE Display.

    FTSE Display  is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. 
''' 
#

import os
from http.server import HTTPServer, CGIHTTPRequestHandler

class Handler(CGIHTTPRequestHandler):

  cgi_directories=["/cgi-bin"]

PORT = 1337 

httpd = HTTPServer(("", PORT), Handler)
os.chdir('/home/pi/python/ftse')

print ("Server running @ ", PORT)
httpd.serve_forever()

