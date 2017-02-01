//modificacion 1 en local
/*
 *
 */
//#include<stdio.h>
#include "mysum.h"

void mysum(int din1, int din2, int *dout){

	int tmp;

	if (din1>=din2){
		tmp = din1-din2;
	}
	else{
		tmp = din1+din2;
	}

	*dout = tmp;

}
