// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <string.h> //strlen 
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8008

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    std::vector<std::string> v = {"Shape","Moment","Mass","Position",
        "Velocity","Acceleration","Orientation","AngularVelocity","AngularAcceleration"};
    //Shape in m,r,h(maybe)
    //Moment Moment(float value, Length::Unit l, Mass::Unit m);
    //Mass large k
    //Position Position(glm::vec3 _value, Length::Unit _unit);
    //velocity velocity(glm::vec3 value, Length::Unit d, Time::Unit t);
    //Acceleration Acceleration(glm::vec3 v, Length::Unit l, Time::Unit t);
    //Orientation Orientation(glm::vec3 value);
    //Angular Veolcity AngularVelocity(glm::vec3 value, Time::Unit timeUnit);
    //AngularAcceleration(glm::vec3 value, Time::Unit timeUnit);

    //{1,}


    char *Orientation = "Shape";

    // stringstream ss("1,231,231,23,1,23123,SS,1,31,3,13,13,13,1,313");
    // std::vector<float> results;

    // while(ss.good()){
    //     std::string temp;
    //     getline(ss,temp,',');
    //     results.push_back(temp)
    // }


    valread = read( sock , buffer, 1024); //Welcome Message
    printf("%s\n",buffer ); 
    send(sock , Orientation , strlen(Orientation),0);
    valread = read( sock , buffer, 1024);
    float x = std::stof(buffer);
    valread = read( sock , buffer, 1024);
    float y = std::stof(buffer);
    valread = read( sock , buffer, 1024);
    float z = std::stof(buffer);
    printf("x:%f,y:%f,z:%f\n", x,y,z);

        //Orientation(x,y,z);

    return 0;
}
