#ifndef TRACE_HELPER
#define TRACE_HELPER
#include "trace_helper.h"
// #include "spdlog/spdlog.h"
#include <chrono>
#include <sstream>

template <typename T>
void load_vector(std::vector<T> &data, std::string filepath)
{
    auto start_load = std::chrono::high_resolution_clock::now();
    std::ifstream trace(filepath, std::ios::in | std::ios::binary);
    if (!trace)
    {
        // spdlog::error("Unable to open file: {}", filepath);
        std::cerr << "Unable to open file: " << filepath << std::endl;
        return;
    }
    for (; !trace.eof();)
    {
        T blockid;
        trace.read((char *)&blockid, sizeof(blockid));
        data.push_back(blockid);
    }
    auto stop_load = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_load - start_load).count();

    // spdlog::info("Loading file: {} duration: {} ns", filepath, duration);
    std::cout << "Loading file: " << filepath << " duration: " << duration << " ns" << std::endl;
}

template <typename T>
void to_csv(std::vector<T> &data, std::string filepath)
{
    // spdlog::info("Writing csv_file: {}", filepath);
    std::cout << "Writing csv_file: " << filepath << std::endl;
    auto start_write = std::chrono::high_resolution_clock::now();
    std::ofstream csvfp(filepath, std::ios::out);
    for (auto d : data)
    {
        csvfp << d << std::endl;
    }
    auto stop_write = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_write - start_write).count();
    // spdlog::info("Writing csv_file_duration: {}", duration);
    std::cout << "Writing csv_file_duration: " << duration << std::endl;
}

template <typename T>
void to_binary_file(std::vector<T> &data, std::string filepath)
{
    // spdlog::info("Writing to binary file: {}", filepath);
    std::cout << "Writing to binary file: " << filepath << std::endl;
    auto start_write = std::chrono::high_resolution_clock::now();
    std::ofstream csvfp(filepath, std::ios::out | std::ios::binary);
    for (auto d : data)
    {
        csvfp.write(reinterpret_cast<char *>(&d), sizeof(T));
    }
    auto stop_write = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_write - start_write).count();
    // spdlog::info("Writing binary_file_duration: {}", duration);
    std::cout << "Writing binary_file_duration: " << duration << std::endl;
}

template <class T>
bool load_text_data(std::vector<T> &array, const std::string &filepath)
{
    // spdlog::info("loading from a text file: {}", filepath);
    std::cout << "loading from a text file: " << filepath << std::endl;
    std::ifstream is(filepath.c_str());
    // std::ifstream is(file_path);
    if (!is.is_open())
    {
        std::cerr << "Unable to open file " << filepath << std::endl;
        return false;
    }
    int i = 0;
    std::string str;
    while (std::getline(is, str))
    {
        std::istringstream ss(str);
        T data;
        ss >> data;
        array.push_back(data);
        i++;
    }
    is.close();
    return true;
}

#endif