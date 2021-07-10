#!/bin/bash

export REQUEST_METHOD=POST
export SERVER_PROTOCOL=HTTP/1.1
export PATH_INFO=/ubuntu_cgi_tester
export REQUEST_URI=http://localhost:4000/ubuntu_cgi_tester
export CONTENT_LENGTH=""
export HTTP_x_secret_header_for_test=1

./cgi_tester
echo