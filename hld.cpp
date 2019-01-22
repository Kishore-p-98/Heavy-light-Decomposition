#include<bits/stdc++.h>
using namespace std;
const int maxn=50005;
vector<int> v[maxn];
int depth[maxn],par[maxn][17];
int subtree[maxn],seg[4*maxn],a[maxn];
int chainhead[maxn],posinbase[maxn],chainInd[maxn];
int ptr,N,chainNo;

void dfs(int s,int p)
{
   par[s][0]=p;
   subtree[s]=1;
   int i,ind;

   for(i=1;par[s][i-1]!=-1;i++)
   par[s][i]=par[par[s][i-1]][i-1];

   for(i=0;i<(int)v[s].size();i++)
   {
      ind=v[s][i];

      if(ind!=p)
      {
         depth[ind]=depth[s]+1;
         dfs(ind,s);
         subtree[s]+=subtree[ind]; 
      } 
   }

}

int lca(int x,int y)
{
  if(depth[y] > depth[x])
   swap(x,y);
  
  for(int i=16;i>=0;i--)
  {
    if(par[x][i]!=-1 && (depth[par[x][i]]>=depth[y]))
     x=par[x][i]; 
  } 
  
  if(x==y)
  return x;

  for(int i=16;i>=0;i--)
  {
    if(par[x][i]!=par[y][i])
    {
      x=par[x][i];
      y=par[y][i];  
    } 
  }
  
  return par[x][0];
}

void update(int low,int high,int pos,int ind,int val)
{
  if(low==high)
  {
    a[low]=seg[pos]=val;
   // cout<<val<<endl;
    return ;  
  }

  int mid=(low+high)>>1;
  
  if(ind<=mid)
  update(low,mid,2*pos+1,ind,val);
  else
  update(mid+1,high,2*pos+2,ind,val);

  seg[pos]=max(seg[2*pos+1],seg[2*pos+2]);
}

int query(int low,int high,int qlow,int qhigh,int pos)
{
 if(high<qlow || qhigh<low)
 return 0;

 else if(qlow<=low && high<=qhigh)
 return seg[pos];

 int mid=(low+high)>>1;

 return max(query(low,mid,qlow,qhigh,2*pos+1),query(mid+1,high,qlow,qhigh,2*pos+2));
}

void HLD(int curr,int prev)
{
  if(chainhead[chainNo]==-1)
    {
     chainhead[chainNo]=curr;  
     
    }
   chainInd[curr]=chainNo;  
   posinbase[curr]=ptr++;
   
   int sc=-1,i,ind;
   
   for(i=0;i<(int)v[curr].size();i++)
   {
      ind=v[curr][i];

      if(ind!=prev)
      {
       if((sc==-1) || (subtree[ind] > subtree[sc]))
       sc=ind;  
      } 
   }

   if(sc!=-1)
   HLD(sc,curr);

   for(i=0;i<(int)v[curr].size();i++)
   {
      ind=v[curr][i];

      if(ind!=sc && ind!=prev)
      {
        chainNo++;
        HLD(ind,curr);  
      } 
   }
}
    
void initialize()
{
  depth[1]=0;
  memset(par,-1,sizeof(par));
  dfs(1,-1);
  
  ptr=0;
  chainNo=0;
  memset(chainhead,-1,sizeof(chainhead));
  HLD(1,-1);
  
  
  memset(seg,0,sizeof(seg));
  memset(a,0,sizeof(a));
  //build segment tree
}

void upd(int x,int val)
{
 update(0,N-1,0,posinbase[x],val);
 //cout<<posinbase[x]<<endl;
 //cout<<seg[0]<<endl;
}

int query_up(int x,int y)
{  
  int ans=0,xchain,ychain=chainInd[y],tmp;
 // cout<<x<<" main  "<<y<<endl;
  while(1)
  {
   xchain=chainInd[x];
   if(xchain==ychain)
   {
     tmp=query(0,N-1,posinbase[y],posinbase[x],0);
     //cout<<tmp<<endl;
     if(tmp>ans)
     ans=tmp;
     
     break;
   }
   
   tmp=query(0,N-1,posinbase[chainhead[xchain]],posinbase[x],0);

   if(tmp>ans)
   ans=tmp;

   x=chainhead[xchain];
   x=par[x][0];
  } 

  return ans;
}

int query_main(int x,int y)
{
  int l=lca(x,y);
  int ans=query_up(x,l);
  int tmp=query_up(y,l);
  
 // cout<<l<<" "<<ans<<" "<<tmp<<endl;
  if(tmp > ans)
  ans=tmp;
  
  return ans;
}


int main()
{
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int n,q;
  cin>>n>>q;

   N=n;
  int i,x,y;
  
  for(i=1;i<n;i++)
  {
    cin>>x>>y;
    x++;
    y++;
    v[x].push_back(y);
    v[y].push_back(x);  
  }

  initialize();  
  
  int type,val;

  while(q--)
  {
   cin>>type;

   if(type==1)
   {
     cin>>x>>val;
     x++;
     upd(x,val);  
   }

    else
    {
     cin>>x>>y;
     x++;y++;
     cout<<query_main(x,y)<<endl;
    } 

  } 
  
   return 0; 
}

