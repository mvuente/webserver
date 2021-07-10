#!/usr/bin/python3

import os

template = '''<!DOCTYPE html>
<html>
<body>
<h1 style="text-align:center">%s</h1>
%s
</body>
</html>'''

contentType = 'Content-type: text/html; charset=utf-8'
contentLength = 'Content-length: %s'

def decode(_str):
	return _str

def mprint(*args, **pargs):
	print(*args, end='\r\n', **pargs)

def printContentLength(htmlPageSize):
	print(contentLength % len(htmlPageSize), end='\r\n\r\n')

def getParam(body, key):
	keypos = body.find(key)
	rval = body[keypos + len(key) : ]
	valend = rval.find('&')
	return decode(rval[:valend])

def notAllowedRequest():
	title = 'METHOD NOT ALLOWED'
	html_page = template % (title, '')
	mprint('405', title)
	mprint(contentType)
	printContentLength(len(html_page))
	print(html_page)

def requestGET():
	title = 'GET YOUR NAME'
	form = '''<form method="POST" action="" enctype="application/x-www-form-urlencoded">
		<input name="name" type="text" style="display: block; margin:auto; margin-bottom: 20px;"/>
		<input name="surname" type="text" style="display: block; margin:auto; margin-bottom: 20px;"/>
		<input name="send" type="submit" style="display: block; margin:auto;"/>
	</form>'''
	html_page = template % (title, form)
	mprint(contentType)
	printContentLength(html_page)
	print(html_page)

def requestPOST():
	body = os.sys.stdin.read()
	title = 'YOUR NAME'
	name_block = '''<div style="text-align: center;">
		<p style="font-size: 28pt;"><span>%s </span><span>%s</span></p>
	</div>''' % (getParam(body, 'name'), getParam(body, 'surname'))
	html_page = template % (title)
	mprint(contentType)
	printContentLength(html_page)
	print(html_page)

if __name__ == '__main__':
	env = os.environ
	method = env.get('REQUEST_METHOD')

	print('HTTP/1.1', end=' ')
	if method in ['GET', 'POST']:
		mprint('200 OK')
		if method == 'GET':
			requestGET()
		else:
			requestPOST()
	else:
		notAllowedRequest()