#include<float.h>
#include<math.h>
#include<stdbool.h>
#include<stddef.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<ap_int.h>
#include<hls_stream.h>

#include<algorithm>


#ifndef BURST_WIDTH
#define BURST_WIDTH 256
#endif//BURST_WIDTH

#ifdef UNROLL_FACTOR
#if UNROLL_FACTOR != 16
#error UNROLL_FACTOR != 16
#endif//UNROLL_FACTOR != 16
#endif//UNROLL_FACTOR
#ifdef TILE_SIZE_DIM_0
#if TILE_SIZE_DIM_0 != 640
#error TILE_SIZE_DIM_0 != 640
#endif//TILE_SIZE_DIM_0 != 640
#endif//TILE_SIZE_DIM_0
#ifdef BURST_WIDTH
#if BURST_WIDTH != 256
#error BURST_WIDTH != 256
#endif//BURST_WIDTH != 256
#endif//BURST_WIDTH

template<typename T> struct Data
{
  T data;
  bool ctrl;
};
template<typename To, typename From>
inline To Reinterpret(const From& val)
{
  return reinterpret_cast<const To&>(val);
}
template<typename T> inline bool ReadData(T* data, hls::stream<Data<T>>* from)
{
#pragma HLS inline
  const Data<T>& tmp = from->read();
  *data = tmp.data;
  return tmp.ctrl;
}
template<typename T> inline void WriteData(hls::stream<Data<T>>* to, const T& data, bool ctrl)
{
#pragma HLS inline
  Data<T> tmp;
  tmp.data = data;
  tmp.ctrl = ctrl;
  to->write(tmp);
}
void BurstRead(hls::stream<Data<ap_uint<BURST_WIDTH>>>* to, ap_uint<BURST_WIDTH>* from, uint64_t data_num)
{
load_epoch:
  for (uint64_t epoch = 0; epoch < data_num;)
  {
#pragma HLS pipeline II=1
    const uint64_t next_epoch = epoch + 1;
    WriteData(to, from[epoch], next_epoch < data_num);
    epoch = next_epoch;
  }
}
void BurstWrite(ap_uint<BURST_WIDTH>* to, hls::stream<Data<ap_uint<BURST_WIDTH>>>* from, uint64_t data_num)
{
store_epoch:
  for (uint64_t epoch = 0; epoch < data_num; ++epoch)
  {
#pragma HLS pipeline II=1
    ap_uint<BURST_WIDTH> buf;
    ReadData(&buf, from);
    to[epoch] = buf;
  }
}
void Module0Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_1, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_2, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_3, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_4, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_5, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_6, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_7, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_8, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_9, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_10, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_11, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_12, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_13, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_14, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_15, 
  /* input*/ hls::stream<Data<ap_uint<256>>>* dram_input_bank_0_fifo)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_st_2
#pragma HLS data_pack variable = fifo_st_3
#pragma HLS data_pack variable = fifo_st_4
#pragma HLS data_pack variable = fifo_st_5
#pragma HLS data_pack variable = fifo_st_6
#pragma HLS data_pack variable = fifo_st_7
#pragma HLS data_pack variable = fifo_st_8
#pragma HLS data_pack variable = fifo_st_9
#pragma HLS data_pack variable = fifo_st_10
#pragma HLS data_pack variable = fifo_st_11
#pragma HLS data_pack variable = fifo_st_12
#pragma HLS data_pack variable = fifo_st_13
#pragma HLS data_pack variable = fifo_st_14
#pragma HLS data_pack variable = fifo_st_15
#pragma HLS data_pack variable = dram_input_bank_0_fifo
module_0_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!dram_input_bank_0_fifo->empty())
    {
      ap_uint<256> dram_input_bank_0_buf;
      const bool dram_input_bank_0_buf_enable = ReadData(&dram_input_bank_0_buf, dram_input_bank_0_fifo);
      const bool enabled = dram_input_bank_0_buf_enable;
      enable = enabled;
      WriteData(fifo_st_0, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(255, 240))), enabled);
      WriteData(fifo_st_1, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(239, 224))), enabled);
      WriteData(fifo_st_2, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(223, 208))), enabled);
      WriteData(fifo_st_3, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(207, 192))), enabled);
      WriteData(fifo_st_4, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(191, 176))), enabled);
      WriteData(fifo_st_5, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(175, 160))), enabled);
      WriteData(fifo_st_6, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(159, 144))), enabled);
      WriteData(fifo_st_7, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(143, 128))), enabled);
      WriteData(fifo_st_8, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(127, 112))), enabled);
      WriteData(fifo_st_9, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(111, 96))), enabled);
      WriteData(fifo_st_10, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(95, 80))), enabled);
      WriteData(fifo_st_11, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(79, 64))), enabled);
      WriteData(fifo_st_12, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(63, 48))), enabled);
      WriteData(fifo_st_13, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(47, 32))), enabled);
      WriteData(fifo_st_14, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(31, 16))), enabled);
      WriteData(fifo_st_15, Reinterpret<uint16_t>(static_cast<ap_uint<16>>(dram_input_bank_0_buf(15, 0))), enabled);
    } // if not empty
  } // for module_0_epoch
} // Module0Func
void Module1Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_1, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
module_1_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, uint16_t(fifo_ref_0), enabled);
      WriteData(fifo_st_1, uint16_t(fifo_ref_0), enabled);
    } // if not empty
  } // for module_1_epoch
} // Module1Func
void Module2Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_1, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_2, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_st_2
#pragma HLS data_pack variable = fifo_ld_0
module_2_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, uint16_t(fifo_ref_0), enabled);
      WriteData(fifo_st_1, uint16_t(fifo_ref_0), enabled);
      WriteData(fifo_st_2, uint16_t(fifo_ref_0), enabled);
    } // if not empty
  } // for module_2_epoch
} // Module2Func
void Module3Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_1, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
  uint16_t fifo_ref_0_delayed_1_buf[1];
  ap_uint<1> fifo_ref_0_delayed_1_ptr = 0;
module_3_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_1_buf inter false
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const uint16_t fifo_ref_0_delayed_1 = fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr];;
      const uint16_t let_0 = fifo_ref_0_delayed_1;
      WriteData(fifo_st_0, uint16_t(let_0), enabled);
      WriteData(fifo_st_1, uint16_t(let_0), enabled);
      fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_1_ptr = fifo_ref_0_delayed_1_ptr < 0 ? ap_uint<1>(fifo_ref_0_delayed_1_ptr+1) : ap_uint<1>(0);
    } // if not empty
  } // for module_3_epoch
} // Module3Func
void Module4Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_1, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_2)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
#pragma HLS data_pack variable = fifo_ld_1
#pragma HLS data_pack variable = fifo_ld_2
module_4_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty() && !fifo_ld_1->empty() && !fifo_ld_2->empty())
    {
      uint16_t fifo_ref_0;
      uint16_t fifo_ref_1;
      uint16_t fifo_ref_2;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool fifo_ref_1_enable = ReadData(&fifo_ref_1, fifo_ld_1);
      const bool fifo_ref_2_enable = ReadData(&fifo_ref_2, fifo_ld_2);
      const bool enabled = fifo_ref_0_enable && fifo_ref_1_enable && fifo_ref_2_enable;
      enable = enabled;
      WriteData(fifo_st_0, uint16_t(static_cast<uint16_t >((static_cast<ap_uint<18> >(fifo_ref_0) + static_cast<ap_uint<18> >(static_cast<ap_uint<17> >(fifo_ref_1) + static_cast<ap_uint<17> >(fifo_ref_2))) / 3)), enabled);
    } // if not empty
  } // for module_4_epoch
} // Module4Func
void Module5Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
  uint16_t fifo_ref_0_delayed_1_buf[1];
  ap_uint<1> fifo_ref_0_delayed_1_ptr = 0;
module_5_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_1_buf inter false
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const uint16_t fifo_ref_0_delayed_1 = fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr];;
      const uint16_t let_0 = fifo_ref_0_delayed_1;
      WriteData(fifo_st_0, uint16_t(let_0), enabled);
      fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_1_ptr = fifo_ref_0_delayed_1_ptr < 0 ? ap_uint<1>(fifo_ref_0_delayed_1_ptr+1) : ap_uint<1>(0);
    } // if not empty
  } // for module_5_epoch
} // Module5Func
void Module6Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_1, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
  uint16_t fifo_ref_0_delayed_40_buf[40];
  ap_uint<6> fifo_ref_0_delayed_40_ptr = 0;
