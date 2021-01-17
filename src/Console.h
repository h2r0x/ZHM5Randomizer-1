#ifndef __ZHM5RANDOMIZER_SRC_CONSOLE_H__
#define __ZHM5RANDOMIZER_SRC_CONSOLE_H__

#include "spdlog/spdlog.h"

namespace hitman_randomizer {

namespace log {

    void spawn();

    extern std::shared_ptr<spdlog::logger> log_;

    template<typename FormatString, typename... Args>
    void info(const FormatString &fmt, Args&&...args)
    {
        log_->info(fmt, std::forward<Args>(args)...);
    }
    template<typename FormatString, typename... Args>
    void warn(const FormatString &fmt, Args&&...args)
    {
        log_->warn(fmt, std::forward<Args>(args)...);
    }
    template<typename FormatString, typename... Args>
    void error(const FormatString &fmt, Args&&...args)
    {
        log_->error(fmt, std::forward<Args>(args)...);
    }

}  // namespace log

}  // namespace hitman_randomizer

#endif  // __ZHM5RANDOMIZER_SRC_CONSOLE_H__