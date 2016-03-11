//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define SIZE 1024

using namespace std;

int pow_arr[SIZE] = {0};
int deg = 0;

int msg_len(char* input){
	int i;
	for(i = 0;input[i] != '\0';i++);
	return i;
}



char* crc(char* input,char* polynomial){
	int in_len = strlen(input);
	int pol_len = strlen(polynomial);
	
	deg = pol_len - 1;
	cout << deg<<endl;
	char* tmp = new char[SIZE];
	strcpy(tmp,input);	
	
	for(int i=0;i<deg;i++)
	 strcat(tmp,"0");
	tmp[in_len + deg] = '\0';
	in_len = strlen(tmp);

	for(int i=0;i<in_len-pol_len+1;){
		cout<<tmp<<endl;
		if(tmp[i] == '0'){
			for(int j=i;j<i+pol_len;j++){
				tmp[j] = (char)((tmp[j]-'0')^0);
		
			}
		}
		else{
			for(int j=i;j<i+pol_len;j++){
				tmp[j] = (char)((tmp[j]-'0')^polynomial[j-i]);
			}
		}
		while(tmp[i]=='0')
			i++;
	}
	
	char* res = new char[SIZE];
	int x = 0;
	cout<<"CRC is : ";
	for(int i=in_len-deg;i<in_len;i++){
		cout<<tmp[i];
		res[x++] = tmp[i];
	}
	
	res[x] = '\0';
	delete[] tmp;
	return res;
}

int main(){
	cout<<"Enter your message : ";
	char* input = new char[SIZE];
	cin>>input;
	
	int len = msg_len(input);
	
	char* polynomial = new char[SIZE];
	cout<<"Enter the polynomial : ";
	cin>>polynomial;
	
	char* res = new char[SIZE];
	
	res = crc(input,polynomial);
	strcat(input,res);
	
	cout<<"\nresult is : ";
	cout<<input<<endl;
	delete[] input;
	delete[] res;
	delete[] polynomial;

	return 0;
}