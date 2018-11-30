#if 0
#include <iostream>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

class Ebupt
{
	public: Ebupt();
	virtual ~Ebupt();
	void dealMsg(long wait_ns);

	private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

Ebupt::Ebupt()
{
    pthread_mutex_init(&mutex, NULL);

    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
	//CLOCK_MONOTONIC    //CLOCK_REALTIME
    pthread_condattr_setclock(&condattr, CLOCK_REALTIME);
	
    pthread_cond_init(&cond, &condattr);
    pthread_condattr_destroy(&condattr);

}

Ebupt::~Ebupt()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}


void Ebupt::dealMsg(long wait_ns)
{
    pthread_mutex_lock(&mutex);

	
#if 0
    struct timespec now;
	struct timespec abstime;

	//CLOCK_MONOTONIC    //CLOCK_REALTIME
    clock_gettime(CLOCK_MONOTONIC, &now);
   

    if (now.tv_nsec + (wait_ns%1000000000) >= 1000000000)
    {
        abstime.tv_sec = now.tv_sec + wait_ns/1000000000 + 1;
        abstime.tv_nsec = (now.tv_nsec + wait_ns%1000000000)%1000000000;
    }
    else
    {
        abstime.tv_sec = now.tv_sec + wait_ns/1000000000;
        abstime.tv_nsec = now.tv_nsec + wait_ns%1000000000;
    }
#else
	struct timeval now;
	struct timespec abstime;
	long wait = wait_ns/1000000;
	gettimeofday(&now, NULL);

	abstime.tv_sec   = now.tv_sec;
	abstime.tv_nsec  = now.tv_usec * 1000u;
	abstime.tv_sec   += wait/1000;
	abstime.tv_nsec  += (wait%1000) * 1000000u;
	
#endif

    pthread_cond_timedwait(&cond, &mutex, &abstime);
    pthread_mutex_unlock(&mutex);
}


int main()
{
Ebupt e;
struct timeval now;
while (true)
{
	gettimeofday(&now, NULL);
	cout<<"++"<<now.tv_sec<<":"<<now.tv_usec<<endl;
	e.dealMsg(200000000);
	gettimeofday(&now, NULL);
	cout<<"--"<<now.tv_sec<<":"<<now.tv_usec<<endl;
	cout<<"  "<<endl;
}
return 0;
}

#else

#include <stdio.h>
#include <osa_sem.h>
#include <osa_thr.h>

OSA_SemHndl sem1;
OSA_ThrHndl pth1;

void * func(void * arg)
{
	while(1)
	{
		OSA_waitMsecs(3000);
	//	OSA_semSignal(&sem1);

	}
}

int main()
{
	
	OSA_semCreate(&sem1, 1, 0);

	OSA_thrCreate(&pth1, func, 0, 0, NULL);

	
	while(1)
	{
		OSA_printf("11111  time :%d \n",OSA_getCurTimeInMsec());
			OSA_semWait(&sem1 , 200);
		OSA_printf("22222  time :%d \n",OSA_getCurTimeInMsec());		

		OSA_waitMsecs(1000);
	}

	return 0;
}


#endif

