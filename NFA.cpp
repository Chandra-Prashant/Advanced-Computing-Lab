#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int states;
int symbolCount;
char alphabet[20];
int trans[20][20][20];
int transCount[20][20];
int epsilonTrans[20][20];
int epsilonCount[20];
int startState;
int finalStates[20];
int finalCount;

bool isFinal(int s){
    for(int i=0;i<finalCount;i++)
        if(finalStates[i]==s) return true;
    return false;
}

int getSymbolIndex(char c){
    for(int i=0;i<symbolCount;i++)
        if(alphabet[i]==c) return i;
    return -1;
}

void printPath(int pathStates[], char pathSymbols[], int length){
    cout<<pathStates[0];
    for(int i=1;i<length;i++)
        cout<<" --"<<pathSymbols[i]<<"--> "<<pathStates[i];
    cout<<endl;
}

void dfs(int state,int pos,char input[],int pathStates[],char pathSymbols[],int pathLen,bool &found){
    if(input[pos]=='\0'){
        if(isFinal(state)){
            found=true;
            cout<<"Accepted Path: ";
            printPath(pathStates,pathSymbols,pathLen);
        }
    }
    for(int i=0;i<epsilonCount[state];i++){
        int next=epsilonTrans[state][i];
        pathStates[pathLen]=next;
        pathSymbols[pathLen]='e';
        dfs(next,pos,input,pathStates,pathSymbols,pathLen+1,found);
    }
    if(input[pos]!='\0'){
        int index=getSymbolIndex(input[pos]);
        if(index==-1) return;
        for(int i=0;i<transCount[state][index];i++){
            int next=trans[state][index][i];
            pathStates[pathLen]=next;
            pathSymbols[pathLen]=input[pos];
            dfs(next,pos+1,input,pathStates,pathSymbols,pathLen+1,found);
        }
    }
}

int main(){
    ifstream file("nfa.txt");
    if(!file){
        cout<<"File not found\n";
        return 0;
    }
    file>>states;
    file>>symbolCount;
    for(int i=0;i<symbolCount;i++)
        file>>alphabet[i];
    file>>startState;
    file>>finalCount;
    for(int i=0;i<finalCount;i++)
        file>>finalStates[i];
    int transitions;
    file>>transitions;
    for(int i=0;i<transitions;i++){
        int from,to;
        char sym;
        file>>from>>sym>>to;
        if(sym=='e')
            epsilonTrans[from][epsilonCount[from]++]=to;
        else{
            int index=getSymbolIndex(sym);
            trans[from][index][transCount[from][index]++]=to;
        }
    }
    file.close();
    while(true){
        char input[100];
        cout<<"\nEnter input string (type exit to stop): ";
        cin>>input;
        if(strcmp(input,"exit")==0)
            break;
        int pathStates[100];
        char pathSymbols[100];
        pathStates[0]=startState;
        bool found=false;
        dfs(startState,0,input,pathStates,pathSymbols,1,found);
        if(!found)
            cout<<"String Rejected\n";
    }
}