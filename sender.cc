/* Project Assignment 6
 * Team 3
 *   Alex Wingate
 *   Luc D'ippolito
 *   Juhyoung Park
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <fstream>
#include <zlib.h>
#include "SimpleHeader.h"

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval;
  struct addrinfo hints, *results, *ptr;
  int bufSize = 512;
  char defaultIP[] = "127.0.0.1";
  char defaultPort[] = "12345";
  char defaultFile[] = "sender.cc";

  char *fileName = defaultFile;
  char *ipAddress = defaultIP;
  char *portNumber = defaultPort;
    
  int opt; 
      
  // put ':' in the starting of the string so that program can distinguish between '?' and ':'  
  while((opt = getopt(argc, argv, ":b:f:i:p:")) != -1) {  
    switch(opt)  
    {  
      case 'b':
        bufSize = atoi(optarg);
        if(bufSize > 512)
          bufSize = 512;
        std::cout << "Buffer: " << bufSize <<std::endl;  
        break;
      case 'f':  
        fileName = optarg;
        std::cout << "Filename: " << fileName <<std::endl;   
        break;  
      // case 'i':  
      //   ipAddress = optarg;
      //   std::cout << "IP Address: " << ipAddress <<std::endl;   
      //   break;  
      // case 'p':  
      //   portNumber = optarg;
      //   std::cout << "Port Number: " << portNumber <<std::endl;   
      //   break;  
      case ':':  
        std::cout << "option needs a value\n";
        return 1;  
      case '?':  
        std::cout << "unknown option: " << optopt <<std::endl;  
        return 1;  
    }  
  }  
    
  // optind is for the extra arguments 
  if((optind + 2) == argc) {      
    ipAddress = argv[optind];
    std::cout << "IP Address: " << ipAddress <<std::endl;   
    
    portNumber = argv[optind + 1];
    if((atoi(portNumber) < 1) || (atoi(portNumber) > 65535)) {
      std::cout << "Wrong Port Number: " << portNumber <<std::endl;       
      return 1;
    }

    std::cout << "Port Number: " << portNumber <<std::endl;   
  } 

  
  std::cout << "Buffer: " << bufSize <<std::endl;  
  std::cout << "Filename: " << fileName <<std::endl;   
  std::cout << "IP Address: " << ipAddress <<std::endl;   
  std::cout << "Port Number: " << portNumber <<std::endl;   
  

  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo(ipAddress, portNumber, &hints, &results)) != 0) { // error
    std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
    return 2;
  }

  // loop through the results from the linked list
  for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {  
    if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) != -1) {
      break;  // socket successful, no need to search further
    }
  }

  // but we might reach the end of the list...
  if (ptr == NULL) {
    std::cerr << "Failed to open socket" << std::endl;
    return 3;
  }

 
  // if not, we are ready to send here
  std::ifstream ifi;

  char buf[bufSize];
  int len, bytes_sent;

  // open file in binary mode
  ifi.open(fileName, std::ios::binary);
  if (ifi.fail()) {
    std::cout << "Cannot open file" << std::endl;
    return 4;
  }
  
  if (!ifi.read(buf, sizeof(buf))) {
    std::cout << ifi.gcount() << " bytes read" << std::endl;
    // std::cout << "Error reading" << std::endl;
  }
  else {
    std::cout << ifi.gcount() << " bytes read" << std::endl;
  }

  len = ifi.gcount();

  if(len == 0) {
    std::cout << "File size is ZERO..." << std::endl;
    close(sock);
    ifi.close();
    return 4;
  }

  // Preparing packet
  unsigned int BUFFER_SZ = 16 + len; // Header + Payload + CRC2
  std::cout << "Buffer_SZ: " << BUFFER_SZ << std::endl;
  SimpleHeader *h_;
  uint8_t *buffer_;

  h_ = new SimpleHeader;  // create a new class before each test to start fresh
  buffer_ = new uint8_t[BUFFER_SZ]; // allocate a buffer to test
  h_->setBuffer(buffer_, BUFFER_SZ);  // tell the class to manage the test buffer
  h_->setType(1);
  h_->setTR(0);
  h_->setWindow(1);
  h_->setSeqnum(0);
  h_->setPayloadLength(len);

  // CRC1
  unsigned int crc;
  crc = crc32(0L, NULL, 0);
  crc = crc32(crc, reinterpret_cast<const Bytef*>(buffer_), 8);
  h_->setCRC1(crc);

  // Payload
  memcpy(h_->getPayloadAddress(), buf, len);

  // CRC2
  crc = crc32(0L, NULL, 0);   
  crc = crc32(crc, reinterpret_cast<const Bytef*>(buf), len);
  h_->setCRC2(crc);

  std::cout << "Size: " << h_->getBufferSize() << std::endl;


  // Send
  bytes_sent = sendto(sock, buffer_, h_->getBufferSize(), 0, ptr->ai_addr, ptr->ai_addrlen);
  


  // Receive 
  recvfrom(sock, buf, sizeof(buf)-1, 0, ptr->ai_addr, &ptr->ai_addrlen);



  // Finalize the connection
  h_->setBuffer(buffer_, 16);  // tell the class to manage the test buffer
  h_->setType(1);
  h_->setTR(0);
  h_->setWindow(0);
  h_->setSeqnum(1);
  h_->setPayloadLength(0);

  crc = crc32(0L, NULL, 0);
  crc = crc32(crc, reinterpret_cast<const Bytef*>(buffer_), 8);
  h_->setCRC1(crc);

  // Send
  bytes_sent = sendto(sock, buffer_, h_->getBufferSize(), 0, ptr->ai_addr, ptr->ai_addrlen);

  // Receive
  recvfrom(sock, buf, sizeof(buf)-1, 0, ptr->ai_addr, &ptr->ai_addrlen);


  close(sock);
  ifi.close();

  delete h_;
  delete [] buffer_;

  return 0;
}
