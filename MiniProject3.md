# Miniproject3


### Thoughts for `state_value_function`

----

- `cal_ratio` function:
    - I use a function to calculate every heuristic, which is :
    - $($myheuristic $-$ opponentheuristic)$/$ (myheuristic $+$ opponentheuristic$)$ $\times$ $100$.
```
int cal_ratio(const int& my, const int& oppo) {
        if((my + oppo) != 0)
            return (my - oppo) / (my + oppo);
        else 
            return 0;
    }
```

----


- Disc number : 
    - Count the total coins of both side, and then use the `cal_ratio` function.
```cpp=
int my_coins = 0, oppo_coins = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(board[i][j] == player) 
                    my_coins++;
                else if(board[i][j] == 3 - player)
                    oppo_coins++;
            }
        }
        coin_heuristic = 100 * cal_ratio(my_coins, oppo_coins);
```

----

- Mobility & Potential Mobility:
    - Mobility is the number of valid spots, I don't care which player is to play this round, I count the mobility for both side and then use `cal_ratio` funtcion.
```cpp=
int my_mobil = 0, oppo_mobil = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                Point p(i, j);
                if(is_point_valid(p, player))
                    my_mobil++;
                if(is_point_valid(p,  3 - player))
                    oppo_mobil++;
            }
        }
bool is_point_valid(Point center, int who) const {
        if(Board[center.x][center.y] != 0)
            return false;
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] != 3 - who)
                continue;
            p = p + d;
            while(is_point_inside(p) && Board[p.x][p.y] != 0) {
                if(Board[p.x][p.y] == who) 
                    return true;
                p = p + d;
            }
        }
        return false;
    }
```

----

- Potential Mobility:
    - Potential Mobility is the number of the spot which is empty might be occupied by the player in the next few rounds. In particular, if the spots which is empty is next to any of the opponent's disc , this spot is your **potential mibility**.

```cpp=
int my_potential_mobil = 0, oppo_potential_mobil = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                Point p(i, j);
                if(is_point_potential_valid(p, player))
                    my_potential_mobil++;
                if(is_point_potential_valid(p, 3 - player))
                    oppo_potential_mobil++;
            }
        }
bool is_point_potential_valid(Point center, int who) {
        if(Board[center.x][center.y] != 0)
            return false;
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] == who)
                continue;
            else if(Board[p.x][p.y] == 3 - who) 
                return true;
        }
        return false;
    }
mobil_heuristic = 100 * cal_ratio(my_mobil, oppo_mobil) + 100 * cal_ratio(my_potential_mobil, oppo_potential_mobil);
```

----

- Corner:
    Corner is the place that can't be flipped , so the corner is important is this game. So I count the number of corner captured by each side and then use `cal_ratio`.
```cpp=
const array<Point, 4> corner {{
    Point(0, 0), Point(0, 7),
    Point(7, 0), Point(7, 7)
}};

int my_corner = 0, oppo_corner = 0;
        for(Point i : corner) {
            if(board[i.x][i.y] == player)
                my_corner++;
            if(board[i.x][i.y] == 3 - player)
                oppo_corner++;
        }
corner_heuristic = 100 * cal_ratio(my_corner, oppo_corner);
```

----

- Stability: 
    - If the disc can't ever be flipped , then it's stable. Otherwise, it's unstable. So if the player have more stable discs , he would have more chance to win.
    - But I just count the stability of discs on edge since it is more easy to tell which disc is stable on edge, and I avoid the corners, which have been counted above,  I also use `cal-ratio` function.


