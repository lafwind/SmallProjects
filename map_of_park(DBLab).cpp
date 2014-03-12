#include <iostream>
using namespace std;

#define	Max 60

struct ViewSpot 
{
	char name[36];
	int happiness;
	int time;
	int cost;
};

ViewSpot VS[Max]; //
int G[Max][Max];
int bevisited[Max]; //Determine if the viewspot was visited: y->1, n->0.
int N = 0; //num_of_spots
int stack[Max];
int F_shortest[Max][Max]; //for floyd alg
int path[Max][Max];
int bev_num;
bool tag;

bool to_visit(int v)
{
	bevisited[v] = 1;
	bev_num ++;

	return 1;
}

int first_adj_spot(int v) //find the first spot the v adject to
{
	for (int i = 1; i <= N; i++){
		if(G[v][i] > 0){
			return i;
		}
	}
	return -1;
}

int next_adj_spot(int v, int w) //find the spot the v adject to after w
{
	for (int i = w+1; i <= N; i++){
		if(G[v][i] > 0){
			return i;
		}
	}
	return -1;
}

bool judge()
{
	for (int i = 1; i <= N; i++){
		if(!bevisited[i]){
			return false;
		}
	}
	return true;
}

void dfs(int v,int top)
{
	int w;
	bevisited[v]=1;
	stack[top]=v;
	for(w = first_adj_spot(v);w >= 0;w = next_adj_spot(v,w)){
		if(!bevisited[w])
			dfs(w, top ++);
	}
	if(judge()){
		tag = 1;
		return;
	}
	bevisited[v]=0;
	stack[top]=-1;
}

void floyd()
{
	int i, j, k;
	for (i = 1; i <= N; i++){
		for (j = 1; j <= N; j++){
			F_shortest[i][j] = G[i][j];
			if(i != j && F_shortest[i][j] > 0)
				path[i][j] = i;
			else path[i][j] = 0;
		}
	}
	for ( k = 1; k <= N; k++){
		for (i = 1; i <= N; i++){
			for (j = 1; j <= N; j++){
				if(F_shortest[i][k] + F_shortest[k][j] < F_shortest[i][j]){
					F_shortest[i][j] = F_shortest[i][k] + F_shortest[k][j];
					path[i][j] = path[k][j];
				}
			}
		}
	}
}

//void is_in_path(int v1, int v2) //Judge if a v in the path
//{
//	for (int i = 1; i < N; i++){
//		int step;
//		step = path[v1][v2];
//		while (step != v1){
//			if(step == stack[i]){
//				stack[i] = v1;
//			}
//			step = path[v1][step];
//		}
//	}
//}

/*
 * Initial the map of the park.
 */
void init_spot(ViewSpot v[])
{
	int i = 0;
	char ans;
	cout<<"Add new view spot?"<<endl;
	cin>>ans;
	while (ans == 'y'){
		N = N+1;
		i = i+1;
		cout<<"name: ";
		cin>>v[i].name;
		cout<<"happiness: ";
		cin>>v[i].happiness;
		cout<<"time: ";
		cin>>v[i].time;
		cout<<"cost: ";
		cin>>v[i].cost;
		cout<<"Continue?"<<endl;
		cin>>ans;
	}
}

void init_route() //Matrix
{
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= i; j++){
			if(j == i){
				G[i][j] = 0;
			}
			else{
				int r_time; //the time between two spots
				cout<<"Time between "<<VS[i].name<<" to "<<VS[j].name<<": ";
				cin>>r_time;
				G[i][j] = r_time;
				G[j][i] = r_time;
			}
		}
	}
}

void init_bevisite() //be or not be
{
	for (int i = 1; i <= N; i++){
		bevisited[i] = 0;
	}
}

void init_stack()
{
	for (int i = 0; i < N; i++){
		stack[i] = -1;
	}
}

/*
 * Design a path to get the most happiness in specified time.
 * Visitors cannot repeatedly visit the same spot.
 * However, they can pass them many times without view. 
 */
//void max_h_in_t(int t)
//{
//	bool ftag = 0;
//	int h_time, l_time, g_time;
//	while (ftag == 0){
//		init_stack();
//		int top = 0;
//		int total_h[60][1024];
//		for (int i = 1; i <= N; i++){
//			for (int j = 0; j <= t; j++){
//				if (j >= VS[i].time){
//					total_h[i][j] = max(total_h[i-1][j], total_h[i-1][j-VS[i].time]+VS[i].happiness);
//				}
//				else{
//					total_h[i][j] = total_h[i-1][j];
//				}
//			}
//		}
//		cout<<total_h[N][t]<<endl;
//		int j = t;
//		for (int i = N; i >= 1; i--){
//			if(total_h[i][j] > total_h[i-1][j]){
//				stack[top] = i;
//				j = j - VS[i].time;
//				top ++;
//			}
//		}
//		for (int i = 0; i < top; i++){
//			h_time = h_time + VS[stack[i]].time;
//			cout<<VS[stack[i]].name<<' ';
//		}
//		l_time = t - h_time;
//		for (int p = 0; p < top; p++){
//			g_time = F_shortest[stack[p]][stack[p+1]];
//			//is_in_path(stack[p], stack[p+1]);
//		}
//		if(l_time < g_time){
//			ftag = 0;
//			t = h_time;
//		}
//		else{
//			ftag = 1;
//		}
//		cout<<endl;
//	}
//} 

/*
 * Design a path to get the most happiness in specified cost.
 */
void Max_h_in_c(int c)
{
	init_stack();
	int top = 0;
	int total_h[60][1024];
	for (int i = 1; i <= N; i++){
		for (int j = 0; j <= c; j++){
			if (j >= VS[i].cost){
				total_h[i][j] = max(total_h[i-1][j], total_h[i-1][j-VS[i].cost]+VS[i].happiness);
			}
			else{
				total_h[i][j] = total_h[i-1][j];
			}
		}
	}
	cout<<total_h[N][c]<<endl;
	int j = c;
	for (int i = N; i >= 1; i--){
		if(total_h[i][j] > total_h[i-1][j]){
			stack[top] = i;
			j = j - VS[i].cost;
			top ++;
		}
	}
	for (int i = 0; i < top; i++){
		cout<<VS[stack[i]].name<<' ';
	}
	cout<<endl;
}


/*
 * Design a path to let visitors tour all spot without repeat
 * Backtracking Methon
 */

void view_all_spot()
{
	tag = 0;
	init_bevisite();
	init_stack();
	dfs(1, 0);
	if(tag == 1){
		cout<<"There is a path："<<endl;
		for(int i=0;i < N; i++){
			//
			if(stack[i] >= 0 && stack[i] <= N)
				cout<<VS[stack[i]].name<<" ";
		}
		cout<<endl;
	}
	else{
		cout<<"There is not a path to tour all view spot without repeat in this park!"<<endl;
	}
}

int main(int argc, const char *argv[])
{

	init_spot(VS); //初始化景点
	init_route(); //初始化路径
	init_bevisite(); //
	floyd(); //
	view_all_spot(); //
	Max_h_in_c(10);
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			cout<<F_shortest[i][j]<<' ';
		}
		cout<<endl;
	}
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			cout<<path[i][j]<<' ';
		}
		cout<<endl;
	}
	//遍历
	//for (int i = 1; i <= N; i++){
	//	cout<<"N: "<<v[i].name<<'\t';
	//	cout<<"h: "<<v[i].happiness<<'\t';
	//	cout<<"t: "<<v[i].time<<'\t';
	//	cout<<"c: "<<v[i].cost<<endl;
	//}
	return 0;
}
