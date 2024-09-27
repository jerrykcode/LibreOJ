#include "stdio.h"
#include "stdlib.h"
#define N 100005
int rt, tot;
int val[N];
int fa[N];
int ch[N][2];
int cnt[N];
int sz[N];
static inline void update_sz(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
}
static inline void clear(int v) {
    sz[v] = ch[v][0] = ch[v][1] = fa[v] = cnt[v] = val[v] = 0;
}
static inline void rotate(int x) {
    int f;
    if ((f = fa[x])) {
        int way = (x == ch[f][1]);
        ch[f][way] = ch[x][1^way];
        if (ch[x][1^way]) fa[ch[x][1^way]] = f;
        ch[x][1^way] = f;
        int g = fa[f];
        fa[f] = x;
        fa[x] = g;
        if (g) ch[g][f == ch[g][1]] = x;
        update_sz(f);
        update_sz(x);
    }
}
static inline void splay(int x) {
    for (int f, g; (f = fa[x]); rotate(x))
        if ((g = fa[f])) rotate(((x == ch[f][1]) ^ (f == ch[g][1])) ? x : f);
    rt = x;
}
static inline void init() {
    rt = tot = 0;
}
static inline void insert(int v) {
    if (!rt) {
        val[++tot] = v;
        rt = tot;
        fa[rt] = ch[rt][0] = ch[rt][1] = 0;
        sz[rt] = cnt[rt] = 1;
        return;
    }
    int cur = rt, f = 0;
    while (cur) {
        if (val[cur] == v) {
            cnt[cur]++;
            sz[cur]++;
            if (f) update_sz(f);
            splay(cur);
            return;
        }
        f = cur;
        cur = ch[cur][val[cur] < v];
    }
    cur = ++tot;
    val[cur] = v;
    sz[cur] = cnt[cur] = 1;
    ch[cur][0] = ch[cur][1] = 0;
    fa[cur] = f;
    ch[f][val[f] < v] = cur;
    update_sz(f);
    splay(cur);
}
static inline int search(int v) {
    int cur = rt;
    while (cur) {
        if (val[cur] == v) {
            return cur;
        }
        cur = ch[cur][val[cur] < v];
    }
    return 0;
}
static inline int rk(int v) {
    int cur = rt, res = 0;
    while (cur) {
        if (val[cur] == v) {
            res += sz[ch[cur][0]] + 1;
            splay(cur);
            return res;
        }
        if (val[cur] < v) {
            res += sz[ch[cur][0]] + cnt[cur];
        }
        cur = ch[cur][val[cur] < v];
    }
    return 0;
}
static inline int kth(int k) {
    int cur = rt;
    while (cur) {
        if (k <= sz[ch[cur][0]]) {
            cur = ch[cur][0];
        } else if (k <= sz[ch[cur][0]] + cnt[cur]) {
            return val[cur];
        } else {
            k -= sz[ch[cur][0]] + cnt[cur];
            cur = ch[cur][1];
        }
    }
    return 0;
}
static inline void delete(int v) {
    if (!rk(v)) return;
    if (--cnt[rt]) {
        update_sz(rt);
        return;
    }
    if (!ch[rt][0] && !ch[rt][1]) {
        clear(rt);
        rt = 0;
    } else if (!ch[rt][0]) {
        rt = ch[rt][1];
        clear(fa[rt]);
        fa[rt] = 0;
    } else if (!ch[rt][1]) {
        rt = ch[rt][0];
        clear(fa[rt]);
        fa[rt] = 0;
    } else {
        int cur = ch[rt][0];
        fa[cur] = 0;
        int old_rt = rt;
        while (ch[cur][1]) cur = ch[cur][1];
        splay(cur);
        ch[cur][1] = ch[old_rt][1];
        fa[ch[old_rt][1]] = cur;
        update_sz(cur);
        clear(old_rt);
    }
}
static inline int pre(int v) {
    int cur = rt, res = 0;
    while (cur) {
        if (val[cur] < v) {
            res = cur;
            cur = ch[cur][1];
        } else {
            cur = ch[cur][0];
        }
    }
    if (res) {
        splay(res);
        return val[res];
    }
    return 0;
}
static inline int next(int v) {
    int cur = rt, res = 0;
    while (cur) {
        if (val[cur] > v) {
            res = cur;
            cur = ch[cur][0];
        } else {
            cur = ch[cur][1];
        }
    }
    if (res) {
        splay(res);
        return val[res];
    }
    return 0;
}
int main() {
    init();
    int n, cmd, v;
    scanf("%d", &n);
    while (n--) {
        scanf("%d %d", &cmd, &v);
        if (cmd == 1) {
            insert(v);
        } else if (cmd == 999) {
            printf("%d", search(v));
        } else if (cmd == 3) {
            printf("%d\n", rk(v));
        } else if (cmd == 4) {
            printf("%d\n", kth(v));
        } else if (cmd == 2) {
            delete(v);
        } else if (cmd == 5) {
            printf("%d\n", pre(v));
        } else if (cmd == 6) {
            printf("%d\n", next(v));
        }
    }
    exit(0);
}
