#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

namespace
{
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using Clock = std::chrono::steady_clock;

struct ExperimentConfig
{
    std::string name = {};
    std::uint64_t n = 0;
    std::uint32_t threadCount = 0;
};

struct ExperimentResult
{
    std::uint64_t singleThreadResult = 0;
    milliseconds::rep singleMs = 0;
    std::uint64_t multiThreadResult = 0;
    milliseconds::rep multiMs = 0;
};

void printExperimentResult(const ExperimentConfig& config, const ExperimentResult& result)
{
    std::cout << "==== " << config.name << " ====\n";
    std::cout << "n = " << config.n << "\n";
    std::cout << "threadCount = " << config.threadCount << "\n\n";
    std::cout << "[单线程] result = " << result.singleThreadResult
              << ", time = " << result.singleMs << " ms\n";
    std::cout << "[多线程] result = " << result.multiThreadResult
              << ", time = " << result.multiMs << " ms\n";
    std::cout << "结果是否一致: "
              << (result.singleThreadResult == result.multiThreadResult ? "是" : "否") << "\n";
    if (result.multiMs > 0)
        std::cout << "加速比约: "
                  << (static_cast<double>(result.singleMs) / result.multiMs) << "x\n";
}

std::uint64_t runTimed(const std::function<std::uint64_t()>& work, milliseconds::rep& outMs)
{
    const Clock::time_point start = Clock::now();
    const std::uint64_t result = work();
    const Clock::time_point end = Clock::now();
    outMs = duration_cast<milliseconds>(end - start).count();
    return result;
}

std::uint64_t sumRange(std::uint64_t begin, std::uint64_t end)
{
    std::uint64_t sum = 0;
    for (std::uint64_t value = begin; value < end; ++value)
    {
        sum += value;
    }
    return sum;
}

std::uint64_t sumMultiThread(std::uint64_t n, std::uint32_t threadCount)
{
    if (threadCount == 0)
        threadCount = 1;

    const std::uint64_t totalCount = n;
    const std::uint64_t chunkSize = totalCount / threadCount;
    const std::uint64_t remainder = totalCount % threadCount;
    std::vector<std::thread> workers;
    std::vector<std::uint64_t> partialSums(threadCount, 0);

    std::uint64_t rangeBegin = 1;
    for (std::uint32_t threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        const std::uint64_t rangeLength = chunkSize + (threadIndex < remainder ? 1 : 0);
        const std::uint64_t rangeEnd = rangeBegin + rangeLength;
        workers.emplace_back([&, threadIndex, rangeBegin, rangeEnd]()
                             {
                                 partialSums[threadIndex] = sumRange(rangeBegin, rangeEnd);
                             });
        rangeBegin = rangeEnd;
    }
    for (std::thread& worker : workers)
        worker.join();

    return std::accumulate(partialSums.begin(), partialSums.end(), std::uint64_t{0});
}

std::uint64_t fibSingle(std::uint64_t n)
{
    if (n <= 1)
        return n;
    return fibSingle(n - 1) + fibSingle(n - 2);
}

std::uint64_t fibMulti(std::uint64_t n, std::uint32_t workersLeft)
{
    if (n <= 1)
        return n;
    if (workersLeft <= 1 || n < 35)
        return fibSingle(n);

    std::uint64_t leftResult = 0;
    std::uint64_t rightResult = 0;
    const std::uint32_t giveToLeft = workersLeft / 2;
    const std::uint32_t giveToRight = workersLeft - giveToLeft;

    std::thread leftWorker([&]()
                           {
                               leftResult = fibMulti(n - 1, giveToLeft);
                           });
    rightResult = fibMulti(n - 2, giveToRight);
    leftWorker.join();
    return leftResult + rightResult;
}

}

int main()
{
    constexpr bool isRunSum = true;
    constexpr std::uint64_t sumN = 300'000'000;
    constexpr std::uint64_t fiboN = 42;
    constexpr std::uint32_t threadCount = 4;

    const auto runAndPrintExperiment =
        [](const ExperimentConfig& config,const std::function<std::uint64_t()>& singleWork,
           const std::function<std::uint64_t()>& multiWork)
    {
        ExperimentResult result{};
        result.singleThreadResult = runTimed(singleWork, result.singleMs);
        result.multiThreadResult = runTimed(multiWork, result.multiMs);
        printExperimentResult(config, result);
    };

    if (isRunSum)
    {
        const ExperimentConfig config{"sum", sumN, threadCount};
        runAndPrintExperiment(
            config,
            [n = config.n]() { return sumRange(1, n + 1); },
            [n = config.n, threadCount = config.threadCount]()
            {
                return sumMultiThread(n, threadCount);
            });
    }
    else
    {
        const ExperimentConfig config{"fib", fiboN, threadCount};
        runAndPrintExperiment(config,[n = config.n]() { return fibSingle(n); },
            [n = config.n, threadCount = config.threadCount](){ return fibMulti(n, threadCount);});
    }

    return 0;
}