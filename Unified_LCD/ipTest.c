/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>

char *
get_MAC(char ifname[IFNAMSIZ])
{
    #define HWADDR_len 6
    int s,i;
    struct ifreq ifr;
    char *MAC_str = malloc(13);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFHWADDR, &ifr);
    for (i=0; i<HWADDR_len; i++)
        sprintf(&MAC_str[i*2],"%02X",((unsigned char*)ifr.ifr_hwaddr.sa_data)[i]);
    MAC_str[12]='\0';
    return MAC_str;
}

char *
getIPaddr ()
{

  struct ifaddrs *interfaceArray = NULL, *tempIfAddr = NULL;
  void *tempAddrPtr = NULL;
  int rc = 0;
  char addressOutputBuffer[INET6_ADDRSTRLEN];
  char *ifaceIP = malloc (40);

  rc = getifaddrs (&interfaceArray);	/* retrieve the current interfaces */
  if (rc == 0)
    {
      for (tempIfAddr = interfaceArray; tempIfAddr != NULL;
	   tempIfAddr = tempIfAddr->ifa_next)
	{
	  if (tempIfAddr->ifa_addr->sa_family == AF_INET)
	    {
	      tempAddrPtr =
		&((struct sockaddr_in *) tempIfAddr->ifa_addr)->sin_addr;

	      if (strncmp (tempIfAddr->ifa_name, "lo", 2))	/* interface ISN'T loopback */
		{ 
		  char *macaddr = get_MAC(tempIfAddr->ifa_name);
		  inet_ntop (tempIfAddr->ifa_addr->sa_family, tempAddrPtr,
			     addressOutputBuffer,
			     sizeof (addressOutputBuffer));
		  sprintf (ifaceIP, "%s %s %s\0", tempIfAddr->ifa_name,
			   addressOutputBuffer, macaddr);
		  free(macaddr);
		}
	    }
	}

      freeifaddrs (interfaceArray);	/* free the dynamic memory */
      interfaceArray = NULL;	/* prevent use after free  */
    }
  else
    {
      printf ("getifaddrs() failed with errno =  %d %s \0",
	      errno, strerror (errno));
    }

  return ifaceIP;
}

int
main ()
{

  char *ifaceIP = getIPaddr ();

  printf("IP: %s\n", ifaceIP);
  free(ifaceIP);

  return 0;
}
