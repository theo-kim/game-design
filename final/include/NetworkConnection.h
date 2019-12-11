#ifndef NETWORKCONN_DEF
#define NETWORKCONN_DEF

#include <stdio.h> 
#include <string.h> //strlen 
#include <vector>
#include <iostream>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

class NetworkConnection {
public:
  void Initialize();
};

#endif