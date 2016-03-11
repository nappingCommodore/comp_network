#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#define SIZE 1024

using namespace std;

int err_bits(int msg_len){
	int res = (int)log2(msg_len);
	int len = msg_len+res;

	if(res < (int)log2(len)){
		return res+1;
	}

	return res;
}

void check_and_skip(char res_arr[],int total_len,int i){
	int skip_bits = i+1;
	int on_bits = 0;
	for(int j=i;j<=total_len;j+=skip_bits){
		int x = skip_bits;
		while(x && j<=total_len){
			if(res_arr[j] == '1'){
				on_bits++;
			}
			j++;
			x--;
		}
	}

	if(on_bits%2){
		res_arr[i] = '1';
	}

	else{
		res_arr[i] = '0';
	}
}


void error_intro(char res_arr[],int total_len){
	int rbit = ((double)rand()/RAND_MAX)*total_len;
	res_arr[rbit] = !(res_arr[rbit] - '0')+'0';
}


int error_bit(char res_arr[],int total_len,int i){
	int skip_bits = i+1;
	int on_bits = 0;
	for(int j=i;j<=total_len;j+=skip_bits){
		int x = skip_bits;
		while(x && j<=total_len){
			if(res_arr[j] == '1'){
				on_bits++;
			}
			j++;
			x--;
		}
	}

	if(on_bits % 2)
		return 1;

	else
		return 0;
}

//0 7 26 63 (x^3-1) 

void hamming_code(char res_arr[],int total_len){
	int i;
	char tmp_arr[SIZE];
	strcpy(tmp_arr,res_arr);

	for(i=0;i<=total_len;i++){
		if(res_arr[i] == '?'){
			check_and_skip(res_arr,total_len,i);
		}
	}

	cout<<"\nSent msg is : ";
	cout<<res_arr<<endl;

	error_intro(res_arr,total_len);
	cout<<"\nReceived msg is : ";
	cout<<res_arr<<endl;

	int res = 0;

	for(i=0;i<total_len;i++){
		int x = 0;
		if(tmp_arr[i] == '?'){
			x = error_bit(res_arr,total_len,i);
		}
		if(x == 1){
			res+=i+1;
		}
	}

	cout<<"\nError bit is : "<<res<<endl;
}


int main(){
	char* msg = new char[SIZE];
	int msg_len;
	int no_err_bits;
	int total_len;

	cout<<"\nEnter msg.. : ";
	cin>>msg;

	msg_len = strlen(msg);
	no_err_bits = err_bits(msg_len);

	total_len = msg_len+no_err_bits;

	char res_arr[total_len+2];

	int j = 0;
	int i = 0;

	for(i=0;i<=total_len;i++){
		if(!((i+1) & i)){
			res_arr[i] = '?';
		}

		else{
			res_arr[i] = msg[j++];
		}
	}
	res_arr[i] = '\0';
	cout<<"\nMessage without redundancy bits : ";
	cout<<res_arr<<endl;

	hamming_code(res_arr,total_len);

return 0;
}