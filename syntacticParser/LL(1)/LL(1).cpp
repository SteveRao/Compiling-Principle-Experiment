//LL(1)�Զ������﷨������

/*�㷨���� 
*1)����������ķ����в���ʽ
*2)���ݲ���ʽ������ս����First��Follow���Լ�����ʽ��Select��
*3)���ݲ���ʽ��Select�����������
*4)���ݹ���ķ������Ծ��ӽ����﷨����
*/
#include<iostream>
#include<cstdio>
#include<cstdlib> 
#include<map>
#include<set>
#include<iterator>
#include<vector>
#include<cstring>
#include<stack>
using namespace std;
FILE* fp;
typedef struct Node{
	char start;
	char end[10]; 
}Production;
const int MAXN=20;//���¹��Ƶķ��ս�����ս������ 
int analysisFigure[MAXN][MAXN];//������ṹ
map<char,int> xMap,yMap;//�ֱ������洢���ս�����ս�� 
map<int,Production> pMap;//�洢����ʽ 
map<int,set<char> > pdFirstSet;//�洢ÿһ������ʽ��first����int���ʾ����ʽ��� 
map<int,set<char> > firstSet;//int ���ʾ���ս�����,set���ϴ洢��Ӧfirst����
map<int,set<char> > followSet;//ͬ��
map<int,set<char> > selectSet;//int ���ʾ����ʽ�ı��,set���ϴ洢��Ӧselect����  
stack<char> alStack;//����ջ
stack<char> rmStack;//��������ջ 

void buildAnalysisGraph(){
	for(int i=0;i<pMap.size();i++){
		char s=pMap[i].start;
		int xid=xMap[s];
		set<char>::iterator it;
		for(it=selectSet[i].begin();it!=selectSet[i].end();it++){
			int yid=yMap[(*it)];
			analysisFigure[xid][yid]=i;
		} 
	}
}

set<char> getFirstSetByString(char end[],int s){
	set<char> Set;
	for(int i=s;i<strlen(end);i++){
		if(!isupper(end[i])){
			Set.insert(end[i]);
			break;
		}
		int sid=xMap[end[i]];
		if(!firstSet[sid].count('&')){
			set<char> fSet=firstSet[sid];
			for(set<char>::iterator it1=fSet.begin();it1!=fSet.end();it1++){
				Set.insert((*it1));
			}
			break;//������� 
		}else if(firstSet[sid].count('&')){
			set<char> fSet=firstSet[sid];
			for(set<char>::iterator it1=fSet.begin();it1!=fSet.end();it1++){
				//if((*it1)!='&')
				char c=(*it1);
				Set.insert((*it1));//�����ս������� 
		    } 
		}
	}
	return Set;
}

void getSelectSet(){
	for(int i=0;i<pMap.size();i++){
		char end[10];
		strcpy(end,pMap[i].end);
		set<char> Set=getFirstSetByString(end,0);
		char start=pMap[i].start;
		int xid=xMap[start];
		if(end[0]=='&'){//SelectSet����FollowSet 	
			selectSet[i]=followSet[xid];
		}else if(strlen(end)>0&&end[0]!='&'&&Set.empty()){//SelectSet����FollowSet��FirstSet 
		    set<char> Set;
		    set<char>::iterator it;
		    for(it=pdFirstSet[i].begin();it!=pdFirstSet[i].end();it++)
		    Set.insert((*it));
		    for(it=followSet[xid].begin();it!=followSet[xid].end();it++)
		    Set.insert((*it));
		    selectSet[i]=Set;
		
		}else if(strlen(end)>0&&end[0]!='&'&&!Set.empty()){//����FirstSet
			selectSet[i]=pdFirstSet[i];
		} 
	}
}


