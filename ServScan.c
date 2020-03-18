#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
     unsigned short int port;
     int socketnr;
     int laenge;
     struct sockaddr_in adressinfo;
     int outcome;
     srand(time(0));

     while (1) {
          int result = getopt(argc, argv, "hp:");
          if (result == -1) break;
          switch (result) {
               case '?':
                    return(0);
                    break;
               case ':':
                    fprintf(stderr, "missing argument.\n");
                    break;
               case 'h':
                    printf("Usage: ServScan -p [PORT]\n");
                    return(0);
                    break;
               case 'p':
                    port = atoi(optarg);
                    break;
               default:
                    break;
        }
    }
    printf("===================================================================\n");
    printf("Written by Sten <del3r> Heimbrodt.\nStarting to generate random ip adresses and try to connect to them.\nThe connecting port is %d\n", port);
    printf("===================================================================\n");
    while(1) {
         char ip_adresse[17];

          int ip1 = rand() % 254+1, ip2 = rand() % 254+1, ip3 = rand() % 254+1, ip4 = rand() % 254+1;
          sprintf(ip_adresse, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);

          printf("Built %s and trying to connect on port %d:   ", ip_adresse, port);
          socketnr = socket(AF_INET, SOCK_STREAM, 0);
          adressinfo.sin_family = AF_INET;

          struct timeval timeout;
          timeout.tv_sec = 1;
          timeout.tv_usec = 0;

         if (setsockopt (socketnr, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
             perror("setsockopt failed\n");

         if (setsockopt (socketnr, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
             perror("setsockopt failed\n");

          inet_pton(AF_INET, ip_adresse, &adressinfo.sin_addr.s_addr);
          adressinfo.sin_port = htons(port);
          laenge = sizeof(adressinfo);

          outcome = connect(socketnr, (struct sockaddr*)&adressinfo, laenge);
          if(outcome == 0) {
               printf("CONNECT! IP is %s\n", ip_adresse, port);
          } else {
               printf("Deadend...\n");
          }
          close(socketnr);
          memset(ip_adresse, 0, sizeof(ip_adresse));
     }

    return 0;
}
