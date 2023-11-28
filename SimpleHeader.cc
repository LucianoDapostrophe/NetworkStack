/* Project Assignment 6
 * Team 3
 *   Alex Wingate
 *   Luc D'ippolito
 *   Juhyoung Park
 */

#include "SimpleHeader.h"

SimpleHeader::SimpleHeader() {
  // silly code, you can do better
  packet = nullptr;
  size = 0;
}

void SimpleHeader::setBuffer(void * buf, int sz) {
  packet = static_cast<uint8_t*> (buf);
  size = sz;
}

void SimpleHeader::setPayloadLength(unsigned int val) {
  if(val > DATA_SZ*8) // if val > DATA_SZ*8 bits, the value set is DATA_SZ
    val = DATA_SZ*8;

  if (packet != nullptr && size > HEADER_SZ) {
    packet[PL] = (val>>8); // shift the integer to right by 8 bits to get the msb
    packet[PL+1] = (val&255); // bitwise AND with 8 LSB bits set to 1.
  }
}

unsigned int SimpleHeader::getPayloadLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[PL+1] | (packet[PL]<<8);
  } else {
    return 0;
  }
}

void SimpleHeader::setType(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // clear the val first
    packet[TYPE] &= 0x3f;

    // set the lowest 2 bits of val to the header field, but do not disturb the other bits
    if (val < 4)
      packet[TYPE] |= (val << 6);
  }
}

unsigned int SimpleHeader::getType() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[TYPE] >> 6);
  } else {
    return 0;
  }
}

void SimpleHeader::setTR(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // clear the val first
    packet[TYPE] &= 0xdf;

    // set the bit of val to the header field, but do not disturb the other bits
    if (val < 2)
      packet[TYPE] |= (val << 5);
  }
}

unsigned int SimpleHeader::getTR() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return ((packet[TYPE] >> 5) & 1);
  } else {
    return 0;
  }
}

void SimpleHeader::setWindow(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // clear the val first
    packet[TYPE] &= 0xe0;

    // set the lowest 5 bits of val to the header field, but do not disturb the other bits
    if (val < 32)
      packet[TYPE] |= val;
  }
}

unsigned int SimpleHeader::getWindow() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[TYPE] & 0x1f);
  } else {
    return 0;
  }
}

void SimpleHeader::setSeqnum(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // set the lowest 8 bits of val to the header field, but do not disturb the other bits
    if (val < 256)
      packet[SEQNUM] = val;
    else
      packet[SEQNUM] = 0;
  }
}

unsigned int SimpleHeader::getSeqnum() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[SEQNUM]);
  } else {
    return 0;
  }
}

void SimpleHeader::setTimestamp(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[TIMESTAMP] = (val >> 24); // shift the integer to right by 24 bits to get the msb
    packet[TIMESTAMP+1] = ((val >> 16) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[TIMESTAMP+2] = ((val >> 8) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[TIMESTAMP+3] = (val & 255); // bitwise AND with 8 LSB bits set to 1.
  }
}

unsigned int SimpleHeader::getTimestamp() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[TIMESTAMP+3]
    | (packet[TIMESTAMP+2] << 8)
    | (packet[TIMESTAMP+1] << 16)
    | (packet[TIMESTAMP] << 24);
  } else {
    return 0;
  }
}

void SimpleHeader::setCRC1(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[CRC1] = (val >> 24); // shift the integer to right by 24 bits to get the msb
    packet[CRC1+1] = ((val >> 16) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[CRC1+2] = ((val >> 8) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[CRC1+3] = (val & 255); // bitwise AND with 8 LSB bits set to 1.
  }
}

unsigned int SimpleHeader::getCRC1() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[CRC1+3]
    | (packet[CRC1+2] << 8)
    | (packet[CRC1+1] << 16)
    | (packet[CRC1] << 24);
  }
  else {
    return 0;
  }
}

void SimpleHeader::setCRC2(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[size-4] = (val >> 24); // shift the integer to right by 24 bits to get the msb
    packet[size-3] = ((val >> 16) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[size-2] = ((val >> 8) & 255); // bitwise AND with 8 LSB bits set to 1.
    packet[size-1] = (val & 255); // bitwise AND with 8 LSB bits set to 1.
  }
}
