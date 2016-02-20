
#ifndef  __COMMON_H__
#define  __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>   
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <sys/mman.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "config.h"
#include "shttp_signal.h"
#include "shttp_socket.h"
#include "shttp_process.h"
#include "shttp_epoll.h"
#include "shttp_request.h"
#include "shttp_response.h"
#include "shttp_log.h"
#include "shttp_lock.h"

#endif

