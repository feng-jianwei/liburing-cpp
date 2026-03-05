#include "benchmark/benchmark.h"

#include <liburing.h>
#include "service/service.h"

using namespace std;

static void BM_Service(benchmark::State& state) {
    for (auto _ : state) {
        EchoService("127.0.0.1", 6666);
    }
}
BENCHMARK(BM_Service)->Unit(benchmark::kMillisecond)->Iterations(1)->Threads(4);

BENCHMARK_MAIN();