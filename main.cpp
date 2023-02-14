/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Stan
 *
 * Created on 15 марта 2022 г., 20:58
 */

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <windows.h>


using namespace std;

//The fuction splitting the string into words
string SplitFind(string x, char y, int z)
{
    int dcount=0;//Счётчик разделителей
    
    //In case of void words
    if (x[x.length()-1]==y) x+=' ';
    if (x[0]==y) x=' '+x;
    
    for(int i=0;i<x.length();i++)
    {
       if(i>0){//In case of void words
       if((x[i]==y)&&(x[i-1]==y)) x.insert(i," ");
       }
       if (x[i]==y) dcount++;//Когда символ i из строки х равен y
       //счётчик прибавляется
    }

    
    if (z>dcount) return "";//Выход из функции в случае слишком большого
    //номера слова

    vector <string> vx;//Объявление строкового вектора слов vx

    istringstream iss(x);//Объявление строкового потока iss с входной
    //строкой x в качестве аргумента
    while(getline(iss,x,y))//Цикл, перебирающий и делящий строку х на слова
    //с разделителем y
    {
        vx.push_back(x);//Добавление слов из строки x в вектор vx
    }
    return vx[z];//Возвращаемое функцией значение в виде слова
    //(элемента вектора vx с номером z)
}

//Decimal-to-binary number converter
string D2B(string num){
float d;//Input decimal number
string b;//String for the output binary number
stringstream is;//String stream for the input decimal number string

//In case of zero input
if (num=="0") return "0";

//Conversion of the input data from string to int
    is<<num;
    is>>d;
    is.str("");

//Basic conversional mechanism from decimal to binary
    while (d!=1){
        d=d/2;
        if (d==(int)d) b+='0';
        else if (d!=(int)d){
          b+='1';
          d=(int)d;
        }
    }

    b+='1';
    reverse(b.begin(),b.end());
    return b;
}

//Binary-to-decimal number converter
string B2D(string num){
    int d=0;//Output decimal number
    int n;//int variable for the separate digit of the input binary number
    int j;//Power to raise number 2 from the calculational formula

    //In case of zero input
    if (num=="0") return "0";
    
    j=num.length()-1;

    //Conversional calculation cycle
    for(int i=0;i<=num.length()-1;i++){
        stringstream *sstr=new stringstream;/*string stream declared as
        a pointer to convert from string to int*/
        *sstr<<num[i];
        *sstr>>n;
        d=d+n*pow(2,j);
        j--;
        delete sstr;
    }

    stringstream is;
    is<<d;
    num=is.str();
    return num;
}

//Convert the int array to string
string IA2S(int array[], int size, char d, int s=0){
    stringstream output;
    for(int i=0;i<size;i++){
        if (s==0) output<<array[i];
        //The feature for conversion to binary system
        else if(s==2) {
            string *b=new string;
            *b=D2B(to_string(array[i]));
            int *l=new int;
            *l=b->length();
            int *d=new int;
            *d=8-*l;
            output<<string(*d,'0')<<*b;
            delete d,b,l;
        }
        if(i<size-1) output<<'.';
    }
    return output.str();
}

//Converter from prefix to decimal form of the mask
string Prefix2DM(int mask){
    int host=32-mask;
    string bm=string(mask,'1')+string(host,'0');
    bm.insert(8,".");
    bm.insert(17,".");
    bm.insert(26,".");
    
    string dm;
    for(int i=0;i<4;i++){
        dm+=B2D(SplitFind(bm,'.',i));
        if(i<3) dm+="."; 
    }
    
    return dm;
}

//Converter from decimal to prefix form of the mask
int DM2Prefix(string mask){
    string bm;
    for(int i=0;i<4;i++){
        bm+=D2B(SplitFind(mask,'.',i));
        if(D2B(SplitFind(mask,'.',i))=="0") bm+=string(7,'0');
    }
        
    int prefix=count(bm.begin(),bm.end(),'1');
    return prefix;
}

