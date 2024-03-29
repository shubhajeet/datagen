// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
#include <iterator>
#include <fstream>
#include "zipf.h"
// #include "spdlog/spdlog.h"

template <typename F>
void display(F begin, F end)
{
  for (F p = begin; p != end; p++)
  {
    std::cout << *p << std::endl;
  }
}

template <typename F>
void record(F begin, F end, std::string filename)
{
  std::cout << "Writing output to: " << filename << std::endl;
  std::ofstream data;
  data.open(filename);
  for (F p = begin; p != end; p++)
  {
    data << *p << std::endl;
  }
  data.close();
}

/*
template < typename F, typename S> void display(F begin, F end) {
  for(std::vector<std::pair<F,S>>::iterator p = begin; p != end; p++) {
    std::cout << p->first << ","  << p->second << std::endl;
  }
}
*/

template <typename F, typename S>
void record(std::vector<std::pair<F, S>> predictions, std::string filename)
{
  std::ofstream data;
  data.open(filename);
  for (auto p : predictions)
  {
    data << p.first << "," << p.second << std::endl;
  }
  data.close();
}

template <class T>
bool load_binary_data(T data[], int length, const std::string &file_path)
{
  std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
  if (!is.is_open())
  {
    std::cerr << "Unable to open file " << file_path << std::endl;
    return false;
  }
  is.read(reinterpret_cast<char *>(data), std::streamsize(length * sizeof(T)));
  is.close();
  return true;
}

template <class T>
bool load_text_data(std::vector<T> &array, int length, const std::string &file_path)
{
  std::cout << "load_text_data" << std::endl;
  std::ifstream is(file_path.c_str());
  // std::ifstream is(file_path);
  if (!is.is_open())
  {
    std::cerr << "Unable to open file " << file_path << std::endl;
    return false;
  }
  int i = 0;
  std::string str;
  while (std::getline(is, str) && i < length)
  {
    // std::cout << str;
    std::istringstream ss(str);
    T data;
    ss >> data;
    array.push_back(data);
    // std::cout << array[i];
    i++;
  }
  is.close();
  return true;
}

template <class T>
bool load_text_data(T array[], int length, const std::string &file_path)
{
  std::cout << "load_text_data" << std::endl;
  std::ifstream is(file_path.c_str());
  // std::ifstream is(file_path);
  if (!is.is_open())
  {
    std::cerr << "Unable to open file " << file_path << std::endl;
    return false;
  }
  int i = 0;
  std::string str;
  while (std::getline(is, str) && i < length)
  {
    // std::cout << str;
    std::istringstream ss(str);
    ss >> array[i];
    // std::cout << array[i];
    i++;
  }
  is.close();
  return true;
}

template <class T>
T *get_search_keys(T array[], int num_keys, int num_searches)
{
  std::mt19937_64 gen(std::random_device{}());
  std::uniform_int_distribution<int> dis(0, num_keys - 1);
  auto *keys = new T[num_searches];
  for (int i = 0; i < num_searches; i++)
  {
    int pos = dis(gen);
    keys[i] = array[pos];
  }
  return keys;
}

template <class T>
T *get_search_keys_zipf(T array[], int num_keys, int num_searches)
{
  auto *keys = new T[num_searches];
  ScrambledZipfianGenerator zipf_gen(num_keys);
  for (int i = 0; i < num_searches; i++)
  {
    int pos = zipf_gen.nextValue();
    keys[i] = array[pos];
  }
  return keys;
}
