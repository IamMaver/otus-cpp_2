#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>

using ip4address = std::array<uint8_t, 4>;

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find(d);

    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

ip4address parse_ip(std::string &s, const char delimiter)
{
    if (auto pos = s.find(delimiter); pos != std::string::npos)
        s.erase(pos);

    auto parts = split(s, '.');

    if (parts.size() != 4)
        throw std::runtime_error("Invalid IP format: " + s);

    ip4address ip{};
    for (size_t i = 0; i < 4; ++i)
    {
        const std::string &t = parts[i];
        if (t.empty())
            throw std::runtime_error("Empty IP octet");

        for (char c : t)
            if (!std::isdigit(static_cast<unsigned char>(c)))
                throw std::runtime_error("Invalid IP octet: " + t);

        if (t.size() > 3)
            throw std::out_of_range("Invalid IP octet: " + t);

        int num = 0;
        for (char c : t)
            num = num * 10 + (c - '0');

        if (num > 255)
            throw std::out_of_range("Invalid IP octet: " + t);

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

inline void print_ip(ip4address const ip)
{
    std::cout << static_cast<uint32_t>(ip[0]) << '.' << static_cast<uint32_t>(ip[1]) << '.' << static_cast<uint32_t>(ip[2]) << '.' << static_cast<uint>(ip[3]) << '\n';
}

template <class Pred>
void print_ip_pool(const std::vector<ip4address> &in, Pred pred)
{
    for (const auto &ip : in)
        if (pred(ip))
        {
            print_ip(ip);
        }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        auto ip_pool = fill_vector_ip_pool();

        std::sort(ip_pool.begin(), ip_pool.end(),
                  std::greater<>());

        print_ip_pool(ip_pool, [](const ip4address ip)
                      { return true; });
        print_ip_pool(ip_pool, [](const ip4address ip)
                      { return ip[0] == 1; });
        print_ip_pool(ip_pool, [](const ip4address ip)
                      { return ip[0] == 46 && ip[1] == 70; });
        uint8_t value = 46;
        print_ip_pool(ip_pool, [value](const ip4address ip)
                      { return ip[0] == value || ip[1] == value || ip[2] == value || ip[3] == value; });
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
