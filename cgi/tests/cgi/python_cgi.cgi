#!/usr/bin/python3

import os
import sys;

if __name__ == '__main__':
	print("ENVIROMENTS:")
	for key in os.environ.keys():
		print("\t", key, "=", os.environ[key])
	body = sys.stdin.read()
	print("Body: ", body)
	print("Body size: ", len(body))
