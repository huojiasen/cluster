// Copyright (C) 2021, since Inc.
// Author: huojiasen (huojiasen@since.com)
//
// Description:
#include "util.h"

struct order_id {
    string id;
    double lng, lat;
    int type;// 1 core 2 board 3 noise
    int cluster_id;
    bool operator< (const order_id& t) const
    {
        return cluster_id<t.cluster_id;
    }
};



void bfs(vector<vector<int> >& gbase, vector<order_id>& orders) {
    int c_id = 0;
    for (int i=0;i<orders.size();i++) {
        //cout<<"point"<<i<<endl;
        if (orders[i].type != 1) continue;
        if (orders[i].cluster_id !=0) continue;
        c_id++;
        //cout<<"index add: "<<i<<" : "<<c_id<<endl;
        orders[i].cluster_id = c_id;
        queue<int> que;
        que.push(i);
        while(!que.empty()) {
            int seed = que.front();que.pop();
            //cout<<i<<" : "<<gbase[i].size()<<endl;
            for (int j=0;j<gbase[seed].size();j++) {
                int index = gbase[seed][j];
                if (orders[index].type == 3) continue;
                if (orders[index].cluster_id != 0) continue;
                orders[index].cluster_id = c_id;
                que.push(index);
            }
        }
    }

    return;
}

const long Eps = 500;
const int MinPts = 10;
int main() {
    // 1 read unbroadcast orders lng lat
    vector<order_id> orders;// 1
    ifstream fin("18.txt");
    string s;
    while(getline(fin,s)) {
        vector<string> t;
        stringSplit(s,t,'\t');
        order_id o;
        o.id = t[0];
        o.lng = string2double(t[2]);
        o.lat = string2double(t[3]);
        o.type = 0;
        o.cluster_id = 0;
        orders.push_back(o);
    }

    // 2 calc the distance n*n matrix
    int size = orders.size();
    vector<vector<long> > matrix(size, vector<long>(size, 0));//2
    for (int i=0;i<size;i++) {
        for (int j=0;j<size;j++) {
            matrix[i][j] = is_distance(orders[i].lng, orders[i].lat, orders[j].lng, orders[j].lat);
        }
    }
    // 3 for each point save the near point
    vector<vector<int> > gbase;//3
    for (int i=0;i<size;i++) {
        vector<int> tmp;
        for (int j=0;j<size;j++) {
            if (matrix[i][j]<=Eps && i!=j)
                tmp.push_back(j);
        }
        gbase.push_back(tmp);
    }

    // 4 core point 
    for (int i=0;i<size;i++) {
        if (gbase[i].size()>=MinPts) {
            orders[i].type = 1;
        }
    }

    // 5 broad point
    for (int i=0;i<size;i++) {
        if (orders[i].type != 1) {
            for (int j=0;j<gbase[i].size();j++) {
                int index = gbase[i][j];
                if (orders[index].type==1) {
                    orders[i].type = 2;
                    break;
                }
            }
        }
    }
    // 6 noise point
    for (int i=0;i<size;i++) {
        if (orders[i].type!=1 && orders[i].type!=2)
            orders[i].type = 3;
    }

    // 7 scc strong connected compent
    bfs(gbase, orders);
    // 8 output
    sort(orders.begin(), orders.end());

    int id = 0;int count =0;
    for (int i=0;i<orders.size();i++) {
        if (orders[i].cluster_id == id) {
            count++;
        } else {
            //cout<<id<<":"<<count<<endl;
            id = orders[i].cluster_id;
            count = 1;
        }
        printf("%lf %lf %d\n", orders[i].lng, orders[i].lat, orders[i].cluster_id);
        //cout<<orders[i].lng<<" "<<orders[i].lat<<" "<<orders[i].cluster_id<<" "<<endl;
    } 

    return 0;
}

