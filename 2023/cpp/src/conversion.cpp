#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>

template <typename T>
std::vector<T> convert_string_vector_to_numbers(const std::vector<std::string>& strVec, const T& specialValue)
{
    std::vector<T> typeVec;
    
    for (const auto& str : strVec)
    {
        if (str.empty())
        {
            typeVec.push_back(specialValue);
        }
        else
        {
            std::stringstream ss(str);
            T value;

            if (ss >> value)
            {
                typeVec.push_back(value);
            }
            else
            {
                std::cerr << "Failed to convert string '" << str << "' to type " << typeid(T).name() << ".\n";
            }
        }
    }
    
    return typeVec;
}

template std::vector<uint32_t> convert_string_vector_to_numbers<uint32_t>(const std::vector<std::string>&, const uint32_t& specialValue);
template std::vector<float> convert_string_vector_to_numbers<float>(const std::vector<std::string>&, const float& specialValue);
