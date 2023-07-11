#include "hw2_output.h"
#include "the2.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include <iostream>
#include <vector>

using namespace std;


// GLOBAL DEFINITIONS
vector<vector<int>> G;      // The grid which is global
vector<vector<sem_t>> S;

int isz, jsz, sem_count, c_br;

bool nowBreak, nowCont, nowStop, breaked;

vector<ProP> VP;            // Proper privates vector
vector<ProP> Working;
vector<Ciggie> CG;
vector<Ciggie> Smoking;
vector<pair<int,string>> Orders;

pthread_mutex_t mutex;

pthread_mutex_t VP_mut;
pthread_mutex_t CG_mut;

pthread_mutex_t check_mut;
pthread_mutex_t check_mut_cg;

pthread_mutex_t work_mut;
pthread_mutex_t work_mut_cg;

pthread_mutex_t break_mut;
pthread_mutex_t mut_like_sem;
pthread_mutex_t cont_mutex;

pthread_mutex_t mut_sleep;
pthread_mutex_t flag_mut;

pthread_mutex_t den;
pthread_mutex_t brcount;

pthread_cond_t condSleep;
pthread_cond_t condSem;
pthread_cond_t semCont;

void print_grid()
{
    cout << "Grid is: " << isz << " " << jsz << endl;
    for(int i=0; i<isz; i++)
    {
        for(int j = 0; j<jsz; j++)
        {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }
}

void Parse()
{
    isz = 0; jsz = 0;
    // GRID SIZE //
    cin >> isz >> jsz;      // Read first line, i.e., grid dimensions

    // GRID INCREDIENTS
    for(int i=0; i<isz; i++)
    {
        G.push_back(*(new vector<int>()));
        for(int j=0; j<jsz; j++)
        {
            int a = 0;
            cin >> a;
            G[i].push_back(a);          // Add all the line to the grid
        }
    }

    // NUMBER OF PRPOER PRIVATES
    int n = 0;
    cin >>  n;

    // STORE PROPER PRIVATE INFO
    int p1, p2, p3, p4, p5;

    for(int i=0; i<n; i++)
    {
        cin >> p1 >> p2 >> p3 >> p4 >> p5;
        ProP *tmProp = new ProP(p1,p2,p3,p4,p5);

        for(int k=0; k<p5; k++)
        {
            int gi, gj;
            cin >> gi >> gj;
            vector<int> TMP;
            TMP.push_back(gi);
            TMP.push_back(gj);
            //tmProp->gFrom.push_back(std::pair<int,int>(gi, gj));
            tmProp->gFrom.push_back(TMP);
        }
        VP.push_back(*tmProp);
        delete tmProp;
    }

    // NUMBER OF ORDERS
    int nOrders = 0;
    cin >> nOrders;

    pair<int, string> tmPair;
    for(int i=0; i<nOrders; i++)
    {
        int time;
        string ordName;
        cin >> time >> ordName;
        tmPair = make_pair(time, ordName);
        Orders.push_back(tmPair);
    }


    // NUMBER OF SNEAKY CIGGIES

    int nCiggies = 0;
    cin >> nCiggies;
    for(int i=0; i<nCiggies; i++)
    {
        int c1,c2,c3;
        cin >> c1 >> c2 >> c3;
        Ciggie C(c1, c2);
        for(int j=0; j<c3; j++)
        {
            int x, y, li;
            cin >> x >> y >> li;
            auto t = make_tuple(x, y, li);
            C.litTo.push_back(t);
        }
        CG.push_back(C);

    }

    //// TO BE CONTIUNED



/*
*_* Debugging for taking input of grid size

    for(int i=0; i<isz; i++)
    {
        for(int j = 0; j<jsz; j++)
        {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }

*-* Debugging for taking input for
    for(int i = 0; i< n; i++)
    {
        cout << VP[i].gid << " " << VP[i].si << " " << VP[i].sj << " " << VP[i].tg << " " << VP[i].ng << endl;
    }

*-* Debugging for taking input for gathering
    for(int i=0; i<n; i++)
    {
        for(int k=0; k<VP[i].ng; k++)
        {
            cout << VP[i].gFrom[k][0] << " " << VP[i].gFrom[k][1] << endl;
        }
    }

    for(int i=0; i<nOrders; i++)
    {
        cout << Orders[i].first << " " << Orders[i].second << endl;
    }


*-* Debugging for taking input for ciggies
    for(int i=0; i<nCiggies; i++)
    {
        cout << CG[i].gid << " " << CG[i].tg << endl;
        for (int j=0; j<CG[i].litTo.size(); j++)
        {
            cout << get<0>(CG[i].litTo[j]) << " " << get<1>(CG[i].litTo[j]) << " " << get<2>(CG[i].litTo[j]) << endl;
        }
    }
*/  

}

// Lock with mutex and decrease the grid
void lock_cell_decrease(int ind, int i, int j, int topleft_i, int topleft_j)
{
    //////////LOCKED//////////
    //hw2_notify(PROPER_PRIVATE_ARRIVED, VP[ind].gid, topleft_i, topleft_j);
    //prop_sleep(VP[ind].tg);


   // pthread_mutex_lock(&mutex);     // Don't wait if G[i][j] is empty TODO
    //if(G[i][j] != 0)
    prop_sleep(VP[ind].tg);
    //pthread_mutex_unlock(&mutex);
    if(nowStop)
    {
        hw2_notify(PROPER_PRIVATE_STOPPED, VP[ind].gid, 0, 0);
        pthread_exit(NULL);
    }
/*
    if(nowBreak)
    {
        hw2_notify(PROPER_PRIVATE_TOOK_BREAK, VP[ind].gid, 0, 0);
        breaked = true;
        pthread_mutex_lock(&mut_like_sem);
        pthread_cond_wait(&condSem, &mut_like_sem);
        pthread_mutex_unlock(&mut_like_sem);
    }
*/
    if(nowBreak) return;
    pthread_mutex_lock(&mutex);
    if(G[i][j]>0) {
        G[i][j]--;
    }

    hw2_notify(PROPER_PRIVATE_GATHERED, VP[ind].gid, i, j);
    pthread_mutex_unlock(&mutex);
    //print_grid();
    /////////UNLOCKED/////////
}

// Lock with mutex and increase the grid
void lock_cell_increase(int ind, int i, int j, int cell_i, int cell_j, int num_of_cigs)
{
    //hw2_notify(SNEAKY_SMOKER_ARRIVED, CG[ind].gid, cell_i, cell_j);
    
    prop_sleep(CG[ind].tg);

    if(nowStop)
    {
        hw2_notify(SNEAKY_SMOKER_STOPPED, VP[ind].gid, 0, 0);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&mutex);
    G[i][j]++;
    hw2_notify(SNEAKY_SMOKER_FLICKED, CG[ind].gid, i, j);
    pthread_mutex_unlock(&mutex);

}

bool is_valid_area(int ind)
{
    //pthread_mutex_lock(&check_mut);

    // Check if there are another private working on same cell
    for(auto & i : Working)
    {
        if((VP[ind].W_tl_i <= i.W_br_i && VP[ind].W_tl_i >= i.W_tl_i
            && VP[ind].W_tl_j <= i.W_br_j && VP[ind].W_tl_j >= i.W_tl_j)
           || (VP[ind].W_br_i <= i.W_br_i && VP[ind].W_br_i >= i.W_tl_i
               && VP[ind].W_br_j <= i.W_br_j && VP[ind].W_br_j >= i.W_tl_j))
        {
            //pthread_mutex_unlock(&check_mut);
            return false;
        }
    }

    for(auto el: Smoking)
    {
        if((VP[ind].W_tl_i <= el.W_br_i && VP[ind].W_tl_i >= el.W_tl_i
            && VP[ind].W_tl_j <= el.W_br_j && VP[ind].W_tl_j >= el.W_tl_j)
           || (VP[ind].W_br_i <= el.W_br_i && VP[ind].W_br_i >= el.W_tl_i
               && VP[ind].W_br_j <= el.W_br_j && VP[ind].W_br_j >= el.W_tl_j))
            //pthread_mutex_unlock(&check_mut);
            return false;   
    }
    Working.push_back(VP[ind]);            // Add working private to a vector
    Working[Working.size()-1].set_working(VP[ind].W_tl_i,VP[ind].W_tl_j);
    //pthread_mutex_unlock(&check_mut);
    return true;
}

bool is_valid_area_cg(int ind) {
    pthread_mutex_lock(&check_mut_cg);
    // Check if there are another private working on same cell

    for (auto &i: Working) {
        if ((CG[ind].W_tl_i <= i.W_br_i && CG[ind].W_tl_i >= i.W_tl_i
             && CG[ind].W_tl_j <= i.W_br_j && CG[ind].W_tl_j >= i.W_tl_j)
            || (CG[ind].W_br_i <= i.W_br_i && CG[ind].W_br_i >= i.W_tl_i
                && CG[ind].W_br_j <= i.W_br_j && CG[ind].W_br_j >= i.W_tl_j)) {
            pthread_mutex_unlock(&check_mut_cg);
            return false;
        }
    }
    bool myf = false;
    for (auto &el: Smoking) {
        if (el.W_tl_i == CG[ind].W_tl_i && el.W_tl_j == CG[ind].W_tl_j) {
            myf = true;
        }
    }
    if (!myf)
    {
    Smoking.push_back(CG[ind]);            // Add working private to a vector
    Smoking[Smoking.size() - 1].set_working(CG[ind].W_tl_i, CG[ind].W_tl_j);
    pthread_mutex_unlock(&check_mut_cg);
    return true;
    }
    pthread_mutex_unlock(&check_mut_cg);
    return false;

}

void gather_from(int ind)
{

    pthread_mutex_lock(&VP_mut);
    int gather_sz = VP[ind].gFrom.size();
    int i, j, isize, jsize, topleft_i, topleft_j, tg = VP[ind].tg;
    isize = VP[ind].si;     // Size of i
    jsize = VP[ind].sj;     // Size of j
    pthread_mutex_unlock(&VP_mut);

    int k = 0;

    while(k<gather_sz)
    {
            pthread_mutex_lock(&break_mut);
            if(nowBreak)        // Break case
            {
                c_br++;
                pthread_mutex_lock(&check_mut);
                VP[ind].breaked = true;
                pthread_mutex_unlock(&check_mut);
                if(nowStop)
                {
                    hw2_notify(PROPER_PRIVATE_STOPPED, VP[ind].gid, 0, 0);
                    pthread_mutex_unlock(&break_mut);
                    pthread_exit(NULL);
                }

                else
                {
                    hw2_notify(PROPER_PRIVATE_TOOK_BREAK, VP[ind].gid, 0, 0);
                    breaked = 1;
                }
                pthread_mutex_unlock(&break_mut);
                pthread_mutex_lock(&mut_like_sem);
                pthread_cond_wait(&condSem, &mut_like_sem);
                pthread_mutex_unlock(&mut_like_sem);
                
                bool myFlag1 = false;
                for(auto &el: VP)
                {
                    if(!el.breaked) 
                    {
                        pthread_mutex_lock(&flag_mut);
                        myFlag1 =true;
                        pthread_mutex_unlock(&flag_mut);
                        break;
                    }
                }

                if(!myFlag1)
                {
                    pthread_mutex_lock(&cont_mutex);
                    pthread_cond_signal(&semCont);
                    pthread_mutex_unlock(&cont_mutex);
                }
                continue;
            }
            pthread_mutex_unlock(&break_mut);
        pthread_mutex_lock(&work_mut);
        topleft_i = VP[ind].gFrom[k][0];       // Initial value of i
        topleft_j = VP[ind].gFrom[k][1];       // Initial value of j
        VP[ind].set_working(topleft_i, topleft_j);
        pthread_mutex_unlock(&work_mut);


        pthread_mutex_lock(&break_mut);

        pthread_mutex_lock(&check_mut);
        bool theFlag = is_valid_area(ind);
        pthread_mutex_unlock(&check_mut);
        if(theFlag && !nowBreak)
        {

            if(nowCont)
            {
                hw2_notify(PROPER_PRIVATE_CONTINUED, VP[ind].gid, 0, 0);
            }   
            pthread_mutex_unlock(&break_mut);

            hw2_notify(PROPER_PRIVATE_ARRIVED, VP[ind].gid, topleft_i, topleft_j);
            for(i=topleft_i; i<isize+topleft_i && !nowBreak; i++)
            {
                for(j = topleft_j; j<jsize+topleft_j && !nowBreak; j++)
                {
                    while(G[i][j] && !nowBreak)
                    {
                        lock_cell_decrease(ind, i, j, topleft_i, topleft_j);
                    }
                }
            }

            hw2_notify(PROPER_PRIVATE_CLEARED, VP[ind].gid, 0, 0);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_broadcast(&condSem);
            pthread_mutex_unlock(&mut_like_sem);

            k++;

            pthread_mutex_lock(&check_mut);
            int szW = Working.size();

            for(int s=0; s<szW && !nowBreak; s++)
            {
                if(Working[s].gid == VP[ind].gid)
                {
                    Working.erase(Working.begin()+ s);
                    break;
                }
            }
            pthread_mutex_unlock(&check_mut);

        }
        else
        {
            pthread_mutex_unlock(&break_mut);

            pthread_mutex_lock(&break_mut);
            
            pthread_mutex_unlock(&break_mut);


            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_wait(&condSem, &mut_like_sem);
            pthread_mutex_unlock(&mut_like_sem);

            continue;
        }
    }
}

void litter_to(int ind)
{
    pthread_mutex_lock(&CG_mut);
    int lit_sz = CG[ind].litTo.size();
    int i, j, cell_i, cell_j, num_cg, tg = CG[ind].tg;
    pthread_mutex_unlock(&CG_mut);

    int k = 0;

    while(k<lit_sz)
    {
        if(nowStop)        // Stop case
        {
            hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
            pthread_exit(NULL);
        }

        pthread_mutex_lock(&work_mut_cg);      // TODO change this mutexes
        cell_i = get<0>(CG[ind].litTo[k]);       // Initial value of i
        cell_j = get<1>(CG[ind].litTo[k]);       // Initial value of j
        num_cg = get<2>(CG[ind].litTo[k]);       // Initial value of number of cigs
        CG[ind].set_working(cell_i-1, cell_j-1);
        pthread_mutex_unlock(&work_mut_cg);


        pthread_mutex_lock(&break_mut);
        if(is_valid_area_cg(ind) && !nowStop)
        {
            pthread_mutex_unlock(&break_mut);
            hw2_notify(SNEAKY_SMOKER_ARRIVED, CG[ind].gid, cell_i, cell_j);
            while(num_cg && !nowStop)
            {
                i=cell_i-1, j = cell_j-1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i-1, j = cell_j;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i-1, j = cell_j+1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i, j = cell_j+1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i+1, j = cell_j+1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i+1, j = cell_j;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }
                         
                i=cell_i+1, j = cell_j-1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

                i=cell_i, j = cell_j-1;
                if(num_cg && !nowStop)
                    lock_cell_increase(ind, i, j, cell_i, cell_j, num_cg--);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }


            }
            if(num_cg == 0)     hw2_notify(SNEAKY_SMOKER_LEFT, CG[ind].gid, 0, 0);
            pthread_mutex_lock(&break_mut);
            if(nowStop)        // Stop case
            {
                pthread_mutex_unlock(&break_mut);
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&break_mut);

            pthread_mutex_lock(&check_mut_cg);
            int szW = Smoking.size();

            for(int s=0; s<szW && !nowStop; s++)
            {
                if(Smoking[s].gid == CG[ind].gid)
                {
                    Smoking.erase(Smoking.begin()+ s);
                    break;
                }
            }
            pthread_mutex_unlock(&check_mut_cg);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_broadcast(&condSem);
            pthread_mutex_unlock(&mut_like_sem);
            if(nowStop)        // Stop case
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_exit(NULL);
            }

            k++;
        }
        else
        {
            if(nowStop)
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_mutex_unlock(&break_mut);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&break_mut);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_wait(&condSem, &mut_like_sem);
            pthread_mutex_unlock(&mut_like_sem);
            if(nowStop)
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, CG[ind].gid, 0, 0);
                pthread_mutex_unlock(&break_mut);
                pthread_exit(NULL);
            }
            continue;
        }
    }
}