//The overall structure for all parameters and calculation results
struct IP{
   int pmask;//Prefix mask number
   int an;//Address number
   int hn;//Host number
   int ip_array[4];//IP array
   int mask_array[4];//Mask array
   int wmask_array[4];//Wildcard mask array
   int net_array[4];
   int bc_array[4];
   int start_array[4];
   int end_array[4];
};

//Scanning the common input string
bool CheckInput(string input){
    int c;//Symbol counter
    bool st=1;//Check status
    //Checking the number of slashes
    c=count(input.begin(), input.end(), '/');
    if(c!=1) {
        st=0;
        cout<<"Incorrect input data: "<<endl;
        cout<<"Wrong number of slashes '/' "<<c<<endl;
        return st;
    }
        
    //Scanning for symbols differing from slashes, dots and digits
    for(int i=0;i<input.length();i++){
        if (((int)input[i]>=48)&&((int)input[i]<=57));
        else if(input[i]=='.'); 
        else if(input[i]=='/');
        //In case of incorrect input data
        else
        {
            st=0;
            cout<<"Incorrect input data: "<<"'"<<input[i]<<"'"<<endl;
            return st;
        }
    }
    //Scanning for double dots
    if(input.find("..")!=-1) {
        st=0;
        cout<<"Incorrect input data: "<<"'"<<".."<<"'"<<endl;
        return st;
    }
        
    /*Scanning for dots at the beginning and 
    at the end of the common input string*/
    if((input[0]=='.')||(input[input.length()-1]=='.')){
       st=0;
       cout<<"Incorrect input data: "<<"'"<<"."<<"'"<<endl;
       return st;
    }
        
    //Scanning for dots from the right and left of the dividing slash
    int found=input.find("/");
    if((input[found-1]=='.')||(input[found+1]=='.')){
        st=0;
        cout<<"Incorrect input data: "<<"'"<<"."<<"'"<<endl;
        return st;
    }
        
    
    return st;
}

//Scanning the IP-address string
bool CheckIP(string ip){
    int c;//Symbol counter
    bool st=1;//Check status
    
    c=count(ip.begin(), ip.end(), '.');
    
    //Checking the number of dots
    if(c==3){
        for(int i=0;i<4;i++){
            if(stoi(SplitFind(ip,'.',i))>255){ 
                st=0;
                cout<<"Incorrect IP-address"<<endl;
                cout<<"Octet: "<<i+1<<" "<<SplitFind(ip,'.',i)<<endl;
                return st;
            }            
        }
    }
    else{
        st=0;
        cout<<"Incorrect IP-address"<<endl;
        return st;
    }          
    
    return st;
}

//Scanning the mask string
bool CheckMask(string mask){
    int c;//Symbol counter
    bool st=1;//Check status
    
    c=count(mask.begin(), mask.end(), '.');
    //Checking the number of dots
    if(c==0){ //The prefix variant
        if ((mask==" ")||(mask=="")||(stoi(mask)>32)){
            st=0;
            cout<<"Incorrect mask value: "<<mask<<endl;
            return st;
        }
    }
    else if(c==3){ //The decimal mask variant
        string mvalues[]={"0","128","192","224","240","248","252","254","255"};
        bool f=0;//Octet number coincidence status
        int step_octet=0;//Octet number determining the step octet
        for(int i=0;i<4;i++){
        //Comparing the numbers in the octets to the ones in the array
        //of the admissible numbers
            for(int j=0;j<9;j++){
                if (SplitFind(mask,'.',i)==mvalues[j]){ 
                    f=1;
                    break;                            
                }            
            }
            if(f==0){ 
                st=0;
                cout<<"Incorrect mask"<<endl;
                cout<<"Octet: "<<i+1<<" "<<SplitFind(mask,'.',i)<<endl;
                return st;
            }    
            //If the octet after step octet is non-zero
            if((step_octet>0)&&(i>step_octet)){
                if(SplitFind(mask,'.',i)!="0"){
                    st=0;
                    cout<<"Incorrect mask"<<endl;
                    cout<<"Octet: "<<i+1<<" "<<SplitFind(mask,'.',i)<<endl;
                    return st;
                }
            }
            //Assigning the step octet number
            if (step_octet==0){
                if (SplitFind(mask,'.',i)!="255") step_octet=i;
            }
               
            f=0;//Octet number coincidence status reset for the 2nd variant
        }
    }
    else{
        st=0;
        cout<<"Incorrect mask value"<<endl;
        return st;
    } 
        
    return st;
}

