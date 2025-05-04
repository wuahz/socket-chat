#pragma once

namespace wuah {
namespace singleton {

template <typename T>
class Singleton
{
public:
    static T& get_instance(); // Meyers Singleton

private:
    Singleton() = default; // 默认构造函数
    Singleton(const Singleton<T>&) = delete; // 禁止拷贝构造函数
    Singleton<T>& operator=(const Singleton<T>&) = delete; // 禁止拷贝赋值运算符
    ~Singleton() = default ; // 默认析构函数
};

template<typename T>
T& Singleton<T>::get_instance()
{
   static T m_instance; // 局部静态变量
   return m_instance; 
}

} // namespace singleton
} // namespace wuah