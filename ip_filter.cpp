#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

using ip4address = std::array<int, 4>;

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

ip4address parse_ip(std::string s, const char delimiter)
{
    size_t pos = s.find(delimiter);
    if (pos != std::string::npos)
    {
        s = s.substr(0, pos);
    }

    auto parts = split(s, '.');
    if (parts.size() != 4)
    {
        throw std::runtime_error("Invalid IP format: " + s);
    }

    ip4address ip{};
    for (size_t i = 0; i < 4; ++i)
    {
        int num = std::stoi(parts[i]);
        if (num < 0 || num > 255)
        {
            throw std::out_of_range("Invalid IP octet: " + parts[i]);
        }
        ip[i] = num;
    }
    return ip;
}

std::vector<ip4address> fill_vector_ip_pool()
{
    std::vector<ip4address> ip_pool;

    for (std::string line; std::getline(std::cin, line);)
    {
        ip_pool.push_back(parse_ip(line, '\t'));
    }
    return ip_pool;
}

void print_ip_pool(std::vector<ip4address> const &ip_pool)
{
    for (auto ip_address = ip_pool.cbegin(); ip_address != ip_pool.cend(); ip_address++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (i != 0)
            {
                std::cout << ".";
            }
            std::cout << (*ip_address)[i];
        }
        std::cout << std::endl;
    }
}

std::vector<std::array<int, 4>>
filter(const std::vector<std::array<int, 4>> &in, std::initializer_list<int> prefix)
{
    std::vector<std::array<int, 4>> out;
    out.reserve(in.size());
    for (const auto &ip : in)
    {
        bool ok = true;
        size_t i = 0;
        for (int want : prefix)
        {
            if (i >= 4 || ip[i] != want)
            {
                ok = false;
                break;
            }
            ++i;
        }
        if (ok)
            out.push_back(ip);
    }
    out.shrink_to_fit();
    return out;
}

std::vector<std::array<int, 4>>
filter_any(const std::vector<std::array<int, 4>> &in, int value)
{
    std::vector<std::array<int, 4>> out;
    out.reserve(in.size());
    for (const auto &ip : in)
    {
        if (ip[0] == value || ip[1] == value || ip[2] == value || ip[3] == value)
        {
            out.push_back(ip);
        }
    }
    out.shrink_to_fit();
    return out;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        std::vector<ip4address> ip_pool;

        ip_pool = fill_vector_ip_pool();
        std::sort(ip_pool.begin(), ip_pool.end(),
                  std::greater<>());
        print_ip_pool(ip_pool);
        print_ip_pool(filter(ip_pool, {1}));
        print_ip_pool(filter(ip_pool, {46, 70}));
        print_ip_pool(filter_any(ip_pool, 46));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
