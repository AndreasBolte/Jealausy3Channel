// https://github.com/emelianov/modbus-esp8266/issues/158

uint16_t f_2uint_int1(float float_number)
{ // split the float and return first unsigned integer

    union f_2uint
    {
        float f;
        uint16_t i[2];
    };

    union f_2uint f_number;
    f_number.f = float_number;

    return f_number.i[0];
}

uint16_t f_2uint_int2(float float_number)
{ // split the float and return first unsigned integer

    union f_2uint
    {
        float f;
        uint16_t i[2];
    };

    union f_2uint f_number;
    f_number.f = float_number;

    return f_number.i[1];
}

float f_2uint_float(uint16_t uint1, uint16_t uint2) {    // reconstruct the float from 2 unsigned integers

  union f_2uint {
    float f;
    uint16_t i[2];
  };

  union f_2uint f_number;
  f_number.i[0] = uint1;
  f_number.i[1] = uint2;

  return f_number.f;

}
