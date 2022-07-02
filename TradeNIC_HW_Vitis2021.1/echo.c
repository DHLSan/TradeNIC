
#include<malloc.h>
#include<memory.h>
#include "lstm.h"
#include "dense.h"
/*weights*/
#include "kernel.h"
#include "reccurent_kernel.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lwip/pbuf.h"
#include <lwip/inet.h>
#include <lwip/sys.h>
#include <lwipopts.h>
#include "lwip/opt.h"
#ifdef __arm__
#include "xil_printf.h"
#include "xuartps.h"
#include "xparameters.h"
#include "xlstm.h"
#include "xdense.h"
#endif
#define NEURONS 100
#define INPUT_DIM 2
#define TIME_STEP 49
#define TEST_X_SIZE TIME_STEP - 1
#define GATES 4 //input, forget, cell, output
#include"xtime_l.h"
XTime tStart,tEnd;
int sec, nsec;
double  ElapsedTime;

XLstm Xlstm;
XDense dense_hw;

typedef struct cbuff_{
    float *buff;
    int start;
    int end;
    int size;
    int count;
} cbuff_t;
float Normalizedbuff[98];
int indexCbuff=0;
int dataLoadNum =0;
int x_testLoadNum =0;
int testnum =0;
int reset = 1;
float x_test[2];
float lstm_out[NEURONS];
int biasnum,densenum,kernelnum,recurrentnum=0;
int kernelrow=0;
int recurrentRow=0;
int states = 0;
bool statechanged=false;
bool firstWindow= true;
float bias[NEURONS * GATES];
/*float bias[NEURONS * GATES] = {
			-0.70160466,-0.5630368,-0.7453706,-0.34631288,-0.52753633,-0.20190616,-0.70361435,-0.30573317,-0.7291368,-0.8464875,-0.91957176,0.47043768,-0.5500856,-0.54291946,-0.6263227,-1.002705,-0.61753076,-0.36476895,-1.1585476,-0.54672754,-0.890195,-1.1726959,-0.26574802,-0.71780556,-0.7139683,-1.0286316,-1.0067493,-0.9073152,-0.09711368,-0.5741155,-0.9615564,-0.4851217,-0.53940433,-0.6642191,-0.6902702,-0.49392202,-0.48313752,-0.40110755,-0.2983082,0.9642439,-0.6716408,-1.082954,-1.2502388,-0.07512437,-0.35637832,-0.5464221,-0.96777946,-0.70890456,-0.6183105,-0.37570828,-0.2862407,0.19939192,0.28983188,-0.36148736,-0.32525274,-0.3864176,-0.64146155,-1.1145715,-0.12459752,-0.7630085,-0.50762194,-0.28766376,-0.73484915,-0.77726126,-0.78162587,0.06807362,-1.0066097,0.115878165,-0.9200951,-1.1359067,-0.52973545,-1.3475814,-0.78100204,-0.5296304,-0.59249854,0.10530165,-0.8738484,-0.87439793,-0.22384875,-0.1713449,-0.44409737,-1.2108297,-0.40015668,-0.87571156,-0.26352793,-0.50968444,0.13098288,-0.3688988,-0.66725457,-0.9953952,-0.36756575,-0.88666356,-0.7881997,0.2396734,-1.01901,-0.58524185,0.048379213,0.092993036,-0.6122738,-0.98144263,0.4551878,0.41756338,0.8002559,1.0193337,1.0401721,0.8276295,-0.07474635,-0.25875416,0.2811736,-0.13070348,-0.09450126,0.7279953,0.3176936,0.054181352,0.15764993,0.09443132,0.23308265,0.96966165,0.53616256,0.3593004,0.3019914,0.5820072,0.7950042,0.34013876,0.93412197,0.5285142,0.23375238,0.21240205,0.9596777,0.0042709694,0.5024858,0.57256687,0.033136103,-0.005681321,-0.11182562,0.5873657,0.37834883,0.17082961,0.42536783,0.908891,0.47424355,0.1714996,0.5319768,0.39637077,0.16593999,0.4776014,0.55638736,0.35446632,0.49860692,-0.17523415,0.5770922,0.5411929,0.72995466,-0.22523886,-0.022238404,-0.019878345,0.09092702,0.503068,0.2758601,0.007916711,0.36123094,0.8774793,0.57503784,0.34711966,0.38082898,-0.0812283,0.30437917,0.3480224,0.17939079,0.15564337,0.78543305,0.2329881,0.18170066,0.7795311,0.5908193,0.76208425,0.089348964,0.14230943,-0.09283163,1.087237,-0.0067944448,0.301185,-0.14285906,-0.067918,-0.2387571,0.8229772,-0.22326057,0.80812055,0.24331445,0.36884022,0.20349862,0.20733531,0.22321641,0.9777567,0.20630498,0.1826617,0.44312137,-0.37459838,0.5724427,0.46851197,0.009590315,-0.008231519,-0.0012372084,0.0012947738,0.0020230198,0.01922578,0.0027123273,0.015673803,0.004807589,-0.013983277,-0.0029540248,-0.0036347746,0.010918382,-0.002733435,-0.0034835166,-0.014364997,-0.009887708,0.0014553122,-0.0110973725,0.0062547484,-0.0043498445,0.001765584,0.013850361,0.004426287,0.02059698,0.0016070886,-0.00027971645,0.0030823573,0.016943071,0.015595876,-0.0072215353,-0.010257656,-0.017578468,0.013622668,0.015022122,0.013661104,0.10375388,-0.019008202,-0.020522205,0.015372412,0.01768521,-0.0019402094,-0.0049885963,-0.0029821766,-0.030945906,0.00003677525,-0.0061724526,-0.004212194,0.001725879,0.0011378486,-0.00014303738,0.00092163833,0.010287499,-0.004820361,0.014372859,0.02782777,0.0068824436,-0.007685436,0.022066133,-0.020281116,0.002539553,0.0069090985,0.0045643663,-0.003356969,0.004008514,-0.0026878098,0.006188548,-0.023899144,0.0045922385,-0.0035474377,0.021475805,-0.0036407898,0.021801218,0.0033074855,-0.0024752717,-0.011034487,-0.011170031,-0.009191102,-0.012401419,0.015616456,-0.019240202,0.0007132166,0.007536773,-0.0139615275,0.012028856,-0.0035581025,0.0060184794,0.009268647,0.0017030719,-0.006722901,-0.006952362,0.004979075,0.01573772,-0.0059068934,-0.0033895622,-0.014834226,-0.020208053,-0.0033434452,-0.009244542,0.0037925974,-0.6929787,-0.5290017,-0.68514776,-0.36339203,-0.5566707,-0.16318685,-0.67806643,-0.28524068,-0.71354055,-0.8449443,-0.9178823,0.33611292,-0.57346815,-0.5660522,-0.562752,-0.99198884,-0.62890106,-0.36349532,-1.0828408,-0.5755965,-0.8912671,-1.1182127,-0.37872943,-0.71164525,-0.7450084,-1.0294499,-0.97958565,-0.89931023,-0.19322307,-0.56551766,-0.9592316,-0.5188343,-0.54567367,-0.689187,-0.6936159,-0.35837325,-0.1994828,-0.41091776,-0.4260223,0.9495956,-0.65943575,-1.0902098,-1.2198914,-0.07114643,-0.3785485,-0.53869724,-0.9832495,-0.69913965,-0.62808686,-0.46017307,-0.37670785,-0.07987521,0.24581525,-0.36006683,-0.31405944,-0.3737784,-0.63836193,-1.1225958,-0.09432793,-0.7710642,-0.46735993,-0.35891777,-0.7109104,-0.73260075,-0.7338381,-0.026007658,-0.9865478,-0.11402989,-0.9119367,-1.1237553,-0.4623444,-1.3385304,-0.7685089,-0.6044801,-0.5964567,-0.28677204,-0.90532583,-0.87644094,-0.3498561,-0.29403618,-0.42073253,-1.1536845,-0.3140815,-0.88966227,-0.2360033,-0.35250786,0.16085054,-0.13101643,-0.6559207,-1.0102733,-0.5368432,-0.8506008,-0.7913158,0.21244906,-1.0204241,-0.5994713,-0.23424172,0.12319061,-0.60988647,-0.94460654
};*/
//float dense_w[NEURONS] = {0.010299869,-0.013958465,-0.0763099,-0.16580215,0.1809934,-0.16051124,-0.011249715,0.013663971,-0.0060081934,-0.020978775,-0.009076708,-0.33787048,0.032517,0.019054716,-0.02905033,0.028050939,0.0016815793,0.16950825,0.08049836,0.058096707,-0.039459378,0.08832708,0.01841564,-0.023049962,0.11344812,-0.055700235,-0.009592007,-0.011404909,0.3319275,-0.021058999,0.033407792,-0.2270017,-0.0049630804,0.012098565,-0.010711817,0.025580455,-0.061309524,-0.020214437,0.026791109,0.53707963,0.09141787,0.020833777,0.044212773,0.04655247,-0.01086574,-0.02795687,-0.07902086,0.088146634,0.022049854,-0.07863125,-0.14977822,-0.26183635,-0.15975763,-0.027181204,0.00014154334,0.0021974621,0.005543602,-0.07867963,-0.015306717,0.044843595,0.06874001,0.106913134,-0.08408778,0.038693998,0.031780634,-0.0037702443,-0.05020762,-0.06954534,-0.0005519091,-0.008113943,-0.16588338,0.04369496,-0.03141565,0.2714834,0.04229218,-0.15212008,-0.023374792,0.012224202,-0.026709221,-0.22890934,0.0033944796,0.024070667,0.014952988,-0.01365405,0.015342665,-0.14303747,-0.025727991,0.16331957,0.024387088,0.0016975971,0.075664215,0.011840762,0.0021902912,0.3104026,0.009398716,0.015903296,0.041287422,0.02401935,0.0019540559,0.042147353};
float dense_w[NEURONS];
float dense_b;//= -0.008892165;
float dense_out[4];
float kernel[4][100 * 4]; //shape=(input_dim, self.units * 4)
float recurrent_kernel[50][100 * 4]; //shape=(self.units, self.units * 4)
float recurrent_kernel2[50][100 * 4]; //shape=(self.units, self.units * 4)
float recurrent_kernel3[50][100 * 4]; //shape=(self.units, self.units * 4)
float recurrent_kernel4[50][100 * 4]; //shape=(self.units, self.units * 4)

