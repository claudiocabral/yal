#pragma once
#include <vector>

template <class T, size_t bucket_size = 128>
struct Channel
{
    using Bucket = std::array<T, bucket_size>;
    using Container = std::vector<Bucket *>;
    size_t size = 0;
    Container data;
    void push_back(T &&t)
    {
        auto new_size = size + 1;
        auto i = new_size / 128;
        auto j = new_size % 128;
        if (i >= data.size())
        {
            Container new_container(new_size);
            new_container = data;
            new_container.push_back(new Bucket());
            (*new_container[i])[j] = std::move(t);
            data = std::move(new_container);
        } else {
            (*data[i])[j] = std::move(t);
        }
    }
    T &pop() { }
    ~Channel() {
        for (auto ptr : data) delete ptr;
    }
};