void getFollowSet(){
	/*�㷨: 
	*1����A���ķ���ʼ���ţ���#����Follow(A)�� 
	*2�������ڲ���ʽB->..Aa..�����a���뵽Follow(A)��
	*3�������ڲ���ʽB->..AB..,���First(B..)���뵽Follow(A)��
	*4�������ڲ���ʽB->..A&,���Follow(B)���뵽Follow(A) 
	*/
	char s=pMap[0].start;
	int sid=xMap[s];followSet[sid].insert('#');//�㷨�е�һ����� 
	map<int,Production>::iterator it;
	for(it=pMap.begin();it!=pMap.end();it++){//�����в���ʽ����ڶ������ 
		char end[10];
		strcpy(end,(*it).second.end);
		for(int i=0;i<strlen(end);i++){
			if(isupper(end[i])&&(islower(end[i+1])||(isprint(end[i+1])&&!isalpha(end[i+1])))){
				int eid=xMap[end[i]];
				followSet[eid].insert(end[i+1]);
				i++;
			}
		}
	} 
	for(it=pMap.begin();it!=pMap.end();it++){//�����в���ʽ������������
		char end[10];
		strcpy(end,(*it).second.end);
		for(int i=0;i+1<strlen(end);i++){
			if(isupper(end[i])&&isupper(end[i+1])){
				set<char> Set=getFirstSetByString(end,i+1);
				int mid=xMap[end[i]];
				for(set<char>::iterator it1=Set.begin();it1!=Set.end();it1++){
				    if((*it1)!='&')
				    followSet[mid].insert((*it1));
				}
			}
		}
	}
	for(it=pMap.begin();it!=pMap.end();it++){//�����в���ʽ������������
	    char start=(*it).second.start;
	    int sid=xMap[start];
		char end[10];
		strcpy(end,(*it).second.end);
		for(int i=strlen(end)-1;i>=0;i--){
			set<char> Set;
			Set=getFirstSetByString(end,i);
			if(!isupper(end[i]))break;
			if(isupper(end[i])&&!Set.count('&')){
				int eid=xMap[end[i]];
				for(set<char>::iterator it1=followSet[sid].begin();it1!=followSet[sid].end();it1++){
				    followSet[eid].insert((*it1));
				}
				break;
			} 
			else if(i>=0&&isupper(end[i])&&Set.count('&')){
				int eid=xMap[end[i]];
				for(set<char>::iterator it1=followSet[sid].begin();it1!=followSet[sid].end();it1++){
					followSet[eid].insert((*it1));
				}
			
			}
		} 
	}	
}

void getFirstSet(){
	/*
	*�㷨��
	*1�����ȼ�����򵥵����A->aB,��a���뵽First(A)
	*2������A->Ba�����,��First(B)�������ռ�,��First(B)���뵽First(A) 
	*3�����ڵڶ����������First(B)�����ռ�,�����ռ���First(B)���뵽First(A)���ڽ�aҲ����First(A)
	*4���ظ����Ϲ���ֱ���ܵ������������� 
	*/ 
	int vis[MAXN]; 
	memset(vis,0,sizeof(vis));
	int l=0,n=0;//��ʼ�����з��ս����first��Ԫ������ 
	while(l==0||l!=n){
		l=n;
		for(map<int,Production>::iterator it=pMap.begin();it!=pMap.end();it++){
			int pid=(*it).first;
			if(!vis[pid]){//����ò���ʽ��ͷ��firstû�м����� 
			    Production p=(*it).second;//ȡ������ʽ 
			    char start=p.start;
			    int sid=xMap[start];n-=firstSet[sid].size();
			    int k=strlen(p.end);
			    for(int i=0;i<k;i++){//����һ������ʽ��ͷ�����ս����first�� 
			    	char x=p.end[i];
				
					if(i==0&&!isupper(x)){//������ 
					    pdFirstSet[pid].insert(x);
						firstSet[sid].insert(x);
						vis[pid]=1;
					    break;//�������,�����������forѭ�� 
					}else if(isupper(p.end[i])&&firstSet[xMap[x]].size()==0){
					    	break;
					}
				    else if(isupper(p.end[i])&&firstSet[xMap[x]].size()!=0){//�μ���� 
				    	int id=xMap[x];//���ս���ı�� 
					    set<char> fSet=firstSet[id];
					    if(!fSet.count('&')){
					    	for(set<char>::iterator it1=fSet.begin();it1!=fSet.end();it1++){
					    		pdFirstSet[pid].insert((*it1));
						        firstSet[sid].insert((*it1));//�����ս����first������p.start���ս��first���� 
					        } 
					        vis[pid]=1;
					        break;//�������,�����������forѭ�� 
					    }
						else if(isupper(p.end[i])&&firstSet[xMap[x]].size()!=0&&firstSet[xMap[x]].count('&')){//������
					    	for(set<char>::iterator it1=fSet.begin();it1!=fSet.end();it1++){
				    		    if((*it1)!='&'){
				    		    	firstSet[sid].insert((*it1));//�����ս����first������p.start���ս��first���� 
				    		    	pdFirstSet[pid].insert((*it1));
				    		    }
						        
					        } 
					    }
				    }
			    } 
			n+=firstSet[sid].size(); 
			}					
	    }
	}	
}