float recurrent_kerneludp[100][100 * 4]; // for network rec_kernel

int transfer_data() {
	return 0;
}

cbuff_t * cbuff_new(int size)
{
  cbuff_t * cb = (cbuff_t*)malloc(sizeof(cbuff_t));
  memset(cb, 0, sizeof(cbuff_t));
  cb->size = size;
  cb->buff = (int*)malloc(sizeof(int)*size);

  return cb;
}
/*custom circular buffer slides data up and adds new element to last */
void cbuff_add(cbuff_t *cb, float elem)
{
  int i;
  int end = cb->end;
  if(cb->count && (end % cb->size) == cb->start){
   // printf("Overflow Elem[%d] %d lost\n", cb->start, cb->buff[cb->start]);
    for(i=0;i<cb->size;i++){
    	cb->buff[i] =cb->buff[i+1];
    }
   /* indexCbuff++;
    if(indexCbuff==2){
    for(i=2; i< 98;i=i+2){
        Normalizedbuff[i] = (((float)cb->buff[i] / (float)cb->buff[0]) - 1);
	}
	for(i=3; i< 98;i=i+2){
		Normalizedbuff[i] = (((float)cb->buff[i] / (float)cb->buff[1]) - 1);
	}
    //printf("size ne %d\n",cb->size);

   /* for (i = 0; i < cb->count; i++) {
		printf("normalizedbuffer= %.10f\n",Normalizedbuff[i]);
	}*/
   // indexCbuff=0;
    //}
    cb->buff[cb->size-1] = elem;
   // cb->start = (cb->start + 1 ) %cb->size;
   // cb->count --;
  }
  else{
 // printf("Added Elem[%d] = %d\n",cb->end, elem);
  cb->buff[cb->count] = elem;
  //printf("count ne %d\n",cb->count);
  cb->end = (cb->end+1)% cb->size;
  cb->count++;
  }
  }


