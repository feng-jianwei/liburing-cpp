#include "service/service.h"
#include "liburing-cpp/future_probe.h"

using namespace std;

int main()
{
    PrintUringProbe();
    EchoService("127.0.0.1", 6666);
}