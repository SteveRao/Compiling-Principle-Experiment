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
				if((*it1)!='&')
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
			if(isupper(end[i])&&islower(end[i+1])){
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
				for(set<char>::iterator it1=Set.begin();it1!=Set.end();it1++)
				followSet[mid].insert((*it1));
			}
		}
	}
	for(it=pMap.begin();it!=pMap.end();it++){//�����в���ʽ������������
	    char start=(*it).second.start;
	    int sid=xMap[start];
		char end[10];
		strcpy(end,(*it).second.end);
		for(int i=0;i<strlen(end);i++){
			set<char> Set;
			if(i+1<strlen(end))
			Set=getFirstSetByString(end,i+1); 
			if(isupper(end[i])&&((i+1)==strlen(end)||Set.empty())){
				int eid=xMap[end[i]];
				for(set<char>::iterator it1=followSet[sid].begin();it1!=followSet[sid].end();it1++)
				followSet[eid].insert((*it1));
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
					}
				    else if(isupper(p.end[i])){//�μ���� 
				    	int id=xMap[x];//���ս���ı�� 
					    set<char> fSet=firstSet[id];
					    if(!fSet.count('&')){
					    	for(set<char>::iterator it1=fSet.begin();it1!=fSet.end();it1++){
					    		pdFirstSet[pid].insert((*it1));
						        firstSet[sid].insert((*it1));//�����ս����first������p.start���ս��first���� 
					        } 
					        vis[pid]=1;
					        break;//�������,�����������forѭ�� 
					    }else{//������
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


int main(){
	while((fp=fopen("E://��̴���/compilingPrinciple/0614_LL(1)_Parser/Input.txt","r"))==NULL){
		printf("Don't open the file!");exit(0);
	}
	char s1;
	char s2[10];
	int i=0,j=0,k=0;//�ֱ��ʾ����ʽ�����ս�����ս������ 
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
	
	char rm[]="aade#";
	for(int i=strlen(rm);i>=0;i--){
		rmStack.push(rm[i]);
	}
	
	int cnt=0;
	alStack.push('#');
	alStack.push('A');
	while(rmStack.size()>1){
		char a=alStack.top();alStack.pop();
		char r=rmStack.top();rmStack.pop();
		if(isupper(a)){
			int xid=xMap[a],yid=yMap[r];
			int pid=analysisFigure[xid][yid];
			if(pid==-1){
				cout<<"analysis Failed!"<<endl;
				break;
			}
			Production pdt=pMap[pid];
			for(int i=strlen(pdt.end);i>=0;i--){
				alStack.push(pdt.end[i]);
			}
		} else if(a==r)continue;
	}
	cout<<"analysis Successfully!"<<endl;
	
	
	
	
	
	
	
	
	/*cout<<"Analysis Figure:"<<endl;
	for(int i=0;i<xMap.size();i++){
		for(int j=0;j<yMap.size();j++){
			cout<<analysisFigure[i][j]<<" ";
		}
		cout<<endl;
	}
	
	cout<<"First Set:"<<endl;
	for(int i=0;i<pdFirstSet.size();i++){
		for(set<char>::iterator it=pdFirstSet[i].begin();it!=pdFirstSet[i].end();it++){
			cout<<(*it)<<" ";
		}
		cout<<endl;
	}
	
	cout<<"Follow Set:"<<endl;
	for(int i=0;i<followSet.size();i++){
		for(set<char>::iterator it=followSet[i].begin();it!=followSet[i].end();it++){
			cout<<(*it)<<" ";
		}
		cout<<endl;
	}
	
	
	cout<<"Select Set:"<<endl;
	for(int i=0;i<selectSet.size();i++){
	   for(set<char>::iterator it=selectSet[i].begin();it!=selectSet[i].end();it++){
			cout<<(*it)<<" ";
		}
		cout<<endl;
	}*/
	
	/*for(int i=0;i<firstSet.size();i++){
		for(set<char>::iterator it=firstSet[i].begin();it!=firstSet[i].end();it++){
			cout<<(*it)<<" ";
		}
		cout<<endl;
	}
	for(int i=0;i<pMap.size();i++)
	cout<<pMap[i].start<<"->"<<pMap[i].end<<endl;*/
	/*for(map<char,int>::iterator it=yMap.begin();it!=yMap.end();it++)
	cout<<(*it).first<<":"<<(*it).second<<" ";
	cout<<endl;
	for(map<char,int>::iterator it=xMap.begin();it!=xMap.end();it++)
	cout<<(*it).first<<":"<<(*it).second<<" ";*/
	return 0;
} 







 
