#include <random>
#include <cmath>
#include <functional>

#define MIN_RANGE 1.0 
#define MAX_RANGE 100.0
template <typename T>
std::function<T()> log_gen(T start, T stop, T base) {
    auto bias = pow(base,MIN_RANGE);
    auto norm = pow(base,MAX_RANGE) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T((pow(base,dis(gen))-bias)/norm*(stop-start) + start);
        };
}

template <typename T>
std::function<T()> nlog_gen(T start, T stop) {
    auto bias = exp(MIN_RANGE);
    auto norm = exp(MAX_RANGE) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T((exp(dis(gen))-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> exp_gen(T start, T stop) {
    auto bias = log(MIN_RANGE);
    auto norm = log(MAX_RANGE) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T((log(dis(gen))-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> power_gen(T start, T stop, T power) {
    auto bias = log(MIN_RANGE)/log(power);
    auto norm = log(MAX_RANGE)/log(power) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T((log(dis(gen))/log(power)-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> linear_gen(T start, T stop) {
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T(dis(gen)*(stop-start) + start);
    };
}

template <typename T>
std::function<T()> linstep_gen(T start, T step) {
    //int counter =  0;
    //return [=,&counter]() {
    return [=]() {
        static int counter = 0;
        counter++;
        return T(start + counter * step);
    };
}

template <typename T>
std::function<T()> logstep_gen(T start, T step, T base) {
    //auto bias = pow(base,MIN_RANGE);
    //auto norm = pow(base,MAX_RANGE) - bias;
    //int counter = 0;
    return [=]() {
        static int counter = 0;
        counter++;
        //std::mt19937_64 gen(std::random_device{}());
        //std::uniform_real_distribution<double> dis(MIN_RANGE,MAX_RANGE);
        return T(pow(base,start + counter * step));
        };
}

template <typename T>
std::function<T()> expstep_gen(T start, T step) {
    return [=]() {
        static int counter = 0;
        counter++;
        return T(log(start + counter * step));
    };
}
/*
template <typename T>
std::function<T()> normaldis_gen(T mean, T std_dev) {
    std::default_random_engine generator;
    return [=]() {
        std::normal_distribution distribution(float(mean),float(std_dev));
        return T(distribution(generator));
    };
}

template <typename T>
std::function<T()> lognormaldis_gen(T mean, T std_dev) {
    std::default_random_engine generator;
    std::lognormal_distribution distribution(mean,std_dev);
    return [=]() {
        return T(distribution(generator));
    };
}
*/