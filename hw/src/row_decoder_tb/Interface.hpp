#pragma once

#include <cstddef>
#include <array>
#include <cstdint>
#include <ostream>

template<size_t IN_PARALLEL, size_t OUT_PARALLEL>
struct RowDecoderInterface
{
    static constexpr size_t PARALLELISM = IN_PARALLEL;

    std::array<uint32_t, IN_PARALLEL> r_beg_data{};
    bool r_beg_valid = false;
    bool r_beg_last = false;
    uint64_t r_beg_bytemask = false;

    bool row_ids_ready = false;
    std::array<uint32_t, OUT_PARALLEL> row_ids_data{};
    bool row_ids_valid = false;
    bool row_ids_last = false;
    uint64_t row_ids_bytemask = false;

    bool operator==(const RowDecoderInterface& other) const
    {
        return r_beg_data == other.r_beg_data &&
               r_beg_valid == other.r_beg_valid &&
               r_beg_last == other.r_beg_last &&
               r_beg_bytemask == other.r_beg_bytemask &&
               row_ids_ready == other.row_ids_ready &&
               row_ids_data == other.row_ids_data &&
               row_ids_valid == other.row_ids_valid &&
               row_ids_last == other.row_ids_last &&
               row_ids_bytemask == other.row_ids_bytemask;
    }

    friend std::ostream& operator<<(std::ostream& os, const RowDecoderInterface& iface)
    {
        os << "row_ids_data: [ ";
        for (const auto& d : iface.row_ids_data)
            os << d << " ";
        os << "] "
           << "valid: " << iface.row_ids_valid << ", "
           << "ready: " << iface.row_ids_ready << ", "
           << "last: " << iface.row_ids_last << ", "
           << "bytemask: " << iface.row_ids_bytemask;
        return os;
    }
};


