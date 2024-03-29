

/*The MIT License (MIT)

Copyright (c) 2014 Nathanaël Lécaudé
https://github.com/natcl/Artnet, http://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef ARTNET_H
#define ARTNET_H
//#define CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM  64
#include <Arduino.h>
//#include "FastLED.h"
#if defined(ARDUINO_SAMD_ZERO)
#include <WiFi101.h>
#include "UdpArtnet.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include "UdpArtnet.h"
//#include "WifiUdp.h"
#elif defined(ESP32)
#include <WiFi.h>
#include "UdpArtnet.h"
//#include "WifiUdp.h"
#else
#include <Ethernet.h>
#include <EthernetUdp.h>
#endif
#include <Udp.h>
// UDP specific
#define ART_NET_PORT 6454
// Opcodes
#define ART_POLL 0x2000
#define ART_POLL_REPLY 0x2100
#define ART_DMX 0x5000
#define ART_SYNC 0x5200
// Buffers
#define MAX_BUFFER_ARTNET 800
// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18
#include <TridentTD_EasyFreeRTOS32.h>
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

//static QueueHandle_t _artnet_queue;
  

struct artnet_reply_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  ip[4];
  uint16_t port;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  subH;
  uint8_t  sub;
  uint8_t  oemH;
  uint8_t  oem;
  uint8_t  ubea;
  uint8_t  status;
  uint8_t  etsaman[2];
  uint8_t  shortname[18];
  uint8_t  longname[64];
  uint8_t  nodereport[64];
  uint8_t  numbportsH;
  uint8_t  numbports;
  uint8_t  porttypes[4];//max of 4 ports per node
  uint8_t  goodinput[4];
  uint8_t  goodoutput[4];
  uint8_t  swin[4];
  uint8_t  swout[4];
  uint8_t  swvideo;
  uint8_t  swmacro;
  uint8_t  swremote;
  uint8_t  sp1;
  uint8_t  sp2;
  uint8_t  sp3;
  uint8_t  style;
  uint8_t  mac[6];
  uint8_t  bindip[4];
  uint8_t  bindindex;
  uint8_t  status2;
  uint8_t  filler[26];
} __attribute__((packed));

class Artnet
{
public:
  Artnet();
    static void artnet_task(void *pvParameters);
    SemaphoreHandle_t Artnet_Semaphore2 = xSemaphoreCreateBinary();
    uint16_t last_size=0;
    uint32_t getsync();
  bool running=false;
     uint8_t * getframe(int framenumber);
    uint8_t * getframe();
    void getframe(uint8_t* leds);
    void getframe2( uint8_t* leds);
    void getframe3( uint8_t* leds);
    uint8_t * getframeread(int buffer);
    uint32_t nbframeread;
    uint32_t frameslues=0;
    uint32_t lostframes=0;
    void resetsync();
    //void setframe(CRGB * frame);
  void begin(byte mac[], byte ip[]);
   void begin();
    void begin(uint16_t nbPixels,uint16_t nbPixelsPerUniverses);
  void setBroadcast(byte bc[]);
  uint16_t read();
     uint16_t read2();
    uint16_t read2(TaskHandle_t task);
    uint16_t read(TaskHandle_t task);
  void printPacketHeader();
  void printPacketContent();
    void stop();
    
 uint32_t sync=0;
    uint32_t syncmax=0;
    uint32_t sync2=0;
    uint32_t syncmax2=0;
    uint32_t elaspe[2];
    uint32_t start_time=0;
    uint32_t current_time=0;
    uint32_t getElaspseTime();
  // Return a pointer to the start of the DMX data
  inline uint8_t* getDmxFrame(void)
  {
    return artnetPacket + ART_DMX_START;
  }

  inline uint16_t getOpcode(void)
  {
    return opcode;
  }

  inline uint8_t getSequence(void)
  {
    return sequence;
  }

  inline uint16_t getUniverse(void)
  {
    return incomingUniverse;
  }

  inline uint16_t getLength(void)
  {
    return dmxDataLength;
  }

  inline IPAddress getRemoteIP(void)
  {
    return remoteIP;
  }

  inline void setArtDmxCallback(void (*fptr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP))
  {
    artDmxCallback = fptr;
  }

  inline void setArtSyncCallback(void (*fptr)(IPAddress remoteIP))
  {
    artSyncCallback = fptr;
  }

private:
  uint8_t  node_ip_address[4];
  uint8_t  id[8];
  #if defined(ARDUINO_SAMD_ZERO) || defined(ESP8266) || defined(ESP32)
    WiFiUDPArtnet Udp;
    //WiFiUDP Udp;
  #else
    EthernetUDP Udp;
  #endif
  struct artnet_reply_s ArtPollReply;
    
    uint16_t nbPixelsPerUniverse;
    uint16_t nbPixels;
    uint16_t nbNeededUniverses;
  uint8_t *artnetleds1;
    uint8_t *artnetleds2;
    uint8_t *artnetleds3;
    uint8_t *currentframe;
    uint8_t *frames[10];
  uint8_t artnetPacket[MAX_BUFFER_ARTNET];
  uint16_t packetSize;
  IPAddress broadcast;
  uint16_t opcode;
  uint8_t sequence;
  uint16_t incomingUniverse;
  uint16_t dmxDataLength;
    uint8_t currentframenumber;
    uint8_t buffernum;
    uint8_t readbuffer;
  IPAddress remoteIP;
  void (*artDmxCallback)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP);
  void (*artSyncCallback)(IPAddress remoteIP);
};

#endif
