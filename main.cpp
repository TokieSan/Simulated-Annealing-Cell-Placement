#include <bits/stdc++.h>
    
using namespace std;

vector<vector<int>> grid;
vector<vector<int>> net_to_cell, cell_to_nets;
vector<pair<int,int>> placement;
vector<int> net_len;
int num_cells, num_nets, ny, nx;
int initial_cost;

int calc_cost(int idx) { // net's index
    int dist = 0;

    auto &cur_n2c = net_to_cell[idx];
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
            if (v!=-1) cout << fixed << setw(3) << v << ' ';
            else cout << fixed << setw(3) << "--" << ' ';
        }
        cout << '\n';
    }
    cout << "COST: " << cost << '\n';
}
void print_binary_grid(){
 for (auto &u : grid) {
        for (auto &v : u) {
            //printf("%d ", v);
            if (v!=-1) cout << 1 << ' ';
            else cout <<  0 << ' ';
        }
        cout << '\n';
    }

}
default_random_engine rd{static_cast<long unsigned int>(1)};
std::mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);
bool do_i_do(double probability) {
    return (dis(gen) < probability);
}
int main(int argc, char *argv[]) {
    ios::sync_with_stdio(0);
    if (argc!= 3){
        cout<<"Enter the file name then the cooling rate\n";
        return -1;
    }
    freopen(argv[1], "r", stdin);
    double cooling_rate = atof(argv[2]);

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

    // i-th position = placement
    placement = vector<pair<int,int>>(num_cells);

    int idx = 0;
    for (auto &u : placement) {
        u.first = gen() % ny, u.second = gen() % nx;
        while(grid[u.first][u.second] != -1) {
            u.first = gen() % ny, u.second = gen() % nx;
        }
        grid[u.first][u.second] = idx++;
    }

    for (int i = 0; i < num_nets; i++)  {
        net_len[i] = calc_cost(i);
        initial_cost += net_len[i];
    }
    
    cout<< "INITIAL INITIAL BINARY:\n";
     print_binary_grid();
    cout << "INITIAL PLACEMENT:\n";
    print_placement();

    cout << "--------------------------------------------------------------------------------------------------------\n";


    
    double T_initial = 500 * initial_cost;
    auto T_cur = T_initial;
    auto cur_cost = initial_cost;
    double T_fin = 5e-6 * initial_cost / (double) num_nets;
    
    int num_moves = 20 * num_cells;
    while (T_cur > T_fin) {
        int ctr = num_moves;
        while (ctr--) {
            int U = gen() % num_cells;
            int Ux = placement[U].first, Uy = placement[U].second;
            int Vx = gen() % ny, Vy = gen() % nx;
            int V = grid[Vx][Vy];

            int new_cost = cur_cost;
            // swap cells
            swap(grid[Vx][Vy], grid[Ux][Uy]);
            // swap placements if nonempty, if has empty don't swap
            if (V != -1) swap(placement[U], placement[V]);
            else placement[U] = make_pair(Vx, Vy);

            vector<int> temp_net_len(num_nets, -1);
            for (auto &u : cell_to_nets[U]){
                new_cost -= net_len[u];
                temp_net_len[u] = calc_cost(u);
                new_cost += temp_net_len[u];
            }
            if (V != -1) {
                for (auto &v: cell_to_nets[V]) {
                    if (temp_net_len[v] != -1)
                        continue;
                    new_cost -= net_len[v];
                    temp_net_len[v] = calc_cost(v);
                    new_cost += temp_net_len[v];
                }
            }
                
            double rej_prop = 1 - exp(-(new_cost - cur_cost) / T_cur);
            if (new_cost >= cur_cost && do_i_do(rej_prop) ) {
                // Revert (Swap back)
                swap(grid[Vx][Vy], grid[Ux][Uy]);
                if (V != -1) swap(placement[U], placement[V]);
                else placement[U] = make_pair(Ux, Uy);
            } else {
                // Swap
                cur_cost = new_cost;
                 for (auto &u : cell_to_nets[U]){
                    net_len[u] = temp_net_len[u];
                 }
                   if (V != -1) {
                for (auto &v: cell_to_nets[V]) {
                    net_len[v] = temp_net_len[v];
                }
            }
            }
        }
        T_cur *= cooling_rate;
    }
    cout << "FINAL PLACEMENT BINARY:\n";
    print_binary_grid();
    cout << "FINAL PLACEMENT:\n";
    print_placement(cur_cost);
    cout << "T_final = " << T_cur;

    return 0;
}