int cbuff_remove(cbuff_t * cb)
{
  int start = cb->start ;
  int ret = -1;
  if(cb->count <= 0) {
    printf("Buffer is empty\n");
    return ret;
  }
  if(cb->count || (start % cb->size) != cb->end) {
    printf("Removed Elem[%d] = %d\n",cb->start, cb->buff[cb->start]);
    ret = cb->buff[cb->start];
    cb->start = (cb->start + 1 ) % cb->size;
    cb->count--;
  } else {
    printf("Buffer is empty\n");
  }
  return ret;
}

void cbuff_delete(cbuff_t * cb)
{
  free(cb->buff);
  free(cb);
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP UDP echo server ------\n\r");
	xil_printf("UDP packets sent to port 9999 will be echoed back\n\r");
}
int i,k,j = 0;
cbuff_t *cb ;
void udp_recvBack(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port){

	/*
	for(j = 0; j < NUM; j++){
			int reset = 1;
			for(i = 0; i < 48; i++){  	// epoch 2  100 LSTM unit
				float x_test[2] = {input_data[j][i][0], input_data[j][i][1]};
				lstm(x_test, kernel, recurrent_kernel,recurrent_kernel2,recurrent_kernel3,recurrent_kernel4, bias, lstm_out, reset);

					//printf(" lstm_Out : %f\n", lstm_out);

				reset = 0;
			}
			dense(lstm_out, dense_w, dense_b, &dense_out);
			printf(" xtest%d result : %.10f\n",c, dense_out);
	}*/
	//printf("is coming %s\n",(char*)p->payload);
	//printf("data here ??");
	unsigned port2 = 9998;
	//struct ip4_addr addr2;
	char buf[1024];
	struct pbuf *txBuf; // packet buffer we are going to send

	/* circular buffer implementation with LSTM function for real time use*/

	if (k == 0) {
		cb = cbuff_new(98);
		k++;
	}
	dataLoadNum++;
	cbuff_add(cb,atof((char*)p->payload));
	if (dataLoadNum ==98  ) {

		for(i=2; i< 98;i=i+2){
			Normalizedbuff[i] = (((float)cb->buff[i] / (float)cb->buff[0]) - 1);
		}
		for(i=3; i< 98;i=i+2){
			Normalizedbuff[i] = (((float)cb->buff[i] / (float)cb->buff[1]) - 1);
		}
		//printf("size ne %d\n",cb->size);

		/*for (i = 0; i < cb->count; i++) {
			printf("normalizedbuffer= %.10f\n",Normalizedbuff[i]);
		}
		firstWindow =false;
*/
	}

	if (dataLoadNum ==98) {
		for(j = 0; j < 1; j++){
			int reset = 1;
			//XTime_GetTime(&tStart);
			XTime_GetTime(&tStart);
			for(i = 2; i < 98; i=i+2){  	// epoch 2  100 LSTM unit
				float x_test[2] = {Normalizedbuff[i],Normalizedbuff[i+1]};
				//printf("xtest[%d] =%.10f  xtest[%d] = %.10f\n",i,Normalizedbuff[i],i+1,Normalizedbuff[i+1]);
				//XLstm_Write_input_data_Words(&Xlstm, 0, x_test, 2);

				//XLstm_Start(&Xlstm);
				//while(!XLstm_IsDone(&Xlstm));
				//XLstm_Read_lstm_out_Words(&Xlstm,0,lstm_out,128);

				lstm(x_test, kernel, recurrent_kerneludp, bias, lstm_out, reset);
				//printf("test0 %.10f test1 %.10f\n",x_test[0],x_test[1]);
				reset = 0;
			}
			XTime_GetTime(&tEnd);
			ElapsedTime = 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND);
			sec = (int)ElapsedTime;
			nsec = (int)((ElapsedTime - sec) * 1000000000);
			printf("clock hızı %.12f\n", COUNTS_PER_SECOND);
			printf("In  LSTM  HW, Output took %d.%09d\n\r",sec,nsec);
			//for(i = 0; i<96; i++)
				//printf("Elem[%d] = %.10f\n", i, cb->buff[i]);
			XDense_Write_h_state_Words(&dense_hw, 0, lstm_out, 128);
			//XTime_GetTime(&tStart);
			XDense_Start(&dense_hw);
			while(!XDense_IsDone(&dense_hw));
			//dense(lstm_out, dense_w, dense_b, dense_out);
			XDense_Read_dense_out_Words(&dense_hw, 0, dense_out, 4);

			//dense(lstm_out, dense_w, dense_b, &dense_out);
			int len = sprintf(buf, "%d",(666666687 /2)); // mixing the client data
			txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);  // allocate memory for this packet buffer
			pbuf_take(txBuf,buf, len); // copy the data into the buffer

			//IP4_ADDR(&addr2, 192,168,1,5);
			udp_sendto(pcb, txBuf, addr, port2);
			pbuf_free(txBuf); // free all the memories that we allocated before
			testnum++;
			printf(" xtest%d result : %.10f\n",testnum,dense_out[0] );
			//printf("cbuff[0]=%.10f\n",cb->buff[0]);
			printf("xtest%d unNormalized result : %.10f\n",testnum, (dense_out[0]+1)*(cb->buff[0]));
			pbuf_free(p);
			//free(Normalizedbuff);
	}
		dataLoadNum=96;
	}

	/*  Receiving 49 * 2 Batch Normalized Data frame for each prediction operation*/
	/*
	XTime_GetTime(&tStart);
	if (x_testLoadNum ==0) {
		x_test[0] = atof((char*)p->payload);
		//printf("test data %.10f\n",x_test[0]);
		x_testLoadNum++;
	}
	else if (x_testLoadNum == 1) {
		x_test[1] = atof((char*)p->payload);

		XLstm_Write_input_data_Words(&lstm, 0, x_test, 2);
		//printf("is ready %d\n",XLstm_IsReady(&lstm));
		XLstm_Start(&lstm);
		while(!XLstm_IsDone(&lstm));
		XLstm_Read_lstm_out_Words(&lstm,0,lstm_out,128);
		reset =0;
		x_testLoadNum =0;
		//pbuf_free(p);
	}
	dataLoadNum++;

	//xil_printf("dataloadnum %d\n",dataLoadNum);
	if (dataLoadNum == 96) {
		XTime_GetTime(&tEnd);
		ElapsedTime = 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND);
		sec = (int)ElapsedTime;
		nsec = (int)((ElapsedTime - sec) * 1000000000);
		printf("In HW, Output took %d.%09d\n\r",sec,nsec);
		dense(lstm_out, dense_w, dense_b, &dense_out);

		int len = sprintf(buf, "%.10f",dense_out); // mixing the client data
		txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);  // allocate memory for this packet buffer
		pbuf_take(txBuf,buf, len); // copy the data into the buffer
		udp_sendto(pcb, txBuf, addr, port2);
		testnum++;
		printf(" xtest%d result : %.10f\n",testnum, dense_out);
		reset = 1;
		dataLoadNum = 0;
		pbuf_free(txBuf); // free all the memories that we allocated before
		pbuf_free(p);
	}*/
	pbuf_free(p);
}