//The common check function
bool Check(string input){
    string ip;
    string mask;
    bool st=1;
           
    if (CheckInput(input)==0){
        st=0;
        cout<<endl;
        return st;
    }
    
    ip=SplitFind(input,'/',0);
    mask=SplitFind(input,'/',1);
        
    if (CheckIP(ip)==0){
        st=0;
        cout<<endl;
        return st;
    }
    
    if (CheckMask(mask)==0){
        st=0;
        cout<<endl;
        return st;
    }
    
    return st;
}

//The start of the forming the structure with the result
void FS(string ip, string mask, IP *IPT){
    string dmask;
    //The cases of decimal and prefix mask forms
    if(mask.length()>2){
        IPT->pmask=DM2Prefix(mask);
        dmask=mask;
    }
    else{
        IPT->pmask=stoi(mask);
        dmask=Prefix2DM(stoi(mask));
    }    
    //Filling the IP-address and decimal mask arrays in the structure
    for(int i=0;i<4;i++){
        IPT->ip_array[i]=stoi(SplitFind(ip,'.',i));
        IPT->mask_array[i]=stoi(SplitFind(dmask,'.',i));        
    }    
}

//Basic calculations
void Calculation(IP *IPT){
    int step;
    int step_octet;
    int step_array[4];
        
    //Determining the step octet number and the step value
    for(int i=0;i<4;i++){
        if (IPT->mask_array[i]!=255){ 
            step_octet=i;
            break;
        }
    }
    if(IPT->pmask==32) step_octet=3;
    step=256-IPT->mask_array[step_octet];
    
    //Gaining the start value of the step array for finding the network number
    for(int i=0;i<4;i++){
        if(i>=step_octet){
            step_array[i]=0;
        }
        else{
            step_array[i]=IPT->ip_array[i];
        }              
    }   
    
    //Determining the network number
    while(IPT->ip_array[step_octet]-step_array[step_octet]>=step)
        step_array[step_octet]+=step;
    
    //Network number, broadcast, start IP, end IP
    for(int i=0;i<4;i++){
        //Filling the network number array
        IPT->net_array[i]=step_array[i];
        
        //Determning broadcast address
        if(i<step_octet) IPT->bc_array[i]=step_array[i];
        else if(i==step_octet) 
            IPT->bc_array[i]=step_array[i]+step-1;
        else if(i>step_octet) IPT->bc_array[i]=255;
        
        //Determining start IP addrtess
        if(i<3) IPT->start_array[i]=step_array[i];
        else if(i==3) IPT->start_array[i]=step_array[i]+1;
        
        //Determining end IP addrtess
        if(i<3) IPT->end_array[i]=IPT->bc_array[i];
        else if(i==3) IPT->end_array[i]=IPT->bc_array[i]-1;
        
    }
    
    
        
    //Determining the wildcard mask
    for(int i=0;i<4;i++){
        if(IPT->mask_array[i]==255) IPT->wmask_array[i]=0;
        else if(IPT->mask_array[i]==0) IPT->wmask_array[i]=255;
        else IPT->wmask_array[i]=step-1;
    }
    
    //Determining the addresses and hosts numbers
    IPT->an=step;
    for(int i=step_octet;i<4;i++) if(i<3) IPT->an*=256;
    IPT->hn=IPT->an - 2;
    //The case of 32 mask to correct from -1 to 0 value
    if(IPT->pmask==32) IPT->hn++;
}

