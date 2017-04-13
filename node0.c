#include <stdio.h>
#include <string.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

#define INFINITY 999

struct distance_table 
{
  int costs[4][4];
} dt0;


/* students to write the following two routines, and maybe some others */

float clocktime;

int connectcosts0[4] = { 0,  1,  3, 7 };
struct rtpkt pkt0[4];
int min_cost_0[4];

int min_0 ( int a, int b ) { return a < b ? a : b; }
int min0 ( int a[] ) {
    return min_0(min_0(min_0(a[0], a[1]), a[2]), a[3]);
}

void calc_min_cost0(){
    //calculate the minimum cost to every other node
    for(int i=0;i<4;i++){
        min_cost_0[i] = min0(dt0.costs[i]);
    }
}
void sendpkt0(){
    //make the pkts
    for(int i=0;i<4;i++) {
        pkt0[i].sourceid = 0;
        pkt0[i].destid = i; // to all 4 coz all are connected
        memcpy(pkt0[i].mincost, min_cost_0, sizeof(min_cost_0));
    }

    // SEND PKTS TO ALL NEIGHBOURS
    for(int i=0;i<4;i++) {
        if(i!=0) {   //not sending self
            tolayer2(pkt0[i]);
            printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
                   clocktime, pkt0[i].sourceid, pkt0[i].destid, pkt0[i].mincost[0], pkt0[i].mincost[1],
                   pkt0[i].mincost[2],pkt0[i].mincost[3]);
        }
    }
}

void calc_send_pkt0() {
    int old_min_cost[4];
    memcpy(old_min_cost, min_cost_0, sizeof(min_cost_0));
    int change = 0;
    calc_min_cost0();
    for(int i=0;i<4;i++){
        if(old_min_cost[i]!=min_cost_0[i]){
            change = 1;
        }
    }
    if(change == 1){    //min cost changed, so send new packets
        sendpkt0();
    }
    else
        printf("\nMinimum cost didn't change. No new packets are sent\n");

}

void rtinit0() 
{
    printf("rtinit0() is called at time t=: %0.3f\n", clocktime);

    // Initialize the distance table with the direct costs
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(i==j)
                dt0.costs[i][j] = connectcosts0[i];
            else
                dt0.costs[i][j] = INFINITY;
        }
    }

    printdt0(&dt0);

    calc_min_cost0();
    sendpkt0();


}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    int dest = rcvdpkt->destid;
    int mincost[4];
    for(int j= 0; j<4;j++)
        mincost[j] =  rcvdpkt->mincost[j];

    printf("rtupdate0() is called at time t=: %0.3f as node %d sent a pkt with (%d  %d  %d  %d)\n", clocktime, src,
           mincost[0], mincost[1], mincost[2], mincost[3]);

    int change;

    //update the distance table
    for(int i=0;i<4;i++){
        //int possibleValue = min_cost_0[src] + mincost[i];   //use the already calculated min_cost_0 path to all node
        int possibleValue = dt0.costs[src][src] + mincost[i];
        //if(possibleValue < dt0.costs[i][src]){
        //    change = 1;
        if(possibleValue<INFINITY)
            dt0.costs[i][src] = possibleValue;
        else
            dt0.costs[i][src] = INFINITY;
        //}
    }

    //if(change == 1) {
        printdt0(&dt0);

        //Finally update the min_cost_0 path to all nodes
        calc_send_pkt0();

}


printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

printmincost0(){
    printf("Minimum cost from %d to other nodes are: %d %d %d %d\n", 0, min_cost_0[0], min_cost_0[1],
           min_cost_0[2], min_cost_0[3] );
}

void linkhandler0(linkid, newcost)
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
    int old_dist_linkid_to_others[4];
    for(int i=0;i<4;i++){
        old_dist_linkid_to_others[i] = dt0.costs[i][linkid] - dt0.costs[linkid][linkid];
    }

    int new_dist_0_to_linkid = newcost;

    for(int i=0;i<4;i++){
        dt0.costs[i][linkid] = new_dist_0_to_linkid + old_dist_linkid_to_others[i]; // = new_dist_0_to_i (0 to i via linkid)
    }

    printdt0(&dt0);

    calc_send_pkt0();
}

