#!/usr/bin/env python3 

import cgi
import cgitb
cgitb.enable() 
print("Content-type: text/html\r\n")
print("<html><body>")
form = cgi.FieldStorage()

print ("<table border=1><tr><th>Name<th>Value</tr>")
for names in form:
  print ("<tr><td>", names, "<td>", form[names].value, "</tr>")

print ("</table>")
print("</body></html>")