char rmStackArray[MAXN];
int rmIndex;
int cnt=0,alIndex=0;
char alStackArray[MAXN]; 
char a,r;
Production pdt;


void Print(int pid=-1){
	printf("%d\t",++cnt);
	for(int i=0;i<alIndex;i++)printf("%c",alStackArray[i]);
	printf("\t");
	for(int i=rmIndex-1;i>=0;i--)printf("%c",rmStackArray[i]);
	if(pid!=-1&&isupper(a)){
		pdt=pMap[pid];
		printf("\t\t%c->%s\n",pdt.start,pdt.end);
	}else printf("\n");
} 



int main(){
	while((fp=fopen("E://��̴���/compilingPrinciple/0614_LL(1)_Parser/Input.txt","r"))==NULL){
		printf("Don't open the file!");exit(0);
	}
	char s1;
	char s2[10];
	int i=0,j=0,k=0;//�ֱ��ʾ����ʽ�����ս�����ս������ 
	fscanf(fp,"%s\n",rmStackArray);
	rmIndex=strlen(rmStackArray);
	while(fscanf(fp,"%c->%s\n",&s1,&s2)!=EOF){
		Production pro;pro.start=s1;strcpy(pro.end,s2);//��ʼ������ʽ������洢 
		pMap[i++]=pro;
		if(!xMap.count(s1))
		xMap[s1]=j++;
		int n=strlen(s2);
		for(int t=0;t<n;t++)
		if(!yMap.count(s2[t])&&isgraph(s2[t])&&!isupper(s2[t]))
		yMap[s2[t]]=k++;
		memset(s2,0,sizeof(s2));//��ʵ��ʼ��s2�����ַ�����; 
	}
	yMap['#']=k; 
	
	getFirstSet();
	getFollowSet();
	getSelectSet();
	memset(analysisFigure,-1,sizeof(analysisFigure));
	buildAnalysisGraph();
	
	
	for(int i=0;i<rmIndex;i++){
		rmStack.push(rmStackArray[i]);
	}
	char start=pMap[0].start;
	alStack.push('#');alStack.push(start);
    alStackArray[alIndex++]='#';alStackArray[alIndex++]=start;
	int pid,xid,yid,flag=0;
	printf("����\t����ջ\t�������봮\t���ò���ʽ\n");
	
	    
	while(rmStack.size()>0){
		//����������� 
		
		a=alStack.top();
		r=rmStack.top();
		
		if(isupper(a)){
			
			xid=xMap[a];yid=yMap[r];
			pid=analysisFigure[xid][yid];
			if(pid==-1){
				flag=1;
			    break;
			}
			Print(pid); 
			pdt=pMap[pid];
			alStack.pop();alIndex--;//����д��ĸ����ջ 
			for(int i=strlen(pdt.end)-1;i>=0;i--){
				if(pdt.end[i]!='&'){
					alStack.push(pdt.end[i]);
				    alStackArray[alIndex++]=pdt.end[i];
				}
			}
		    //Print();
			a=alStack.top();
		    r=rmStack.top();
		    if(a==r){
		    	Print();
		    	alStack.pop();alIndex--;
			    rmStack.pop();rmIndex--;
		    }
			continue;
		} else if(a==r){//������ջ����ͬ��Сд��ĸͬʱ���� 
			alStack.pop();alIndex--;
			rmStack.pop();rmIndex--;
			Print();
			continue;
		}else{
			flag=1;
			break;
		}
		
	}
	if(flag)printf("\nanalysis Failed!\n");
	else printf("\nanalysis Successfully!\n");
	return 0;
} 







 
