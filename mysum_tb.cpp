/*
 *
 */
// modificacion 1 en local
// modificacion 2 en local. segundo commit
#include <stdio.h>
#include "mysum.h"

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



int main(int argc, char *argv[]){

	int databuf1[]={10,9,8,7,6,5,4,3,2,1};
	int databuf2[]={1,2,3,4,5,6,7,8,9,10};

	int result_sw[10];
	int result_hw[10];

	int dout_sw;
	int dout_hw;
	int i;

	bool errorflag=0;

	for(i=0;i<10;i++){
		mysum_sw(databuf1[i],databuf2[i],&dout_sw);
		result_sw[i] = dout_sw;
		mysum(databuf1[i],databuf2[i],&dout_hw);
		result_hw[i] = dout_hw;
	}


	for(i=0;i<10;i++){
		printf("result_sw,resulthw = %d,%d\n\r",result_sw[i],result_hw[i]);
		if (result_sw[i]!=result_hw[i]){
			printf("Errors found between sw and hw outputs.\n\r");
			errorflag = 1;
		}
	}


	return errorflag;
}
