/*
 * Copyright (c) 2009-2013 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *--- UDP interface

The UDP interface is similar to that of TCP, but due to the lower
level of complexity of UDP, the interface is significantly simpler.

- struct udp_pcb *udp_new(void)

  Creates a new UDP pcb which can be used for UDP communication. The
  pcb is not active until it has either been bound to a local address
  or connected to a remote address.

- void udp_remove(struct udp_pcb *pcb)

  Removes and deallocates the pcb.

- err_t udp_bind(struct udp_pcb *pcb, ip_addr_t *ipaddr,
                 u16_t port)

  Binds the pcb to a local address. The IP-address argument "ipaddr"
  can be IP_ADDR_ANY to indicate that it should listen to any local IP
  address. The function currently always return ERR_OK.

- err_t udp_connect(struct udp_pcb *pcb, ip_addr_t *ipaddr,
                    u16_t port)

  Sets the remote end of the pcb. This function does not generate any
  network traffic, but only set the remote address of the pcb.

- err_t udp_disconnect(struct udp_pcb *pcb)

  Remove the remote end of the pcb. This function does not generate
  any network traffic, but only removes the remote address of the pcb.

- err_t udp_send(struct udp_pcb *pcb, struct pbuf *p)

  Sends the pbuf p. The pbuf is not deallocated.

- void udp_recv(struct udp_pcb *pcb,
                void (* recv)(void *arg, struct udp_pcb *upcb,
                                         struct pbuf *p,
                                         ip_addr_t *addr,
                                         u16_t port),
                              void *recv_arg)

  Specifies a callback function that should be called when a UDP
  datagram is received.

 */

#include <stdio.h>
#include <string.h>

#include "lwip/pbuf.h"
#include <lwip/inet.h>
#include <lwip/sys.h>
#include <lwipopts.h>
#include "lwip/opt.h"
#ifdef __arm__
#include "xil_printf.h"
#include "xuartps.h"
#include "xparameters.h"
#endif

int transfer_data() {
	return 0;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP UDP echo server ------\n\r");
	xil_printf("UDP packets sent to port 9999 will be echoed back\n\r");
}
void udp_recvBack(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port){

/* 1-Create UDP socket
   2-Bind the socket to server adress
   3- Wait until datagram packets arrives from client
   4-Process datagram packets and send to reply to client
   5- Go back to step 3.
*/
	// pbuf is the packet buffer structer it contains the info the packet send by client info like payload size etc.
	// udp_pcp  udp control block info about client and server  ip port etc.

	unsigned port2 = 9998;

	struct pbuf *txBuf; // packet buffer we are going to send

	char buf[100]; //

	int len = sprintf(buf, "Hello %s coming from FPGA\n", (char*)p->payload ); // mixing the client data

	txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);  // allocate memory for this packet buffer

	pbuf_take(txBuf,buf, len); // copy the data into the buffer

	xil_printf("FPGA data: %d\n",txBuf);


	udp_sendto(pcb, txBuf, addr, port2);

	/*udp_connect(pcb, addr, port);  // connecting the remote client

	udp_send(pcb, txBuf);  // Send data to the client

	udp_disconnect(pcb); // disconnects udp connection*/

	pbuf_free(txBuf); // free all the memories that we allocated before

	pbuf_free(p);




}

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct
					ip_addr *addr, u16_t port)
{
	// pbuf is the packet buffer structer it contains the info the packet send by client info like payload size etc.
	// udp_pcp  udp control block info about client and server  ip port etc.
	//
	/*Parameters for udp_sendto();
		pcb			UDP PCB used to send the data.
		p			chain of pbuf's to be sent.
		dst_ip		Destination IP address.
		dst_port	Destination UDP port.
	*/

	if (p != NULL) {
        /* send received packet back to sender */
        udp_sendto(pcb, p, addr, port);
        xil_printf("%d\n",(u32_t)addr);
        /* free the pbuf */
        pbuf_free(p);
    }
}
/*int Recv_to(){
	unsigned port2 = 9998;
	struct udp_pcb *pcb;
	pcb = udp_new();

	udp_recv(pcb, udp_recvBack, NULL);
}
*/
int start_application()
{
	struct udp_pcb *pcb;
	unsigned port = 9999;
	int err;

	/* create new UDP PCB structure */
	pcb = udp_new(); //creating new UDP  control block
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = udp_bind(pcb, IP_ADDR_ANY, port);  //binding the pcb to the local port  any ýp4 adress will be binding for this.
	if (err != 0) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* Receive data */
	udp_recv(pcb, udp_recvBack, NULL);


	xil_printf("UDP echo server started @ port %d\n\r", port);

	return 0;


}

