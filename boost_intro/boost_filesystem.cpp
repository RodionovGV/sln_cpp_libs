/*
Работа с файлами
1. Вернуть список папок.
2. Вернуть список файлов.
3. Вернуть список файлов с заданным расширением.
4. Обойти список файлов и применить к каждому файлу callback функцию.

*/
#include <iostream>
#include <boost/filesystem.hpp>

#include <vector>
#include <string>
#include <functional>

namespace fs = boost::filesystem;

class FileSystemManager {
public:
    // Конструктор с указанием базовой директории
    explicit FileSystemManager(const std::string& path) : rootPath(path) {
        if (!fs::exists(rootPath) || !fs::is_directory(rootPath)) {
            throw std::invalid_argument("Указанный путь не существует или не является директорией");
        }
    }

    // 1. Получить список папок в текущей директории
    std::vector<std::string> getDirectories() const {
        std::vector<std::string> directories;

        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (fs::is_directory(entry.path())) {
                directories.push_back(entry.path().string());
            }
        }

        return directories;
    }

    // 2. Получить список файлов в текущей директории
    std::vector<std::string> getFiles() const {
        std::vector<std::string> files;

        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (fs::is_regular_file(entry.path())) {
                files.push_back(entry.path().string());
            }
        }

        return files;
    }

    // 3. Получить список файлов с заданным расширением
    std::vector<std::string> getFilesByExtension(const std::string& extension) const {
        std::vector<std::string> files;

        // Нормализуем расширение (добавляем точку, если её нет)
        std::string ext = extension;
        if (!ext.empty() && ext[0] != '.') {
            ext = "." + ext;
        }

        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (fs::is_regular_file(entry.path()) &&
                entry.path().extension() == ext) {
                files.push_back(entry.path().string());
            }
        }

        return files;
    }

    // 4. Применить callback-функцию ко всем файлам
    void processFiles(const std::function<void(const std::string&)>& callback) const {
        for (const auto& entry : fs::directory_iterator(rootPath)) {
            if (fs::is_regular_file(entry.path())) {
                callback(entry.path().string());
            }
        }
    }

private:
    fs::path rootPath;  // Базовая директория для работы
};

int fs_main() {
    try {
        FileSystemManager manager("d:/g_rodionov/mega/sw");

        // 1. Список папок
        auto directories = manager.getDirectories();
        std::cout << "Directories:\n";
        for (const auto& dir : directories) {
            std::cout << dir << "\n";
        }

        // 2. Список файлов
        auto files = manager.getFiles();
        std::cout << "\nFiles:\n";
        for (const auto& file : files) {
            std::cout << file << "\n";
        }

        // 3. Файлы с расширением .txt
        auto txtFiles = manager.getFilesByExtension(".txt");
        std::cout << "\n.txt files:\n";
        for (const auto& file : txtFiles) {
            std::cout << file << "\n";
        }

        // 4. Обработка файлов (вывод размера)
        auto printFileSize = [](const std::string& filePath) {
            fs::path p(filePath);
            std::cout << filePath << " size: "
                << fs::file_size(p) << " bytes\n";
            };
        manager.processFiles(printFileSize);

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
