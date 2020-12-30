#pragma once
#include <optional>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <cstdio>

namespace akura {
    struct File {
        int fd = -1;
        std::string_view text {nullptr, 0};
        static std::optional<File> open(std::string_view filename) {
            int fd = ::open(filename.data(), O_RDONLY);
            if (fd == -1) return {};
            struct stat statbuf;
            fstat(fd, &statbuf);
            size_t offset = 0;
            auto ptr = mmap(nullptr, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, offset);
            if (!ptr) {
                close(fd);
                return {};
            }
            return std::optional<File>{
                    std::in_place,
                    fd,
                    std::string_view(static_cast<const char *>(ptr), statbuf.st_size)
                    };
        }
        File() = default;
        File(int fd, std::string_view text) : fd(fd), text(text) {}
        ~File() {
            munmap(
                    reinterpret_cast<void *>(const_cast<char *>(text.data())),
                    text.size()
                    );
            close(fd);
        }
    };
}
