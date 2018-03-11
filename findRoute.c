#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include "myfile.c"
typedef struct			//消费节点信息
	{
		int adjNode;		//邻接网络节点
		int contain;		//消费节点所需带宽
	}costNode;
void main(){
	/********获取输入文件信息**********************/
	void d_alg(int v,int **adjList,int n,int costNodeN,costNode *costNodes,int **contain,int **flood);
	void d_alg2(int v,int **adjList,int n,int costNodeN,costNode *costNodes,int **contain,int **flood);
	void arr_copy1(int know_array[],int unknow_array[],int dim);
	void arr_copy2(int **know_array,int **unknow_array,int one_dim,int two_dim);
	int ctrNodeN;
	int costNodeN;
	int serverCost;
	int total_route;
	int i,j,m,n,p,**adjList,**contain,**flood;
	int **bp_adjList,**bp_contain;
	costNode *costNodes;
	FILE *example,*fp;
	char isEnd,filename[50];
	if((example = fopen("test.txt","r")) == NULL)
	{
		printf("open this text fail!");
		exit(0);
	}
	ctrNodeN = fgetnum(example);
	serverCost = fgetnum(example);
	costNodeN = fgetnum(example);
	costNodes = (costNode *)malloc(costNodeN*sizeof(costNode));
	adjList = (int **)malloc(ctrNodeN*sizeof(int*));
	for(i = 0; i<ctrNodeN;i++)
		adjList[i] = (int*)malloc(ctrNodeN*sizeof(int));
	bp_adjList = (int **)malloc(ctrNodeN*sizeof(int*));
	for(i = 0; i<ctrNodeN;i++)
		bp_adjList[i] = (int*)malloc(ctrNodeN*sizeof(int));
	flood = (int **)malloc(ctrNodeN*sizeof(int*));
	for(i = 0; i<ctrNodeN;i++)
		flood[i] = (int*)malloc(ctrNodeN*sizeof(int));
	contain = (int **)malloc(ctrNodeN*sizeof(int*));
	for(i = 0; i<ctrNodeN;i++)
		contain[i] = (int*)malloc(ctrNodeN*sizeof(int));
	bp_contain = (int **)malloc(ctrNodeN*sizeof(int*));
	for(i = 0; i<ctrNodeN;i++)
		bp_contain[i] = (int*)malloc(ctrNodeN*sizeof(int));
	fgotoline(example,3);
	serverCost = fgetnum(example);
	fgotoline(example,5);
	while(fgetc(example) != '\n')
	{
		fseek(example,-1,1);
		i = fgetnum(example);
		j = fgetnum(example);
		contain[i][j] = fgetnum(example);
		contain[j][i] = contain[i][j];
		adjList[i][j] = fgetnum(example);
		adjList[j][i] = adjList[i][j];
	}
	for(i = 0; i<ctrNodeN;i++)
		for(j = i;j<ctrNodeN;j++){
			if(i == j) adjList[i][j] = 0;
			else if(adjList[i][j] < 0) 
			{
				adjList[i][j] = 5000;
				adjList[j][i] = 5000;
			}
		}
	for(i = 0; i<ctrNodeN;i++)
		for(j = i;j<ctrNodeN;j++){
			if(i == j) contain[i][j] = 5000;
			else if(contain[i][j] < 0)
			{
				contain[i][j] = 0;
				contain[j][i] = 0;
			}
		}
	fgetc(example);
	i = 0;
	while(1)
	{
		if((isEnd = fgetc(example)) == EOF) break;
		if(isEnd == '\n') break;
		fseek(example,-1,1);
		fgetnum(example);
		costNodes[i].adjNode = fgetnum(example);
		costNodes[i].contain = fgetnum(example);
		i++;
	}
	fclose(example);
	
	for(i = 0;i < ctrNodeN;i++)
	{
		sprintf(filename,"ctrNode_info\\ctrNode%d.txt",i);
		if((fp = fopen(filename,"w+")) == NULL) {
			printf("can't open this file");
			return;
		}
		fclose(fp);
	}
	if((fp = fopen("add.txt","w+")) == NULL) {
			printf("can't open this file");
			return;
		}
		fclose(fp);
	
/*****************获取输入文件信息结束*********************/
	for(m = 0; m<ctrNodeN;m++)
	{
		for(p = 0; p < costNodeN;p++)
		{	
			sprintf(filename,"ctrNode_info\\ctrNode%d.txt",m);
			if((fp = fopen(filename,"a+")) == NULL)
			{
				printf("can't open this file");
				return;
			}
			fprintf(fp,"%d to %d(%d) \n",m,p,costNodes[p].adjNode);
			if(m == costNodes[p].adjNode)
			{
				for(n = 0;n < 5; n++)
					fprintf(fp,"-1 E -1\n");
				fprintf(fp,"\n");
				fclose(fp);
				continue;
			}
			fclose(fp);
			arr_copy2(adjList,bp_adjList,ctrNodeN,ctrNodeN);
			arr_copy2(contain,bp_contain,ctrNodeN,ctrNodeN);
			total_route = 0;
			for(n = 0;n < 5; n++)
			{
				/*for(i = 0; i<ctrNodeN;i++)
				{
					for(j = 0;j<ctrNodeN;j++)
						printf("%d\t",bp_contain[i][j]);
					printf("\n");
				}
				printf("\n");
				*/
				for(i = 0; i<ctrNodeN;i++)
					for(j = 0;j<ctrNodeN;j++)
						flood[i][j] = 0;
										//好在文件中记录第几次
				flood[1][1] = p;		//flood[1][1]记录节点数，flood[ctrNodeN-1][ctrNodeN-1]记录是否路径异常,flood[0][0]记录路径最大容量

				d_alg(m,bp_adjList,ctrNodeN,costNodeN,costNodes,bp_contain,flood);
				total_route += flood[0][0];
				if(flood[ctrNodeN - 1][ctrNodeN - 1] == -1)
				{	
					if((fp = fopen(filename,"a+")) == NULL)
					{
						printf("can't open this file");
						return;
					}
					for(i = 0; i < (5-n);i++)
						fprintf(fp,"-404 F 404\n");
					fclose(fp);
					break;

				}
				for(i = 0; i<ctrNodeN;i++)
					for(j = 0;j<ctrNodeN;j++)
					{
						if(i == j) continue;
						else if((flood[i][j]) > 0)
						{
							if((bp_contain[i][j]-=flood[i][j]) <= 0)
								bp_adjList[i][j] = 5000;
							
						}
					}
			}
			if((fp = fopen(filename,"a+")) == NULL)
					{
						printf("can't open this file");
						return;
					}
			fprintf(fp,"A %d\n",total_route);
			fclose(fp);
			if(!(flood[ctrNodeN - 1][ctrNodeN - 1] == -1 || flood[ctrNodeN - 1][ctrNodeN - 1] == -2 || m == costNodes[p].adjNode))
			{
				while(total_route < costNodes[p].contain)
				{
					for(i = 0; i<ctrNodeN;i++)
						for(j = 0;j<ctrNodeN;j++)
							flood[i][j] = 0;
					flood[1][1] = p;
					if((fp = fopen("add.txt","a+")) == NULL)
					{
						printf("can't open this file");
						return;
					}
					fprintf(fp," %d to %d (%d) :",m,p,costNodes[p].adjNode);
					fclose(fp);
					d_alg2(m,bp_adjList,ctrNodeN,costNodeN,costNodes,bp_contain,flood);
					total_route += flood[0][0];
					if(flood[ctrNodeN - 1][ctrNodeN - 1] == -1 || flood[ctrNodeN - 1][ctrNodeN - 1] == -2)
					{
						fprintf(fp,"\n");
						break;
					}
					for(i = 0; i<ctrNodeN;i++)
						for(j = 0;j<ctrNodeN;j++)
						{
							if(i == j) continue;
							else if((flood[i][j]) > 0)
							{
								if((bp_contain[i][j]-=flood[i][j]) <= 0)
									bp_adjList[i][j] = 5000;
							
							}
						}
				}

			
			}
			
			
		}

		
	}



}
void d_alg(int v,int **adjList,int n,int costNodeN,costNode *costNodes,int **contain,int **flood)			//n is equal to ctrNodeN
{
	void Dispath(int dist[],int path[],int s[],int n,int v,int costNodeN,costNode *costNodes,int **contain,int **flood);
	int *dist,*path;
	int *s;
	int f = 0;
	int mindis,i,j,u,p = costNodes[flood[1][1]].adjNode;
	dist = (int *)malloc(n*sizeof(int));
	path = (int *)malloc(n*sizeof(int));
	s = (int *)malloc(n*sizeof(int));
	for(i = 0;i<n;i++)
	{
		dist[i] = adjList[v][i];
		s[i] = 0;
		if(adjList[v][i] < 5000)
			path[i] = v;
		else 
			path[i] = -1;
	}
	s[v] = 1;
	path[v] = v;
	for(i = 0;i < n;i++)
	{
		mindis = 5000;
		for(j = 0;j < n;j++)
			if(s[j] == 0 && dist[j] < mindis)
			{
				u = j;
				mindis = dist[j];
				f += 1;
			}
		if(f == 0)
		{
			flood[n-1][n-1] = -1;
			return;
		}
		s[u] = 1;
		if(u == p) break;
		for(j = 0;j < n; j++)
			if(s[j] == 0)
				if(adjList[u][j] < 5000 && adjList[u][j]+dist[u] < dist[j])
				{
					dist[j] = dist[u] +adjList[u][j];
					path[j] = u;
				}
	}
	
	Dispath(dist,path,s,n,v,costNodeN,costNodes,contain,flood);
	free(dist);
	free(path);
	free(s);
}
void Ppath(int path[],int i,int v,int road[],int *j)
{
	int k;
	k = path[i];
	if(k == -1)
	{
		road[20] = -2;
		return;
	}
	if(k == v) return;
	Ppath(path,k,v,road,j);
	road[(*j)++] = k;
	/*printf("%d,",k);*/
}
void Dispath(int dist[],int path[],int s[],int n,int v,int costNodeN,costNode *costNodes,int **contain,int **flood)
{
	int i,minCost,j = 0,maxRoute = 5000,l;
	int *p_j = &j;
	int road_pr,p = flood[1][1];
	char filename[50];
	char s_input[200] = {'\0'};
	FILE *fp;
	int road[20] = {-1};
	sprintf(filename,"ctrNode_info\\ctrNode%d.txt",v);
	if((fp = fopen(filename,"a+")) == NULL)
			{
				printf("open %s fail!",filename);
				return;
			}
	l = costNodes[p].adjNode;
	if(s[l] == 1)
	{	
		/*if(costNodes[i].contain < minRoute) 
			minRoute = costNodes[i].contain;*/
		
		minCost = dist[l];
		road[j++] = v;
		/*
		printf("\n?*?\npath:\n");
		for(l = 0;l <n;l++)
		{
			printf("%d\t",path[l]);
			if((l+1)%10 == 0) printf("\n");
		}
		printf("m = %d\n",m);
		printf("v = %d\n",v);
		printf("road:\t");
		for(l = 0;l <20;l++){
			printf("%d\t",road[l]);
			if((l+1)%10 == 0) printf("\n");
		}
		printf("j = %d\n",*p_j);
		*/	
			/*printf("%d,",v);*/
			Ppath(path,l,v,road,p_j);
			if(road[20] == -2)
			{
				flood[n-1][n-1] = -1;
			}
			else 
			{
				road[j] = l;
				/*printf("%d\n",p);*/
				for(i = 0;i < j;i++)	/*获取路径最大通量*/
					if(contain[road[i]][road[i+1]] < maxRoute)
						maxRoute = contain[(road[i])][road[i+1]];
				road_pr = costNodes[p].contain*minCost;
				flood[0][0] = maxRoute;
				/*printf("maxRoute = %d\t,minCost = %d\t,road_pr = %d\n",maxRoute,minCost,road_pr);*/
				for(i = 0;i <= j;i++)
					sprintf(s_input,"%s%d ",s_input,road[i]);
				sprintf(s_input,"%s%d E %d %d %d\n",s_input,p,maxRoute,minCost,road_pr);
				fputs(s_input,fp);
				for(i = 0;i < j;i++)
				{
					flood[road[i]][road[i+1]]+=maxRoute;
					/*printf("%d -> %d :%d\t",road[l],road[l+1],flood[road[l]][road[l+1]]);
					printf("\n");*/
				}

			}
		
			/*printf("******");
			for(l = 0;l <= j; l++)
				printf("%d\t",road[l]);
			printf("*******\n");*/
	}
	else
	{
		fprintf(fp,"-1 B -1\n");
		flood[n - 1][n - 1] = -2;
	}
	fclose(fp);
}
void Dispath2(int dist[],int path[],int s[],int n,int v,int costNodeN,costNode *costNodes,int **contain,int **flood)
{
	int i,minCost,j = 0,maxRoute = 5000,l;
	int *p_j = &j;
	int road_pr,p = flood[1][1];
	char filename[50];
	char s_input[200] = {'\0'};
	FILE *fp;
	int road[20] = {-1};
	sprintf(filename,"add.txt",v);
	if((fp = fopen(filename,"a+")) == NULL)
			{
				printf("open %s fail!",filename);
				return;
			}
	l = costNodes[p].adjNode;
	if(s[l] == 1)
	{	
		/*if(costNodes[i].contain < minRoute) 
			minRoute = costNodes[i].contain;*/
		
		minCost = dist[l];
		road[j++] = v;
		/*
		printf("\n?*?\npath:\n");
		for(l = 0;l <n;l++)
		{
			printf("%d\t",path[l]);
			if((l+1)%10 == 0) printf("\n");
		}
		printf("m = %d\n",m);
		printf("v = %d\n",v);
		printf("road:\t");
		for(l = 0;l <20;l++){
			printf("%d\t",road[l]);
			if((l+1)%10 == 0) printf("\n");
		}
		printf("j = %d\n",*p_j);
		*/	
			/*printf("%d,",v);*/
			Ppath(path,l,v,road,p_j);
			if(road[20] == -2)
			{
				flood[n-1][n-1] = -1;
			}
			else 
			{
				road[j] = l;
				/*printf("%d\n",p);*/
				for(i = 0;i < j;i++)	/*获取路径最大通量*/
					if(contain[road[i]][road[i+1]] < maxRoute)
						maxRoute = contain[(road[i])][road[i+1]];
				road_pr = costNodes[p].contain*minCost;
				flood[0][0] = maxRoute;
				/*printf("maxRoute = %d\t,minCost = %d\t,road_pr = %d\n",maxRoute,minCost,road_pr);*/
				for(i = 0;i <= j;i++)
					sprintf(s_input,"%s%d ",s_input,road[i]);
				sprintf(s_input,"%s%d E %d %d %d\n",s_input,p,maxRoute,minCost,road_pr);
				fputs(s_input,fp);
				for(i = 0;i < j;i++)
				{
					flood[road[i]][road[i+1]]+=maxRoute;
					/*printf("%d -> %d :%d\t",road[l],road[l+1],flood[road[l]][road[l+1]]);
					printf("\n");*/
				}

			}
		
			/*printf("******");
			for(l = 0;l <= j; l++)
				printf("%d\t",road[l]);
			printf("*******\n");*/
	}
	else
	{
		/*fprintf(fp,"-1 B -1\n");*/
		flood[n - 1][n - 1] = -2;
	}
	fclose(fp);
}
void d_alg2(int v,int **adjList,int n,int costNodeN,costNode *costNodes,int **contain,int **flood)			//n is equal to ctrNodeN
{
	void Dispath2(int dist[],int path[],int s[],int n,int v,int costNodeN,costNode *costNodes,int **contain,int **flood);
	int *dist,*path;
	int *s;
	int f = 0;
	int mindis,i,j,u,p = costNodes[flood[1][1]].adjNode;
	dist = (int *)malloc(n*sizeof(int));
	path = (int *)malloc(n*sizeof(int));
	s = (int *)malloc(n*sizeof(int));
	for(i = 0;i<n;i++)
	{
		dist[i] = adjList[v][i];
		s[i] = 0;
		if(adjList[v][i] < 5000)
			path[i] = v;
		else 
			path[i] = -1;
	}
	s[v] = 1;
	path[v] = v;
	for(i = 0;i < n;i++)
	{
		mindis = 5000;
		for(j = 0;j < n;j++)
			if(s[j] == 0 && dist[j] < mindis)
			{
				u = j;
				mindis = dist[j];
				f += 1;
			}
		if(f == 0)
		{
			flood[n-1][n-1] = -1;
			return;
		}
		s[u] = 1;
		if(u == p) break;
		for(j = 0;j < n; j++)
			if(s[j] == 0)
				if(adjList[u][j] < 5000 && adjList[u][j]+dist[u] < dist[j])
				{
					dist[j] = dist[u] +adjList[u][j];
					path[j] = u;
				}
	}
	
	Dispath2(dist,path,s,n,v,costNodeN,costNodes,contain,flood);
	free(dist);
	free(path);
	free(s);
}
void arr_copy1(int know_array[],int unknow_array[],int dim)
{
	int i;
	for(i = 0;i < dim;i++)
		unknow_array[i] = know_array[i];
}
void arr_copy2(int **know_array,int **unknow_array,int one_dim,int two_dim)
{
	int i,j;
	for(i = 0;i < one_dim;i++)
		for(j = 0;j < two_dim;j++)
			unknow_array[i][j]= know_array[i][j];
}
