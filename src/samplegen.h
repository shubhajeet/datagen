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
