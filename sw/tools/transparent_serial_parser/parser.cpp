/*
 * Copyright (C) 2010 Eric Parsonage
 *
 * file    : parser.cpp
 * purpose : defines the entry point for a parser that outputs
 *           paparazzi format messages in human readable form.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * it is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this code; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void HANDLER_DEFAULT(unsigned char *payload, unsigned char length);

// this file includes the message handlers that actually output the messages
// in human readable form. This file is generated from the matlab code
// gencode.m
#include "handlers_datalink.h"


/* the basic form of a paparazzi
   message
   message format: ABCxxx...xxxxDE
   A PPRZ_START (0x99)
   B LENGTH (A->E)
   C PPRZ_DATA
     0 SENDER_ID
     1 MSG_ID
     2 MSG_PAYLOAD
     . DATA defined in messages.xml
   D PPRZ_CHECKSUM_A (sum[B->C])
   E PPRZ_CHECKSUM_B (sum[ck_a])
*/

/* state machine states */

#define START               0x99
#define UNINIT              0
#define GOT_START           1
#define GOT_LENGTH          2
#define GOT_PAYLOAD         3
#define GOT_CRC1            4
#define PAYLOAD_LENGTH 256
#define PPRZ_DATA_OFFSET 2


unsigned char payload[PAYLOAD_LENGTH];
volatile unsigned char payload_length;
unsigned char errors = 0;
unsigned int messages_count = 0;
int bytes_read = 0;


void HANDLER_DEFAULT(unsigned char *payload, unsigned char length)
{
    /* does nowt */
}

void parse(unsigned char c)
{
  static unsigned char status = UNINIT;
  static unsigned char chk_1, chk_2, payload_index, i;

  switch (status) {
  case UNINIT:
    if (c == START) // 0x99 indicates the start of a message
    {
      status++;     // we have syncronisation move to next state
    }
    break;

  case GOT_START:         // the first byte of a message contains the length
                          // of the message.
    payload_length = c-4; // subtract 4 for START, LENGTH and CRC1 and CRC2
    chk_1 = chk_2 = c;    // start calculating checksums
    status++;             // move to the next state
    payload_index = 0;    // reinitialise payload counter
    break;

  case GOT_LENGTH:               // now we are receiving the payload
    payload[payload_index] = c;
    chk_1 += c; chk_2 += chk_1;
    payload_index++;
    if (payload_index == payload_length)
      status++;                  // all the payload has been received
    break;                       // move to the next state

  case GOT_PAYLOAD:
    if (c != chk_1){             // does the first checksum tally ?
       errors++;
       status = UNINIT;
       break;
    }
    status++;
    break;

  case GOT_CRC1:
    if (c != chk_2)              // does the second checksum tally ?
    {
       errors++;
       status = UNINIT;
       break;
    }
    messages_count++;
    status = UNINIT;

    // output the basic message information and hex dump of payload.
    printf("Sender ID <%02d> : ", payload[0]);
    printf("Message # <%02d> : ", payload[1]);
    printf("Payload <");
    for (i=2; i<payload_length; i++)
      printf("%02X", payload[i]);
    printf("> : ");
    printf("Checksum <%02X%02X>\n", chk_1, chk_2);

    // now call the generated message handlers to
    // get a human readable form of the message
    (*funcArray[payload[1]])(payload + 2, payload_length);

  }
}



// The entry point called from the command line with a single parameter
// the name of the input file which can also be a com port thus enabling
// real time parsing of a message stream from papaprazzi
int main(int argc, char* argv[])
{
    int fd;
    unsigned char c;
    // open the input file for binary read
    fd = open(argv[1], O_RDONLY);
    if(fd < 0)
    {
        perror("open file");
        exit(1);
    }

    // for every byte we get from input send it to the parser state machine
    while(read(fd, (unsigned char *)&c, 1) == 1)
    {
        parse(c);
        bytes_read++;
    }

    // close the input file
    close(fd);

    // since we check crc we can tell how many error messages there are
    // ouput the number of messages read, bytes read, and errors encountered
    printf("\nmessages found %d\nbytes in %d,\nerrors %d\n",
           messages_count,
           bytes_read,
           errors);

    return(0);
}
