#include <stdio.h>
#include <stdlib.h>
#include "app_scheduler.h"

#define SCHED_MAX_EVENT_DATA_SIZE      	20 	 /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE 				200  /**< Maximum size of scheduler queue. */

typedef enum {
    THREAD_INIT,
    DEMO_TEST,
    //add your msg
}msg_id_t;

typedef struct {
  uint8_t id;
  uint8_t len;
  uint8_t *data;
}thread_msg_t;

static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void msg_send_to_back(void *handler, void *msg, bool use_scher){
  if(handler==0)return;
  if(use_scher)
     app_sched_event_put(msg,sizeof(thread_msg_t),(app_sched_event_handler_t)handler);
  else
    ((app_sched_event_handler_t)handler)(msg,sizeof(thread_msg_t));
}

static void demo2_thread(void *para, uint16_t para_size)
{
    printf("demo2_thread!\n");
    thread_msg_t msg=*((thread_msg_t *)para);
    {
        switch(msg.id){
            case DEMO_TEST:{
                printf("%d,%d,%s \n",msg.id, msg.len, msg.data);
            }
            break;
        }
    }
}

static void demo1_thread(void *para, uint16_t para_size)
{
    printf("demo1_thread!\n");
    const *data = "hello,world !";
    thread_msg_t msg={DEMO_TEST,sizeof(thread_msg_t),data};
    msg_send_to_back((void *)demo2_thread,&msg,true);
}

int main()
{
    scheduler_init();

    thread_msg_t msg={THREAD_INIT,0,0};
    msg_send_to_back((void *)demo1_thread,&msg,true);

    printf("ready go!\n");
    for(;;){
      app_sched_execute();
    }
    return 0;
}
