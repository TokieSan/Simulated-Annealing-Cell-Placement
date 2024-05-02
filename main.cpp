#include <bits/stdc++.h>
#pragma once

using namespace std;

vector<vector<int>> grid;
vector<vector<int>> net_to_cell, cell_to_nets;
vector<pair<int,int>> placement;
vector<int> net_len;
int num_cells, num_nets, ny, nx;
int initial_cost;

int calc_cost(int idx) { // net's index
    assert(idx < net_to_cell.size() && idx >= 0);
    int dist = 0;

    auto &cur_n2c = net_to_cell[idx];
    assert(cur_n2c.size() > 0);
    int maxx = 0, maxy = 0, minx = INT_MAX, miny = INT_MAX;

    for (auto &x : cur_n2c) {
        auto u = placement[x];
        maxx = max(maxx, u.first);
        maxy = max(maxy, u.second);
        minx = min(minx, u.first);
        miny = min(miny, u.second);
    }

    return (maxx - minx) + (maxy - miny);
}

void print_placement(int cost = initial_cost) {
    for (auto &u : grid) {
        for (auto &v : u) {
            //printf("%d ", v);
            if (~v) cout << fixed << setw(3) << v << ' ';
            else cout << fixed << setw(3) << "--" << ' ';
        }
        cout << '\n';
    }
    cout << "CUR COST: " << cost << '\n';
}

bool do_i_do(double probability) {
    // Create a uniform distribution between 0 and 1
    std::mt19937 gen(rand());
    uniform_real_distribution<> dis(0.0, 1.0);

    return (dis(gen) < probability);
}
int main(int argc, char *argv[]) {
    ios::sync_with_stdio(0);
    freopen(argv[1], "r", stdin);
    //freopen("out", "w", stdout);

    cin >> num_cells >> num_nets >> ny >> nx;

    net_len = vector<int>(num_nets);
    
    net_to_cell = vector<vector<int>>(num_nets);
    cell_to_nets = vector<vector<int>>(num_cells);

    grid = vector<vector<int>>(ny, vector<int>(nx, -1));

    for (int i = 0; i < num_nets; i++) {
        int num_comp; 
        cin >> num_comp;
        for (int j = 0; j < num_comp; j++) {
            int comp;
            cin >> comp;
            cell_to_nets[comp].push_back(i);
            net_to_cell[i].push_back(comp);
        }
    }

    srand(97);

    // i-th position = placement
    placement = vector<pair<int,int>>(num_cells);

    int idx = 0;
    for (auto &u : placement) {
        u.first = rand() % ny, u.second = rand() % nx;
        while(grid[u.first][u.second] != -1) {
            u.first = rand() % ny, u.second = rand() % nx;
        }
        grid[u.first][u.second] = idx++;
    }

    for (int i = 0; i < num_nets; i++)  {
        net_len[i] = calc_cost(i);
        initial_cost += net_len[i];
    }
    
    
    cout << "INITIAL PLACEMENT:\n";
    print_placement();
    cout << "------------------\n";


    
    double T_initial = 500 * initial_cost;
    auto T_cur = T_initial;
    auto cur_cost = initial_cost;
    double cooling_rate = 0.95;
    double T_fin = 5e-6 * initial_cost / (double) num_nets;
    
    int num_moves = 20 * num_cells;
    while (T_cur > T_fin) {
        int ctr = num_moves;
        while (ctr--) {
            int U = rand() % num_cells;
            int Vx = rand() % ny, Vy = rand() % nx;
            int V = grid[Vx][Vy];

            if (Vx == placement[U].first && Vy == placement[U].second) {
                ctr++;
                if (nx == 1 && ny == 1) break;
                continue;
            }

            vector<int> to_br;
            for (auto &u : cell_to_nets[U]) to_br.push_back(u);
            if (~V) for (auto &u : cell_to_nets[V]) to_br.push_back(u);

            sort(to_br.begin(), to_br.end());
            to_br.erase(unique(to_br.begin(), to_br.end()), to_br.end());

            int new_cost = cur_cost;
            
            assert(Vx < grid.size() && Vy < grid[0].size());
            assert(U < placement.size());
            // swap grid cells
            swap(grid[Vx][Vy], grid[placement[U].first][placement[U].second]);

            // swap placements if nonempty, if has empty don't swap
            if (V != -1) swap(placement[U], placement[V]);
            else placement[U] = make_pair(Vx, Vy);


            vector<int> temp_net_len(to_br.size());
            for (int i = 0; i < to_br.size(); i++) {
                new_cost -= net_len[to_br[i]];
                temp_net_len[i] = calc_cost(to_br[i]);
                new_cost += temp_net_len[i];
            }
                
            double rej_prop = 1 - exp(-(new_cost - cur_cost) / T_cur);
            if (new_cost >= cur_cost && do_i_do(rej_prop) ) {
                swap(grid[Vx][Vy], grid[placement[U].first][placement[U].second]);
                if (V != -1) swap(placement[U], placement[V]);
                else placement[U] = make_pair(Vx, Vy);
            } else {
                //cout << "ACCEDPTECD STH\n";
                cur_cost = new_cost;
                for (int i = 0; i < to_br.size(); i++) net_len[to_br[i]] = temp_net_len[i];
            }
        }
        T_cur *= cooling_rate;
    }
    print_placement(cur_cost);
    cout << "T_cur = " << T_cur << "\n----------------------\n";


    

    return 0;
}

