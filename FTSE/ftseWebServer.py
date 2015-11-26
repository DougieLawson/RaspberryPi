#!/usr/bin/python3
from http.server import HTTPServer, CGIHTTPRequestHandler

class Handler(CGIHTTPRequestHandler):

  cgi_directories=["/cgi-bin"]

PORT = 80

httpd = HTTPServer(("", PORT), Handler)

print ("Server running @ ", PORT)
httpd.serve_forever()

