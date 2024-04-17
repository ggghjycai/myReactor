/*
 * 程序名：bankserver.cpp，网上银行（BankServer）服务器。
 * 作者：吴从周
*/
#include <signal.h>
#include "BankServer.h"

// 1、设置2和15的信号。
// 2、在信号处理函数中停止主从事件循环和工作线程。
// 3、服务程序主动退出。

void ignoreallsignal()
{
  for(int ii=0;ii<64l;ii++)signal(ii,SIG_IGN);
}

BankServer *bankserver;

void Stop(int sig)    // 信号2和15的处理函数，功能是停止服务程序。
{
    printf("sig=%d\n",sig);
    // 调用BankServer::Stop()停止服务。
    bankserver->Stop();
    printf("bankserver已停止。\n");
    delete bankserver;
    printf("delete bankserver。\n");
    exit(0); 
}

int main(int argc,char *argv[])
{
  if (argc != 8)
  {
    printf("usage:./bankserver ip port IO_THREAD WORK_THREAD LOOP_TIMEVAL CONNECTION_TIMEOUT ACCEPTTYPE\n");
    printf("example:./bankserver 192.168.126.200 5055 3 3 10 5 1\n\n");
    return -1;
  }

    ignoreallsignal();
    signal(SIGTERM,Stop);    // 信号15，系统kill或killall命令默认发送的信号。
    signal(SIGINT,Stop);        // 信号2，按Ctrl+C发送的信号。

    bankserver=new BankServer(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
    bankserver->Start();

    return 0;
}
