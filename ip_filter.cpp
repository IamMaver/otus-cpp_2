#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void print_ip_pool(std::vector<std::vector<std::string>> const &ip_pool)
{
    for (std::vector<std::vector<std::string>>::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for (std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<std::string>> filter_1(std::vector<std::vector<std::string>> &in)
{
    std::vector<std::vector<std::string>> rv;

    for (auto v : in)
    {
        if (std::stoi(v[0]) == 1)
        {
            rv.push_back(v);
        }
    }
    return rv;
}

std::vector<std::vector<std::string>> filter_46_70(std::vector<std::vector<std::string>> &in)
{
    std::vector<std::vector<std::string>> rv;

    for (auto v : in)
    {
        if (std::stoi(v[0]) == 46 && std::stoi(v[1]) == 70)
        {
            rv.push_back(v);
        }
    }
    return rv;
}

std::vector<std::vector<std::string>> filter_any_46(std::vector<std::vector<std::string>> &in)
{
    std::vector<std::vector<std::string>> rv;

    for (auto v : in)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (std::stoi(v[i]) == 46)
            {

                rv.push_back(v);
                break;
            }
        }
    }
    return rv;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        std::vector<std::vector<std::string>> ip_pool;

        for (std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        std::sort(ip_pool.begin(), ip_pool.end(),
                  [](const std::vector<std::string> &a, const std::vector<std::string> &b)
                  {
                      for (int i = 0; i < 4; ++i)
                      {
                          int ai = std::stoi(a[i]);
                          int bi = std::stoi(b[i]);
                          if (ai != bi)
                              return ai > bi;
                      }
                      return false;
                  });

        print_ip_pool(ip_pool);
        print_ip_pool(filter_1(ip_pool));
        print_ip_pool(filter_46_70(ip_pool));
        print_ip_pool(filter_any_46(ip_pool));
        // хотел сделать на базе ranges view, но вспомнил по 11 стандарт
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