int start_application()
{

	//ModelLoad();
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
	err = udp_bind(pcb, IP_ADDR_ANY, port);  //binding the pcb to the local port  any ıp4 adress will be binding for this.
	if (err != 0) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* Receive data */
	udp_recv(pcb, udp_recvBack, NULL);


	xil_printf("UDP echo server started @ port %d\n\r", port);

	return 0;


}
/* Loading Weights by hand*/
void ModelLoad(){
	int i, j, k;
	int count;

	/*Loading training model!!!!!!*/
	// kernel (weights)
		count =0;
		for(i = 0; i < 2; i++){
			for(j = 0; j < 400; j++){
				kernel[i][j] = kernel_h[count];
				count++;
			}
		}
		// for(i = 0; i < 10; i++) printf("kernel %d - %d: %.10f\n",Krnindex2, Krnindex1,kernel[0][i]);

		//recurrent_kernel (hidden matrix)
		count =0;
		for(i = 0; i < 25; i++){
				for(j = 0; j < 400; j++){
					recurrent_kernel[i][j]= reccurent_kernel_h1[count];
					count++;
				}
			}
		count =0;
		for(i = 25; i < 50; i++){
					for(j = 0; j < 400; j++){
						recurrent_kernel[i][j]= reccurent_kernel_h2[count];
						count++;
					}
				}
		count =0;
			for(i = 50; i < 75; i++){
						for(j = 0; j < 400; j++){
							recurrent_kernel[i][j]= reccurent_kernel_h3[count];
							count++;
						}
					}
		count =0;
				for(i = 75; i < 100; i++){
							for(j = 0; j < 400; j++){
								recurrent_kernel[i][j]= reccurent_kernel_h4[count];
								count++;
							}
						}

	xil_printf("TRAINED MODEL LOAD SUCCESS!\n");


}
/*Loading weights from network at 9995 port*/
void udp_recvWeights(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port){


	if (states ==0) {
		dense_b = atof((char*)p->payload);
		printf("dense_b=%.10f\n",dense_b);
		states =1;
		xil_printf("#######Dense_b LOADED########");
		pbuf_free(p);
	}

	if (states == 1 && statechanged == true ) {
		bias[biasnum] =atof((char*)p->payload);
		biasnum++;
		if (biasnum ==400) {
			for (i = 0; i < 400; i++) {
				//printf("bias[%d]=%.10f\n",i,bias[i]);
			}
			states=2;
			statechanged = false;
			xil_printf("#######LSTM Bias LOADED########");
		}
		pbuf_free(p);
	}
	if (states == 2 && statechanged == true ) {

		dense_w[densenum] = atof((char*)p->payload);
		densenum++;
		if (densenum == 100) {
			for (i = 0; i < 100; i++) {
				//printf("debse_w[%d]=%.10f\n",i,dense_w[i]);
			}
			states=3;
			statechanged =false;
			xil_printf("#######Dense Kernel LOADED########");
		}
		pbuf_free(p);
	}
	if (states == 3 && statechanged == true ) {

		kernel[kernelrow][kernelnum] = atof((char*)p->payload);
		kernelnum++;
		if (kernelnum == 400) {
			if (kernelrow==1) {
				for (j = 0; j < 2; j++) {
					for (i = 0; i < 10; i++) {
						printf("kernel[%d][%d]=%.10f\n",j,i,kernel[j][i]);
					}
				}
				states =4;
				statechanged = false;
				xil_printf("#######LSTM Kernel LOADED########");
			}
			kernelrow=1;
			kernelnum=0;
		}
		pbuf_free(p);
	}
	if (states == 4 && statechanged == true ) {

		recurrent_kerneludp[recurrentRow][recurrentnum] = atof((char*)p->payload);
		recurrentnum++;
		if (recurrentnum ==400) {
			if (recurrentRow==99) {
				//for (j = 0; j < 3; j++) {
					//for (i = 0; i < 400; i++) {
						printf("reccu[0][0]=%.10f\n",recurrent_kerneludp[0][0]);
						printf("reccu[0][1]=%.10f\n",recurrent_kerneludp[0][1]);
						printf("reccu[0][398]=%.10f\n",recurrent_kerneludp[0][398]);
						printf("reccu[0][399]=%.10f\n",recurrent_kerneludp[0][399]);
						printf("reccu[1][0]=%.10f\n",recurrent_kerneludp[1][0]);
						printf("reccu[1][1]=%.10f\n",recurrent_kerneludp[1][1]);
						printf("reccu[1][398]=%.10f\n",recurrent_kerneludp[1][398]);
						printf("reccu[1][399]=%.10f\n",recurrent_kerneludp[1][399]);
						printf("reccu[2][0]=%.10f\n",recurrent_kerneludp[2][0]);
						printf("reccu[2][1]=%.10f\n",recurrent_kerneludp[2][1]);
						printf("reccu[2][398]=%.10f\n",recurrent_kerneludp[2][398]);
						printf("reccu[2][399]=%.10f\n",recurrent_kerneludp[2][399]);
						printf("reccu[99][0]=%.10f\n",recurrent_kerneludp[99][0]);
						printf("reccu[99][1]=%.10f\n",recurrent_kerneludp[99][1]);
						printf("reccu[99][398]=%.10f\n",recurrent_kerneludp[99][398]);
						printf("reccu[99][399]=%.10f\n",recurrent_kerneludp[99][399]);
					//}
				//}
				states =5;
				statechanged = false;
				xil_printf("#######LSTM Recurrent Kernel LOADED########\n");
			}
			recurrentRow++;
			recurrentnum=0;

		}
		pbuf_free(p);
	}
	if (states == 5 && statechanged == false ) {

		XLstm_Initialize(&Xlstm,0);
		XLstm_Write_kernel_Words(&Xlstm, 0, kernel, 1024);
		XLstm_Write_recurrent_kernel_Words(&Xlstm, 0, recurrent_kerneludp, 65536);
		XLstm_Write_bias_Words(&Xlstm, 0, bias, 512);
				//XLstm_Set_reset(&lstm, reset);
		xil_printf("#######HW LSTM Initialized ########\n");
		XDense_Initialize(&dense_hw, 0);
		XDense_Set_dense_b(&dense_hw, dense_b);
		XDense_Write_dense_w_Words(&dense_hw, 0, dense_w, 128);
		xil_printf("#######HW DENSE Initialized ########\n");


	}


	statechanged =true;
}
int Model_weights_socket()
{

	struct udp_pcb *pcb;
	unsigned port = 9995;
	int err;

	/* create new UDP PCB structure */
	pcb = udp_new(); //creating new UDP  control block
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = udp_bind(pcb, IP_ADDR_ANY, port);  //binding the pcb to the local port  any ıp4 adress will be binding for this.
	if (err != 0) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* Receive data */
	udp_recv(pcb, udp_recvWeights, NULL);


	xil_printf("UDP echo server started @ port %d\n\r", port);

	return 0;


}

