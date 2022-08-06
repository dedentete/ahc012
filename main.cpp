#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define ALL(v) (v).begin(), (v).end()
using ll = long long;

constexpr int END = 1000000000;
constexpr double TIMELIMIT = 2.95;
constexpr int M = 10;
int N, K;
vector<int> a(M);
vector<tuple<int, int, int>> xy, yx;

struct XorShift {
    unsigned int x, y, z, w, t;

    XorShift(int seed) {
        mt19937 rnd(seed);
        x = rnd();
        y = rnd();
        z = rnd();
        w = rnd();
        t = 1;
    }

    int rand() {
        t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w & 0x7fffffff;
    }
} rnd(rand());

struct Timer {
    chrono::system_clock::time_point start, now;

    Timer() {
        start = chrono::system_clock::now();
    }

    double getTime() {
        now = chrono::system_clock::now();
        return chrono::duration<double>(now - start).count();
    }
};

struct State {
    vector<int> X, Y;
    int score;
};

void calc(State& state) {
    vector<vector<int>> pieces(state.X.size() + 1,
                               vector<int>(state.Y.size() + 1));
    vector<int> X(N, -1);
    int xidx = 0, yidx = 0;
    rep(i, N) {
        if (xidx < (int)state.X.size() && state.X[xidx] <= get<0>(xy[i]))
            xidx++;
        if (xidx && get<0>(xy[i]) <= state.X[xidx - 1]) continue;
        X[get<2>(xy[i])] = xidx;
    }
    rep(i, N) {
        if (X[get<2>(yx[i])] == -1) continue;
        if (yidx < (int)state.Y.size() && state.Y[yidx] <= get<0>(yx[i]))
            yidx++;
        if (yidx && get<0>(yx[i]) <= state.Y[yidx - 1]) continue;
        pieces[X[get<2>(yx[i])]][yidx]++;
    }
    vector<int> b(M);
    rep(x, state.X.size() + 1) {
        rep(y, state.Y.size() + 1) {
            if (pieces[x][y] == 0 || pieces[x][y] > 10) continue;
            b[pieces[x][y] - 1]++;
        }
    }
    int num = 0, den = 0;
    rep(d, M) {
        num += min(a[d], b[d]);
        den += a[d];
    }
    state.score = 1e6 * num / den;
}

void init(State& state) {
    rep(i, K / 2) {
        int rXY = rnd.rand() % 2;
        int rxy = rnd.rand() % (int)(2e4 + 1) - 1e4;
        if (rXY == 0) {
            state.X.emplace_back(rxy);
        } else {
            state.Y.emplace_back(rxy);
        }
    }
    sort(ALL(state.X));
    sort(ALL(state.Y));
    calc(state);
}

void modify(State& state) {
    int rXY = rnd.rand() % 2;
    if (rXY == 0) {
        int r = rnd.rand() % 5;
        if (r <= 1 && !state.X.empty()) {
            state.X.erase(state.X.begin() + rnd.rand() % state.X.size());
        } else if (r <= 3 && (int)state.X.size() + (int)state.Y.size() < K) {
            int rxy = rnd.rand() % (int)(2e4 + 1) - 1e4;
            while (binary_search(ALL(state.X), rxy))
                rxy = rnd.rand() % (int)(2e4 + 1) - 1e4;
            state.X.emplace_back(rxy);
            sort(ALL(state.X));
        } else {
            int idx = rnd.rand() % state.X.size();
            if (idx == 0) {
                if (state.X[idx + 1] == (int)-1e4) return;
                state.X[idx] = rnd.rand() % (state.X[idx + 1] + (int)1e4) +
                               state.X[idx - 1];
            } else if (idx == state.X.size() - 1) {
                if (state.X[idx - 1] == (int)1e4) return;
                state.X[idx] = rnd.rand() % ((int)1e4 - state.X[idx - 1]) +
                               state.X[idx - 1];
            } else {
                if (state.X[idx + 1] - state.X[idx - 1] - 1 <= 0) return;
                state.X[idx] =
                    rnd.rand() % (state.X[idx + 1] - state.X[idx - 1] - 1) +
                    state.X[idx - 1] + 1;
            }
            sort(ALL(state.X));
            unique(ALL(state.X));
        }
    } else {
        int r = rnd.rand() % 5;
        if (r <= 1 && !state.Y.empty()) {
            state.Y.erase(state.Y.begin() + rnd.rand() % state.Y.size());;
        } else if (r <= 3 && (int)state.X.size() + (int)state.Y.size() < K) {
            int rxy = rnd.rand() % (int)(2e4 + 1) - 1e4;
            while (binary_search(ALL(state.Y), rxy))
                rxy = rnd.rand() % (int)(2e4 + 1) - 1e4;
            state.Y.emplace_back(rxy);
            sort(ALL(state.Y));
        } else {
            int idx = rnd.rand() % state.Y.size();
            if (idx == 0) {
                if (state.Y[idx + 1] == (int)-1e4) return;
                state.Y[idx] = rnd.rand() % (state.Y[idx + 1] + (int)1e4 + 1) +
                               state.Y[idx - 1];
            } else if (idx == state.Y.size() - 1) {
                if (state.Y[idx - 1] == (int)1e4) return;
                state.Y[idx] = rnd.rand() % ((int)1e4 - state.Y[idx - 1] + 1) +
                               state.Y[idx - 1];
            } else {
                if (state.Y[idx + 1] - state.Y[idx - 1] - 1 <= 0) return;
                state.Y[idx] =
                    rnd.rand() % (state.Y[idx + 1] - state.Y[idx - 1] - 1) +
                    state.Y[idx - 1] + 1;
            }
            sort(ALL(state.Y));
            unique(ALL(state.Y));
        }
    }
    calc(state);
}

void output(State& state) {
    int k = state.X.size() + state.Y.size();
    cout << k << endl;
    for (int x : state.X) {
        cout << x << " " << END - 1 << " " << x << " " << -END << endl;
    }
    for (int y : state.Y) {
        cout << END - 1 << " " << y << " " << -END << " " << y << endl;
    }
}

void solve(State& state) {
    int steps = 0;
    Timer tmr;
    double nowclock = 0;
    double startclock = tmr.getTime();
    double starttemp = 1000, endtemp = 0;
    while (true) {
        if (steps % 1000 == 0) {
            nowclock = tmr.getTime();
            if (nowclock - startclock > TIMELIMIT) break;
        }
        State newstate = state;
        modify(newstate);
        /*
        if (newstate.score > state.score) {
            state = newstate;
        }
        */
        double temp = starttemp + (endtemp - starttemp) *
                                      (nowclock - startclock) / TIMELIMIT;
        double prob = exp((newstate.score - state.score) / temp);
        if (prob > (rnd.rand() % (int)1e9) / 1e9) {
            state = newstate;
        }
        steps++;
    }
    cerr << "score : " << state.score << endl;
    cerr << "steps : " << steps << endl;
}

void input() {
    cin >> N >> K;
    rep(i, M) {
        cin >> a[i];
    }
    rep(i, N) {
        int x, y;
        cin >> x >> y;
        xy.emplace_back(x, y, i);
        yx.emplace_back(y, x, i);
    }
    sort(ALL(xy));
    sort(ALL(yx));
}

signed main() {
    input();
    State state;
    init(state);
    solve(state);
    output(state);
    return 0;
}