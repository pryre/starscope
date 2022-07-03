#ifndef _STARSCOPE_DRIVERS_MPU6050_H
#define _STARSCOPE_DRIVERS_MPU6050_H

#include <array>
#include <span>
#include <inttypes.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "starscope/drivers/utils.hpp"

namespace Starscope::Drivers::MPU6050 {

//https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
const std::byte DEFAULT_ADDRESS = std::byte{0x68};
const size_t LEN_REG_WRITE = 2;

const std::byte REG_PWR_MGMT_1 = std::byte{0x6B};
const std::byte VAL_PWR_MGMT_1_RUN = std::byte{0x00};

const std::byte REG_CONFIG = std::byte{0x1A};
const std::byte VAL_CONFIG_EXT_SYNC_SET_NONE = std::byte{0x00};
const std::byte VAL_CONFIG_EXT_SYNC_SET_T = std::byte{0x01 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_GX = std::byte{0x02 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_GY = std::byte{0x03 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_GZ = std::byte{0x04 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_AX = std::byte{0x05 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_AY = std::byte{0x06 << 3};
const std::byte VAL_CONFIG_EXT_SYNC_SET_AZ = std::byte{0x07 << 3};
const std::byte VAL_CONFIG_DLPF_CFG_A260HZ_G256HZ = std::byte{0x00};
const std::byte VAL_CONFIG_DLPF_CFG_A184HZ_G188HZ = std::byte{0x01};
const std::byte VAL_CONFIG_DLPF_CFG_A94HZ_G98HZ = std::byte{0x02};
const std::byte VAL_CONFIG_DLPF_CFG_A44HZ_G42HZ = std::byte{0x03};
const std::byte VAL_CONFIG_DLPF_CFG_A21HZ_G20HZ = std::byte{0x04};
const std::byte VAL_CONFIG_DLPF_CFG_A10HZ_G10HZ = std::byte{0x05};
const std::byte VAL_CONFIG_DLPF_CFG_A5HZ_G5HZ = std::byte{0x06};

const std::byte REG_RAW_VALUES = std::byte{0x3B};
const size_t LEN_RAW_VALUES = 14;
const std::byte VAL_INVERT_RESPONSE = std::byte{0x80};

const std::byte REG_GYRO_CONFIG = std::byte{0x1B};
const std::byte VAL_GYRO_CONFIG_RANGE_250_DPS = std::byte{0x00 << 3};
const std::byte VAL_GYRO_CONFIG_RANGE_500_DPS = std::byte{0x01 << 3};
const std::byte VAL_GYRO_CONFIG_RANGE_1000_DPS = std::byte{0x02 << 3};
const std::byte VAL_GYRO_CONFIG_RANGE_2000_DPS = std::byte{0x03 << 3};
const float SCALE_GYRO_CONFIG_RANGE_250_DPS = 131;
const float SCALE_GYRO_CONFIG_RANGE_500_DPS = 65.5;
const float SCALE_GYRO_CONFIG_RANGE_1000_DPS = 32.8;
const float SCALE_GYRO_CONFIG_RANGE_2000_DPS = 16.4;
const std::byte REG_ACCEL_CONFIG = std::byte{0x1C};
const std::byte VAL_ACCEL_CONFIG_RANGE_2_G = std::byte{0x00 << 3};
const std::byte VAL_ACCEL_CONFIG_RANGE_4_G = std::byte{0x01 << 3};
const std::byte VAL_ACCEL_CONFIG_RANGE_8_G = std::byte{0x02 << 3};
const std::byte VAL_ACCEL_CONFIG_RANGE_16_G = std::byte{0x03 << 3};
const unsigned int SCALE_ACCEL_CONFIG_RANGE_2_G = 16384;
const unsigned int SCALE_ACCEL_CONFIG_RANGE_4_G = 8192;
const unsigned int SCALE_ACCEL_CONFIG_RANGE_8_G = 4096;
const unsigned int SCALE_ACCEL_CONFIG_RANGE_16_G = 2048;

constexpr float SCALE_TEMP_MUL = 1.0/340;
const float SCALE_TEMP_OFFSET = 36.53;
constexpr float SCALE_DEG_TO_RAD = M_PI / 180.0;
const float SCALE_G_TO_MS2 = 9.80665;

inline std::byte flip(const std::byte b) {
    const std::byte f = std::byte{0xFF};
    return b & f;
}

typedef enum {
    G_250_DPS,
    G_500_DPS,
    G_1000_DPS,
    G_2000_DPS,
} GYRO_RANGE;

typedef enum {
    A_2G,
    A_4G,
    A_8G,
    A_16G
} ACCEL_RANGE;

struct MPU6060Values {
    Utils::Vector3 acceleration = Utils::Vector3();
    Utils::Vector3 rates = Utils::Vector3();
    float temperature = 0.0;
};

class Driver : public Utils::StatefulSystem {
    private:
    i2c_inst_t* _i2c;
    std::byte _addr;
    float _scale_accel;
    float _scale_gyro;

    public:
    Driver(i2c_inst_t* i2c, std::byte addr);

    void set_scaling(const ACCEL_RANGE accel_scaling = ACCEL_RANGE::A_4G, const GYRO_RANGE gyro_scaling = GYRO_RANGE::G_500_DPS);

    std::array<std::byte, LEN_RAW_VALUES> get_raw_values() const;
    MPU6060Values get_values() const;
    MPU6060Values get_values_from_raw_data(const std::span<const std::byte, LEN_RAW_VALUES> &data) const;

    //Takes a span of data, consumes the next set of bytes, returns the decoded packet
    int16_t data_decoder(std::span<const std::byte> &data) const;

    private:
    bool _init();
    void _deinit();

    void write(const std::byte mem_addr, const std::byte data) const;
    //std::vector<std::byte> read(std::byte mem_addr, size_t count) const;

};

}

#endif
