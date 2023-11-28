/* Project Assignment 6
 * Team 3
 *   Alex Wingate
 *   Luc D'ippolito
 *   Juhyoung Park
 */

#ifndef __SIMPLEHEADER_H
#define __SIMPLEHEADER_H

#include <cstdint>

#define DATA_SZ 512    // maximum bytes for the payload 
#define HEADER_SZ 12    // size of header
#define TYPE 0          // start index of Type header field
#define SEQNUM 1        // start index of Sequence Number header field
#define PL 2            // start index of the payload length field
#define TIMESTAMP 4     // start index of Timestamp field
#define CRC1 8          // start index of CRC1 field
// #define PAYLOAD 12      // start index of Payload field


struct simplepacket {
  uint8_t header[HEADER_SZ];
  char data[DATA_SZ];  // payload
  unsigned long crc2;
};

//  01 2 3 7 8 15 16 23 24 31
// +--+-+---+----+-----+-----+
// |Ty|T|Win|Seq |  Payload  |
// |pe|R|dow| num|   Length  |
// +--+-+---+----+-----+-----+
// |   Timestamp (4 bytes)   |
// +-------------------------+
// |     CRC1 (4 bytes)      |
// +-------------------------+
// | Payload (max 512 bytes) |
// .                         .
// .                         .
// .                         .
// +-------------------------+
// |CRC2 (4 bytes, optional) |
// +-------------------------+
// NOTE: this class does not allocate memory for the packet buffer. Memory for the buffer must be already allocated by the calling code.

class SimpleHeader {
private:
  uint8_t * packet;  // address of the buffer
  int size = 0;          // size of buffer in bytes


public:
  // default constructor, packet buffer address set to NULL
  SimpleHeader();

  // assigns an externally allocated buffer to the class; the set/get member functions will manage the headers fields for this buffer
  // buf = the address of the buffer
  // sz = the size in bytes of the buffer
  void setBuffer(void * buf, int sz);


  // sets the value of the payload length
  // val = length; if val > DATA_SZ, the value set is DATA_SZ
  // if buffer address is NULL, does nothing.
  void setPayloadLength(unsigned int val);

  // returns the length of the payload
  // if buffer address is NULL, returns 0
  unsigned int getPayloadLength() const;

  // sets the Type field of the header.
  // If the val is not between 0..3, the value set is 0
  // if buffer address is NULL, does nothing
  void setType(unsigned int val);

  // returns the Type field of the header. Must be between 0..3 since
  // it is a 2 bit value
  // if buffer address is NULL, returns 0
  unsigned int getType() const;

  // sets the TR field of the header.
  // If the val is not 0 or 1, the value set is 0
  // if buffer address is NULL, does nothing
  void setTR(unsigned int val);

  // returns the TR field of the header. Must be 0 or 1 since
  // it is a 1 bit value
  // if buffer address is NULL, returns 0
  unsigned int getTR() const;

  // sets the Window field of the header.
  // If the val is not between 0..31, the value set is 0
  // if buffer address is NULL, does nothing
  void setWindow(unsigned int val);

  // returns the Window field of the header. Must be between 0..31 since
  // it is a 5 bit value
  // if buffer address is NULL, returns 0
  unsigned int getWindow() const;

  // sets the Seqnum field of the header.
  // If the val is not between 0..255, the value set is 0
  // if buffer address is NULL, does nothing
  void setSeqnum(unsigned int val);

  // returns the Seqnum field of the header. Must be between 0..255 since
  // it is a 8 bit value
  // if buffer address is NULL, returns 0
  unsigned int getSeqnum() const;

  // sets the Timestamp field of the header.
  // if buffer address is NULL, does nothing
  void setTimestamp(unsigned int val);

  // returns the Timestamp field of the header.
  // if buffer address is NULL, returns 0
  unsigned int getTimestamp() const;

  // sets the CRC1 field of the header.
  // if buffer address is NULL, does nothing
  void setCRC1(unsigned int val);

  // returns the CRC1 field of the header.
  // if buffer address is NULL, returns 0
  unsigned int getCRC1() const;






  // returns the size of the packet in bytes, including headers and data
  // to be used with recvfrom() or sendto()
  unsigned int getBufferSize() const {
    return size;
  }

  // returns pointer to the structure holding the thePacket, including the headers
  // To be used with recvfrom or sendto
  void * getBufferAddress() {
    return packet;
  }

  // returns a pointer to the first byte of the payload from the packet buffer;
  // returns NULL if the buffer is NULL
  void * getPayloadAddress() {
    if (packet != nullptr && size > HEADER_SZ) {
      return packet+HEADER_SZ;
    } else {
      return nullptr;
    }
  }

  void * getCRC2Address() {
    if (packet != nullptr && size > HEADER_SZ) {
      return packet+size-4;
    } else {
      return nullptr;
    }
  }

  void setCRC2(unsigned int val);
};

#endif
