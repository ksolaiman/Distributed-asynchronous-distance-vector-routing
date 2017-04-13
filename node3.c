#include <stdio.h>
#include<string.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt3;

/* students to write the following two routines, and maybe some others */
#define INFINITY 999

float clocktime;

int connectcosts3[4] = { 7,  999,  2, 0 };
struct rtpkt pkt3[4];
int min_cost_3[4];

int min_3 ( int a, int b ) { return a < b ? a : b; }
int min3 ( int a[] ) {
    return min_3(min_3(min_3(a[0], a[1]), a[2]), a[3]);
}

void calc_min_cost3() {
    //calculate the minimum cost to every other node
    for(int i=0;i<4;i++){
        min_cost_3[i] = min3(dt3.costs[i]);
    }
}
void sendpkt3() {
    //make the pkts
    for(int i=0;i<4;i++) {
        pkt3[i].sourceid = 3;
        pkt3[i].destid = i; // to all 4 coz all are connected
        memcpy(pkt3[i].mincost, min_cost_3, sizeof(min_cost_3));
    }
    // SEND PKTS TO ALL NEIGHBOURS
    for(int i=0;i<4;i++) {
        if(i!=3 && i!=1) {
            tolayer2(pkt3[i]);
            printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
                   clocktime, pkt3[i].sourceid, pkt3[i].destid, pkt3[i].mincost[0], pkt3[i].mincost[1],
                   pkt3[i].mincost[2],pkt3[i].mincost[3]);
        }
    }
}


void calc_send_pkt3() {
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_3, sizeof(min_cost_3));
    int change = 0;
    calc_min_cost3();
    for (int i = 0; i < 4; i++) {
        if (old_min_cost[i] != min_cost_3[i]) {
            change = 1;
        }
    }
    if (change == 1) {    //min cost changed, so send new packets
        sendpkt3();
    }
    else
        printf("\nMinimum cost didn't change. No new packets are sent\n");

}
void rtinit3() 
{
  printf("rtinit3() is called at time t=: %0.3f\n", clocktime);

  // Initialize the distance table with the direct costs
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      if(i==j)
        dt3.costs[i][j] = connectcosts3[i];
      else
        dt3.costs[i][j] = INFINITY;
    }
  }
  printdt3(&dt3);
    calc_min_cost3();
    sendpkt3();
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  int src = rcvdpkt->sourceid;
  int dest = rcvdpkt->destid;
  int mincost[4];
  for(int j= 0; j<4;j++)
    mincost[j] =  rcvdpkt->mincost[j];

  printf("rtupdate3() is called at time t=: %0.3f as node %d sent a pkt with (%d  %d  %d  %d)\n", clocktime, src,
         mincost[0], mincost[1], mincost[2], mincost[3]);


    //update the distance table
    for(int i=0;i<4;i++){
        int possibleValue = dt3.costs[src][src] + mincost[i];   //use the already calculated min_cost_0 path to all node
        //if(possibleValue < dt3.costs[i][src]){
        //    change = 1;
        if(possibleValue<INFINITY)
            dt3.costs[i][src] = possibleValue;
        else
            dt3.costs[i][src] = INFINITY;
        //}
    }

    //if(change == 1) {
        printdt3(&dt3);

        //Finally update the min_cost_3 path to all nodes
        calc_send_pkt3();
}


printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}

printmincost3(){
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 3, min_cost_3[0], min_cost_3[1],
           min_cost_3[2], min_cost_3[3] );
}






