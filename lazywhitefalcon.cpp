#include <bits/stdc++.h>
using namespace std;
const int maxn = 100005;
vector<int> v[maxn];
int depth[maxn], par[maxn][17],st[maxn],ed[maxn],tym;
int lazy[4*maxn],seg[4*maxn],a[maxn];

void dfs(int curr, int prev) 
{ 
  st[curr]=++tym;  
  par[curr][0] = prev;
  int i, ind;

  for (i = 1; par[curr][i - 1] != -1; i++)
    par[curr][i] = par[par[curr][i - 1]][i - 1];

  for (i = 0; i < (int)v[curr].size(); i++) {
    ind = v[curr][i];

    if (ind != prev) {
      depth[ind] = 1 + depth[curr];
      dfs(ind, curr);
    }
  }

  ed[curr]=tym;
}

int LCA(int x, int y) {
  if (depth[y] > depth[x])
    swap(x, y);

  for (int i = 16; i >= 0; i--) {
    if (par[x][i] != -1 && (depth[par[x][i]] >= depth[y]))
      x = par[x][i];
  }

  if (x == y)
    return x;

  for (int i = 16; i >= 0; i--) {
    if (par[x][i] != par[y][i]) {
      x = par[x][i];
      y = par[y][i];
    }
  }

  return par[x][0];
}



void initialize() {
  
  depth[1] = 0;
  memset(par, -1, sizeof(par));
  tym=-1;
  dfs(1, -1);

  memset(seg, 0, sizeof(seg));
  memset(lazy,0,sizeof(lazy));
  memset(a,0,sizeof(a));
}

void update(int low,int high,int qlow,int qhigh,int pos,int val)
{
  if(lazy[pos])
  {
   seg[pos]+=(high-low+1)*lazy[pos];
   
   if(low!=high)
   {
     lazy[2*pos+1]+=lazy[pos];
     lazy[2*pos+2]+=lazy[pos];   
   }
   lazy[pos]=0;
  }

  if(high<qlow || qhigh<low)
  return;

  else if(qlow<=low && high<=qhigh)
  {
   seg[pos]+=(high-low+1)*val; 
  
  if(low!=high)
  {
     lazy[2*pos+1]+=val;
     lazy[2*pos+2]+=val; 
  }
     return; 
  } 

  int mid=(low+high)>>1;

  update(low,mid,qlow,qhigh,2*pos+1,val);
  update(mid+1,high,qlow,qhigh,2*pos+2,val);

  seg[pos]=seg[2*pos+1]+seg[2*pos+2];
}

int query(int low,int high,int qlow,int qhigh,int pos)
{
  if(lazy[pos])
  {
   seg[pos]+=(high-low+1)*lazy[pos];
   
   if(low!=high)
   {
     lazy[2*pos+1]+=lazy[pos];
     lazy[2*pos+2]+=lazy[pos];   
   }
   lazy[pos]=0;
  }

  if(high<qlow || qhigh<low)
  return 0;

  else if(qlow<=low && high<=qhigh)
  return seg[pos];

  int mid=(low+high)>>1;

  int q1=query(low,mid,qlow,qhigh,2*pos+1);
  int q2=query(mid+1,high,qlow,qhigh,2*pos+2);

  return q1+q2;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int n, q;

  cin >> n >> q;

  int i, x, y;
  for (i = 1; i < n; i++) {
    cin >> x >> y;
    x++;
    y++;
    v[x].push_back(y);
    v[y].push_back(x);
  }

  initialize();

  int type, val;
  while (q--) {
    cin >> type;

    if (type == 1) {
      cin >> x >> val;
      x++;
      update(0,n-1,st[x],ed[x],0,val-a[x]);
      a[x]=val;
    }

    else {
      cin >> x >> y;
      x++;
      y++;
      
      int lca=LCA(x,y);

      int ans1=query(0,n-1,st[x],st[x],0);
      int ans2=query(0,n-1,st[y],st[y],0);
      int ans3=query(0,n-1,st[lca],st[lca],0);

      int ans=ans1+ans2-2*ans3+a[lca];

      cout<<ans<<endl;
    }
  }

  return 0;
}

