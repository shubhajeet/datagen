#include <random>
#include <cmath>
#include <functional>

template <typename T>
std::function<T()> log_gen(T start, T stop, T base) {
    auto bias = pow(base,1.0);
    auto norm = pow(base,2.0) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(1.0,2.0);
        return T((pow(base,dis(gen))-bias)/norm*(stop-start) + start);
        };
}

template <typename T>
std::function<T()> nlog_gen(T start, T stop) {
    auto bias = exp(1.0);
    auto norm = exp(2.0) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(1.0,2.0);
        return T((exp(dis(gen))-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> exp_gen(T start, T stop) {
    auto bias = log(1.0);
    auto norm = log(2.0) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(1.0,2.0);
        return T((log(dis(gen))-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> power_gen(T start, T stop, T power) {
    auto bias = log(1.0)/log(power);
    auto norm = log(2.0)/log(power) - bias;
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(1.0,2.0);
        return T((log(dis(gen))/log(power)-bias)/norm*(stop-start) + start);
        };
}


template <typename T>
std::function<T()> linear_gen(T start, T stop) {
    return [=]() {
        std::mt19937_64 gen(std::random_device{}());
        std::uniform_real_distribution<double> dis(0.0,1.0);
        return T(dis(gen)*(stop-start) + start);
        };
}
