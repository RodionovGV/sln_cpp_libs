// boost_intro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <locale.h>
#include <chrono>

// boost_filesystem.cpp
int fs_main();

extern class Buffer;

class ScopedTimer {
public:

    // RAII = конструктор через initializer list
    ScopedTimer()
    :start_(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = end - start_;
        printf("-- Time elapsed %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ms).count());
    }

    // Если ты не запретил копирование — значит разрешил
    // ресурс один, владелец один.
    // Запрет копирования
    // Disable copying (RAII)
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

    //разрешаем: передача владения, а не копия
    // Если класс владеет ресурсом — сначала подумай о копировании, а не о логике.
    ScopedTimer(ScopedTimer&) = default;
    ScopedTimer& operator=(ScopedTimer&&) = default;

private:
    std::chrono::high_resolution_clock::time_point start_;
};

// Запрещено:
//ScopedTimer makeTimer() {
//    ScopedTimer t;
//    return t;
//}

// ScopedTimer t = std::move(other);

/*
тренировка gpt

*/
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cassert> // assert

class Buffer {
public:
    explicit Buffer(size_t size)
        : size_{ size },
        data_{new uint8_t[size]} {
    }

    ~Buffer() {
        delete[] data_;
    };

    // копирование?
    // copy ctor
    Buffer(const Buffer& other)
        : size_(other.size_),
        data_(new uint8_t[other.size_]) {
        // what copy, where copy
        if (size_ > 0) {
            std::memcpy(data_, other.data_, size_);
        }
    };

    // копирующее присваивание
    // copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_; // удаляем данные которые есть
            size_ = other.size_; // берём размер другого
            data_ = new uint8_t[size_];
            // what? , where? - как будто перепутано, проверим.
            std::memcpy(data_, other.data_, size_);
        }
        return *this;
    };

    // move constructor
    // передача ресурса
    Buffer(Buffer&& other) noexcept
        : size_(other.size_),
        data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr; // тут не понятно что происходит, мы забираем у other его данные себе?
    }

    // move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
                delete[] data_;     // освобождаем свои ресурсы
                data_ = other.data_; // забираем чужие 
                size_ = other.size_;
                other.data_ = nullptr; // оставляем other ни с чем
                other.size_ = 0;
        }
        return *this;
    };

    size_t size() const;
    unsigned char* data();

private:
    size_t size_{ 0 };
    unsigned char* data_{ nullptr };
};

size_t Buffer::size() const {
    return size_;
}

unsigned char* Buffer::data() {
    return data_; 
}

int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Hello World!\n";

    auto start = std::chrono::high_resolution_clock::now();

    ScopedTimer  t{};
    {
        ScopedTimer  t1{};
    }
     fs_main();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;

    std::cout << "\nTime elapsed:"
        << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
        << "mc\n";
    {
        Buffer a(160);
        std::memset(a.data(), 0xAA, a.size());

        //Buffer b(160);
        //std::memset(b.data(), 0xBB, b.size());
        Buffer b = a;
        b.data()[0] = 0xFF;
        printf("%02X %02X\n", a.data()[0], b.data()[0]);


    }
    {
        Buffer a(160);
        std::memset(a.data(), 0xAA, a.size());

        Buffer b(16);
        std::memset(b.data(), 0xBB, b.size());
        printf("Before:\n%3d %3d\n", a.size(), b.size());
        b = a;

        printf("After:\n%3d %3d\n", a.size(), b.size());

    }
    {
        Buffer src(5);
        src.data()[0] = 42;

        Buffer dst(1);
        dst = std::move(src);  // вызываем move assignment


        // Проверяем:
        assert(dst.size() == 5);           // размер перенёсся
        assert(dst.data()[0] == 42);     // данные сохранились
        assert(src.size() == 0);         // источник опустошён
        assert(src.data() == nullptr);    // данные не дублируются

        std::cout << "Move assignment OK!\n";
    }
    return 0;
} 