```cpp=
bool is_point_stable(Point center, int who) {
        for(Point d : directions) {
            for(Point c : corner) {
                    if(d == c)
                        return true;
                } 
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] != who)
                continue;
            while(Board[p.x][p.y] == who && is_point_inside(p)) {
                for(Point c : corner) {
                    if(p == c)
                        return true;
                } 
                p = p + d;
            }
        }
        return false;
    }
for(int i : {0, 7}) {
            for(int j : {2, 3, 4, 5, 6}) {
                Point p(i, j);
                if(Board[i][j] == player) {
                    if(is_point_danger(p)) {
                        if(is_point_stable(p, player))
                            my_edge ++;
                        else 
                            my_edge -= 3;
                    }
                    else {
                        if(is_point_stable(p, player))
                            my_edge++;
                        else 
                            my_edge--;
                    }
                }
                if(Board[i][j] == 3 - player) {
                    if(is_point_danger(p)) {
                        if(is_point_stable(p, 3 - player))
                            oppo_edge ++;
                        else 
                            oppo_edge -= 3;
                    }
                    else {
                        if(is_point_stable(p, 3 - player))
                            oppo_edge++;
                        else 
                            oppo_edge--;
                    }
                }
                    
            }
        }
          for(int i : {0, 7}) {
            for(int j : {2, 3, 4, 5, 6}) {
                Point p(i, j);
                if(Board[j][i] == player) {
                    if(is_point_danger(p)) {
                        if(is_point_stable(p, player))
                            my_edge ++;
                        else 
                            my_edge -= 3;
                    }
                    else {
                        if(is_point_stable(p, player))
                            my_edge++;
                        else 
                            my_edge--;
                    }
                }
                if(Board[j][i] == 3 - player) {
                    if(is_point_danger(p)) {
                        if(is_point_stable(p, 3 - player))
                            oppo_edge ++;
                        else 
                            oppo_edge -= 3;
                    }
                    else {
                        if(is_point_stable(p, 3 - player))
                            oppo_edge++;
                        else 
                            oppo_edge--;
                    }
                }
                    
            }
        }
edge_heuristic = 100 * cal_ratio(my_edge, oppo_edge);
```

----

- Danger Zone
    - Danger Zone is the spots next to corner, since if the corners have not been occupied by your disc but you occupied the spots next to corner , then it could easily be occupied by the opponent. Hence, we should check the danger zone and if it is stable.

```cpp=
int my_danger = 0, oppo_danger = 0;
        for(int i : {1, 6}) {
            for(int j : {1, 6}) {
                Point p(i, j);
                if(Board[i][j] == player) {
                    if(!is_point_stable(p, player)) 
                        my_danger -= 2;
                    else 
                        my_danger++;
                }
                if(Board[i][j] == 3 - player) {
                    if(!is_point_stable(p, 3 - player))
                        oppo_danger -= 2;
                    else
                        oppo_danger++;
                }
                if(Board[j][i] == player) {
                    if(!is_point_stable(p, player)) 
                        my_danger -= 2;
                    else 
                        my_danger++;
                }
                if(Board[j][i] == 3 - player) {
                    if(!is_point_stable(p, 3 - player))
                        oppo_danger -= 2;
                    else
                        oppo_danger++;
                }
            }
        }
edge_heuristic += 100 * cal_ratio(my_danger, oppo_danger);
```

---

### MiniMax 

```cpp=
int miniMax(State node, int depth, int Player) {
    if(depth == 0) 
        return node.Cal_heuristic();
    if(Player == player) {
        int best_h = -1000000;
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            int next_h = miniMax(next, depth - 1, 3 - Player);
            best_h = max(best_h, next_h);
        }
        return best_h;
    }
    else {
        int best_h = 1000000;
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            int next_h = miniMax(next, depth - 1, 3 - Player);
            best_h = min(best_h, next_h);
        }
        return best_h;
    }
}

```
---

### Alpha_Beta_pruning


```cpp=
void write_valid_point(ofstream& fout) {
    State cur(board);
    int n_valid_points = next_valid_points.size();
    int best_heuristic = -10000000;
    for(int i = 0; i < n_valid_points; i++) {
        State next = cur;
        Point p = next_valid_points[i];
        next.flip_coins(p, player);
        int h = alpha_beta_prune(next, 5, -1000000, 1000000,  3 - player);
        if(h > best_heuristic) {
            best_heuristic = h;
            fout << p.x << " " << p.y << endl;
            fout.flush();
        }
    }
}
int alpha_beta_prune(State node, int depth, int alpha, int beta, int Player) {
    if(depth == 0) 
        return node.Cal_heuristic();
    if(Player == player) {
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            alpha = max(alpha, alpha_beta_prune(next, depth - 1, alpha, beta, 3 - Player));
            if(beta <= alpha)
                break;
        }
        return alpha;
    }
    else {
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            beta = min(beta, alpha_beta_prune(next, depth - 1, alpha, beta, 3 - Player));
            if(beta <= alpha)
                break;
        }
        return beta;
    }
}
```

---

### Version control

![image alt](https://imgur.com/dYVlw1f.png)

![image alt](https://imgur.com/zUm36IU.png)



