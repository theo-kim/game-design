#include "game/controllers/Network.h"
#include "NetworkConnection.h"

int client_socket[8];
int max_clients = 8;

void removesocket(int socket)
{
	int addrlen;
	struct sockaddr_in myaddress;
	for (int i = 0; i < max_clients; i++){
		if (client_socket[i]==socket){
		client_socket[i] = 0;
		getpeername(socket , (struct sockaddr*)&myaddress , \ 
			(socklen_t*)&addrlen); 
		printf("Host disconnected , ip %s , port %d \n" , 
			inet_ntoa(myaddress.sin_addr) , ntohs(myaddress.sin_port)); 
		}
	}
}

// std::string ftos(glm::vec3 banana)
// {
// 	std::string  =  glm::to_string(banana);
// }

void* Connection(void* sock)
{
	char tbuffer[1024];
	int valread;
	int id = 3;
	std::vector<std::string> v = {"10.0f","1.23f","1.23f"};
	//std::string test[5] = {"10.0f","1.23f"};
	//package.pushback("10.0f");
	//vector<float> dv ;
	//std::transform(str.begin(), str.end(), back_inserter(dv), [](const string & astr){ return stod( astr) ; } ) ;
	int socket = *((int *)sock);
	while(socket)
	{
		valread = read(socket , tbuffer, 1024);
		if(valread == 0)
		{
			break;
		}
		printf("Player %d: %s\n",id,tbuffer);
		if(strcmp(tbuffer,"Shape")==0)
		{
			puts("Twighlights");
			for(int i = 0; i < 3; i++)
			{
				send(socket ,&v[i] , sizeof(&v[i]),0);
				printf("%s\n",&v[i]);
				sleep(0.1);
			}
		}
	}
	removesocket(socket);	
	//Close the socket and mark as 0 in list for reuse 
	close( socket ); 
	printf("Socket %d\n",socket);
}

void *start(void *thread) 
{ 
	int opt = true; 
	int PORT = 8008;
	int master_socket , addrlen , new_socket ,  
		activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in myaddress; 
	pthread_t tid[10];
		
	char buffer[1025]; //data buffer of 1K 
	char* ip = "127.0.0.1"; //ip
		
	//set of socket descriptors 
	fd_set readfds; 
		
	//a message 
	char *message = "Server V1.0 \r\n"; 
	
	//initialise all client_socket[] to 0 so not checked 

	// if (argc > 3) {
	// fprintf(stderr, "Too much Input\n");
	// exit(1);
	// }
	// else if (argc == 3) {
	// PORT = atoi(argv[1]);
	// ip = (char*) argv[2];
	// }
	// else if (argc == 2) {
	// PORT = atoi(argv[1]);
	// }

	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if((setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR,(char *)&opt, sizeof(opt)))<0)
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	if(ip==NULL){
        	myaddress.sin_addr.s_addr = INADDR_ANY; // GET ANY FREE IP
	}
	else{
		inet_pton(AF_INET,ip, &myaddress.sin_addr.s_addr); //SET INOUT IP
	}
	myaddress.sin_port = htons( PORT ); 
	myaddress.sin_family = AF_INET;   
 //    myaddress.sin_addr.s_addr = INADDR_ANY;   
 //    myaddress.sin_port = htons( PORT ); 
	puts("NOW BINDING");
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&myaddress, sizeof(myaddress))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(myaddress); 

	puts("Waiting for connections ..."); 
		
	while(true) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&myaddress, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(myaddress.sin_addr) , ntohs(myaddress.sin_port)); 
		
			//send new connection greeting message 
			if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
			{ 
				perror("send"); 
			} 
				
			puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0) 
				{ 
					client_socket[i] = new_socket;
					if(pthread_create(&tid[i], NULL,Connection,&new_socket)!=0)
					{
						perror("FAILED Pthread_d");
					}
					printf("Adding to list of sockets as %d\n" , i); 
					//pthread_join(tid[i],NULL);
					break;
				}

			} 
		} 
		//puts("Hamster Power Increases");
	}
	puts("Server Exiting");
}


int BeginServer(int argc, char const *argv[])
{
	pthread_t thread1;
    int i1;
    i1 = pthread_create( &thread1, NULL, start,(void*) "thread 1");
    pthread_join(thread1,NULL);
	return 0;
}