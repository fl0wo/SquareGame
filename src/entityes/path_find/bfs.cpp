
#include <bits/stdc++.h> 
using namespace std; 
#define ROW 100
#define COL 100
struct Point {
	int x; 
	int y; 
}; 
struct queueNode {
	Point pt; 
	int dist; 
}; 

bool isValid(int row, int col) {
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL); 
} 

int rowNum[] = {-1, 0, 0, 1}; 
int colNum[] = {0, -1, 1, 0}; 

int BFS(int mat[][COL], Point src, Point dest) {
	if (!mat[src.x][src.y] || !mat[dest.x][dest.y]) 
		return -1; 

	bool visited[ROW][COL]; 
	memset(visited, false, sizeof visited); 
	visited[src.x][src.y] = true; 
	queue<queueNode> q; 
	queueNode s = {src, 0}; 
	q.push(s);

	while (!q.empty()) {
		queueNode curr = q.front(); 
		Point pt = curr.pt; 

		if (pt.x == dest.x && pt.y == dest.y) 
			return curr.dist; 

		q.pop(); 

		for (int i = 0; i < 4; i++) {
			int row = pt.x + rowNum[i]; 
			int col = pt.y + colNum[i]; 
			
			if (isValid(row, col) && mat[row][col] && !visited[row][col]) {
				visited[row][col] = true; 
				queueNode Adjcell = { {row, col}, curr.dist + 1 }; 
				q.push(Adjcell); 
			} 
		} 
	} 

	return -1; 
}

// Driver program to test above function 
int main() 
{ 
	int mat[ROW][COL] = 
	{ 
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 }, 
		{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 }, 
		{ 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 }, 
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, 
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 }, 
		{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 }, 
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 }, 
		{ 1, 1, 0, 0, 0, 0, 1, 0, 0, 1 } 
	}; 

	Point source = {0, 0}; 
	Point dest = {3, 4}; 

	int dist = BFS(mat, source, dest); 

	if (dist != INT_MAX) 
		cout << "Shortest Path is " << dist ; 
	else
		cout << "Shortest Path doesn't exist"; 

	return 0; 
} 



/*

    bool path_between_ric(int r1,int c1,int r2,int c2,vector<pair<int,int>> &ans,bool v[MAXN][MAXN]){
        if(!isLegit(r1,c1))return false;
        if(r1==r2 && c1==c2)return true;
        if(v[r1][c1]) return false;

        v[r1][c1]=true;
        int dirs[4][2] = {{0,1},{1,0},{-1,0},{0,-1}};

        for(auto d:dirs){
            if(isLegit(r1+d[0],c1+d[1]) && path_between_r(r1+d[0],c1+d[1],r2,c2,ans)){
                //ans.emplace_back(make_pair(r1+d[0],c1+d[1]));
                return true;
            }
        }

        return false;
    }

    bool path_between_r(int r1,int c1,int r2,int c2,vector<pair<int,int>> &ans){
        bool visited[MAXN][MAXN];
        memset(visited,false,sizeof(visited));
        return path_between_ric(r1,c1,r2,c2,ans,visited);
    }


    
     * r1 = riga enemy
     * c1 = colonna enemy
     * r2 = riga target
     * c2 = colonna target
     * ans = shortest path in grid
     * Time : O(n*m) (where n is abs(r1-r2) and m is abs(c1-c2))
     * bfs (cause on short distance has smaller ms time than A*)
     * (even tho the complexity is higher)
     
    bool path_between(int r1,int c1,int r2,int c2,vector<pi> &ans){

        cout << "CERCHIAMO wdwdwd PATH : (" << r1 << "," << c1 << ") - (" << r2 << "," << c2<<")\n";

        if(r1==r2 && c1==c2)return true;

        int h = getNRows();
        if (h == 0) return false;
        int l = getNCols();

        bool v[h][l]; // visited false all
        //pi paths[abs(r1-r2)+1][abs(c1-c2)+1]; // shortest paths
        pi paths[h][l];

        priority_queue<pi> q;   // queue

        q.push({r1,c1}); // where im situated at the beginning
        paths[r1][c1] = {r1,c1}; // to get where i am, i dont have to move

        bool found = false;

        while(!q.empty() && !found){   // till is not empty
            pi x = q.top();q.pop(); // get the first inserted
            int r = x.first;
            int c = x.second;
          //  cout << " sono  a (" << r <<"," << c <<")\n";

            if (!isLegit(r,c) || v[r][c] || isWall(r,c))continue;
            v[r][c] = true;
            pi curPos = {r,c};

            if(r==r2 && c==c2){found=true;break;}

            if(isLegit(r,c-1)){q.push({r,c-1});if(!v[r][c-1]) paths[r][c-1]=curPos;} //go left
            if(isLegit(r,c+1)){q.push({r,c+1});if(!v[r][c+1]) paths[r][c+1]=curPos;} //go right
            if(isLegit(r-1,c)){q.push({r-1,c});if(!v[r-1][c]) paths[r-1][c]=curPos;} //go up
            if(isLegit(r+1,c)){q.push({r+1,c});if(!v[r+1][c]) paths[r+1][c]=curPos;} //go down
        }

        if(!found)return false;

        pi step = paths[r2][c2];

        cout << "di fatto trovata\n";
        int tmp=20;
        while(tmp--){
            cout << "siamo a : " << step.first << "," << step.second << "\n";
            ans.emplace_back(step);
            step = paths[step.first][step.second];
        }

        return true;
    }

*/
