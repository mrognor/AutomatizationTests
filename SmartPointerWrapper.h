#pragma once

/**
    \brief A class for storing pointers.
    
    It stores a pointer inside itself, so when copying or assigning objects of this class, 
    the pointer will be copied, but the contents will be the same. The memory for the pointer will be allocated only when the object is created, 
    no memory will be allocated when copying or assigning. The deletion of the pointer will occur when the last object of the class storing the pointer is destroyed. 
    A counter is implemented inside the class, which increases when copying or assigning an object, and decreases when deleting or re-assigning
*/
template <class T>
class SmartPointerWrapper
{
private:
    T* Data = nullptr;
    std::size_t* RefCounter = nullptr;
public:

    // Copy constructor
    SmartPointerWrapper(const SmartPointerWrapper& other)
    {
        *this = other;
    }

    // Copy constructor
    SmartPointerWrapper(const SmartPointerWrapper&& other)
    {
        *this = other;
    }

    // Copy constructor
    SmartPointerWrapper(SmartPointerWrapper& other)
    {
        *this = other;
    }

    // Copy constructor
    SmartPointerWrapper(SmartPointerWrapper&& other)
    {
        *this = other;
    }

    // Default constructor thar creates T type pointer
    template <class... Args>
    SmartPointerWrapper(Args&&... args)
    {
        Data = new T(args...);
        RefCounter = new std::size_t(1);
    }

    // Operator =
    SmartPointerWrapper& operator=(const SmartPointerWrapper& other)
    {
        // Check is other same as this
        if (&other != this)
        {
            // Checking whether the data was previously assigned
            if (RefCounter != nullptr)
            {
                // Destroy previously data if this object was last
                if (*RefCounter == 1)
                {
                    delete RefCounter;
                    delete Data;
                }
                else
                {
                    --*RefCounter;
                }
            }

            Data = other.Data;
            RefCounter = other.RefCounter;
            ++*RefCounter;
        }

        return *this;
    }

    void SetData(const T& data)
    {
        *Data = data;
    }

    T GetData()
    {
        return *Data;
    }

    ~SmartPointerWrapper()
    {
        if(RefCounter != nullptr)
        {
            if (*RefCounter == 1)
            {
                delete RefCounter;
                delete Data;
            }
            else
            {
                --*RefCounter;
            }
        }
    }
};