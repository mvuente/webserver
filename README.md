# webserver

Here I need to write an HTTP server in C++ 98.
C-functions allowed are:
 htons, htonl, ntohs, ntohl,
select, poll, epoll, kqueue, socket, accept,
listen, send, recv, bind, connect, inet_addr,
setsockopt, getsockname, fcntl. 
No external C++ library, like Boost, etc..
The server must be compatible with the web browser of my choice (Mozilla).
There will be considered that Nginx is HTTP 1.1 compliant and may be used to compare
headers and answer behaviors.
I need at least GET, POST, and DELETE methods.

References:
https://datatracker.ietf.org/doc/html/rfc7231
http://rsdn.org/article/unix/sockets.xml

IMPORTANT: files CGI_TESTER, TESTER, folders YOUPIBANANE are for testing procedure, provided by school. To pass this test, you need compile the project, launch webserv binary and launch tester binary. Next you need to follow instructions.
