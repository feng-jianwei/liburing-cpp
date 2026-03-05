#include "gtest/gtest.h"

#include <liburing.h>

#include "service/service.h"

using namespace std;

TEST(EchoService, EchoService)
{
    EchoService("127.0.0.1", 6666);
}