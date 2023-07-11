#include "hw2_output.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

class ProP
{
public:
    int gid;
    int si, sj;         // current value of i and j
    int tg;             // time to gather
    int ng;             // ng
    int curi, curj;     // MAYBE no need, if you need add it to constructors
    int W_tl_i, W_tl_j, W_br_i, W_br_j, W_isz, W_jsz;

    bool breaked;
    //vector<std::pair<int,int>> gFrom;
    vector<vector<int>> gFrom;
    ProP(int id, int sz_i, int sz_j, int time, int NG): gid(id), si(sz_i), sj(sz_j), tg(time), ng(NG){breaked = false;}
    ProP(const ProP &p1) {gid = p1.gid ; si = p1.si ; sj = p1.sj ; tg = p1.tg ; ng = p1.ng; gFrom = p1.gFrom;}
    void set_working(int topleft_i, int topleft_j)
    {

        this->W_br_i = si + topleft_i - 1;
        this->W_br_j = sj + topleft_j - 1;
        this->W_tl_i = topleft_i;
        this->W_tl_j = topleft_j;
    }
    void reset_working()
    {
        W_br_i = -1;
        W_br_j = -1;
        W_tl_i = -1;
        W_tl_j = -1;
    }
    // CC is erronous, fix it!
};

class Ciggie
{
    public:
    int gid, tg;
    int W_tl_i, W_tl_j, W_br_i, W_br_j;

    vector<tuple<int, int, int>> litTo;

    Ciggie(int cid, int t): gid(cid), tg(t){}
    void set_working(int topleft_i, int topleft_j)
    {

        this->W_br_i = 2 + topleft_i;
        this->W_br_j = 2 + topleft_j;
        this->W_tl_i = topleft_i;
        this->W_tl_j = topleft_j;
    }

};

class Area
{

};

class Global
{
public:
    int isz, jsz;
};

void Parse();
void wait_semaphores(int ind, int topleft_i, int topleft_j);
void lock_cell_decrease(int i, int j);
void lock_cell_increase(int i, int j);
void lock_area(int ind, int isize, int jsize, int topleft_i, int topleft_j);
void is_valid_area(int ind, int topleft_i, int topleft_j);
void unlock_area(int i, int j, int isz, int jsz);
void gather_from(int ind);
void prop_sleep(int tg);
void *after_thread(void *ind);
void init_semaphores();
void destroy_semaphores();