// Sleeps for given proP's gathering time
void prop_sleep(int tg)
{
    // Something MAYBE wrong with miliseconds
    struct timespec   ts;
    struct timeval    tp;

    gettimeofday(&tp, NULL);

    int res;

    ts.tv_sec  = tp.tv_sec + tg/1000;
    res = tp.tv_usec * 1000 + (tg%1000)*1000000;     // Convert millisecond to nanosecond
    
    if(res > 1000000000L) ts.tv_sec++;
    // cond_wait
    ts.tv_nsec = res % 1000000000L;       // * 1.000.000 

    pthread_mutex_lock(&mut_sleep);
    pthread_cond_timedwait(&condSleep, &mut_sleep, &ts);
    pthread_mutex_unlock(&mut_sleep);

 
}


void *command_thread(void *a)
{
    for(int i=0; i<Orders.size(); i++)
    {
        if(i == 0)  usleep(Orders[i].first * 1000);
        else    usleep((Orders[i].first - Orders[i-1].first) * 1000);

        if (Orders[i].second ==  "break")
        {

            c_br = 0;
            hw2_notify(ORDER_BREAK, 0, 0, 0);

            pthread_mutex_lock(&break_mut);
            nowStop = false;
            nowBreak = true;        // Set break flag
            nowCont = false;
            pthread_mutex_unlock(&break_mut);


            pthread_mutex_lock(&mut_sleep);
            pthread_cond_broadcast(&condSleep);
            pthread_mutex_unlock(&mut_sleep);


            pthread_mutex_lock(&check_mut); 
            Working.clear();
            pthread_mutex_unlock(&check_mut);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_broadcast(&condSem);
            pthread_mutex_unlock(&mut_like_sem);

        }

        else if (Orders[i].second ==  "continue")
        {
            pthread_mutex_lock(&break_mut);
            bool theF = nowBreak;
            pthread_mutex_unlock(&break_mut);

            if(theF)
            {
/*                pthread_mutex_lock(&cont_mutex);
                pthread_cond_wait(&semCont, &cont_mutex);
                pthread_mutex_unlock(&cont_mutex);
*/            }

            pthread_mutex_lock(&break_mut);

 /*           if(!nowCont)      MAYBE
            {
                int sz = sem_count;
                while(sz--) sem_post(&semaphore);       // notify all
            }
*/

            nowStop = false;
            nowBreak = false;        // Set break flag
            nowCont = true;

            hw2_notify(ORDER_CONTINUE, 0, 0, 0);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_broadcast(&condSem);
            pthread_mutex_unlock(&mut_like_sem);

            pthread_mutex_unlock(&break_mut);

        }
        else if (Orders[i].second ==  "stop")
        {
            hw2_notify(ORDER_STOP, 0, 0, 0);
            pthread_mutex_lock(&break_mut);
            pthread_mutex_lock(&den);
            nowStop = true;
            nowBreak = true;        // Set break flag
            nowCont = false;
            pthread_mutex_unlock(&den);
            pthread_mutex_unlock(&break_mut);

            pthread_mutex_lock(&check_mut); 
            Working.clear();
            pthread_mutex_unlock(&check_mut);
            pthread_mutex_lock(&check_mut_cg); 
            Smoking.clear();
            pthread_mutex_unlock(&check_mut_cg);

            pthread_mutex_lock(&mut_like_sem);
            pthread_cond_broadcast(&condSem);
            pthread_mutex_unlock(&mut_like_sem);

        }
    }
    pthread_exit(NULL);
}