module_6_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_40_buf inter false
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const uint16_t fifo_ref_0_delayed_40 = fifo_ref_0_delayed_40_buf[fifo_ref_0_delayed_40_ptr];;
      const uint16_t let_0 = fifo_ref_0_delayed_40;
      WriteData(fifo_st_0, uint16_t(let_0), enabled);
      WriteData(fifo_st_1, uint16_t(let_0), enabled);
      fifo_ref_0_delayed_40_buf[fifo_ref_0_delayed_40_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_40_ptr = fifo_ref_0_delayed_40_ptr < 39 ? ap_uint<6>(fifo_ref_0_delayed_40_ptr+1) : ap_uint<6>(0);
    } // if not empty
  } // for module_6_epoch
} // Module6Func
void Module7Func(
  /*output*/ hls::stream<Data<uint16_t>>* fifo_st_0, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
  uint16_t fifo_ref_0_delayed_40_buf[40];
  ap_uint<6> fifo_ref_0_delayed_40_ptr = 0;
module_7_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_40_buf inter false
    if (!fifo_ld_0->empty())
    {
      uint16_t fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const uint16_t fifo_ref_0_delayed_40 = fifo_ref_0_delayed_40_buf[fifo_ref_0_delayed_40_ptr];;
      const uint16_t let_0 = fifo_ref_0_delayed_40;
      WriteData(fifo_st_0, uint16_t(let_0), enabled);
      fifo_ref_0_delayed_40_buf[fifo_ref_0_delayed_40_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_40_ptr = fifo_ref_0_delayed_40_ptr < 39 ? ap_uint<6>(fifo_ref_0_delayed_40_ptr+1) : ap_uint<6>(0);
    } // if not empty
  } // for module_7_epoch
} // Module7Func
void Module8Func(
  /*output*/ hls::stream<Data<ap_uint<256>>>* dram_blur_y_0_bank_0_fifo, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_0, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_1, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_2, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_3, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_4, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_5, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_6, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_7, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_8, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_9, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_10, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_11, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_12, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_13, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_14, 
  /* input*/ hls::stream<Data<uint16_t>>* fifo_ld_15)
{
#pragma HLS data_pack variable = dram_blur_y_0_bank_0_fifo
#pragma HLS data_pack variable = fifo_ld_0
#pragma HLS data_pack variable = fifo_ld_1
#pragma HLS data_pack variable = fifo_ld_2
#pragma HLS data_pack variable = fifo_ld_3
#pragma HLS data_pack variable = fifo_ld_4
#pragma HLS data_pack variable = fifo_ld_5
#pragma HLS data_pack variable = fifo_ld_6
#pragma HLS data_pack variable = fifo_ld_7
#pragma HLS data_pack variable = fifo_ld_8
#pragma HLS data_pack variable = fifo_ld_9
#pragma HLS data_pack variable = fifo_ld_10
#pragma HLS data_pack variable = fifo_ld_11
#pragma HLS data_pack variable = fifo_ld_12
#pragma HLS data_pack variable = fifo_ld_13
#pragma HLS data_pack variable = fifo_ld_14
#pragma HLS data_pack variable = fifo_ld_15
module_8_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty() && !fifo_ld_1->empty() && !fifo_ld_2->empty() && !fifo_ld_3->empty() && !fifo_ld_4->empty() && !fifo_ld_5->empty() && !fifo_ld_6->empty() && !fifo_ld_7->empty() && !fifo_ld_8->empty() && !fifo_ld_9->empty() && !fifo_ld_10->empty() && !fifo_ld_11->empty() && !fifo_ld_12->empty() && !fifo_ld_13->empty() && !fifo_ld_14->empty() && !fifo_ld_15->empty())
    {
      uint16_t fifo_ref_0;
      uint16_t fifo_ref_1;
      uint16_t fifo_ref_2;
      uint16_t fifo_ref_3;
      uint16_t fifo_ref_4;
      uint16_t fifo_ref_5;
      uint16_t fifo_ref_6;
      uint16_t fifo_ref_7;
      uint16_t fifo_ref_8;
      uint16_t fifo_ref_9;
      uint16_t fifo_ref_10;
      uint16_t fifo_ref_11;
      uint16_t fifo_ref_12;
      uint16_t fifo_ref_13;
      uint16_t fifo_ref_14;
      uint16_t fifo_ref_15;
      ap_uint<256> dram_blur_y_0_bank_0_buf;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool fifo_ref_1_enable = ReadData(&fifo_ref_1, fifo_ld_1);
      const bool fifo_ref_2_enable = ReadData(&fifo_ref_2, fifo_ld_2);
      const bool fifo_ref_3_enable = ReadData(&fifo_ref_3, fifo_ld_3);
      const bool fifo_ref_4_enable = ReadData(&fifo_ref_4, fifo_ld_4);
      const bool fifo_ref_5_enable = ReadData(&fifo_ref_5, fifo_ld_5);
      const bool fifo_ref_6_enable = ReadData(&fifo_ref_6, fifo_ld_6);
      const bool fifo_ref_7_enable = ReadData(&fifo_ref_7, fifo_ld_7);
      const bool fifo_ref_8_enable = ReadData(&fifo_ref_8, fifo_ld_8);
      const bool fifo_ref_9_enable = ReadData(&fifo_ref_9, fifo_ld_9);
      const bool fifo_ref_10_enable = ReadData(&fifo_ref_10, fifo_ld_10);
      const bool fifo_ref_11_enable = ReadData(&fifo_ref_11, fifo_ld_11);
      const bool fifo_ref_12_enable = ReadData(&fifo_ref_12, fifo_ld_12);
      const bool fifo_ref_13_enable = ReadData(&fifo_ref_13, fifo_ld_13);
      const bool fifo_ref_14_enable = ReadData(&fifo_ref_14, fifo_ld_14);
      const bool fifo_ref_15_enable = ReadData(&fifo_ref_15, fifo_ld_15);
      const bool enabled = fifo_ref_0_enable && fifo_ref_1_enable && fifo_ref_2_enable && fifo_ref_3_enable && fifo_ref_4_enable && fifo_ref_5_enable && fifo_ref_6_enable && fifo_ref_7_enable && fifo_ref_8_enable && fifo_ref_9_enable && fifo_ref_10_enable && fifo_ref_11_enable && fifo_ref_12_enable && fifo_ref_13_enable && fifo_ref_14_enable && fifo_ref_15_enable;
      enable = enabled;
      dram_blur_y_0_bank_0_buf(255, 240) = Reinterpret<ap_uint<16>>(fifo_ref_0);
      dram_blur_y_0_bank_0_buf(239, 224) = Reinterpret<ap_uint<16>>(fifo_ref_1);
      dram_blur_y_0_bank_0_buf(223, 208) = Reinterpret<ap_uint<16>>(fifo_ref_2);
      dram_blur_y_0_bank_0_buf(207, 192) = Reinterpret<ap_uint<16>>(fifo_ref_3);
      dram_blur_y_0_bank_0_buf(191, 176) = Reinterpret<ap_uint<16>>(fifo_ref_4);
      dram_blur_y_0_bank_0_buf(175, 160) = Reinterpret<ap_uint<16>>(fifo_ref_5);
      dram_blur_y_0_bank_0_buf(159, 144) = Reinterpret<ap_uint<16>>(fifo_ref_6);
      dram_blur_y_0_bank_0_buf(143, 128) = Reinterpret<ap_uint<16>>(fifo_ref_7);
      dram_blur_y_0_bank_0_buf(127, 112) = Reinterpret<ap_uint<16>>(fifo_ref_8);
      dram_blur_y_0_bank_0_buf(111, 96) = Reinterpret<ap_uint<16>>(fifo_ref_9);
      dram_blur_y_0_bank_0_buf(95, 80) = Reinterpret<ap_uint<16>>(fifo_ref_10);
      dram_blur_y_0_bank_0_buf(79, 64) = Reinterpret<ap_uint<16>>(fifo_ref_11);
      dram_blur_y_0_bank_0_buf(63, 48) = Reinterpret<ap_uint<16>>(fifo_ref_12);
      dram_blur_y_0_bank_0_buf(47, 32) = Reinterpret<ap_uint<16>>(fifo_ref_13);
      dram_blur_y_0_bank_0_buf(31, 16) = Reinterpret<ap_uint<16>>(fifo_ref_14);
      dram_blur_y_0_bank_0_buf(15, 0) = Reinterpret<ap_uint<16>>(fifo_ref_15);
      WriteData(dram_blur_y_0_bank_0_fifo, dram_blur_y_0_bank_0_buf, enabled);
    } // if not empty
  } // for module_8_epoch
} // Module8Func
extern "C"
{

void default_function_kernel(
  ap_uint<256>* bank_0_blur_y_0,
  ap_uint<256>* bank_0_input,
  uint64_t coalesced_data_num)
{
#pragma HLS interface m_axi port=bank_0_blur_y_0 offset=slave depth=65536 bundle=blur_y_0_bank_0
#pragma HLS interface m_axi port=bank_0_input offset=slave depth=65536 bundle=input_bank_0

#pragma HLS interface s_axilite port=bank_0_blur_y_0 bundle=control
#pragma HLS interface s_axilite port=bank_0_input bundle=control
#pragma HLS interface s_axilite port=coalesced_data_num bundle=control
#pragma HLS interface s_axilite port=return bundle=control

  hls::stream<Data<ap_uint<256>>> bank_0_input_buf("bank_0_input_buf");
#pragma HLS stream variable=bank_0_input_buf depth=32
#pragma HLS data_pack variable=bank_0_input_buf
  hls::stream<Data<ap_uint<256>>> bank_0_blur_y_0_buf("bank_0_blur_y_0_buf");
#pragma HLS stream variable=bank_0_blur_y_0_buf depth=32
#pragma HLS data_pack variable=bank_0_blur_y_0_buf

  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_0("from_super_source_to_input_offset_0");
#pragma HLS stream variable=from_super_source_to_input_offset_0 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_0
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_1("from_super_source_to_input_offset_1");
#pragma HLS stream variable=from_super_source_to_input_offset_1 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_1
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_2("from_super_source_to_input_offset_2");
#pragma HLS stream variable=from_super_source_to_input_offset_2 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_2
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_3("from_super_source_to_input_offset_3");
#pragma HLS stream variable=from_super_source_to_input_offset_3 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_3
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_4("from_super_source_to_input_offset_4");
#pragma HLS stream variable=from_super_source_to_input_offset_4 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_4
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_5("from_super_source_to_input_offset_5");
#pragma HLS stream variable=from_super_source_to_input_offset_5 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_5
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_6("from_super_source_to_input_offset_6");
#pragma HLS stream variable=from_super_source_to_input_offset_6 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_6
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_7("from_super_source_to_input_offset_7");
#pragma HLS stream variable=from_super_source_to_input_offset_7 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_7
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_8("from_super_source_to_input_offset_8");
#pragma HLS stream variable=from_super_source_to_input_offset_8 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_8
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_9("from_super_source_to_input_offset_9");
#pragma HLS stream variable=from_super_source_to_input_offset_9 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_9
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_10("from_super_source_to_input_offset_10");
#pragma HLS stream variable=from_super_source_to_input_offset_10 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_10
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_11("from_super_source_to_input_offset_11");
#pragma HLS stream variable=from_super_source_to_input_offset_11 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_11
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_12("from_super_source_to_input_offset_12");
#pragma HLS stream variable=from_super_source_to_input_offset_12 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_12
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_13("from_super_source_to_input_offset_13");
#pragma HLS stream variable=from_super_source_to_input_offset_13 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_13
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_14("from_super_source_to_input_offset_14");
#pragma HLS stream variable=from_super_source_to_input_offset_14 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_14
  hls::stream<Data<uint16_t>> from_super_source_to_input_offset_15("from_super_source_to_input_offset_15");
#pragma HLS stream variable=from_super_source_to_input_offset_15 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_15
  hls::stream<Data<uint16_t>> from_input_offset_0_to_input_offset_16("from_input_offset_0_to_input_offset_16");
#pragma HLS stream variable=from_input_offset_0_to_input_offset_16 depth=2
#pragma HLS data_pack variable=from_input_offset_0_to_input_offset_16
  hls::stream<Data<uint16_t>> from_input_offset_0_to_blur_x_0_pe_15("from_input_offset_0_to_blur_x_0_pe_15");
#pragma HLS stream variable=from_input_offset_0_to_blur_x_0_pe_15 depth=2
#pragma HLS data_pack variable=from_input_offset_0_to_blur_x_0_pe_15
  hls::stream<Data<uint16_t>> from_input_offset_1_to_input_offset_17("from_input_offset_1_to_input_offset_17");
#pragma HLS stream variable=from_input_offset_1_to_input_offset_17 depth=2
#pragma HLS data_pack variable=from_input_offset_1_to_input_offset_17
  hls::stream<Data<uint16_t>> from_input_offset_1_to_blur_x_0_pe_15("from_input_offset_1_to_blur_x_0_pe_15");
#pragma HLS stream variable=from_input_offset_1_to_blur_x_0_pe_15 depth=2
#pragma HLS data_pack variable=from_input_offset_1_to_blur_x_0_pe_15
  hls::stream<Data<uint16_t>> from_input_offset_1_to_blur_x_0_pe_14("from_input_offset_1_to_blur_x_0_pe_14");
#pragma HLS stream variable=from_input_offset_1_to_blur_x_0_pe_14 depth=2
#pragma HLS data_pack variable=from_input_offset_1_to_blur_x_0_pe_14
  hls::stream<Data<uint16_t>> from_input_offset_2_to_blur_x_0_pe_15("from_input_offset_2_to_blur_x_0_pe_15");
#pragma HLS stream variable=from_input_offset_2_to_blur_x_0_pe_15 depth=2
#pragma HLS data_pack variable=from_input_offset_2_to_blur_x_0_pe_15
  hls::stream<Data<uint16_t>> from_input_offset_2_to_blur_x_0_pe_14("from_input_offset_2_to_blur_x_0_pe_14");
#pragma HLS stream variable=from_input_offset_2_to_blur_x_0_pe_14 depth=2
#pragma HLS data_pack variable=from_input_offset_2_to_blur_x_0_pe_14
  hls::stream<Data<uint16_t>> from_input_offset_2_to_blur_x_0_pe_13("from_input_offset_2_to_blur_x_0_pe_13");
#pragma HLS stream variable=from_input_offset_2_to_blur_x_0_pe_13 depth=2
#pragma HLS data_pack variable=from_input_offset_2_to_blur_x_0_pe_13
  hls::stream<Data<uint16_t>> from_input_offset_3_to_blur_x_0_pe_14("from_input_offset_3_to_blur_x_0_pe_14");
#pragma HLS stream variable=from_input_offset_3_to_blur_x_0_pe_14 depth=2
#pragma HLS data_pack variable=from_input_offset_3_to_blur_x_0_pe_14
  hls::stream<Data<uint16_t>> from_input_offset_3_to_blur_x_0_pe_13("from_input_offset_3_to_blur_x_0_pe_13");
#pragma HLS stream variable=from_input_offset_3_to_blur_x_0_pe_13 depth=2
#pragma HLS data_pack variable=from_input_offset_3_to_blur_x_0_pe_13
  hls::stream<Data<uint16_t>> from_input_offset_3_to_blur_x_0_pe_12("from_input_offset_3_to_blur_x_0_pe_12");
#pragma HLS stream variable=from_input_offset_3_to_blur_x_0_pe_12 depth=2
#pragma HLS data_pack variable=from_input_offset_3_to_blur_x_0_pe_12
  hls::stream<Data<uint16_t>> from_input_offset_4_to_blur_x_0_pe_13("from_input_offset_4_to_blur_x_0_pe_13");
#pragma HLS stream variable=from_input_offset_4_to_blur_x_0_pe_13 depth=2
#pragma HLS data_pack variable=from_input_offset_4_to_blur_x_0_pe_13
  hls::stream<Data<uint16_t>> from_input_offset_4_to_blur_x_0_pe_12("from_input_offset_4_to_blur_x_0_pe_12");
#pragma HLS stream variable=from_input_offset_4_to_blur_x_0_pe_12 depth=2
#pragma HLS data_pack variable=from_input_offset_4_to_blur_x_0_pe_12
  hls::stream<Data<uint16_t>> from_input_offset_4_to_blur_x_0_pe_11("from_input_offset_4_to_blur_x_0_pe_11");
#pragma HLS stream variable=from_input_offset_4_to_blur_x_0_pe_11 depth=2
#pragma HLS data_pack variable=from_input_offset_4_to_blur_x_0_pe_11
  hls::stream<Data<uint16_t>> from_input_offset_5_to_blur_x_0_pe_12("from_input_offset_5_to_blur_x_0_pe_12");
#pragma HLS stream variable=from_input_offset_5_to_blur_x_0_pe_12 depth=2
#pragma HLS data_pack variable=from_input_offset_5_to_blur_x_0_pe_12
  hls::stream<Data<uint16_t>> from_input_offset_5_to_blur_x_0_pe_11("from_input_offset_5_to_blur_x_0_pe_11");
#pragma HLS stream variable=from_input_offset_5_to_blur_x_0_pe_11 depth=2
#pragma HLS data_pack variable=from_input_offset_5_to_blur_x_0_pe_11
  hls::stream<Data<uint16_t>> from_input_offset_5_to_blur_x_0_pe_10("from_input_offset_5_to_blur_x_0_pe_10");
#pragma HLS stream variable=from_input_offset_5_to_blur_x_0_pe_10 depth=2
#pragma HLS data_pack variable=from_input_offset_5_to_blur_x_0_pe_10
  hls::stream<Data<uint16_t>> from_input_offset_6_to_blur_x_0_pe_11("from_input_offset_6_to_blur_x_0_pe_11");
#pragma HLS stream variable=from_input_offset_6_to_blur_x_0_pe_11 depth=2
#pragma HLS data_pack variable=from_input_offset_6_to_blur_x_0_pe_11
  hls::stream<Data<uint16_t>> from_input_offset_6_to_blur_x_0_pe_10("from_input_offset_6_to_blur_x_0_pe_10");
#pragma HLS stream variable=from_input_offset_6_to_blur_x_0_pe_10 depth=2
#pragma HLS data_pack variable=from_input_offset_6_to_blur_x_0_pe_10
  hls::stream<Data<uint16_t>> from_input_offset_6_to_blur_x_0_pe_9("from_input_offset_6_to_blur_x_0_pe_9");
#pragma HLS stream variable=from_input_offset_6_to_blur_x_0_pe_9 depth=2
#pragma HLS data_pack variable=from_input_offset_6_to_blur_x_0_pe_9
  hls::stream<Data<uint16_t>> from_input_offset_7_to_blur_x_0_pe_10("from_input_offset_7_to_blur_x_0_pe_10");
#pragma HLS stream variable=from_input_offset_7_to_blur_x_0_pe_10 depth=2
#pragma HLS data_pack variable=from_input_offset_7_to_blur_x_0_pe_10
  hls::stream<Data<uint16_t>> from_input_offset_7_to_blur_x_0_pe_9("from_input_offset_7_to_blur_x_0_pe_9");
#pragma HLS stream variable=from_input_offset_7_to_blur_x_0_pe_9 depth=2
#pragma HLS data_pack variable=from_input_offset_7_to_blur_x_0_pe_9
  hls::stream<Data<uint16_t>> from_input_offset_7_to_blur_x_0_pe_8("from_input_offset_7_to_blur_x_0_pe_8");
#pragma HLS stream variable=from_input_offset_7_to_blur_x_0_pe_8 depth=2
#pragma HLS data_pack variable=from_input_offset_7_to_blur_x_0_pe_8
  hls::stream<Data<uint16_t>> from_input_offset_8_to_blur_x_0_pe_9("from_input_offset_8_to_blur_x_0_pe_9");
#pragma HLS stream variable=from_input_offset_8_to_blur_x_0_pe_9 depth=2
#pragma HLS data_pack variable=from_input_offset_8_to_blur_x_0_pe_9
  hls::stream<Data<uint16_t>> from_input_offset_8_to_blur_x_0_pe_8("from_input_offset_8_to_blur_x_0_pe_8");
#pragma HLS stream variable=from_input_offset_8_to_blur_x_0_pe_8 depth=2
#pragma HLS data_pack variable=from_input_offset_8_to_blur_x_0_pe_8
  hls::stream<Data<uint16_t>> from_input_offset_8_to_blur_x_0_pe_7("from_input_offset_8_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_input_offset_8_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_input_offset_8_to_blur_x_0_pe_7
  hls::stream<Data<uint16_t>> from_input_offset_9_to_blur_x_0_pe_8("from_input_offset_9_to_blur_x_0_pe_8");
#pragma HLS stream variable=from_input_offset_9_to_blur_x_0_pe_8 depth=2
#pragma HLS data_pack variable=from_input_offset_9_to_blur_x_0_pe_8
  hls::stream<Data<uint16_t>> from_input_offset_9_to_blur_x_0_pe_7("from_input_offset_9_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_input_offset_9_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_input_offset_9_to_blur_x_0_pe_7
  hls::stream<Data<uint16_t>> from_input_offset_9_to_blur_x_0_pe_6("from_input_offset_9_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_input_offset_9_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_input_offset_9_to_blur_x_0_pe_6
  hls::stream<Data<uint16_t>> from_input_offset_10_to_blur_x_0_pe_7("from_input_offset_10_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_input_offset_10_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_input_offset_10_to_blur_x_0_pe_7
  hls::stream<Data<uint16_t>> from_input_offset_10_to_blur_x_0_pe_6("from_input_offset_10_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_input_offset_10_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_input_offset_10_to_blur_x_0_pe_6
  hls::stream<Data<uint16_t>> from_input_offset_10_to_blur_x_0_pe_5("from_input_offset_10_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_input_offset_10_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_input_offset_10_to_blur_x_0_pe_5
  hls::stream<Data<uint16_t>> from_input_offset_11_to_blur_x_0_pe_6("from_input_offset_11_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_input_offset_11_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_input_offset_11_to_blur_x_0_pe_6
  hls::stream<Data<uint16_t>> from_input_offset_11_to_blur_x_0_pe_5("from_input_offset_11_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_input_offset_11_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_input_offset_11_to_blur_x_0_pe_5
  hls::stream<Data<uint16_t>> from_input_offset_11_to_blur_x_0_pe_4("from_input_offset_11_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_input_offset_11_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_input_offset_11_to_blur_x_0_pe_4
  hls::stream<Data<uint16_t>> from_input_offset_12_to_blur_x_0_pe_5("from_input_offset_12_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_input_offset_12_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_input_offset_12_to_blur_x_0_pe_5
  hls::stream<Data<uint16_t>> from_input_offset_12_to_blur_x_0_pe_4("from_input_offset_12_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_input_offset_12_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_input_offset_12_to_blur_x_0_pe_4
  hls::stream<Data<uint16_t>> from_input_offset_12_to_blur_x_0_pe_3("from_input_offset_12_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_input_offset_12_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_input_offset_12_to_blur_x_0_pe_3
  hls::stream<Data<uint16_t>> from_input_offset_13_to_blur_x_0_pe_4("from_input_offset_13_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_input_offset_13_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_input_offset_13_to_blur_x_0_pe_4
  hls::stream<Data<uint16_t>> from_input_offset_13_to_blur_x_0_pe_3("from_input_offset_13_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_input_offset_13_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_input_offset_13_to_blur_x_0_pe_3
  hls::stream<Data<uint16_t>> from_input_offset_13_to_blur_x_0_pe_2("from_input_offset_13_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_input_offset_13_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_input_offset_13_to_blur_x_0_pe_2
  hls::stream<Data<uint16_t>> from_input_offset_14_to_blur_x_0_pe_3("from_input_offset_14_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_input_offset_14_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_input_offset_14_to_blur_x_0_pe_3
  hls::stream<Data<uint16_t>> from_input_offset_14_to_blur_x_0_pe_2("from_input_offset_14_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_input_offset_14_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_input_offset_14_to_blur_x_0_pe_2
  hls::stream<Data<uint16_t>> from_input_offset_14_to_blur_x_0_pe_1("from_input_offset_14_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_input_offset_14_to_blur_x_0_pe_1 depth=4
#pragma HLS data_pack variable=from_input_offset_14_to_blur_x_0_pe_1
  hls::stream<Data<uint16_t>> from_input_offset_15_to_blur_x_0_pe_2("from_input_offset_15_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_input_offset_15_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_input_offset_15_to_blur_x_0_pe_2
  hls::stream<Data<uint16_t>> from_input_offset_15_to_blur_x_0_pe_1("from_input_offset_15_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_input_offset_15_to_blur_x_0_pe_1 depth=4
#pragma HLS data_pack variable=from_input_offset_15_to_blur_x_0_pe_1
  hls::stream<Data<uint16_t>> from_input_offset_15_to_blur_x_0_pe_0("from_input_offset_15_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_input_offset_15_to_blur_x_0_pe_0 depth=4
#pragma HLS data_pack variable=from_input_offset_15_to_blur_x_0_pe_0
  hls::stream<Data<uint16_t>> from_input_offset_16_to_blur_x_0_pe_1("from_input_offset_16_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_input_offset_16_to_blur_x_0_pe_1 depth=2
#pragma HLS data_pack variable=from_input_offset_16_to_blur_x_0_pe_1
  hls::stream<Data<uint16_t>> from_input_offset_16_to_blur_x_0_pe_0("from_input_offset_16_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_input_offset_16_to_blur_x_0_pe_0 depth=2
#pragma HLS data_pack variable=from_input_offset_16_to_blur_x_0_pe_0
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_15_to_blur_x_0_offset_0("from_blur_x_0_pe_15_to_blur_x_0_offset_0");
#pragma HLS stream variable=from_blur_x_0_pe_15_to_blur_x_0_offset_0 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_15_to_blur_x_0_offset_0
  hls::stream<Data<uint16_t>> from_input_offset_17_to_blur_x_0_pe_0("from_input_offset_17_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_input_offset_17_to_blur_x_0_pe_0 depth=2
#pragma HLS data_pack variable=from_input_offset_17_to_blur_x_0_pe_0
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_14_to_blur_x_0_offset_1("from_blur_x_0_pe_14_to_blur_x_0_offset_1");
#pragma HLS stream variable=from_blur_x_0_pe_14_to_blur_x_0_offset_1 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_14_to_blur_x_0_offset_1
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_13_to_blur_x_0_offset_2("from_blur_x_0_pe_13_to_blur_x_0_offset_2");
#pragma HLS stream variable=from_blur_x_0_pe_13_to_blur_x_0_offset_2 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_13_to_blur_x_0_offset_2
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_12_to_blur_x_0_offset_3("from_blur_x_0_pe_12_to_blur_x_0_offset_3");
#pragma HLS stream variable=from_blur_x_0_pe_12_to_blur_x_0_offset_3 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_12_to_blur_x_0_offset_3
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_11_to_blur_x_0_offset_4("from_blur_x_0_pe_11_to_blur_x_0_offset_4");
#pragma HLS stream variable=from_blur_x_0_pe_11_to_blur_x_0_offset_4 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_11_to_blur_x_0_offset_4
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_10_to_blur_x_0_offset_5("from_blur_x_0_pe_10_to_blur_x_0_offset_5");
#pragma HLS stream variable=from_blur_x_0_pe_10_to_blur_x_0_offset_5 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_10_to_blur_x_0_offset_5
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_9_to_blur_x_0_offset_6("from_blur_x_0_pe_9_to_blur_x_0_offset_6");
#pragma HLS stream variable=from_blur_x_0_pe_9_to_blur_x_0_offset_6 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_9_to_blur_x_0_offset_6
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_8_to_blur_x_0_offset_7("from_blur_x_0_pe_8_to_blur_x_0_offset_7");
#pragma HLS stream variable=from_blur_x_0_pe_8_to_blur_x_0_offset_7 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_8_to_blur_x_0_offset_7
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_7_to_blur_x_0_offset_8("from_blur_x_0_pe_7_to_blur_x_0_offset_8");
#pragma HLS stream variable=from_blur_x_0_pe_7_to_blur_x_0_offset_8 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_7_to_blur_x_0_offset_8
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_6_to_blur_x_0_offset_9("from_blur_x_0_pe_6_to_blur_x_0_offset_9");
#pragma HLS stream variable=from_blur_x_0_pe_6_to_blur_x_0_offset_9 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_6_to_blur_x_0_offset_9
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_5_to_blur_x_0_offset_10("from_blur_x_0_pe_5_to_blur_x_0_offset_10");
#pragma HLS stream variable=from_blur_x_0_pe_5_to_blur_x_0_offset_10 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_5_to_blur_x_0_offset_10
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_4_to_blur_x_0_offset_11("from_blur_x_0_pe_4_to_blur_x_0_offset_11");
#pragma HLS stream variable=from_blur_x_0_pe_4_to_blur_x_0_offset_11 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_4_to_blur_x_0_offset_11
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_3_to_blur_x_0_offset_12("from_blur_x_0_pe_3_to_blur_x_0_offset_12");
#pragma HLS stream variable=from_blur_x_0_pe_3_to_blur_x_0_offset_12 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_3_to_blur_x_0_offset_12
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_2_to_blur_x_0_offset_13("from_blur_x_0_pe_2_to_blur_x_0_offset_13");
#pragma HLS stream variable=from_blur_x_0_pe_2_to_blur_x_0_offset_13 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_2_to_blur_x_0_offset_13
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_1_to_blur_x_0_offset_14("from_blur_x_0_pe_1_to_blur_x_0_offset_14");
#pragma HLS stream variable=from_blur_x_0_pe_1_to_blur_x_0_offset_14 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_1_to_blur_x_0_offset_14
  hls::stream<Data<uint16_t>> from_blur_x_0_pe_0_to_blur_x_0_offset_15("from_blur_x_0_pe_0_to_blur_x_0_offset_15");
#pragma HLS stream variable=from_blur_x_0_pe_0_to_blur_x_0_offset_15 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_0_to_blur_x_0_offset_15
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_0_to_blur_x_0_offset_640("from_blur_x_0_offset_0_to_blur_x_0_offset_640");
#pragma HLS stream variable=from_blur_x_0_offset_0_to_blur_x_0_offset_640 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_0_to_blur_x_0_offset_640
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_0_to_blur_y_0_pe_15("from_blur_x_0_offset_0_to_blur_y_0_pe_15");
#pragma HLS stream variable=from_blur_x_0_offset_0_to_blur_y_0_pe_15 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_0_to_blur_y_0_pe_15
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1_to_blur_x_0_offset_641("from_blur_x_0_offset_1_to_blur_x_0_offset_641");
#pragma HLS stream variable=from_blur_x_0_offset_1_to_blur_x_0_offset_641 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1_to_blur_x_0_offset_641
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1_to_blur_y_0_pe_14("from_blur_x_0_offset_1_to_blur_y_0_pe_14");
#pragma HLS stream variable=from_blur_x_0_offset_1_to_blur_y_0_pe_14 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_1_to_blur_y_0_pe_14
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_2_to_blur_x_0_offset_642("from_blur_x_0_offset_2_to_blur_x_0_offset_642");
#pragma HLS stream variable=from_blur_x_0_offset_2_to_blur_x_0_offset_642 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_2_to_blur_x_0_offset_642
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_2_to_blur_y_0_pe_13("from_blur_x_0_offset_2_to_blur_y_0_pe_13");
#pragma HLS stream variable=from_blur_x_0_offset_2_to_blur_y_0_pe_13 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_2_to_blur_y_0_pe_13
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_3_to_blur_x_0_offset_643("from_blur_x_0_offset_3_to_blur_x_0_offset_643");
#pragma HLS stream variable=from_blur_x_0_offset_3_to_blur_x_0_offset_643 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_3_to_blur_x_0_offset_643
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_3_to_blur_y_0_pe_12("from_blur_x_0_offset_3_to_blur_y_0_pe_12");
#pragma HLS stream variable=from_blur_x_0_offset_3_to_blur_y_0_pe_12 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_3_to_blur_y_0_pe_12
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_4_to_blur_x_0_offset_644("from_blur_x_0_offset_4_to_blur_x_0_offset_644");
#pragma HLS stream variable=from_blur_x_0_offset_4_to_blur_x_0_offset_644 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_4_to_blur_x_0_offset_644
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_4_to_blur_y_0_pe_11("from_blur_x_0_offset_4_to_blur_y_0_pe_11");
#pragma HLS stream variable=from_blur_x_0_offset_4_to_blur_y_0_pe_11 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_4_to_blur_y_0_pe_11
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_5_to_blur_x_0_offset_645("from_blur_x_0_offset_5_to_blur_x_0_offset_645");
#pragma HLS stream variable=from_blur_x_0_offset_5_to_blur_x_0_offset_645 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_5_to_blur_x_0_offset_645
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_5_to_blur_y_0_pe_10("from_blur_x_0_offset_5_to_blur_y_0_pe_10");
#pragma HLS stream variable=from_blur_x_0_offset_5_to_blur_y_0_pe_10 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_5_to_blur_y_0_pe_10
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_6_to_blur_x_0_offset_646("from_blur_x_0_offset_6_to_blur_x_0_offset_646");
#pragma HLS stream variable=from_blur_x_0_offset_6_to_blur_x_0_offset_646 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_6_to_blur_x_0_offset_646
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_6_to_blur_y_0_pe_9("from_blur_x_0_offset_6_to_blur_y_0_pe_9");
#pragma HLS stream variable=from_blur_x_0_offset_6_to_blur_y_0_pe_9 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_6_to_blur_y_0_pe_9
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_7_to_blur_x_0_offset_647("from_blur_x_0_offset_7_to_blur_x_0_offset_647");
#pragma HLS stream variable=from_blur_x_0_offset_7_to_blur_x_0_offset_647 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_7_to_blur_x_0_offset_647
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_7_to_blur_y_0_pe_8("from_blur_x_0_offset_7_to_blur_y_0_pe_8");
#pragma HLS stream variable=from_blur_x_0_offset_7_to_blur_y_0_pe_8 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_7_to_blur_y_0_pe_8
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_8_to_blur_x_0_offset_648("from_blur_x_0_offset_8_to_blur_x_0_offset_648");
#pragma HLS stream variable=from_blur_x_0_offset_8_to_blur_x_0_offset_648 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_8_to_blur_x_0_offset_648
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_8_to_blur_y_0_pe_7("from_blur_x_0_offset_8_to_blur_y_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_8_to_blur_y_0_pe_7 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_8_to_blur_y_0_pe_7
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_9_to_blur_x_0_offset_649("from_blur_x_0_offset_9_to_blur_x_0_offset_649");
#pragma HLS stream variable=from_blur_x_0_offset_9_to_blur_x_0_offset_649 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_9_to_blur_x_0_offset_649
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_9_to_blur_y_0_pe_6("from_blur_x_0_offset_9_to_blur_y_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_9_to_blur_y_0_pe_6 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_9_to_blur_y_0_pe_6
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_10_to_blur_x_0_offset_650("from_blur_x_0_offset_10_to_blur_x_0_offset_650");
#pragma HLS stream variable=from_blur_x_0_offset_10_to_blur_x_0_offset_650 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_10_to_blur_x_0_offset_650
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_10_to_blur_y_0_pe_5("from_blur_x_0_offset_10_to_blur_y_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_10_to_blur_y_0_pe_5 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_10_to_blur_y_0_pe_5
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_11_to_blur_x_0_offset_651("from_blur_x_0_offset_11_to_blur_x_0_offset_651");
#pragma HLS stream variable=from_blur_x_0_offset_11_to_blur_x_0_offset_651 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_11_to_blur_x_0_offset_651
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_11_to_blur_y_0_pe_4("from_blur_x_0_offset_11_to_blur_y_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_11_to_blur_y_0_pe_4 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_11_to_blur_y_0_pe_4
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_12_to_blur_x_0_offset_652("from_blur_x_0_offset_12_to_blur_x_0_offset_652");
#pragma HLS stream variable=from_blur_x_0_offset_12_to_blur_x_0_offset_652 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_12_to_blur_x_0_offset_652
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_12_to_blur_y_0_pe_3("from_blur_x_0_offset_12_to_blur_y_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_12_to_blur_y_0_pe_3 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_12_to_blur_y_0_pe_3
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_13_to_blur_x_0_offset_653("from_blur_x_0_offset_13_to_blur_x_0_offset_653");
#pragma HLS stream variable=from_blur_x_0_offset_13_to_blur_x_0_offset_653 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_13_to_blur_x_0_offset_653
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_13_to_blur_y_0_pe_2("from_blur_x_0_offset_13_to_blur_y_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_13_to_blur_y_0_pe_2 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_13_to_blur_y_0_pe_2
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_14_to_blur_x_0_offset_654("from_blur_x_0_offset_14_to_blur_x_0_offset_654");
#pragma HLS stream variable=from_blur_x_0_offset_14_to_blur_x_0_offset_654 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_14_to_blur_x_0_offset_654
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_14_to_blur_y_0_pe_1("from_blur_x_0_offset_14_to_blur_y_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_14_to_blur_y_0_pe_1 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_14_to_blur_y_0_pe_1
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_15_to_blur_x_0_offset_655("from_blur_x_0_offset_15_to_blur_x_0_offset_655");
#pragma HLS stream variable=from_blur_x_0_offset_15_to_blur_x_0_offset_655 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_15_to_blur_x_0_offset_655
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_15_to_blur_y_0_pe_0("from_blur_x_0_offset_15_to_blur_y_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_15_to_blur_y_0_pe_0 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_15_to_blur_y_0_pe_0
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_640_to_blur_x_0_offset_1280("from_blur_x_0_offset_640_to_blur_x_0_offset_1280");
#pragma HLS stream variable=from_blur_x_0_offset_640_to_blur_x_0_offset_1280 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_640_to_blur_x_0_offset_1280
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_640_to_blur_y_0_pe_15("from_blur_x_0_offset_640_to_blur_y_0_pe_15");
#pragma HLS stream variable=from_blur_x_0_offset_640_to_blur_y_0_pe_15 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_640_to_blur_y_0_pe_15
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_641_to_blur_x_0_offset_1281("from_blur_x_0_offset_641_to_blur_x_0_offset_1281");
#pragma HLS stream variable=from_blur_x_0_offset_641_to_blur_x_0_offset_1281 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_641_to_blur_x_0_offset_1281
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_641_to_blur_y_0_pe_14("from_blur_x_0_offset_641_to_blur_y_0_pe_14");
#pragma HLS stream variable=from_blur_x_0_offset_641_to_blur_y_0_pe_14 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_641_to_blur_y_0_pe_14
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_642_to_blur_x_0_offset_1282("from_blur_x_0_offset_642_to_blur_x_0_offset_1282");
#pragma HLS stream variable=from_blur_x_0_offset_642_to_blur_x_0_offset_1282 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_642_to_blur_x_0_offset_1282
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_642_to_blur_y_0_pe_13("from_blur_x_0_offset_642_to_blur_y_0_pe_13");
#pragma HLS stream variable=from_blur_x_0_offset_642_to_blur_y_0_pe_13 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_642_to_blur_y_0_pe_13
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_643_to_blur_x_0_offset_1283("from_blur_x_0_offset_643_to_blur_x_0_offset_1283");
#pragma HLS stream variable=from_blur_x_0_offset_643_to_blur_x_0_offset_1283 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_643_to_blur_x_0_offset_1283
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_643_to_blur_y_0_pe_12("from_blur_x_0_offset_643_to_blur_y_0_pe_12");
#pragma HLS stream variable=from_blur_x_0_offset_643_to_blur_y_0_pe_12 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_643_to_blur_y_0_pe_12
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_644_to_blur_x_0_offset_1284("from_blur_x_0_offset_644_to_blur_x_0_offset_1284");
#pragma HLS stream variable=from_blur_x_0_offset_644_to_blur_x_0_offset_1284 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_644_to_blur_x_0_offset_1284
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_644_to_blur_y_0_pe_11("from_blur_x_0_offset_644_to_blur_y_0_pe_11");
#pragma HLS stream variable=from_blur_x_0_offset_644_to_blur_y_0_pe_11 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_644_to_blur_y_0_pe_11
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_645_to_blur_x_0_offset_1285("from_blur_x_0_offset_645_to_blur_x_0_offset_1285");
#pragma HLS stream variable=from_blur_x_0_offset_645_to_blur_x_0_offset_1285 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_645_to_blur_x_0_offset_1285
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_645_to_blur_y_0_pe_10("from_blur_x_0_offset_645_to_blur_y_0_pe_10");
#pragma HLS stream variable=from_blur_x_0_offset_645_to_blur_y_0_pe_10 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_645_to_blur_y_0_pe_10
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_646_to_blur_x_0_offset_1286("from_blur_x_0_offset_646_to_blur_x_0_offset_1286");
#pragma HLS stream variable=from_blur_x_0_offset_646_to_blur_x_0_offset_1286 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_646_to_blur_x_0_offset_1286
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_646_to_blur_y_0_pe_9("from_blur_x_0_offset_646_to_blur_y_0_pe_9");
#pragma HLS stream variable=from_blur_x_0_offset_646_to_blur_y_0_pe_9 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_646_to_blur_y_0_pe_9
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_647_to_blur_x_0_offset_1287("from_blur_x_0_offset_647_to_blur_x_0_offset_1287");
#pragma HLS stream variable=from_blur_x_0_offset_647_to_blur_x_0_offset_1287 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_647_to_blur_x_0_offset_1287
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_647_to_blur_y_0_pe_8("from_blur_x_0_offset_647_to_blur_y_0_pe_8");
#pragma HLS stream variable=from_blur_x_0_offset_647_to_blur_y_0_pe_8 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_647_to_blur_y_0_pe_8
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_648_to_blur_x_0_offset_1288("from_blur_x_0_offset_648_to_blur_x_0_offset_1288");
#pragma HLS stream variable=from_blur_x_0_offset_648_to_blur_x_0_offset_1288 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_648_to_blur_x_0_offset_1288
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_648_to_blur_y_0_pe_7("from_blur_x_0_offset_648_to_blur_y_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_648_to_blur_y_0_pe_7 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_648_to_blur_y_0_pe_7
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_649_to_blur_x_0_offset_1289("from_blur_x_0_offset_649_to_blur_x_0_offset_1289");
#pragma HLS stream variable=from_blur_x_0_offset_649_to_blur_x_0_offset_1289 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_649_to_blur_x_0_offset_1289
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_649_to_blur_y_0_pe_6("from_blur_x_0_offset_649_to_blur_y_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_649_to_blur_y_0_pe_6 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_649_to_blur_y_0_pe_6
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_650_to_blur_x_0_offset_1290("from_blur_x_0_offset_650_to_blur_x_0_offset_1290");
#pragma HLS stream variable=from_blur_x_0_offset_650_to_blur_x_0_offset_1290 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_650_to_blur_x_0_offset_1290
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_650_to_blur_y_0_pe_5("from_blur_x_0_offset_650_to_blur_y_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_650_to_blur_y_0_pe_5 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_650_to_blur_y_0_pe_5
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_651_to_blur_x_0_offset_1291("from_blur_x_0_offset_651_to_blur_x_0_offset_1291");
#pragma HLS stream variable=from_blur_x_0_offset_651_to_blur_x_0_offset_1291 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_651_to_blur_x_0_offset_1291
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_651_to_blur_y_0_pe_4("from_blur_x_0_offset_651_to_blur_y_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_651_to_blur_y_0_pe_4 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_651_to_blur_y_0_pe_4
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_652_to_blur_x_0_offset_1292("from_blur_x_0_offset_652_to_blur_x_0_offset_1292");
#pragma HLS stream variable=from_blur_x_0_offset_652_to_blur_x_0_offset_1292 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_652_to_blur_x_0_offset_1292
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_652_to_blur_y_0_pe_3("from_blur_x_0_offset_652_to_blur_y_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_652_to_blur_y_0_pe_3 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_652_to_blur_y_0_pe_3
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_653_to_blur_x_0_offset_1293("from_blur_x_0_offset_653_to_blur_x_0_offset_1293");
#pragma HLS stream variable=from_blur_x_0_offset_653_to_blur_x_0_offset_1293 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_653_to_blur_x_0_offset_1293
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_653_to_blur_y_0_pe_2("from_blur_x_0_offset_653_to_blur_y_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_653_to_blur_y_0_pe_2 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_653_to_blur_y_0_pe_2
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_654_to_blur_x_0_offset_1294("from_blur_x_0_offset_654_to_blur_x_0_offset_1294");
#pragma HLS stream variable=from_blur_x_0_offset_654_to_blur_x_0_offset_1294 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_654_to_blur_x_0_offset_1294
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_654_to_blur_y_0_pe_1("from_blur_x_0_offset_654_to_blur_y_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_654_to_blur_y_0_pe_1 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_654_to_blur_y_0_pe_1
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_655_to_blur_x_0_offset_1295("from_blur_x_0_offset_655_to_blur_x_0_offset_1295");
#pragma HLS stream variable=from_blur_x_0_offset_655_to_blur_x_0_offset_1295 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_655_to_blur_x_0_offset_1295
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_655_to_blur_y_0_pe_0("from_blur_x_0_offset_655_to_blur_y_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_655_to_blur_y_0_pe_0 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_655_to_blur_y_0_pe_0
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1280_to_blur_y_0_pe_15("from_blur_x_0_offset_1280_to_blur_y_0_pe_15");
#pragma HLS stream variable=from_blur_x_0_offset_1280_to_blur_y_0_pe_15 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1280_to_blur_y_0_pe_15
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_15_to_super_sink("from_blur_y_0_pe_15_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_15_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_15_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1281_to_blur_y_0_pe_14("from_blur_x_0_offset_1281_to_blur_y_0_pe_14");
#pragma HLS stream variable=from_blur_x_0_offset_1281_to_blur_y_0_pe_14 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1281_to_blur_y_0_pe_14
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_14_to_super_sink("from_blur_y_0_pe_14_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_14_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_14_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1282_to_blur_y_0_pe_13("from_blur_x_0_offset_1282_to_blur_y_0_pe_13");
#pragma HLS stream variable=from_blur_x_0_offset_1282_to_blur_y_0_pe_13 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1282_to_blur_y_0_pe_13
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_13_to_super_sink("from_blur_y_0_pe_13_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_13_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_13_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1283_to_blur_y_0_pe_12("from_blur_x_0_offset_1283_to_blur_y_0_pe_12");
#pragma HLS stream variable=from_blur_x_0_offset_1283_to_blur_y_0_pe_12 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1283_to_blur_y_0_pe_12
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_12_to_super_sink("from_blur_y_0_pe_12_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_12_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_12_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1284_to_blur_y_0_pe_11("from_blur_x_0_offset_1284_to_blur_y_0_pe_11");
#pragma HLS stream variable=from_blur_x_0_offset_1284_to_blur_y_0_pe_11 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1284_to_blur_y_0_pe_11
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_11_to_super_sink("from_blur_y_0_pe_11_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_11_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_11_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1285_to_blur_y_0_pe_10("from_blur_x_0_offset_1285_to_blur_y_0_pe_10");
#pragma HLS stream variable=from_blur_x_0_offset_1285_to_blur_y_0_pe_10 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1285_to_blur_y_0_pe_10
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_10_to_super_sink("from_blur_y_0_pe_10_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_10_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_10_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1286_to_blur_y_0_pe_9("from_blur_x_0_offset_1286_to_blur_y_0_pe_9");
#pragma HLS stream variable=from_blur_x_0_offset_1286_to_blur_y_0_pe_9 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1286_to_blur_y_0_pe_9
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_9_to_super_sink("from_blur_y_0_pe_9_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_9_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_9_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1287_to_blur_y_0_pe_8("from_blur_x_0_offset_1287_to_blur_y_0_pe_8");
#pragma HLS stream variable=from_blur_x_0_offset_1287_to_blur_y_0_pe_8 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1287_to_blur_y_0_pe_8
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_8_to_super_sink("from_blur_y_0_pe_8_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_8_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_8_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1288_to_blur_y_0_pe_7("from_blur_x_0_offset_1288_to_blur_y_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_1288_to_blur_y_0_pe_7 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1288_to_blur_y_0_pe_7
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_7_to_super_sink("from_blur_y_0_pe_7_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_7_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_7_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1289_to_blur_y_0_pe_6("from_blur_x_0_offset_1289_to_blur_y_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_1289_to_blur_y_0_pe_6 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1289_to_blur_y_0_pe_6
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_6_to_super_sink("from_blur_y_0_pe_6_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_6_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_6_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1290_to_blur_y_0_pe_5("from_blur_x_0_offset_1290_to_blur_y_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_1290_to_blur_y_0_pe_5 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1290_to_blur_y_0_pe_5
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_5_to_super_sink("from_blur_y_0_pe_5_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_5_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_5_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1291_to_blur_y_0_pe_4("from_blur_x_0_offset_1291_to_blur_y_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_1291_to_blur_y_0_pe_4 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1291_to_blur_y_0_pe_4
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_4_to_super_sink("from_blur_y_0_pe_4_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_4_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_4_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1292_to_blur_y_0_pe_3("from_blur_x_0_offset_1292_to_blur_y_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_1292_to_blur_y_0_pe_3 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1292_to_blur_y_0_pe_3
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_3_to_super_sink("from_blur_y_0_pe_3_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_3_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_3_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1293_to_blur_y_0_pe_2("from_blur_x_0_offset_1293_to_blur_y_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_1293_to_blur_y_0_pe_2 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1293_to_blur_y_0_pe_2
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_2_to_super_sink("from_blur_y_0_pe_2_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_2_to_super_sink depth=4
#pragma HLS data_pack variable=from_blur_y_0_pe_2_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1294_to_blur_y_0_pe_1("from_blur_x_0_offset_1294_to_blur_y_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_1294_to_blur_y_0_pe_1 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1294_to_blur_y_0_pe_1
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_1_to_super_sink("from_blur_y_0_pe_1_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_1_to_super_sink depth=2
#pragma HLS data_pack variable=from_blur_y_0_pe_1_to_super_sink
  hls::stream<Data<uint16_t>> from_blur_x_0_offset_1295_to_blur_y_0_pe_0("from_blur_x_0_offset_1295_to_blur_y_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_1295_to_blur_y_0_pe_0 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1295_to_blur_y_0_pe_0
  hls::stream<Data<uint16_t>> from_blur_y_0_pe_0_to_super_sink("from_blur_y_0_pe_0_to_super_sink");
#pragma HLS stream variable=from_blur_y_0_pe_0_to_super_sink depth=2
#pragma HLS data_pack variable=from_blur_y_0_pe_0_to_super_sink

#pragma HLS dataflow
  BurstRead(&bank_0_input_buf, bank_0_input, coalesced_data_num);
  Module0Func(
    /*output*/ &from_super_source_to_input_offset_0, 
    /*output*/ &from_super_source_to_input_offset_1, 
    /*output*/ &from_super_source_to_input_offset_2, 
    /*output*/ &from_super_source_to_input_offset_3, 
    /*output*/ &from_super_source_to_input_offset_4, 
    /*output*/ &from_super_source_to_input_offset_5, 
    /*output*/ &from_super_source_to_input_offset_6, 
    /*output*/ &from_super_source_to_input_offset_7, 
    /*output*/ &from_super_source_to_input_offset_8, 
    /*output*/ &from_super_source_to_input_offset_9, 
    /*output*/ &from_super_source_to_input_offset_10, 
    /*output*/ &from_super_source_to_input_offset_11, 
    /*output*/ &from_super_source_to_input_offset_12, 
    /*output*/ &from_super_source_to_input_offset_13, 
    /*output*/ &from_super_source_to_input_offset_14, 
    /*output*/ &from_super_source_to_input_offset_15, 
    /* input*/ &bank_0_input_buf);
  Module1Func(
    /*output*/ &from_input_offset_0_to_input_offset_16, 
    /*output*/ &from_input_offset_0_to_blur_x_0_pe_15, 
    /* input*/ &from_super_source_to_input_offset_0);
  Module2Func(
    /*output*/ &from_input_offset_1_to_input_offset_17, 
    /*output*/ &from_input_offset_1_to_blur_x_0_pe_15, 
    /*output*/ &from_input_offset_1_to_blur_x_0_pe_14, 
    /* input*/ &from_super_source_to_input_offset_1);
  Module2Func(
    /*output*/ &from_input_offset_2_to_blur_x_0_pe_15, 
    /*output*/ &from_input_offset_2_to_blur_x_0_pe_14, 
    /*output*/ &from_input_offset_2_to_blur_x_0_pe_13, 
    /* input*/ &from_super_source_to_input_offset_2);
  Module2Func(
    /*output*/ &from_input_offset_3_to_blur_x_0_pe_14, 
    /*output*/ &from_input_offset_3_to_blur_x_0_pe_13, 
    /*output*/ &from_input_offset_3_to_blur_x_0_pe_12, 
    /* input*/ &from_super_source_to_input_offset_3);
  Module2Func(
    /*output*/ &from_input_offset_4_to_blur_x_0_pe_13, 
    /*output*/ &from_input_offset_4_to_blur_x_0_pe_12, 
    /*output*/ &from_input_offset_4_to_blur_x_0_pe_11, 
    /* input*/ &from_super_source_to_input_offset_4);
  Module2Func(
    /*output*/ &from_input_offset_5_to_blur_x_0_pe_12, 
    /*output*/ &from_input_offset_5_to_blur_x_0_pe_11, 
    /*output*/ &from_input_offset_5_to_blur_x_0_pe_10, 
    /* input*/ &from_super_source_to_input_offset_5);
  Module2Func(
    /*output*/ &from_input_offset_6_to_blur_x_0_pe_11, 
    /*output*/ &from_input_offset_6_to_blur_x_0_pe_10, 
    /*output*/ &from_input_offset_6_to_blur_x_0_pe_9, 
    /* input*/ &from_super_source_to_input_offset_6);
  Module2Func(
    /*output*/ &from_input_offset_7_to_blur_x_0_pe_10, 
    /*output*/ &from_input_offset_7_to_blur_x_0_pe_9, 
    /*output*/ &from_input_offset_7_to_blur_x_0_pe_8, 
    /* input*/ &from_super_source_to_input_offset_7);
  Module2Func(
    /*output*/ &from_input_offset_8_to_blur_x_0_pe_9, 
    /*output*/ &from_input_offset_8_to_blur_x_0_pe_8, 
    /*output*/ &from_input_offset_8_to_blur_x_0_pe_7, 
    /* input*/ &from_super_source_to_input_offset_8);
  Module2Func(
    /*output*/ &from_input_offset_9_to_blur_x_0_pe_8, 
    /*output*/ &from_input_offset_9_to_blur_x_0_pe_7, 
    /*output*/ &from_input_offset_9_to_blur_x_0_pe_6, 
    /* input*/ &from_super_source_to_input_offset_9);
  Module2Func(
    /*output*/ &from_input_offset_10_to_blur_x_0_pe_7, 
    /*output*/ &from_input_offset_10_to_blur_x_0_pe_6, 
    /*output*/ &from_input_offset_10_to_blur_x_0_pe_5, 
    /* input*/ &from_super_source_to_input_offset_10);
  Module2Func(
    /*output*/ &from_input_offset_11_to_blur_x_0_pe_6, 
    /*output*/ &from_input_offset_11_to_blur_x_0_pe_5, 
    /*output*/ &from_input_offset_11_to_blur_x_0_pe_4, 
    /* input*/ &from_super_source_to_input_offset_11);
  Module2Func(
    /*output*/ &from_input_offset_12_to_blur_x_0_pe_5, 
    /*output*/ &from_input_offset_12_to_blur_x_0_pe_4, 
    /*output*/ &from_input_offset_12_to_blur_x_0_pe_3, 
    /* input*/ &from_super_source_to_input_offset_12);
  Module2Func(
    /*output*/ &from_input_offset_13_to_blur_x_0_pe_4, 
    /*output*/ &from_input_offset_13_to_blur_x_0_pe_3, 
    /*output*/ &from_input_offset_13_to_blur_x_0_pe_2, 
    /* input*/ &from_super_source_to_input_offset_13);
  Module2Func(
    /*output*/ &from_input_offset_14_to_blur_x_0_pe_3, 
    /*output*/ &from_input_offset_14_to_blur_x_0_pe_2, 
    /*output*/ &from_input_offset_14_to_blur_x_0_pe_1, 
    /* input*/ &from_super_source_to_input_offset_14);
  Module2Func(
    /*output*/ &from_input_offset_15_to_blur_x_0_pe_2, 
    /*output*/ &from_input_offset_15_to_blur_x_0_pe_1, 
    /*output*/ &from_input_offset_15_to_blur_x_0_pe_0, 
    /* input*/ &from_super_source_to_input_offset_15);
  Module3Func(
    /*output*/ &from_input_offset_16_to_blur_x_0_pe_1, 
    /*output*/ &from_input_offset_16_to_blur_x_0_pe_0, 
    /* input*/ &from_input_offset_0_to_input_offset_16);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_15_to_blur_x_0_offset_0, 
    /* input*/ &from_input_offset_0_to_blur_x_0_pe_15, 
    /* input*/ &from_input_offset_2_to_blur_x_0_pe_15, 
    /* input*/ &from_input_offset_1_to_blur_x_0_pe_15);
  Module5Func(
    /*output*/ &from_input_offset_17_to_blur_x_0_pe_0, 
    /* input*/ &from_input_offset_1_to_input_offset_17);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_14_to_blur_x_0_offset_1, 
    /* input*/ &from_input_offset_1_to_blur_x_0_pe_14, 
    /* input*/ &from_input_offset_3_to_blur_x_0_pe_14, 
    /* input*/ &from_input_offset_2_to_blur_x_0_pe_14);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_13_to_blur_x_0_offset_2, 
    /* input*/ &from_input_offset_2_to_blur_x_0_pe_13, 
    /* input*/ &from_input_offset_4_to_blur_x_0_pe_13, 
    /* input*/ &from_input_offset_3_to_blur_x_0_pe_13);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_12_to_blur_x_0_offset_3, 
    /* input*/ &from_input_offset_3_to_blur_x_0_pe_12, 
    /* input*/ &from_input_offset_5_to_blur_x_0_pe_12, 
    /* input*/ &from_input_offset_4_to_blur_x_0_pe_12);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_11_to_blur_x_0_offset_4, 
    /* input*/ &from_input_offset_4_to_blur_x_0_pe_11, 
    /* input*/ &from_input_offset_6_to_blur_x_0_pe_11, 
    /* input*/ &from_input_offset_5_to_blur_x_0_pe_11);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_10_to_blur_x_0_offset_5, 
    /* input*/ &from_input_offset_5_to_blur_x_0_pe_10, 
    /* input*/ &from_input_offset_7_to_blur_x_0_pe_10, 
    /* input*/ &from_input_offset_6_to_blur_x_0_pe_10);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_9_to_blur_x_0_offset_6, 
    /* input*/ &from_input_offset_6_to_blur_x_0_pe_9, 
    /* input*/ &from_input_offset_8_to_blur_x_0_pe_9, 
    /* input*/ &from_input_offset_7_to_blur_x_0_pe_9);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_8_to_blur_x_0_offset_7, 
    /* input*/ &from_input_offset_7_to_blur_x_0_pe_8, 
    /* input*/ &from_input_offset_9_to_blur_x_0_pe_8, 
    /* input*/ &from_input_offset_8_to_blur_x_0_pe_8);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_7_to_blur_x_0_offset_8, 
    /* input*/ &from_input_offset_8_to_blur_x_0_pe_7, 
    /* input*/ &from_input_offset_10_to_blur_x_0_pe_7, 
    /* input*/ &from_input_offset_9_to_blur_x_0_pe_7);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_6_to_blur_x_0_offset_9, 
    /* input*/ &from_input_offset_9_to_blur_x_0_pe_6, 
    /* input*/ &from_input_offset_11_to_blur_x_0_pe_6, 
    /* input*/ &from_input_offset_10_to_blur_x_0_pe_6);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_5_to_blur_x_0_offset_10, 
    /* input*/ &from_input_offset_10_to_blur_x_0_pe_5, 
    /* input*/ &from_input_offset_12_to_blur_x_0_pe_5, 
    /* input*/ &from_input_offset_11_to_blur_x_0_pe_5);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_4_to_blur_x_0_offset_11, 
    /* input*/ &from_input_offset_11_to_blur_x_0_pe_4, 
    /* input*/ &from_input_offset_13_to_blur_x_0_pe_4, 
    /* input*/ &from_input_offset_12_to_blur_x_0_pe_4);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_3_to_blur_x_0_offset_12, 
    /* input*/ &from_input_offset_12_to_blur_x_0_pe_3, 
    /* input*/ &from_input_offset_14_to_blur_x_0_pe_3, 
    /* input*/ &from_input_offset_13_to_blur_x_0_pe_3);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_2_to_blur_x_0_offset_13, 
    /* input*/ &from_input_offset_13_to_blur_x_0_pe_2, 
    /* input*/ &from_input_offset_15_to_blur_x_0_pe_2, 
    /* input*/ &from_input_offset_14_to_blur_x_0_pe_2);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_1_to_blur_x_0_offset_14, 
    /* input*/ &from_input_offset_14_to_blur_x_0_pe_1, 
    /* input*/ &from_input_offset_16_to_blur_x_0_pe_1, 
    /* input*/ &from_input_offset_15_to_blur_x_0_pe_1);
  Module4Func(
    /*output*/ &from_blur_x_0_pe_0_to_blur_x_0_offset_15, 
    /* input*/ &from_input_offset_15_to_blur_x_0_pe_0, 
    /* input*/ &from_input_offset_17_to_blur_x_0_pe_0, 
    /* input*/ &from_input_offset_16_to_blur_x_0_pe_0);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_0_to_blur_x_0_offset_640, 
    /*output*/ &from_blur_x_0_offset_0_to_blur_y_0_pe_15, 
    /* input*/ &from_blur_x_0_pe_15_to_blur_x_0_offset_0);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_1_to_blur_x_0_offset_641, 
    /*output*/ &from_blur_x_0_offset_1_to_blur_y_0_pe_14, 
    /* input*/ &from_blur_x_0_pe_14_to_blur_x_0_offset_1);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_2_to_blur_x_0_offset_642, 
    /*output*/ &from_blur_x_0_offset_2_to_blur_y_0_pe_13, 
    /* input*/ &from_blur_x_0_pe_13_to_blur_x_0_offset_2);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_3_to_blur_x_0_offset_643, 
    /*output*/ &from_blur_x_0_offset_3_to_blur_y_0_pe_12, 
    /* input*/ &from_blur_x_0_pe_12_to_blur_x_0_offset_3);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_4_to_blur_x_0_offset_644, 
    /*output*/ &from_blur_x_0_offset_4_to_blur_y_0_pe_11, 
    /* input*/ &from_blur_x_0_pe_11_to_blur_x_0_offset_4);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_5_to_blur_x_0_offset_645, 
    /*output*/ &from_blur_x_0_offset_5_to_blur_y_0_pe_10, 
    /* input*/ &from_blur_x_0_pe_10_to_blur_x_0_offset_5);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_6_to_blur_x_0_offset_646, 
    /*output*/ &from_blur_x_0_offset_6_to_blur_y_0_pe_9, 
    /* input*/ &from_blur_x_0_pe_9_to_blur_x_0_offset_6);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_7_to_blur_x_0_offset_647, 
    /*output*/ &from_blur_x_0_offset_7_to_blur_y_0_pe_8, 
    /* input*/ &from_blur_x_0_pe_8_to_blur_x_0_offset_7);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_8_to_blur_x_0_offset_648, 
    /*output*/ &from_blur_x_0_offset_8_to_blur_y_0_pe_7, 
    /* input*/ &from_blur_x_0_pe_7_to_blur_x_0_offset_8);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_9_to_blur_x_0_offset_649, 
    /*output*/ &from_blur_x_0_offset_9_to_blur_y_0_pe_6, 
    /* input*/ &from_blur_x_0_pe_6_to_blur_x_0_offset_9);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_10_to_blur_x_0_offset_650, 
    /*output*/ &from_blur_x_0_offset_10_to_blur_y_0_pe_5, 
    /* input*/ &from_blur_x_0_pe_5_to_blur_x_0_offset_10);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_11_to_blur_x_0_offset_651, 
    /*output*/ &from_blur_x_0_offset_11_to_blur_y_0_pe_4, 
    /* input*/ &from_blur_x_0_pe_4_to_blur_x_0_offset_11);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_12_to_blur_x_0_offset_652, 
    /*output*/ &from_blur_x_0_offset_12_to_blur_y_0_pe_3, 
    /* input*/ &from_blur_x_0_pe_3_to_blur_x_0_offset_12);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_13_to_blur_x_0_offset_653, 
    /*output*/ &from_blur_x_0_offset_13_to_blur_y_0_pe_2, 
    /* input*/ &from_blur_x_0_pe_2_to_blur_x_0_offset_13);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_14_to_blur_x_0_offset_654, 
    /*output*/ &from_blur_x_0_offset_14_to_blur_y_0_pe_1, 
    /* input*/ &from_blur_x_0_pe_1_to_blur_x_0_offset_14);
  Module1Func(
    /*output*/ &from_blur_x_0_offset_15_to_blur_x_0_offset_655, 
    /*output*/ &from_blur_x_0_offset_15_to_blur_y_0_pe_0, 
    /* input*/ &from_blur_x_0_pe_0_to_blur_x_0_offset_15);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_640_to_blur_x_0_offset_1280, 
    /*output*/ &from_blur_x_0_offset_640_to_blur_y_0_pe_15, 
    /* input*/ &from_blur_x_0_offset_0_to_blur_x_0_offset_640);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_641_to_blur_x_0_offset_1281, 
    /*output*/ &from_blur_x_0_offset_641_to_blur_y_0_pe_14, 
    /* input*/ &from_blur_x_0_offset_1_to_blur_x_0_offset_641);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_642_to_blur_x_0_offset_1282, 
    /*output*/ &from_blur_x_0_offset_642_to_blur_y_0_pe_13, 
    /* input*/ &from_blur_x_0_offset_2_to_blur_x_0_offset_642);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_643_to_blur_x_0_offset_1283, 
    /*output*/ &from_blur_x_0_offset_643_to_blur_y_0_pe_12, 
    /* input*/ &from_blur_x_0_offset_3_to_blur_x_0_offset_643);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_644_to_blur_x_0_offset_1284, 
    /*output*/ &from_blur_x_0_offset_644_to_blur_y_0_pe_11, 
    /* input*/ &from_blur_x_0_offset_4_to_blur_x_0_offset_644);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_645_to_blur_x_0_offset_1285, 
    /*output*/ &from_blur_x_0_offset_645_to_blur_y_0_pe_10, 
    /* input*/ &from_blur_x_0_offset_5_to_blur_x_0_offset_645);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_646_to_blur_x_0_offset_1286, 
    /*output*/ &from_blur_x_0_offset_646_to_blur_y_0_pe_9, 
    /* input*/ &from_blur_x_0_offset_6_to_blur_x_0_offset_646);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_647_to_blur_x_0_offset_1287, 
    /*output*/ &from_blur_x_0_offset_647_to_blur_y_0_pe_8, 
    /* input*/ &from_blur_x_0_offset_7_to_blur_x_0_offset_647);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_648_to_blur_x_0_offset_1288, 
    /*output*/ &from_blur_x_0_offset_648_to_blur_y_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_8_to_blur_x_0_offset_648);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_649_to_blur_x_0_offset_1289, 
    /*output*/ &from_blur_x_0_offset_649_to_blur_y_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_9_to_blur_x_0_offset_649);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_650_to_blur_x_0_offset_1290, 
    /*output*/ &from_blur_x_0_offset_650_to_blur_y_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_10_to_blur_x_0_offset_650);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_651_to_blur_x_0_offset_1291, 
    /*output*/ &from_blur_x_0_offset_651_to_blur_y_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_11_to_blur_x_0_offset_651);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_652_to_blur_x_0_offset_1292, 
    /*output*/ &from_blur_x_0_offset_652_to_blur_y_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_12_to_blur_x_0_offset_652);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_653_to_blur_x_0_offset_1293, 
    /*output*/ &from_blur_x_0_offset_653_to_blur_y_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_13_to_blur_x_0_offset_653);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_654_to_blur_x_0_offset_1294, 
    /*output*/ &from_blur_x_0_offset_654_to_blur_y_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_14_to_blur_x_0_offset_654);
  Module6Func(
    /*output*/ &from_blur_x_0_offset_655_to_blur_x_0_offset_1295, 
    /*output*/ &from_blur_x_0_offset_655_to_blur_y_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_15_to_blur_x_0_offset_655);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1280_to_blur_y_0_pe_15, 
    /* input*/ &from_blur_x_0_offset_640_to_blur_x_0_offset_1280);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_15_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_0_to_blur_y_0_pe_15, 
    /* input*/ &from_blur_x_0_offset_1280_to_blur_y_0_pe_15, 
    /* input*/ &from_blur_x_0_offset_640_to_blur_y_0_pe_15);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1281_to_blur_y_0_pe_14, 
    /* input*/ &from_blur_x_0_offset_641_to_blur_x_0_offset_1281);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_14_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_1_to_blur_y_0_pe_14, 
    /* input*/ &from_blur_x_0_offset_1281_to_blur_y_0_pe_14, 
    /* input*/ &from_blur_x_0_offset_641_to_blur_y_0_pe_14);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1282_to_blur_y_0_pe_13, 
    /* input*/ &from_blur_x_0_offset_642_to_blur_x_0_offset_1282);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_13_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_2_to_blur_y_0_pe_13, 
    /* input*/ &from_blur_x_0_offset_1282_to_blur_y_0_pe_13, 
    /* input*/ &from_blur_x_0_offset_642_to_blur_y_0_pe_13);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1283_to_blur_y_0_pe_12, 
    /* input*/ &from_blur_x_0_offset_643_to_blur_x_0_offset_1283);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_12_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_3_to_blur_y_0_pe_12, 
    /* input*/ &from_blur_x_0_offset_1283_to_blur_y_0_pe_12, 
    /* input*/ &from_blur_x_0_offset_643_to_blur_y_0_pe_12);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1284_to_blur_y_0_pe_11, 
    /* input*/ &from_blur_x_0_offset_644_to_blur_x_0_offset_1284);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_11_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_4_to_blur_y_0_pe_11, 
    /* input*/ &from_blur_x_0_offset_1284_to_blur_y_0_pe_11, 
    /* input*/ &from_blur_x_0_offset_644_to_blur_y_0_pe_11);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1285_to_blur_y_0_pe_10, 
    /* input*/ &from_blur_x_0_offset_645_to_blur_x_0_offset_1285);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_10_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_5_to_blur_y_0_pe_10, 
    /* input*/ &from_blur_x_0_offset_1285_to_blur_y_0_pe_10, 
    /* input*/ &from_blur_x_0_offset_645_to_blur_y_0_pe_10);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1286_to_blur_y_0_pe_9, 
    /* input*/ &from_blur_x_0_offset_646_to_blur_x_0_offset_1286);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_9_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_6_to_blur_y_0_pe_9, 
    /* input*/ &from_blur_x_0_offset_1286_to_blur_y_0_pe_9, 
    /* input*/ &from_blur_x_0_offset_646_to_blur_y_0_pe_9);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1287_to_blur_y_0_pe_8, 
    /* input*/ &from_blur_x_0_offset_647_to_blur_x_0_offset_1287);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_8_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_7_to_blur_y_0_pe_8, 
    /* input*/ &from_blur_x_0_offset_1287_to_blur_y_0_pe_8, 
    /* input*/ &from_blur_x_0_offset_647_to_blur_y_0_pe_8);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1288_to_blur_y_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_648_to_blur_x_0_offset_1288);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_7_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_8_to_blur_y_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_1288_to_blur_y_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_648_to_blur_y_0_pe_7);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1289_to_blur_y_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_649_to_blur_x_0_offset_1289);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_6_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_9_to_blur_y_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_1289_to_blur_y_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_649_to_blur_y_0_pe_6);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1290_to_blur_y_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_650_to_blur_x_0_offset_1290);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_5_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_10_to_blur_y_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_1290_to_blur_y_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_650_to_blur_y_0_pe_5);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1291_to_blur_y_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_651_to_blur_x_0_offset_1291);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_4_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_11_to_blur_y_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_1291_to_blur_y_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_651_to_blur_y_0_pe_4);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1292_to_blur_y_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_652_to_blur_x_0_offset_1292);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_3_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_12_to_blur_y_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_1292_to_blur_y_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_652_to_blur_y_0_pe_3);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1293_to_blur_y_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_653_to_blur_x_0_offset_1293);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_2_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_13_to_blur_y_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_1293_to_blur_y_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_653_to_blur_y_0_pe_2);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1294_to_blur_y_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_654_to_blur_x_0_offset_1294);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_1_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_14_to_blur_y_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_1294_to_blur_y_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_654_to_blur_y_0_pe_1);
  Module7Func(
    /*output*/ &from_blur_x_0_offset_1295_to_blur_y_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_655_to_blur_x_0_offset_1295);
  Module4Func(
    /*output*/ &from_blur_y_0_pe_0_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_15_to_blur_y_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_1295_to_blur_y_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_655_to_blur_y_0_pe_0);
  Module8Func(
    /*output*/ &bank_0_blur_y_0_buf, 
    /* input*/ &from_blur_y_0_pe_15_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_14_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_13_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_12_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_11_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_10_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_9_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_8_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_7_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_6_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_5_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_4_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_3_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_2_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_1_to_super_sink, 
    /* input*/ &from_blur_y_0_pe_0_to_super_sink);
  BurstWrite(bank_0_blur_y_0, &bank_0_blur_y_0_buf, coalesced_data_num);
}

}//extern "C"
