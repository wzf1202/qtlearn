#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
#include <cstdint>
#include <functional>

namespace
{
std::atomic<std::int64_t> total_produced = {0};
std::atomic<std::int64_t> total_consumed = {0};
std::atomic<std::int32_t> max_queue_size = {0};
}



class BoundedBuffer
{
public:
    explicit BoundedBuffer(std::size_t capacity)
        : m_capacity(capacity)
        , m_buffer(capacity)
    {
        if (capacity == 0)
            return;
    }

    void push(int item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cvNotFull.wait(lock, [this]()
        {
            return m_size < m_capacity || m_bClosed;
        });

        if (m_bClosed)
            return;

        m_buffer[m_tail] = item;
        m_tail = (m_tail + 1) % m_capacity;
        ++m_size;

        total_produced.fetch_add(1, std::memory_order_relaxed);

        const int currentSize = static_cast<int>(m_size);
        int observed = max_queue_size.load(std::memory_order_relaxed);
        while (currentSize > observed)
        {
            if (max_queue_size.compare_exchange_strong(observed,
                currentSize, std::memory_order_relaxed))
                break;
        }

        m_cvNotEmpty.notify_one();
    }

    bool pop(int& outItem)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cvNotEmpty.wait(lock, [this]()
        {
            return m_size > 0 || m_bClosed;
        });

        if (m_size == 0)
            return false;

        outItem = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
        --m_size;

        total_consumed.fetch_add(1, std::memory_order_relaxed);

        m_cvNotFull.notify_one();
        return true;
    }

    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_bClosed = true;
        }
        m_cvNotFull.notify_all();
        m_cvNotEmpty.notify_all();
    }

private:
    const std::size_t m_capacity = {};
    std::vector<int> m_buffer = {};
    std::size_t m_head = 0;
    std::size_t m_tail = 0;
    std::size_t m_size = 0;
    bool m_bClosed = false;

    std::mutex m_mutex = {};
    std::condition_variable m_cvNotFull = {};
    std::condition_variable m_cvNotEmpty = {};
};

int main()
{
    constexpr int producerCount = 3;
    constexpr int consumerCount = 2;
    constexpr int itemsPerProducer = 100;
    constexpr std::size_t bufferCapacity = 30;

    BoundedBuffer buffer(bufferCapacity);

    std::vector<std::thread> producers = {};
    std::vector<std::thread> consumers = {};
    std::vector<std::int64_t> consumerSums(consumerCount, 0);

    producers.reserve(producerCount);
    consumers.reserve(consumerCount);

    for (int i = 0; i < producerCount; ++i)
    {
        producers.emplace_back([&buffer, itemsPerProducer, seed = static_cast<std::int32_t>(i + 1)]()
        {
            std::mt19937 gen(seed);
            std::uniform_int_distribution<int> dist(1, 100);
            for (int n = 0; n < itemsPerProducer; ++n)
            {
                buffer.push(dist(gen));
            }
        });
    }

    for (int i = 0; i < consumerCount; ++i)
    {
        consumers.emplace_back([&buffer, &sum = consumerSums[i]]()
        {
            int item = 0;
            while (buffer.pop(item))
            {
                sum += item;
            }
        });
    }

    std::function<void(std::vector<std::thread>&)> joinAll =
        [](std::vector<std::thread>& threads)
    {
        for (std::thread& t : threads)
        {
            t.join();
        }
    };
    joinAll(producers);
    buffer.close();
    joinAll(consumers);

    std::int64_t sumOfConsumerSums = 0;
    for (std::int64_t s : consumerSums)
    {
        sumOfConsumerSums += s;
    }

    std::cout << "total_produced  = " << total_produced.load() << '\n';
    std::cout << "total_consumed  = " << total_consumed.load() << '\n';
    std::cout << "consumer_sum    = " << sumOfConsumerSums << '\n';
    std::cout << "max_queue_size  = " << max_queue_size.load() << '\n';

    if (total_produced.load() != total_consumed.load())
    {
        std::cerr << "ERROR: produced != consumed\n";
        return 1;
    }

    return 0;
}
