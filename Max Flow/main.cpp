#include <iostream>
#include <fstream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

ifstream fin("maxflow.in");
ofstream fout("maxflow.out");

#define NMAX 100001

class Graf{
private:
    int nrNod, nrMuch;
    bool orientat;
    vector<vector<int>> listaAd;

public:
    Graf(int nrNoduri = 0, int nrMuchii = 0, bool eOrientat = false)
    {
        this->nrNod = nrNoduri;
        this->nrMuch = nrMuchii;
        this->orientat = eOrientat;
    }

    ~Graf()
    {
        this->nrNod = 0;
        this->nrMuch = 0;
        listaAd.clear();
    }

    void set_nrNod(int &);
    void set_nrMuch(int &);
    int get_nrNod();
    int get_nrMuch();
    void citire_muchii();
    void citire_matrice(vector<vector<int>>&);
    void citire_capacitate(vector<vector<int>>&);
//lab 3
    void bfs(int);
//lab 4
    void init();
    int reprez(int);
    void unite(int,int);
    void apm_kruskall();
    void dijkstra(int);
    bool bellmanford(int);
    void disjoint();
//lab 5
    int darb(int,int&);
    void roy_floyd(vector<vector<int>>&);
    bool bfs_flow(int, int, vector<int>&, vector<vector<int>>&, vector<vector<int>>&, vector<int>&);
    int max_flow(vector<int>&, vector<vector<int>>&,vector<vector<int>>&,vector<int>&);
};


void Graf::set_nrNod(int &n) {nrNod = n;}

void Graf::set_nrMuch(int &m) {nrMuch = m;}

int Graf::get_nrNod() {return nrNod;}

int Graf::get_nrMuch() {return nrMuch;}

void Graf::citire_muchii()
{
    int x, y;

    listaAd.resize(nrNod+1);

    for(int i = 0; i < nrMuch; i++)
    {
        fin >> x >> y;
        listaAd[x].push_back(y);
        listaAd[y].push_back(x);
    }
}

void Graf::citire_matrice(vector<vector<int>>& A)
{
    int x;
    A.resize(nrNod+1);

    for(int i = 1; i <= nrNod; i++)
    {
        A[i].resize(nrNod+1);
        for(int j = 1; j <= nrNod; j++)
        {
            fin >> x;
            A[i][j] = x;
        }
    }

}

void Graf::citire_capacitate(vector<vector<int>>& c)
{
    int x,y,cap;

    listaAd.resize(nrNod+2);
    c.resize(nrNod+2);

    for(int i = 1; i <= nrNod; i++)
    {
        c[i].resize(nrNod+2);
    }

    for(int i = 0; i < nrMuch; i++)
    {
        fin >> x >> y >> cap;

        listaAd[x].push_back(y);
        listaAd[y].push_back(x);

        c[x][y] = cap;
        c[y][x] = 0;
    }
}

int Graf::darb(int nod, int& diam)
{
    int viz[NMAX+1], d[NMAX+1], cap;
    queue<int> q;

    for(int i = 1; i <= NMAX; i++)
    {
        d[i] = 0;
        viz[i] = 0;
    }

    q.push(nod);
    d[nod] = 1;
    viz[nod] = 1;

    int x;

    while(!q.empty())
    {
        x = q.front();

        for(auto vecin:listaAd[x])
        {
            if(!viz[vecin])
            {
                q.push(vecin);
                viz[vecin] = 1;

                d[vecin] = d[x] + 1;

                diam = d[vecin];
                cap = vecin;
            }
        }

        q.pop();
    }

    return cap;
}

void Graf::roy_floyd(vector<vector<int>>& A)
{
    for(int k = 1; k <= nrNod; k++)
        for(int i = 1; i <= nrNod; i++)
            for(int j = 1; j <= nrNod; j++)
                if((i!=j) && A[i][k] && A[k][j] && (A[i][j] > A[i][k] + A[k][j] || !A[i][j]))
                    A[i][j] = A[i][k] + A[k][j];
}

bool Graf::bfs_flow(int s, int fin, vector<int>& t, vector<vector<int>>& c, vector<vector<int>>& f, vector<int>& viz)
{
    int x;

    for(int i = 1; i <= nrNod; i++)
        viz[i] = 0;

    queue<int> q;
    q.push(s);
    viz[s] = 1;

    while(!q.empty())
    {
        x = q.front();
        q.pop();

        if (x == fin)
            return true;

        for(auto vecin:listaAd[x])
        {
            if (!viz[vecin] && (c[x][vecin] != f[x][vecin]))
            {
                q.push(vecin);
                t[vecin] = x;
                viz[vecin] = 1;
            }
        }
    }

    return false;

}

int Graf::max_flow(vector<int>& t, vector<vector<int>>& c, vector<vector<int>>& f, vector<int>& viz)
{
    int rasp = 0, p, nod;
    int path_flow;

    while(bfs_flow(1, nrNod, t, c, f,viz))
    {
        for(auto vecin: listaAd[nrNod])
        {
            if((c[vecin][nrNod] != f[vecin][nrNod]) && viz[vecin])
            {
                t[nrNod] = vecin;

                path_flow = INT_MAX;

                for(nod = nrNod; nod != 1; nod = t[nod])
                {
                    p = t[nod];
                    path_flow = min(path_flow, c[p][nod] - f[p][nod]);
                }

                for(nod = nrNod; nod != 1; nod = t[nod])
                {
                    p = t[nod];
                    f[p][nod] += path_flow;
                    f[nod][p] -= path_flow;
                }

                rasp += path_flow;
            }
        }
    }

    return rasp;

}

void darb_infoarena()
{
    int n;

    fin >> n;

    Graf G(0,n,false);

    int cap, diam;

    G.citire_muchii();

    cap = G.darb(1, diam);
    G.darb(cap, diam);

    fout << diam;
}

void roy_floyd_infoarena()
{
    int n;
    vector<vector<int>> A;

    fin >> n;

    Graf G(n,0,true);

    G.citire_matrice(A);

    G.roy_floyd(A);

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            fout << A[i][j] << ' ';
        }

        fout << endl;
    }

}

void maxflow_infoarena()
{
    int n, m, rasp = 0;

    vector<vector<int>> c,f;
    vector<int> viz,t;

    fin >> n >> m;

    t.resize(n+2);
    viz.resize(n+2);

    f.resize(n+2);

    for(int i = 1; i <= n; i++)
    {
        f[i].resize(n+2);
    }

    Graf G (n,m,true);

    G.citire_capacitate(c);
    rasp = G.max_flow(t,c,f,viz);



    fout << rasp;
}


int main()
{
    maxflow_infoarena();

    fin.close();
    fout.close();
    return 0;
}
