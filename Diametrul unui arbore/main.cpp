#include <iostream>
#include <fstream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

ifstream fin("darb.in");
ofstream fout("darb.out");

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
};


void Graf::set_nrNod(int &n) {nrNod = n;}

void Graf::set_nrMuch(int &m) {nrMuch = m;}

int Graf::get_nrNod() {return nrNod;}

int Graf::get_nrMuch() {return nrMuch;}

void Graf::citire_muchii()
{
    int x, y;

    listaAd.resize(NMAX+1);

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
    for(int k = 1; k < nrNod; k++)
        for(int i = 1; i < nrNod; i++)
            for(int j = 1; j < nrNod; j++)
                if((i!=j) && A[i][k] && A[k][j] && (A[i][j] < A[i][k] + A[k][j]))
                    A[i][j] = A[i][k] + A[k][j];
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
int main()
{
    darb_infoarena();

    fin.close();
    fout.close();
    return 0;
}
