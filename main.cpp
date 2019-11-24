#include <iostream>
#include <vector>

using namespace std;

#define CHECKING_NODE map[checking_y][checking_x] 
#define START_NODE map[start_y][start_x] 
#define TARGET_NODE map[target_y][target_x] 
#define CURRENT_NODE map[cur_y][cur_x]
#define MAX_TILE 10
enum NODE_STATE{
	UNCHECKED=0,
	OPEN,
	CLOSE,
	OBSTACLE,
	TRAVERSED
};

class Node{
public:	
	NODE_STATE state;
	int x,y; //Node's coordinate
	int f,g,h; // f(x)=g(x)+h(x) with x is the current node
	Node* parent;
};

Node map[10][10];
int tiles[10][10]={{0,0,0,0,0,0,0,0,0,0},
				  {0,0,0,0,0,0,0,0,0,0},
				  {0,0,0,0,3,0,0,0,0,0},
				  {0,0,0,0,3,0,0,0,0,0},
				  {0,0,0,0,3,0,0,0,0,0},
				  {0,0,0,0,3,0,0,0,0,0},
				  {0,0,0,0,0,3,0,0,0,0},
				  {0,0,0,0,0,0,0,0,0,0},
				  {0,0,0,0,0,0,0,0,0,0},
				  {0,0,0,0,0,0,0,0,0,0}};

int adjacent_node[8][2]={{-1,-1},
						 {-1, 0},
						 {-1, 1},
						 { 0, 1},
						 { 1, 1},
						 { 1, 0},
						 { 1,-1},
						 { 0,-1}};

void Initialize()
{
	for(int i=0;i<MAX_TILE;++i)
	{
		for(int j=0;j<MAX_TILE;++j)
		{
			map[i][j].x=j;
			map[i][j].y=i;
			map[i][j].state=(NODE_STATE)tiles[i][j];
			map[i][j].parent=NULL;
			map[i][j].f=map[i][j].g=map[i][j].h=0;
		}
	}
}

//Heuristic function is simply that return the square of distance between the current node and target node
int Heuristic(Node cur,Node target)
{
	return (cur.x-target.x)*(cur.x-target.x) + (cur.y-target.y)*(cur.y-target.y);
}

Node& FindMinF(vector<Node>& open_list)
{
	if(open_list.size()==1)
		return open_list[0];
	else
	{
		int size=open_list.size();
		int F_min=open_list[0].f;
		int min_idx=0;
		for(int i=1;i<size;++i)
		{
			if(open_list[i].f<F_min)
			{
				min_idx=i;
				F_min=open_list[i].f;
			}
		}
		return open_list[min_idx];
	}
}

void RemoveFromList(vector<Node>& open_list,Node rm_node)
{
	if(open_list.size()>0)
	{
		int size=open_list.size();
		for(int i=0;i<size;++i)
		{
			if(open_list[i].x==rm_node.x && open_list[i].y==rm_node.y)
			{
				open_list.erase(open_list.begin()+i);
				return;
			}
		}
	}
}

int Astar(Node map[10][10],int start_x,int start_y,int target_x,int target_y)
{
	if(start_x<0 || start_x>=MAX_TILE || start_y<0 || start_y>=MAX_TILE)
		return -1;
	if(START_NODE.state==OBSTACLE)
		return -1;

	vector<Node> open_list;
	TARGET_NODE.state=CLOSE;
	START_NODE.g=1;
	START_NODE.h=Heuristic(START_NODE,TARGET_NODE);
	START_NODE.f=START_NODE.g+START_NODE.h;
	START_NODE.state=CLOSE;

	open_list.push_back(START_NODE);
	Node cur_node;
	int cur_x,cur_y;
	int checking_x,checking_y;
	int checking_g_val;
	while(open_list.size()!=0)
	{
		cur_node=FindMinF(open_list);
		RemoveFromList(open_list,cur_node);
		cur_x=cur_node.x;
		cur_y=cur_node.y;
		checking_g_val=CURRENT_NODE.g+1;//Represent for g value of checking node, 1 is the cost of traveling from current node to adjacent node	

		for(int i=0;i<8;++i)
		{
			checking_x=cur_x+adjacent_node[i][0];
			checking_y=cur_y+adjacent_node[i][1];
			if(checking_x<0 || checking_x>=MAX_TILE || checking_y<0 || checking_y>=MAX_TILE)
				continue;
			if(checking_x==target_x && checking_y==target_y)//Fing target
			{
				TARGET_NODE.parent=&CURRENT_NODE;
				for(Node* it=&TARGET_NODE;it!=NULL;it=it->parent)
				{
					map[it->y][it->x].state=TRAVERSED;
				}
				return 0;
			}
			else if(CHECKING_NODE.state==OBSTACLE)
				continue;
			else if(CHECKING_NODE.state==OPEN)
			{
				if(checking_g_val>CHECKING_NODE.g)
					continue;
			}
			else if(CHECKING_NODE.state==CLOSE)
			{
				if(checking_g_val>CHECKING_NODE.g)
					continue;
				else
				{
					CHECKING_NODE.state=OPEN;
				}
			}
			else
			{
				CHECKING_NODE.state=OPEN;
			}	
			CHECKING_NODE.parent=&CURRENT_NODE;
			CHECKING_NODE.g=checking_g_val;
			CHECKING_NODE.f=checking_g_val+Heuristic(CHECKING_NODE,TARGET_NODE);
			open_list.push_back(CHECKING_NODE);
		}
		CURRENT_NODE.state=CLOSE;
	}
	return -1;
}

void UpdateResult()
{
	for(int i=0;i<MAX_TILE;++i)
	{
		for(int j=0;j<MAX_TILE;++j)
			tiles[i][j]=map[i][j].state;
	}
}

void PrintMatrix()
{
	for(int i=0;i<MAX_TILE;++i)
	{
		for(int j=0;j<MAX_TILE;++j)
			cout<<tiles[i][j]<<" ";
		cout<<endl;
	}
}

int main()
{
	Initialize();
	PrintMatrix();
	Astar(map,0,9,9,0);
	UpdateResult();
	cout<<"\n\n\n";
	PrintMatrix();
	system("pause");
	return 0;
}