//Calculating the binary values and displaying the result
void Display(IP *IPT){
    //Declaration and conversion from array to string form
    string ip=IA2S(IPT->ip_array,4,'.');
    string mask=IA2S(IPT->mask_array,4,'.');
    string wmask=IA2S(IPT->wmask_array,4,'.');
    string netnum=IA2S(IPT->net_array,4,'.');
    string bc;
    string start_ip;
    string end_ip;
    
    //Declaration and conversion from array to string form. Binary values.
    string bin_ip=IA2S(IPT->ip_array,4,'.',2);
    string bin_mask=IA2S(IPT->mask_array,4,'.',2);
    string bin_wmask=IA2S(IPT->wmask_array,4,'.',2);
    string bin_netnum=IA2S(IPT->net_array,4,'.',2);
    string bin_bc;
    string bin_start_ip;
    string bin_end_ip;
    
    //The exclusive cases of masks 31 and 32
    if(IPT->pmask>30){
        start_ip="-";
        end_ip="-";
        bin_start_ip="-";
        bin_end_ip="-";
        if(IPT->pmask==32){
            bc="-";
            bin_bc="-";
        }
        else if(IPT->pmask==31){
            bc=IA2S(IPT->bc_array,4,'.');
            bin_bc=IA2S(IPT->bc_array,4,'.',2);
        }
    }
    /*Decimal and binary values of the broadcast address, start and
     * end addresses in regular cases*/
    else{
        bc=IA2S(IPT->bc_array,4,'.');
        start_ip=IA2S(IPT->start_array,4,'.');
        end_ip=IA2S(IPT->end_array,4,'.');
        bin_bc=IA2S(IPT->bc_array,4,'.',2);
        bin_start_ip=IA2S(IPT->start_array,4,'.',2);
        bin_end_ip=IA2S(IPT->end_array,4,'.',2);
    }
    
    //Display 
    cout<<endl;
    cout<<"IP address: "<<ip<<"  "<<bin_ip<<endl;
    cout<<"Mask: "<<mask<<"  "<<bin_mask<<endl;
    cout<<"Mask prefix: "<<IPT->pmask<<endl;
    cout<<"Wildcard mask: "<<wmask<<"  "<<bin_wmask<<endl;
    cout<<"Network number: "<<netnum<<"  "<<bin_netnum<<endl;
    cout<<"Broadcast address: "<<bc<<"  "<<bin_bc<<endl;
    cout<<"Start IP address: "<<start_ip<<"  "<<bin_start_ip<<endl;
    cout<<"End IP address: "<<end_ip<<"  "<<bin_end_ip<<endl;
    cout<<"Addresses: "<<IPT->an<<endl;
    cout<<"Hosts: "<<IPT->hn<<endl;
}

int main(int argc, char **argv){
    char x;//Switch symbol to exit the program
    while(x!='x') {   
        string *input=new string;
        string *ip=new string;
        string *mask=new string;

        //Starting and checking the input data
        if(argc==1){//When running the program directly
            do{
                cout<<"Enter the IP-address and mask in address/mask"
                " or address/prefix format."<<endl;
                cout<<"To close the program press 'x'."<<endl;
                cout<<endl;
                cout<<"Enter the IP-address and mask: ";
                getline(cin, *input);
                if (*input=="x"){ //To close the program
                    x='x';            
                    system("pause");
                    return 0;       
                }                
            }while(Check(*input)!=1);
        }
        else if(argc==2){//When passing the parameters through cmd
            *input=argv[1];
            if(Check(*input)!=1) return 0;
        }
        //Splitting the input string into IP-address and mask
        //strings after successfull check
        *ip=SplitFind(*input,'/',0);
        *mask=SplitFind(*input,'/',1);

        IP *IPT=new IP;

        FS(*ip,*mask, IPT);//Forming the structure with the result

        delete input, ip, mask;

        Calculation(IPT);
        Display(IPT);
        
        delete IPT;
        cout<<endl;
        if(argc==2) break;
    }
    
if(argc==1) system("pause");
return 0;
}                                              