void *sneaky_thread(void *cgid)
{
    // SET VARIABLES
    pthread_mutex_lock(&CG_mut);
    int gid = *((int*) cgid);
    int iP;
    for(int i=0; i<CG.size(); i++)
    {
        if(CG[i].gid == gid)
            iP = i;
    }
    int tg = CG[iP].tg;
    pthread_mutex_unlock(&CG_mut);

    litter_to(iP);

    // NOTIFY EXIT
    hw2_notify(SNEAKY_SMOKER_EXITED, gid, 0, 0);
    pthread_exit(NULL);

}

void *after_thread(void *ppid)
{
    // SET VARIABLES
    pthread_mutex_lock(&VP_mut);
    int gid = *((int*) ppid);
    int iP;
    for(int i=0; i<VP.size(); i++)
    {
        if(VP[i].gid == gid)
            iP = i;
    }
    int tg = VP[iP].tg;
    pthread_mutex_unlock(&VP_mut);

    gather_from(iP);

    // NOTIFY EXIT
                pthread_mutex_lock(&check_mut);
                VP[iP].breaked = true;
                pthread_mutex_unlock(&check_mut);
    hw2_notify(PROPER_PRIVATE_EXITED, gid, 0, 0);
    pthread_exit(NULL);

}

// TODO: Take a look for quick succession
int main()
{
    nowBreak = false;
    nowCont = false;        // start false in order to make notification
    nowStop = false;
    breaked = false;
    c_br = 0;
    hw2_init_notifier();
    Parse();

    int sz = VP.size(), szCig = CG.size();
    pthread_t tids[sz+szCig+1];
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&VP_mut, NULL);
    pthread_mutex_init(&CG_mut, NULL);
    pthread_mutex_init(&check_mut, NULL);
    pthread_mutex_init(&check_mut_cg, NULL);
    pthread_mutex_init(&work_mut, NULL);
    pthread_mutex_init(&work_mut_cg, NULL);
    pthread_mutex_init(&mut_sleep, NULL);
    pthread_mutex_init(&break_mut, NULL);
    pthread_mutex_init(&mut_like_sem, NULL);
    pthread_mutex_init(&cont_mutex, NULL);
    pthread_mutex_init(&flag_mut, NULL);

    pthread_mutex_init(&den, NULL);
    pthread_mutex_init(&brcount, NULL);

    pthread_cond_init(&condSleep, NULL);
    pthread_cond_init(&condSem, NULL);
    pthread_cond_init(&semCont, NULL);

    sem_count = 1;

    // Create the threads for each proper private
    for (int i = 0; i < sz; i++)
    {
        pthread_create(&tids[i], NULL, after_thread, (void *)&(VP[i].gid));
        hw2_notify(PROPER_PRIVATE_CREATED, VP[i].gid, 0, 0);
    }
    pthread_create(&tids[sz+szCig], NULL, command_thread, NULL);      // Create commander
    for (int i = 0; i < szCig; i++)
    {
        pthread_create(&tids[i+sz], NULL, sneaky_thread, (void *)&(CG[i].gid));
        hw2_notify(SNEAKY_SMOKER_CREATED, CG[i].gid, 0, 0);
    }

    // Join all threads to main thread
    for (int i=0; i<sz+szCig+1; i++)
        pthread_join(tids[i], NULL);
    //pthread_join(tids[sz], NULL);       // Join commander

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&VP_mut);
    pthread_mutex_destroy(&CG_mut);
    pthread_mutex_destroy(&check_mut);
    pthread_mutex_destroy(&check_mut_cg);
    pthread_mutex_destroy(&work_mut);
    pthread_mutex_destroy(&work_mut_cg);
    pthread_mutex_destroy(&mut_sleep);
    pthread_mutex_destroy(&break_mut);
    pthread_mutex_destroy(&mut_like_sem);
    pthread_mutex_destroy(&cont_mutex);
    pthread_mutex_destroy(&flag_mut);

    pthread_mutex_destroy(&den);
    pthread_mutex_destroy(&brcount);

    pthread_cond_destroy(&condSleep);
    pthread_cond_destroy(&condSem);
    pthread_cond_destroy(&semCont);

    
    print_grid();
    cout << "\n \n" << "Bittim sabunu" << endl;
    pthread_exit(NULL);

    return 0;
}
