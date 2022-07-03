#include "starscope/drivers/mpu6050.hpp"

namespace Starscope::Drivers::MPU6050
{

Driver::Driver(i2c_inst_t* i2c, std::byte addr) :
    _i2c(i2c),
    _addr(addr),
    _scale_accel(1.0),
    _scale_gyro(1.0) {
}

//Updates the internal data from a data packet
MPU6060Values Driver::get_values_from_raw_data(const std::span<const std::byte, LEN_RAW_VALUES> &data) const {
    std::span<const std::byte> raw = data.subspan(0, data.size());

    MPU6060Values values = MPU6060Values();
    values.acceleration.x = data_decoder(raw) * _scale_accel;
    values.acceleration.y = data_decoder(raw) * _scale_accel;
    values.acceleration.z = data_decoder(raw) * _scale_accel;

    values.temperature = data_decoder(raw) * SCALE_TEMP_MUL + SCALE_TEMP_OFFSET;

    values.rates.x = data_decoder(raw) * _scale_gyro;
    values.rates.y = data_decoder(raw) * _scale_gyro;
    values.rates.z = data_decoder(raw) * _scale_gyro;

    return values;
}

int16_t Driver::data_decoder(std::span<const std::byte> &data) const {
        int16_t value = 0;

        if(data.size() >= 2) {
            //Convert bytes to signed int
            value = ((uint8_t)data[0] << 8) | (uint8_t)data[1];
            //Reduce the span size (consume)
            data = data.subspan(2, data.size() - 2);
        } else {
            // printf("End of buffer reached!");
            data = std::span<const std::byte>{};
        }

        return value;
}

std::array<std::byte, LEN_RAW_VALUES> Driver::get_raw_values() const {
    std::array<std::byte, LEN_RAW_VALUES> data;
    //Write the memory address to start reading data from
    i2c_write_blocking(_i2c, (uint8_t)_addr, (uint8_t*)&REG_RAW_VALUES, 1, true);
    i2c_read_blocking(_i2c, (uint8_t)_addr, (uint8_t*)data.data(), LEN_RAW_VALUES, false);
    // for(const auto d : data)
    //     printf("%x\n", (uint8_t)d);
    return data;
}

MPU6060Values Driver::get_values() const {
    return get_values_from_raw_data(get_raw_values());
}

bool Driver::_init() {
    write(REG_PWR_MGMT_1, VAL_PWR_MGMT_1_RUN);
    write(REG_CONFIG, VAL_CONFIG_EXT_SYNC_SET_NONE | VAL_CONFIG_DLPF_CFG_A10HZ_G10HZ);
    set_scaling(); //Set to default scaling

    return true;
}

void Driver::set_scaling(const ACCEL_RANGE accel_scaling, const GYRO_RANGE gyro_scaling) {
    switch(accel_scaling) {
        case ACCEL_RANGE::A_2G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_2_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_2_G;
            break;
        }
        case ACCEL_RANGE::A_4G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_4_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_4_G;
            break;
        }
        case ACCEL_RANGE::A_8G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_8_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_8_G;
            break;
        }
        case ACCEL_RANGE::A_16G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_16_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_16_G;
            break;
        }
    }

    switch(gyro_scaling) {
        case GYRO_RANGE::G_250_DPS : {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_250_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_250_DPS;
            break;
        }
        case GYRO_RANGE::G_500_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_500_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_500_DPS;
            break;
        }
        case GYRO_RANGE::G_1000_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_1000_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_1000_DPS;
            break;
        }
        case GYRO_RANGE::G_2000_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_2000_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_2000_DPS;
            break;
        }
    }
}

void Driver::_deinit() {
    //TODO: Enter low power mode?
}

void Driver::write(const std::byte mem_addr, const std::byte data) const {
    const std::byte src[LEN_REG_WRITE] = {mem_addr, data};
    i2c_write_blocking(_i2c, (uint8_t)_addr, (uint8_t*)src, LEN_REG_WRITE, false);
}

}
