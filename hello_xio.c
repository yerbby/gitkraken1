/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"

/*
 * this is new, instead of mysum.h
 */
#include "xmysum.h" //
#include "xmysum_hw.h" // to access low-level driver functions
#include "xparameters.h"

/*
 *mysum_sw copied identical from hls sources
 */
void mysum_sw(int din1, int din2, int *dout){

	int tmp;

	if (din1>=din2){
		tmp = din1-din2;
	}
	else{
		tmp = din1+din2;
	}

	*dout = tmp;

}

//void print(char *str);

int main()
{
	int databuf1[]={10,9,8,7,6,5,4,3,2,1};
	int databuf2[]={1,2,3,4,5,6,7,8,9,10};

	int result_sw[10];
	int result_hw[10];

	int dout_sw;
	int dout_hw;
	int i;
	int j;

	int errorflag=0;
	char rept;

    init_platform();

    printf("Hello World\n\r");
    printf("Application using Xil_IO functions to communicate with hw\n\r");

    printf("sizeof(int) = %d \n\r", sizeof(int));

    while(1){

		for(i=0;i<10;i++){

			/*
			 * calculate output from mysum implemented as sw
			 */
			mysum_sw(databuf1[i],databuf2[i],&dout_sw);
			result_sw[i] = dout_sw;

			/*
			 * calculate output from mysum implemented as hw
			 */
			// mysum(databuf1[i],databuf2[i],&dout_hw);
			// result_hw[i] = dout_hw;

			/*
			 * send data to peripheral inputs
			 */
			Xil_Out32(XPAR_MYSUM_0_S_AXI_MYAXI_BASEADDR+XMYSUM_MYAXI_ADDR_DIN1_DATA, databuf1[i]);
			Xil_Out32(XPAR_MYSUM_0_S_AXI_MYAXI_BASEADDR+XMYSUM_MYAXI_ADDR_DIN2_DATA, databuf2[i]);

			/*
			 * pulse peripheral ap_start
			 */
			Xil_Out32(XPAR_MYSUM_0_S_AXI_MYAXI_BASEADDR+XMYSUM_MYAXI_ADDR_AP_CTRL, 0x1); //

			/*
			 * wait until corresponding output becomes available (ap_vld=1. Output had ap_vld interface by default.)
			 */
			j=0;
			while(1){
				if(Xil_In32(XPAR_MYSUM_0_S_AXI_MYAXI_BASEADDR+XMYSUM_MYAXI_ADDR_DOUT_CTRL))
					break;
				else{
					j++;
					continue;
				}
			}
			printf("hw wait loops: %d\n\r",j);

			/*
			 * read peripheral output data
			 */
			dout_hw = Xil_In32(XPAR_MYSUM_0_S_AXI_MYAXI_BASEADDR+XMYSUM_MYAXI_ADDR_DOUT_DATA);
			result_hw[i] = dout_hw;
		}

		for(i=0;i<10;i++){
			printf("result_sw,resulthw = %d,%d\n\r",result_sw[i],result_hw[i]);
			if (result_sw[i]!=result_hw[i]){
				printf("Errors found between sw and hw outputs.\n\r");
				errorflag = 1;
			}
		}

		printf("continue s/n: ");
		scanf("%s",&rept);
		printf("%c\n\r",rept);
		if (rept=='n')
			break;
    }

    cleanup_platform();
    return errorflag;
}
