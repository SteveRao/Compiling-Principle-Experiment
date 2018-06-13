//�ʷ�������
#include<iostream>
#include<cstdio> 
#include<ctype.h>
#include<cstdlib>
#include<cstring>
#include<map>
using namespace std;
map<string,int> reMap;
map<string,int> odMap;
//�����ֱ�
static char reserveWord[32][20] = {
"auto", "break", "case", "char", "const", "continue",
"default", "do", "double", "else", "enum", "extern",
"float", "for", "goto", "if", "int", "long",
"register", "return", "short", "signed", "sizeof", "static",
"struct", "switch", "typedef", "union", "unsigned", "void",
"volatile", "while"
};

//����������
static char operatorOrDelimiter[37][10] = {
"+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
"!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
"&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
"}", "\\", ".", "\?", ":", "!","\\n"
};

const int MAXN=10000;
char file[MAXN],tempFile[MAXN];
FILE *fp,*fp1;

void Input(){
	if((fp=fopen("E://��̴���/compilingPrinciple/0611_lexicalAnalyzer/origin.txt","r"))==NULL){
		printf("The file can't open!");exit(0);
	}
	int n=0;
	while((tempFile[n++]=fgetc(fp))!=EOF);
}
//������������Ԥ����,ɾ��ע�ͺͷǱ�Ҫ�Ŀո� 
void Preprocess(){
	int n=sizeof(tempFile)/sizeof(char);
	int k=0;
	for(int i=0;i<n;){
		switch (tempFile[i]){
			case ' '://ɾ���ո� 
				if(tempFile[i+1]==' '){
					i++;
				}else file[k++]=tempFile[i++];
				break;
			case '/':
				if(tempFile[i+1]=='*'){//ɾ��/**/�ڵ����� 
					while(!(tempFile[i]=='*'&&tempFile[i+1]=='/'))i++;
					i++;
				} else if (tempFile[i+1]=='/'){//ɾ��//���һ��ע�� 
					while(tempFile[++i]!='\n');
				}
				i++;
				break;
			case '\n':
				i++;
				break;
			default:
				file[k++]=tempFile[i++];
				break;
		}	
	}
}

/*
1��ϵͳ�����֣�ϵͳ�У� 
2���û���ʶ�����û����壩 
3�����֣��û����壩 
4����������������ϵͳ�У� 
*/ 
void lexicalAnalyzer(){
//	fp1=fopen("E://��̴���/compilingPrinciple/0611_lexicalAnalyzer/result.txt","w+");
	int n=sizeof(file)/sizeof(char);
	char token[20];
	int k=0;//token�ַ�����λ�� 
	for(int i=0;i<n;i++){//��������λ��ָ��
		if(isspace(file[i]))continue;
		else if(isalpha(file[i])){
			token[k=0]=file[i];
			for(i++;i<n&&isalnum(file[i]);){
				token[++k]=file[i++];
			}
			i--;token[k+1]='\0';
			if(reMap.count(token))printf("(%d,%s)\n",reMap[token],token);//����ؼ��� 
			else printf("(100,%s)\n",token);//����Ϊ��ʶ�� 
		}
		else if(isdigit(file[i])){//���Ϊ���� 
			token[k=0]=file[i];
			for(i++;i<n&&isdigit(file[i]);){
				token[++k]=file[i++];
			}
			i--;
			printf("(101,%s)\n",token);
		}
		else if(ispunct(file[i])){//��������ǳ���ĸ�����ֻ�ո�����Ŀɴ�ӡ�ַ� 
			token[k=0]=file[i];
			token[++k]=file[++i];
			if(odMap.count(token)){
				printf("(%d,%s)\n",odMap[token],token);
			}else{
				token[k]='\0';
				i--;k--;
				if(odMap.count(token))
					printf("(%d,%s)\n",odMap[token],token);
			}
		}
		memset(token,0,sizeof(token));	
	}
}

int main(){
	Input();
	Preprocess();
	printf("%s\n",tempFile);
	printf("\n%s\n",file);
	string s1,s2;
	int n=sizeof(operatorOrDelimiter)/10*sizeof(char); 
	int m=sizeof(reserveWord)/20*sizeof(char);
	for(int i=0;i<n;i++){
		s1=operatorOrDelimiter[i];
		odMap.insert(make_pair(s1,i));
		if(i<m){
			s2=reserveWord[i];
			reMap.insert(make_pair(s2,i+n)); 
		}
	}
	lexicalAnalyzer();
	return 0;
}

