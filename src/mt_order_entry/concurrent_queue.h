//
// Created by HaiNt on 7/18/18.
//
#pragma once
#ifndef LIQUIBOOK_CONCURRENT_QUEUE_H
#define LIQUIBOOK_CONCURRENT_QUEUE_H

#endif //LIQUIBOOK_CONCURRENT_QUEUE_H


#include <queue>
#include <boost/thread.hpp>

template <class T>
class concurrent_queue
{
private:
    std::queue<T> the_queue;
    mutable boost::mutex the_mutex;
    boost::condition_variable the_condition_variable;

public:
    void push(T const& data);
    void pop();
    bool empty();
    T front();
    int size();
};

template <class T>
void concurrent_queue<T>::push(T const& data)
{
    boost::mutex::scoped_lock lock(the_mutex);
    the_queue.push(data);
    the_condition_variable.notify_one();
}

template <class T>
void concurrent_queue<T>::pop()
{
    boost::mutex::scoped_lock lock(the_mutex);

    if (the_queue.empty())
    {
        return;
    }

    the_queue.pop();
}

template <class T>
bool concurrent_queue<T>::empty()
{
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.empty();
}

template <class T>
T concurrent_queue<T>::front()
{
    boost::mutex::scoped_lock lock(the_mutex);

    while (the_queue.empty())
    {
        the_condition_variable.wait(lock);
    }

    T popped_value;
    popped_value = the_queue.front();

    return popped_value;
}

template <class T>
int concurrent_queue<T>::size()
{
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.size();
}