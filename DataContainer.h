#pragma once

#include <unordered_map>

#include "DataSaver.h"

// A container class for storing data of any type. Allows to specify required container.
// Container have to support find(), begin(), end(), emplace(), erase()
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
template <class C>
class DataContainer
{
protected:
    // Hash map to store all DataSaver's
    C Data;

public:
    // Iterators
    typedef typename C::iterator iterator;
    typedef typename C::const_iterator const_iterator;

    // Begin and end functions to work with iterators and foreach loop
    inline iterator begin() noexcept { return Data.begin(); }
    inline const_iterator cbegin() const noexcept { return Data.cbegin(); }
    inline iterator end() noexcept { return Data.end(); }
    inline const_iterator cend() const noexcept { return Data.cend(); }

    // Method to add new element with data inside container
    template <class T>
    void AddData(const std::string& key, const T& data)
    {
        Data.emplace(key, DataSaver(data));
    }

    // Method to add new element with data and custom delete function inside container
    template <class T, class F>
    void AddData(const std::string& key, const T& data, F&& deleteFunc)
    {
        Data.emplace(key, DataSaver(data, deleteFunc));
    }

    // Method to update element data
    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        SetData(key, data, nullptr);
    }

    // Method to update element data and delete function
    template <class T, class F>
    void SetData(const std::string& key, const T& data, F&& deleteFunc)
    {
        auto f = Data.find(key);

        if (f == Data.end())
            AddData(key, data, deleteFunc);
        else
            f->second.SetData(data, deleteFunc);
    }

    // Method to get data from container using key
    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        auto f = Data.find(key);
        if (f == Data.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    // Method for checking the presence of data in the container
    bool IsData(const std::string& key)
    {
        return Data.find(key) != Data.end();
    }

    // Custom data deletion
    void EraseData(const std::string& key)
    {
        auto f = Data.find(key);
        if (f != Data.end())
        {
            f->second.ResetData();
            Data.erase(key);
        }
    }

    // Method for clear all data inside container
    // Note that if pointers were stored in the container, they must be cleaned manually
    void Clear()
    {
        Data.clear();
    }
};

// A container class for storing data of any type. 
// As the base class of the container, std::unordered_map is used, which is a hash map
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
class DataHashMap : public DataContainer<std::unordered_map<std::string, DataSaver>> {};

// A container class for storing data of any type. 
// As the base class of the container, std::unordered_multimap is used, which is a hash map
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
class DataMultiHashMap : public DataContainer<std::unordered_multimap<std::string, DataSaver>>
{
public:
    // Returns a pair of iterators. The first iterator points to the first element with the same key, and the second to the last element
    // Equals std::unordered_multimap<...>::equal_range
    std::pair<DataContainer::iterator, DataContainer::iterator> GetAllData(const std::string& key)
    {
        return Data.equal_range(key);
    }
};
