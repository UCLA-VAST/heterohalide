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
#if UNROLL_FACTOR != 8
#error UNROLL_FACTOR != 8
#endif//UNROLL_FACTOR != 8
#endif//UNROLL_FACTOR
#ifdef TILE_SIZE_DIM_0
#if TILE_SIZE_DIM_0 != 768
#error TILE_SIZE_DIM_0 != 768
#endif//TILE_SIZE_DIM_0 != 768
#endif//TILE_SIZE_DIM_0
#ifdef TILE_SIZE_DIM_1
#if TILE_SIZE_DIM_1 != 1280
#error TILE_SIZE_DIM_1 != 1280
#endif//TILE_SIZE_DIM_1 != 1280
#endif//TILE_SIZE_DIM_1
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
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /*output*/ hls::stream<Data<float>>* fifo_st_1, 
  /*output*/ hls::stream<Data<float>>* fifo_st_2, 
  /*output*/ hls::stream<Data<float>>* fifo_st_3, 
  /*output*/ hls::stream<Data<float>>* fifo_st_4, 
  /*output*/ hls::stream<Data<float>>* fifo_st_5, 
  /*output*/ hls::stream<Data<float>>* fifo_st_6, 
  /*output*/ hls::stream<Data<float>>* fifo_st_7, 
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
      WriteData(fifo_st_0, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(255, 224))), enabled);
      WriteData(fifo_st_1, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(223, 192))), enabled);
      WriteData(fifo_st_2, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(191, 160))), enabled);
      WriteData(fifo_st_3, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(159, 128))), enabled);
      WriteData(fifo_st_4, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(127, 96))), enabled);
      WriteData(fifo_st_5, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(95, 64))), enabled);
      WriteData(fifo_st_6, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(63, 32))), enabled);
      WriteData(fifo_st_7, Reinterpret<float>(static_cast<ap_uint<32>>(dram_input_bank_0_buf(31, 0))), enabled);
    } // if not empty
  } // for module_0_epoch
} // Module0Func
void Module1Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
module_1_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(fifo_ref_0), enabled);
    } // if not empty
  } // for module_1_epoch
} // Module1Func
void Module2Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
module_2_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(((0.0404500F < fifo_ref_0) ? powf(((fifo_ref_0 * 0.947867F) + 0.0521330F), 2.40000F) : (fifo_ref_0 * 0.0773990F))), enabled);
    } // if not empty
  } // for module_2_epoch
} // Module2Func
void Module3Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /*output*/ hls::stream<Data<float>>* fifo_st_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
module_3_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(fifo_ref_0), enabled);
      WriteData(fifo_st_1, float(fifo_ref_0), enabled);
    } // if not empty
  } // for module_3_epoch
} // Module3Func
void Module4Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /*output*/ hls::stream<Data<float>>* fifo_st_1, 
  /*output*/ hls::stream<Data<float>>* fifo_st_2, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_st_2
#pragma HLS data_pack variable = fifo_ld_0
module_4_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(fifo_ref_0), enabled);
      WriteData(fifo_st_1, float(fifo_ref_0), enabled);
      WriteData(fifo_st_2, float(fifo_ref_0), enabled);
    } // if not empty
  } // for module_4_epoch
} // Module4Func
void Module5Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /*output*/ hls::stream<Data<float>>* fifo_st_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
  float fifo_ref_0_delayed_1_buf[1];
  ap_uint<1> fifo_ref_0_delayed_1_ptr = 0;
module_5_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_1_buf inter false
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const float fifo_ref_0_delayed_1 = fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr];;
      const float let_0 = fifo_ref_0_delayed_1;
      WriteData(fifo_st_0, float(let_0), enabled);
      WriteData(fifo_st_1, float(let_0), enabled);
      fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_1_ptr = fifo_ref_0_delayed_1_ptr < 0 ? ap_uint<1>(fifo_ref_0_delayed_1_ptr+1) : ap_uint<1>(0);
    } // if not empty
  } // for module_5_epoch
} // Module5Func
void Module6Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_2)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
#pragma HLS data_pack variable = fifo_ld_1
#pragma HLS data_pack variable = fifo_ld_2
module_6_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty() && !fifo_ld_1->empty() && !fifo_ld_2->empty())
    {
      float fifo_ref_0;
      float fifo_ref_1;
      float fifo_ref_2;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool fifo_ref_1_enable = ReadData(&fifo_ref_1, fifo_ld_1);
      const bool fifo_ref_2_enable = ReadData(&fifo_ref_2, fifo_ld_2);
      const bool enabled = fifo_ref_0_enable && fifo_ref_1_enable && fifo_ref_2_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(((fifo_ref_0 + fifo_ref_1 + fifo_ref_2) * 0.333333F)), enabled);
    } // if not empty
  } // for module_6_epoch
} // Module6Func
void Module7Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
  float fifo_ref_0_delayed_1_buf[1];
  ap_uint<1> fifo_ref_0_delayed_1_ptr = 0;
module_7_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_1_buf inter false
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const float fifo_ref_0_delayed_1 = fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr];;
      const float let_0 = fifo_ref_0_delayed_1;
      WriteData(fifo_st_0, float(let_0), enabled);
      fifo_ref_0_delayed_1_buf[fifo_ref_0_delayed_1_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_1_ptr = fifo_ref_0_delayed_1_ptr < 0 ? ap_uint<1>(fifo_ref_0_delayed_1_ptr+1) : ap_uint<1>(0);
    } // if not empty
  } // for module_7_epoch
} // Module7Func
void Module8Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /*output*/ hls::stream<Data<float>>* fifo_st_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_st_1
#pragma HLS data_pack variable = fifo_ld_0
  float fifo_ref_0_delayed_96_buf[96];
  ap_uint<7> fifo_ref_0_delayed_96_ptr = 0;
module_8_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_96_buf inter false
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const float fifo_ref_0_delayed_96 = fifo_ref_0_delayed_96_buf[fifo_ref_0_delayed_96_ptr];;
      const float let_0 = fifo_ref_0_delayed_96;
      WriteData(fifo_st_0, float(let_0), enabled);
      WriteData(fifo_st_1, float(let_0), enabled);
      fifo_ref_0_delayed_96_buf[fifo_ref_0_delayed_96_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_96_ptr = fifo_ref_0_delayed_96_ptr < 95 ? ap_uint<7>(fifo_ref_0_delayed_96_ptr+1) : ap_uint<7>(0);
    } // if not empty
  } // for module_8_epoch
} // Module8Func
void Module9Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
  float fifo_ref_0_delayed_96_buf[96];
  ap_uint<7> fifo_ref_0_delayed_96_ptr = 0;
module_9_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
#pragma HLS dependence variable=fifo_ref_0_delayed_96_buf inter false
    if (!fifo_ld_0->empty())
    {
      float fifo_ref_0;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool enabled = fifo_ref_0_enable;
      enable = enabled;
      const float fifo_ref_0_delayed_96 = fifo_ref_0_delayed_96_buf[fifo_ref_0_delayed_96_ptr];;
      const float let_0 = fifo_ref_0_delayed_96;
      WriteData(fifo_st_0, float(let_0), enabled);
      fifo_ref_0_delayed_96_buf[fifo_ref_0_delayed_96_ptr] = fifo_ref_0;
      fifo_ref_0_delayed_96_ptr = fifo_ref_0_delayed_96_ptr < 95 ? ap_uint<7>(fifo_ref_0_delayed_96_ptr+1) : ap_uint<7>(0);
    } // if not empty
  } // for module_9_epoch
} // Module9Func
void Module10Func(
  /*output*/ hls::stream<Data<float>>* fifo_st_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_2)
{
#pragma HLS data_pack variable = fifo_st_0
#pragma HLS data_pack variable = fifo_ld_0
#pragma HLS data_pack variable = fifo_ld_1
#pragma HLS data_pack variable = fifo_ld_2
module_10_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty() && !fifo_ld_1->empty() && !fifo_ld_2->empty())
    {
      float fifo_ref_0;
      float fifo_ref_1;
      float fifo_ref_2;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool fifo_ref_1_enable = ReadData(&fifo_ref_1, fifo_ld_1);
      const bool fifo_ref_2_enable = ReadData(&fifo_ref_2, fifo_ld_2);
      const bool enabled = fifo_ref_0_enable && fifo_ref_1_enable && fifo_ref_2_enable;
      enable = enabled;
      WriteData(fifo_st_0, float(((0.00313100F < ((fifo_ref_0 + fifo_ref_1 + fifo_ref_2) * 0.333333F)) ? ((powf(((fifo_ref_0 + fifo_ref_1 + fifo_ref_2) * 0.333333F), 0.416667F) * 1.05500F) + -0.0550000F) : ((fifo_ref_0 + fifo_ref_1 + fifo_ref_2) * 0.333333F * 12.9200F))), enabled);
    } // if not empty
  } // for module_10_epoch
} // Module10Func
void Module11Func(
  /*output*/ hls::stream<Data<ap_uint<256>>>* dram_output_0_bank_0_fifo, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_0, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_1, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_2, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_3, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_4, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_5, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_6, 
  /* input*/ hls::stream<Data<float>>* fifo_ld_7)
{
#pragma HLS data_pack variable = dram_output_0_bank_0_fifo
#pragma HLS data_pack variable = fifo_ld_0
#pragma HLS data_pack variable = fifo_ld_1
#pragma HLS data_pack variable = fifo_ld_2
#pragma HLS data_pack variable = fifo_ld_3
#pragma HLS data_pack variable = fifo_ld_4
#pragma HLS data_pack variable = fifo_ld_5
#pragma HLS data_pack variable = fifo_ld_6
#pragma HLS data_pack variable = fifo_ld_7
module_11_epoch:
  for (bool enable = true; enable;)
  {
#pragma HLS pipeline II=1
    if (!fifo_ld_0->empty() && !fifo_ld_1->empty() && !fifo_ld_2->empty() && !fifo_ld_3->empty() && !fifo_ld_4->empty() && !fifo_ld_5->empty() && !fifo_ld_6->empty() && !fifo_ld_7->empty())
    {
      float fifo_ref_0;
      float fifo_ref_1;
      float fifo_ref_2;
      float fifo_ref_3;
      float fifo_ref_4;
      float fifo_ref_5;
      float fifo_ref_6;
      float fifo_ref_7;
      ap_uint<256> dram_output_0_bank_0_buf;
      const bool fifo_ref_0_enable = ReadData(&fifo_ref_0, fifo_ld_0);
      const bool fifo_ref_1_enable = ReadData(&fifo_ref_1, fifo_ld_1);
      const bool fifo_ref_2_enable = ReadData(&fifo_ref_2, fifo_ld_2);
      const bool fifo_ref_3_enable = ReadData(&fifo_ref_3, fifo_ld_3);
      const bool fifo_ref_4_enable = ReadData(&fifo_ref_4, fifo_ld_4);
      const bool fifo_ref_5_enable = ReadData(&fifo_ref_5, fifo_ld_5);
      const bool fifo_ref_6_enable = ReadData(&fifo_ref_6, fifo_ld_6);
      const bool fifo_ref_7_enable = ReadData(&fifo_ref_7, fifo_ld_7);
      const bool enabled = fifo_ref_0_enable && fifo_ref_1_enable && fifo_ref_2_enable && fifo_ref_3_enable && fifo_ref_4_enable && fifo_ref_5_enable && fifo_ref_6_enable && fifo_ref_7_enable;
      enable = enabled;
      dram_output_0_bank_0_buf(255, 224) = Reinterpret<ap_uint<32>>(fifo_ref_0);
      dram_output_0_bank_0_buf(223, 192) = Reinterpret<ap_uint<32>>(fifo_ref_1);
      dram_output_0_bank_0_buf(191, 160) = Reinterpret<ap_uint<32>>(fifo_ref_2);
      dram_output_0_bank_0_buf(159, 128) = Reinterpret<ap_uint<32>>(fifo_ref_3);
      dram_output_0_bank_0_buf(127, 96) = Reinterpret<ap_uint<32>>(fifo_ref_4);
      dram_output_0_bank_0_buf(95, 64) = Reinterpret<ap_uint<32>>(fifo_ref_5);
      dram_output_0_bank_0_buf(63, 32) = Reinterpret<ap_uint<32>>(fifo_ref_6);
      dram_output_0_bank_0_buf(31, 0) = Reinterpret<ap_uint<32>>(fifo_ref_7);
      WriteData(dram_output_0_bank_0_fifo, dram_output_0_bank_0_buf, enabled);
    } // if not empty
  } // for module_11_epoch
} // Module11Func
extern "C"
{

void default_function_kernel(
  ap_uint<256>* bank_0_output_0,
  ap_uint<256>* bank_0_input,
  uint64_t coalesced_data_num)
{
#pragma HLS interface m_axi port=bank_0_output_0 offset=slave depth=65536 bundle=output_0_bank_0
#pragma HLS interface m_axi port=bank_0_input offset=slave depth=65536 bundle=input_bank_0

#pragma HLS interface s_axilite port=bank_0_output_0 bundle=control
#pragma HLS interface s_axilite port=bank_0_input bundle=control
#pragma HLS interface s_axilite port=coalesced_data_num bundle=control
#pragma HLS interface s_axilite port=return bundle=control

  hls::stream<Data<ap_uint<256>>> bank_0_input_buf("bank_0_input_buf");
#pragma HLS stream variable=bank_0_input_buf depth=32
#pragma HLS data_pack variable=bank_0_input_buf
  hls::stream<Data<ap_uint<256>>> bank_0_output_0_buf("bank_0_output_0_buf");
#pragma HLS stream variable=bank_0_output_0_buf depth=32
#pragma HLS data_pack variable=bank_0_output_0_buf

  hls::stream<Data<float>> from_super_source_to_input_offset_0("from_super_source_to_input_offset_0");
#pragma HLS stream variable=from_super_source_to_input_offset_0 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_0
  hls::stream<Data<float>> from_super_source_to_input_offset_1("from_super_source_to_input_offset_1");
#pragma HLS stream variable=from_super_source_to_input_offset_1 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_1
  hls::stream<Data<float>> from_super_source_to_input_offset_2("from_super_source_to_input_offset_2");
#pragma HLS stream variable=from_super_source_to_input_offset_2 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_2
  hls::stream<Data<float>> from_super_source_to_input_offset_3("from_super_source_to_input_offset_3");
#pragma HLS stream variable=from_super_source_to_input_offset_3 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_3
  hls::stream<Data<float>> from_super_source_to_input_offset_4("from_super_source_to_input_offset_4");
#pragma HLS stream variable=from_super_source_to_input_offset_4 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_4
  hls::stream<Data<float>> from_super_source_to_input_offset_5("from_super_source_to_input_offset_5");
#pragma HLS stream variable=from_super_source_to_input_offset_5 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_5
  hls::stream<Data<float>> from_super_source_to_input_offset_6("from_super_source_to_input_offset_6");
#pragma HLS stream variable=from_super_source_to_input_offset_6 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_6
  hls::stream<Data<float>> from_super_source_to_input_offset_7("from_super_source_to_input_offset_7");
#pragma HLS stream variable=from_super_source_to_input_offset_7 depth=2
#pragma HLS data_pack variable=from_super_source_to_input_offset_7
  hls::stream<Data<float>> from_input_offset_0_to_linear_0_pe_7("from_input_offset_0_to_linear_0_pe_7");
#pragma HLS stream variable=from_input_offset_0_to_linear_0_pe_7 depth=2
#pragma HLS data_pack variable=from_input_offset_0_to_linear_0_pe_7
  hls::stream<Data<float>> from_input_offset_1_to_linear_0_pe_6("from_input_offset_1_to_linear_0_pe_6");
#pragma HLS stream variable=from_input_offset_1_to_linear_0_pe_6 depth=2
#pragma HLS data_pack variable=from_input_offset_1_to_linear_0_pe_6
  hls::stream<Data<float>> from_input_offset_2_to_linear_0_pe_5("from_input_offset_2_to_linear_0_pe_5");
#pragma HLS stream variable=from_input_offset_2_to_linear_0_pe_5 depth=2
#pragma HLS data_pack variable=from_input_offset_2_to_linear_0_pe_5
  hls::stream<Data<float>> from_input_offset_3_to_linear_0_pe_4("from_input_offset_3_to_linear_0_pe_4");
#pragma HLS stream variable=from_input_offset_3_to_linear_0_pe_4 depth=2
#pragma HLS data_pack variable=from_input_offset_3_to_linear_0_pe_4
  hls::stream<Data<float>> from_input_offset_4_to_linear_0_pe_3("from_input_offset_4_to_linear_0_pe_3");
#pragma HLS stream variable=from_input_offset_4_to_linear_0_pe_3 depth=2
#pragma HLS data_pack variable=from_input_offset_4_to_linear_0_pe_3
  hls::stream<Data<float>> from_input_offset_5_to_linear_0_pe_2("from_input_offset_5_to_linear_0_pe_2");
#pragma HLS stream variable=from_input_offset_5_to_linear_0_pe_2 depth=2
#pragma HLS data_pack variable=from_input_offset_5_to_linear_0_pe_2
  hls::stream<Data<float>> from_input_offset_6_to_linear_0_pe_1("from_input_offset_6_to_linear_0_pe_1");
#pragma HLS stream variable=from_input_offset_6_to_linear_0_pe_1 depth=2
#pragma HLS data_pack variable=from_input_offset_6_to_linear_0_pe_1
  hls::stream<Data<float>> from_input_offset_7_to_linear_0_pe_0("from_input_offset_7_to_linear_0_pe_0");
#pragma HLS stream variable=from_input_offset_7_to_linear_0_pe_0 depth=2
#pragma HLS data_pack variable=from_input_offset_7_to_linear_0_pe_0
  hls::stream<Data<float>> from_linear_0_pe_7_to_linear_0_offset_0("from_linear_0_pe_7_to_linear_0_offset_0");
#pragma HLS stream variable=from_linear_0_pe_7_to_linear_0_offset_0 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_7_to_linear_0_offset_0
  hls::stream<Data<float>> from_linear_0_pe_6_to_linear_0_offset_1("from_linear_0_pe_6_to_linear_0_offset_1");
#pragma HLS stream variable=from_linear_0_pe_6_to_linear_0_offset_1 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_6_to_linear_0_offset_1
  hls::stream<Data<float>> from_linear_0_pe_5_to_linear_0_offset_2("from_linear_0_pe_5_to_linear_0_offset_2");
#pragma HLS stream variable=from_linear_0_pe_5_to_linear_0_offset_2 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_5_to_linear_0_offset_2
  hls::stream<Data<float>> from_linear_0_pe_4_to_linear_0_offset_3("from_linear_0_pe_4_to_linear_0_offset_3");
#pragma HLS stream variable=from_linear_0_pe_4_to_linear_0_offset_3 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_4_to_linear_0_offset_3
  hls::stream<Data<float>> from_linear_0_pe_3_to_linear_0_offset_4("from_linear_0_pe_3_to_linear_0_offset_4");
#pragma HLS stream variable=from_linear_0_pe_3_to_linear_0_offset_4 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_3_to_linear_0_offset_4
  hls::stream<Data<float>> from_linear_0_pe_2_to_linear_0_offset_5("from_linear_0_pe_2_to_linear_0_offset_5");
#pragma HLS stream variable=from_linear_0_pe_2_to_linear_0_offset_5 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_2_to_linear_0_offset_5
  hls::stream<Data<float>> from_linear_0_pe_1_to_linear_0_offset_6("from_linear_0_pe_1_to_linear_0_offset_6");
#pragma HLS stream variable=from_linear_0_pe_1_to_linear_0_offset_6 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_1_to_linear_0_offset_6
  hls::stream<Data<float>> from_linear_0_pe_0_to_linear_0_offset_7("from_linear_0_pe_0_to_linear_0_offset_7");
#pragma HLS stream variable=from_linear_0_pe_0_to_linear_0_offset_7 depth=2
#pragma HLS data_pack variable=from_linear_0_pe_0_to_linear_0_offset_7
  hls::stream<Data<float>> from_linear_0_offset_0_to_linear_0_offset_8("from_linear_0_offset_0_to_linear_0_offset_8");
#pragma HLS stream variable=from_linear_0_offset_0_to_linear_0_offset_8 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_0_to_linear_0_offset_8
  hls::stream<Data<float>> from_linear_0_offset_0_to_blur_x_0_pe_7("from_linear_0_offset_0_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_linear_0_offset_0_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_0_to_blur_x_0_pe_7
  hls::stream<Data<float>> from_linear_0_offset_1_to_linear_0_offset_9("from_linear_0_offset_1_to_linear_0_offset_9");
#pragma HLS stream variable=from_linear_0_offset_1_to_linear_0_offset_9 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_1_to_linear_0_offset_9
  hls::stream<Data<float>> from_linear_0_offset_1_to_blur_x_0_pe_7("from_linear_0_offset_1_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_linear_0_offset_1_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_1_to_blur_x_0_pe_7
  hls::stream<Data<float>> from_linear_0_offset_1_to_blur_x_0_pe_6("from_linear_0_offset_1_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_linear_0_offset_1_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_1_to_blur_x_0_pe_6
  hls::stream<Data<float>> from_linear_0_offset_2_to_blur_x_0_pe_7("from_linear_0_offset_2_to_blur_x_0_pe_7");
#pragma HLS stream variable=from_linear_0_offset_2_to_blur_x_0_pe_7 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_2_to_blur_x_0_pe_7
  hls::stream<Data<float>> from_linear_0_offset_2_to_blur_x_0_pe_6("from_linear_0_offset_2_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_linear_0_offset_2_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_2_to_blur_x_0_pe_6
  hls::stream<Data<float>> from_linear_0_offset_2_to_blur_x_0_pe_5("from_linear_0_offset_2_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_linear_0_offset_2_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_2_to_blur_x_0_pe_5
  hls::stream<Data<float>> from_linear_0_offset_3_to_blur_x_0_pe_6("from_linear_0_offset_3_to_blur_x_0_pe_6");
#pragma HLS stream variable=from_linear_0_offset_3_to_blur_x_0_pe_6 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_3_to_blur_x_0_pe_6
  hls::stream<Data<float>> from_linear_0_offset_3_to_blur_x_0_pe_5("from_linear_0_offset_3_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_linear_0_offset_3_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_3_to_blur_x_0_pe_5
  hls::stream<Data<float>> from_linear_0_offset_3_to_blur_x_0_pe_4("from_linear_0_offset_3_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_linear_0_offset_3_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_3_to_blur_x_0_pe_4
  hls::stream<Data<float>> from_linear_0_offset_4_to_blur_x_0_pe_5("from_linear_0_offset_4_to_blur_x_0_pe_5");
#pragma HLS stream variable=from_linear_0_offset_4_to_blur_x_0_pe_5 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_4_to_blur_x_0_pe_5
  hls::stream<Data<float>> from_linear_0_offset_4_to_blur_x_0_pe_4("from_linear_0_offset_4_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_linear_0_offset_4_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_4_to_blur_x_0_pe_4
  hls::stream<Data<float>> from_linear_0_offset_4_to_blur_x_0_pe_3("from_linear_0_offset_4_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_linear_0_offset_4_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_4_to_blur_x_0_pe_3
  hls::stream<Data<float>> from_linear_0_offset_5_to_blur_x_0_pe_4("from_linear_0_offset_5_to_blur_x_0_pe_4");
#pragma HLS stream variable=from_linear_0_offset_5_to_blur_x_0_pe_4 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_5_to_blur_x_0_pe_4
  hls::stream<Data<float>> from_linear_0_offset_5_to_blur_x_0_pe_3("from_linear_0_offset_5_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_linear_0_offset_5_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_5_to_blur_x_0_pe_3
  hls::stream<Data<float>> from_linear_0_offset_5_to_blur_x_0_pe_2("from_linear_0_offset_5_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_linear_0_offset_5_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_5_to_blur_x_0_pe_2
  hls::stream<Data<float>> from_linear_0_offset_6_to_blur_x_0_pe_3("from_linear_0_offset_6_to_blur_x_0_pe_3");
#pragma HLS stream variable=from_linear_0_offset_6_to_blur_x_0_pe_3 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_6_to_blur_x_0_pe_3
  hls::stream<Data<float>> from_linear_0_offset_6_to_blur_x_0_pe_2("from_linear_0_offset_6_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_linear_0_offset_6_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_6_to_blur_x_0_pe_2
  hls::stream<Data<float>> from_linear_0_offset_6_to_blur_x_0_pe_1("from_linear_0_offset_6_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_linear_0_offset_6_to_blur_x_0_pe_1 depth=4
#pragma HLS data_pack variable=from_linear_0_offset_6_to_blur_x_0_pe_1
  hls::stream<Data<float>> from_linear_0_offset_7_to_blur_x_0_pe_2("from_linear_0_offset_7_to_blur_x_0_pe_2");
#pragma HLS stream variable=from_linear_0_offset_7_to_blur_x_0_pe_2 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_7_to_blur_x_0_pe_2
  hls::stream<Data<float>> from_linear_0_offset_7_to_blur_x_0_pe_1("from_linear_0_offset_7_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_linear_0_offset_7_to_blur_x_0_pe_1 depth=4
#pragma HLS data_pack variable=from_linear_0_offset_7_to_blur_x_0_pe_1
  hls::stream<Data<float>> from_linear_0_offset_7_to_blur_x_0_pe_0("from_linear_0_offset_7_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_linear_0_offset_7_to_blur_x_0_pe_0 depth=4
#pragma HLS data_pack variable=from_linear_0_offset_7_to_blur_x_0_pe_0
  hls::stream<Data<float>> from_linear_0_offset_8_to_blur_x_0_pe_1("from_linear_0_offset_8_to_blur_x_0_pe_1");
#pragma HLS stream variable=from_linear_0_offset_8_to_blur_x_0_pe_1 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_8_to_blur_x_0_pe_1
  hls::stream<Data<float>> from_linear_0_offset_8_to_blur_x_0_pe_0("from_linear_0_offset_8_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_linear_0_offset_8_to_blur_x_0_pe_0 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_8_to_blur_x_0_pe_0
  hls::stream<Data<float>> from_blur_x_0_pe_7_to_blur_x_0_offset_0("from_blur_x_0_pe_7_to_blur_x_0_offset_0");
#pragma HLS stream variable=from_blur_x_0_pe_7_to_blur_x_0_offset_0 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_7_to_blur_x_0_offset_0
  hls::stream<Data<float>> from_linear_0_offset_9_to_blur_x_0_pe_0("from_linear_0_offset_9_to_blur_x_0_pe_0");
#pragma HLS stream variable=from_linear_0_offset_9_to_blur_x_0_pe_0 depth=2
#pragma HLS data_pack variable=from_linear_0_offset_9_to_blur_x_0_pe_0
  hls::stream<Data<float>> from_blur_x_0_pe_6_to_blur_x_0_offset_1("from_blur_x_0_pe_6_to_blur_x_0_offset_1");
#pragma HLS stream variable=from_blur_x_0_pe_6_to_blur_x_0_offset_1 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_6_to_blur_x_0_offset_1
  hls::stream<Data<float>> from_blur_x_0_pe_5_to_blur_x_0_offset_2("from_blur_x_0_pe_5_to_blur_x_0_offset_2");
#pragma HLS stream variable=from_blur_x_0_pe_5_to_blur_x_0_offset_2 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_5_to_blur_x_0_offset_2
  hls::stream<Data<float>> from_blur_x_0_pe_4_to_blur_x_0_offset_3("from_blur_x_0_pe_4_to_blur_x_0_offset_3");
#pragma HLS stream variable=from_blur_x_0_pe_4_to_blur_x_0_offset_3 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_4_to_blur_x_0_offset_3
  hls::stream<Data<float>> from_blur_x_0_pe_3_to_blur_x_0_offset_4("from_blur_x_0_pe_3_to_blur_x_0_offset_4");
#pragma HLS stream variable=from_blur_x_0_pe_3_to_blur_x_0_offset_4 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_3_to_blur_x_0_offset_4
  hls::stream<Data<float>> from_blur_x_0_pe_2_to_blur_x_0_offset_5("from_blur_x_0_pe_2_to_blur_x_0_offset_5");
#pragma HLS stream variable=from_blur_x_0_pe_2_to_blur_x_0_offset_5 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_2_to_blur_x_0_offset_5
  hls::stream<Data<float>> from_blur_x_0_pe_1_to_blur_x_0_offset_6("from_blur_x_0_pe_1_to_blur_x_0_offset_6");
#pragma HLS stream variable=from_blur_x_0_pe_1_to_blur_x_0_offset_6 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_1_to_blur_x_0_offset_6
  hls::stream<Data<float>> from_blur_x_0_pe_0_to_blur_x_0_offset_7("from_blur_x_0_pe_0_to_blur_x_0_offset_7");
#pragma HLS stream variable=from_blur_x_0_pe_0_to_blur_x_0_offset_7 depth=2
#pragma HLS data_pack variable=from_blur_x_0_pe_0_to_blur_x_0_offset_7
  hls::stream<Data<float>> from_blur_x_0_offset_0_to_blur_x_0_offset_768("from_blur_x_0_offset_0_to_blur_x_0_offset_768");
#pragma HLS stream variable=from_blur_x_0_offset_0_to_blur_x_0_offset_768 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_0_to_blur_x_0_offset_768
  hls::stream<Data<float>> from_blur_x_0_offset_0_to_output_0_pe_7("from_blur_x_0_offset_0_to_output_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_0_to_output_0_pe_7 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_0_to_output_0_pe_7
  hls::stream<Data<float>> from_blur_x_0_offset_1_to_blur_x_0_offset_769("from_blur_x_0_offset_1_to_blur_x_0_offset_769");
#pragma HLS stream variable=from_blur_x_0_offset_1_to_blur_x_0_offset_769 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1_to_blur_x_0_offset_769
  hls::stream<Data<float>> from_blur_x_0_offset_1_to_output_0_pe_6("from_blur_x_0_offset_1_to_output_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_1_to_output_0_pe_6 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_1_to_output_0_pe_6
  hls::stream<Data<float>> from_blur_x_0_offset_2_to_blur_x_0_offset_770("from_blur_x_0_offset_2_to_blur_x_0_offset_770");
#pragma HLS stream variable=from_blur_x_0_offset_2_to_blur_x_0_offset_770 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_2_to_blur_x_0_offset_770
  hls::stream<Data<float>> from_blur_x_0_offset_2_to_output_0_pe_5("from_blur_x_0_offset_2_to_output_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_2_to_output_0_pe_5 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_2_to_output_0_pe_5
  hls::stream<Data<float>> from_blur_x_0_offset_3_to_blur_x_0_offset_771("from_blur_x_0_offset_3_to_blur_x_0_offset_771");
#pragma HLS stream variable=from_blur_x_0_offset_3_to_blur_x_0_offset_771 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_3_to_blur_x_0_offset_771
  hls::stream<Data<float>> from_blur_x_0_offset_3_to_output_0_pe_4("from_blur_x_0_offset_3_to_output_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_3_to_output_0_pe_4 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_3_to_output_0_pe_4
  hls::stream<Data<float>> from_blur_x_0_offset_4_to_blur_x_0_offset_772("from_blur_x_0_offset_4_to_blur_x_0_offset_772");
#pragma HLS stream variable=from_blur_x_0_offset_4_to_blur_x_0_offset_772 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_4_to_blur_x_0_offset_772
  hls::stream<Data<float>> from_blur_x_0_offset_4_to_output_0_pe_3("from_blur_x_0_offset_4_to_output_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_4_to_output_0_pe_3 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_4_to_output_0_pe_3
  hls::stream<Data<float>> from_blur_x_0_offset_5_to_blur_x_0_offset_773("from_blur_x_0_offset_5_to_blur_x_0_offset_773");
#pragma HLS stream variable=from_blur_x_0_offset_5_to_blur_x_0_offset_773 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_5_to_blur_x_0_offset_773
  hls::stream<Data<float>> from_blur_x_0_offset_5_to_output_0_pe_2("from_blur_x_0_offset_5_to_output_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_5_to_output_0_pe_2 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_5_to_output_0_pe_2
  hls::stream<Data<float>> from_blur_x_0_offset_6_to_blur_x_0_offset_774("from_blur_x_0_offset_6_to_blur_x_0_offset_774");
#pragma HLS stream variable=from_blur_x_0_offset_6_to_blur_x_0_offset_774 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_6_to_blur_x_0_offset_774
  hls::stream<Data<float>> from_blur_x_0_offset_6_to_output_0_pe_1("from_blur_x_0_offset_6_to_output_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_6_to_output_0_pe_1 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_6_to_output_0_pe_1
  hls::stream<Data<float>> from_blur_x_0_offset_7_to_blur_x_0_offset_775("from_blur_x_0_offset_7_to_blur_x_0_offset_775");
#pragma HLS stream variable=from_blur_x_0_offset_7_to_blur_x_0_offset_775 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_7_to_blur_x_0_offset_775
  hls::stream<Data<float>> from_blur_x_0_offset_7_to_output_0_pe_0("from_blur_x_0_offset_7_to_output_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_7_to_output_0_pe_0 depth=6
#pragma HLS data_pack variable=from_blur_x_0_offset_7_to_output_0_pe_0
  hls::stream<Data<float>> from_blur_x_0_offset_768_to_blur_x_0_offset_1536("from_blur_x_0_offset_768_to_blur_x_0_offset_1536");
#pragma HLS stream variable=from_blur_x_0_offset_768_to_blur_x_0_offset_1536 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_768_to_blur_x_0_offset_1536
  hls::stream<Data<float>> from_blur_x_0_offset_768_to_output_0_pe_7("from_blur_x_0_offset_768_to_output_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_768_to_output_0_pe_7 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_768_to_output_0_pe_7
  hls::stream<Data<float>> from_blur_x_0_offset_769_to_blur_x_0_offset_1537("from_blur_x_0_offset_769_to_blur_x_0_offset_1537");
#pragma HLS stream variable=from_blur_x_0_offset_769_to_blur_x_0_offset_1537 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_769_to_blur_x_0_offset_1537
  hls::stream<Data<float>> from_blur_x_0_offset_769_to_output_0_pe_6("from_blur_x_0_offset_769_to_output_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_769_to_output_0_pe_6 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_769_to_output_0_pe_6
  hls::stream<Data<float>> from_blur_x_0_offset_770_to_blur_x_0_offset_1538("from_blur_x_0_offset_770_to_blur_x_0_offset_1538");
#pragma HLS stream variable=from_blur_x_0_offset_770_to_blur_x_0_offset_1538 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_770_to_blur_x_0_offset_1538
  hls::stream<Data<float>> from_blur_x_0_offset_770_to_output_0_pe_5("from_blur_x_0_offset_770_to_output_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_770_to_output_0_pe_5 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_770_to_output_0_pe_5
  hls::stream<Data<float>> from_blur_x_0_offset_771_to_blur_x_0_offset_1539("from_blur_x_0_offset_771_to_blur_x_0_offset_1539");
#pragma HLS stream variable=from_blur_x_0_offset_771_to_blur_x_0_offset_1539 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_771_to_blur_x_0_offset_1539
  hls::stream<Data<float>> from_blur_x_0_offset_771_to_output_0_pe_4("from_blur_x_0_offset_771_to_output_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_771_to_output_0_pe_4 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_771_to_output_0_pe_4
  hls::stream<Data<float>> from_blur_x_0_offset_772_to_blur_x_0_offset_1540("from_blur_x_0_offset_772_to_blur_x_0_offset_1540");
#pragma HLS stream variable=from_blur_x_0_offset_772_to_blur_x_0_offset_1540 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_772_to_blur_x_0_offset_1540
  hls::stream<Data<float>> from_blur_x_0_offset_772_to_output_0_pe_3("from_blur_x_0_offset_772_to_output_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_772_to_output_0_pe_3 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_772_to_output_0_pe_3
  hls::stream<Data<float>> from_blur_x_0_offset_773_to_blur_x_0_offset_1541("from_blur_x_0_offset_773_to_blur_x_0_offset_1541");
#pragma HLS stream variable=from_blur_x_0_offset_773_to_blur_x_0_offset_1541 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_773_to_blur_x_0_offset_1541
  hls::stream<Data<float>> from_blur_x_0_offset_773_to_output_0_pe_2("from_blur_x_0_offset_773_to_output_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_773_to_output_0_pe_2 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_773_to_output_0_pe_2
  hls::stream<Data<float>> from_blur_x_0_offset_774_to_blur_x_0_offset_1542("from_blur_x_0_offset_774_to_blur_x_0_offset_1542");
#pragma HLS stream variable=from_blur_x_0_offset_774_to_blur_x_0_offset_1542 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_774_to_blur_x_0_offset_1542
  hls::stream<Data<float>> from_blur_x_0_offset_774_to_output_0_pe_1("from_blur_x_0_offset_774_to_output_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_774_to_output_0_pe_1 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_774_to_output_0_pe_1
  hls::stream<Data<float>> from_blur_x_0_offset_775_to_blur_x_0_offset_1543("from_blur_x_0_offset_775_to_blur_x_0_offset_1543");
#pragma HLS stream variable=from_blur_x_0_offset_775_to_blur_x_0_offset_1543 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_775_to_blur_x_0_offset_1543
  hls::stream<Data<float>> from_blur_x_0_offset_775_to_output_0_pe_0("from_blur_x_0_offset_775_to_output_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_775_to_output_0_pe_0 depth=4
#pragma HLS data_pack variable=from_blur_x_0_offset_775_to_output_0_pe_0
  hls::stream<Data<float>> from_blur_x_0_offset_1536_to_output_0_pe_7("from_blur_x_0_offset_1536_to_output_0_pe_7");
#pragma HLS stream variable=from_blur_x_0_offset_1536_to_output_0_pe_7 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1536_to_output_0_pe_7
  hls::stream<Data<float>> from_output_0_pe_7_to_super_sink("from_output_0_pe_7_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_7_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_7_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1537_to_output_0_pe_6("from_blur_x_0_offset_1537_to_output_0_pe_6");
#pragma HLS stream variable=from_blur_x_0_offset_1537_to_output_0_pe_6 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1537_to_output_0_pe_6
  hls::stream<Data<float>> from_output_0_pe_6_to_super_sink("from_output_0_pe_6_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_6_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_6_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1538_to_output_0_pe_5("from_blur_x_0_offset_1538_to_output_0_pe_5");
#pragma HLS stream variable=from_blur_x_0_offset_1538_to_output_0_pe_5 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1538_to_output_0_pe_5
  hls::stream<Data<float>> from_output_0_pe_5_to_super_sink("from_output_0_pe_5_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_5_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_5_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1539_to_output_0_pe_4("from_blur_x_0_offset_1539_to_output_0_pe_4");
#pragma HLS stream variable=from_blur_x_0_offset_1539_to_output_0_pe_4 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1539_to_output_0_pe_4
  hls::stream<Data<float>> from_output_0_pe_4_to_super_sink("from_output_0_pe_4_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_4_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_4_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1540_to_output_0_pe_3("from_blur_x_0_offset_1540_to_output_0_pe_3");
#pragma HLS stream variable=from_blur_x_0_offset_1540_to_output_0_pe_3 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1540_to_output_0_pe_3
  hls::stream<Data<float>> from_output_0_pe_3_to_super_sink("from_output_0_pe_3_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_3_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_3_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1541_to_output_0_pe_2("from_blur_x_0_offset_1541_to_output_0_pe_2");
#pragma HLS stream variable=from_blur_x_0_offset_1541_to_output_0_pe_2 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1541_to_output_0_pe_2
  hls::stream<Data<float>> from_output_0_pe_2_to_super_sink("from_output_0_pe_2_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_2_to_super_sink depth=4
#pragma HLS data_pack variable=from_output_0_pe_2_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1542_to_output_0_pe_1("from_blur_x_0_offset_1542_to_output_0_pe_1");
#pragma HLS stream variable=from_blur_x_0_offset_1542_to_output_0_pe_1 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1542_to_output_0_pe_1
  hls::stream<Data<float>> from_output_0_pe_1_to_super_sink("from_output_0_pe_1_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_1_to_super_sink depth=2
#pragma HLS data_pack variable=from_output_0_pe_1_to_super_sink
  hls::stream<Data<float>> from_blur_x_0_offset_1543_to_output_0_pe_0("from_blur_x_0_offset_1543_to_output_0_pe_0");
#pragma HLS stream variable=from_blur_x_0_offset_1543_to_output_0_pe_0 depth=2
#pragma HLS data_pack variable=from_blur_x_0_offset_1543_to_output_0_pe_0
  hls::stream<Data<float>> from_output_0_pe_0_to_super_sink("from_output_0_pe_0_to_super_sink");
#pragma HLS stream variable=from_output_0_pe_0_to_super_sink depth=2
#pragma HLS data_pack variable=from_output_0_pe_0_to_super_sink

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
    /* input*/ &bank_0_input_buf);
  Module1Func(
    /*output*/ &from_input_offset_0_to_linear_0_pe_7, 
    /* input*/ &from_super_source_to_input_offset_0);
  Module1Func(
    /*output*/ &from_input_offset_1_to_linear_0_pe_6, 
    /* input*/ &from_super_source_to_input_offset_1);
  Module1Func(
    /*output*/ &from_input_offset_2_to_linear_0_pe_5, 
    /* input*/ &from_super_source_to_input_offset_2);
  Module1Func(
    /*output*/ &from_input_offset_3_to_linear_0_pe_4, 
    /* input*/ &from_super_source_to_input_offset_3);
  Module1Func(
    /*output*/ &from_input_offset_4_to_linear_0_pe_3, 
    /* input*/ &from_super_source_to_input_offset_4);
  Module1Func(
    /*output*/ &from_input_offset_5_to_linear_0_pe_2, 
    /* input*/ &from_super_source_to_input_offset_5);
  Module1Func(
    /*output*/ &from_input_offset_6_to_linear_0_pe_1, 
    /* input*/ &from_super_source_to_input_offset_6);
  Module1Func(
    /*output*/ &from_input_offset_7_to_linear_0_pe_0, 
    /* input*/ &from_super_source_to_input_offset_7);
  Module2Func(
    /*output*/ &from_linear_0_pe_7_to_linear_0_offset_0, 
    /* input*/ &from_input_offset_0_to_linear_0_pe_7);
  Module2Func(
    /*output*/ &from_linear_0_pe_6_to_linear_0_offset_1, 
    /* input*/ &from_input_offset_1_to_linear_0_pe_6);
  Module2Func(
    /*output*/ &from_linear_0_pe_5_to_linear_0_offset_2, 
    /* input*/ &from_input_offset_2_to_linear_0_pe_5);
  Module2Func(
    /*output*/ &from_linear_0_pe_4_to_linear_0_offset_3, 
    /* input*/ &from_input_offset_3_to_linear_0_pe_4);
  Module2Func(
    /*output*/ &from_linear_0_pe_3_to_linear_0_offset_4, 
    /* input*/ &from_input_offset_4_to_linear_0_pe_3);
  Module2Func(
    /*output*/ &from_linear_0_pe_2_to_linear_0_offset_5, 
    /* input*/ &from_input_offset_5_to_linear_0_pe_2);
  Module2Func(
    /*output*/ &from_linear_0_pe_1_to_linear_0_offset_6, 
    /* input*/ &from_input_offset_6_to_linear_0_pe_1);
  Module2Func(
    /*output*/ &from_linear_0_pe_0_to_linear_0_offset_7, 
    /* input*/ &from_input_offset_7_to_linear_0_pe_0);
  Module3Func(
    /*output*/ &from_linear_0_offset_0_to_linear_0_offset_8, 
    /*output*/ &from_linear_0_offset_0_to_blur_x_0_pe_7, 
    /* input*/ &from_linear_0_pe_7_to_linear_0_offset_0);
  Module4Func(
    /*output*/ &from_linear_0_offset_1_to_linear_0_offset_9, 
    /*output*/ &from_linear_0_offset_1_to_blur_x_0_pe_7, 
    /*output*/ &from_linear_0_offset_1_to_blur_x_0_pe_6, 
    /* input*/ &from_linear_0_pe_6_to_linear_0_offset_1);
  Module4Func(
    /*output*/ &from_linear_0_offset_2_to_blur_x_0_pe_7, 
    /*output*/ &from_linear_0_offset_2_to_blur_x_0_pe_6, 
    /*output*/ &from_linear_0_offset_2_to_blur_x_0_pe_5, 
    /* input*/ &from_linear_0_pe_5_to_linear_0_offset_2);
  Module4Func(
    /*output*/ &from_linear_0_offset_3_to_blur_x_0_pe_6, 
    /*output*/ &from_linear_0_offset_3_to_blur_x_0_pe_5, 
    /*output*/ &from_linear_0_offset_3_to_blur_x_0_pe_4, 
    /* input*/ &from_linear_0_pe_4_to_linear_0_offset_3);
  Module4Func(
    /*output*/ &from_linear_0_offset_4_to_blur_x_0_pe_5, 
    /*output*/ &from_linear_0_offset_4_to_blur_x_0_pe_4, 
    /*output*/ &from_linear_0_offset_4_to_blur_x_0_pe_3, 
    /* input*/ &from_linear_0_pe_3_to_linear_0_offset_4);
  Module4Func(
    /*output*/ &from_linear_0_offset_5_to_blur_x_0_pe_4, 
    /*output*/ &from_linear_0_offset_5_to_blur_x_0_pe_3, 
    /*output*/ &from_linear_0_offset_5_to_blur_x_0_pe_2, 
    /* input*/ &from_linear_0_pe_2_to_linear_0_offset_5);
  Module4Func(
    /*output*/ &from_linear_0_offset_6_to_blur_x_0_pe_3, 
    /*output*/ &from_linear_0_offset_6_to_blur_x_0_pe_2, 
    /*output*/ &from_linear_0_offset_6_to_blur_x_0_pe_1, 
    /* input*/ &from_linear_0_pe_1_to_linear_0_offset_6);
  Module4Func(
    /*output*/ &from_linear_0_offset_7_to_blur_x_0_pe_2, 
    /*output*/ &from_linear_0_offset_7_to_blur_x_0_pe_1, 
    /*output*/ &from_linear_0_offset_7_to_blur_x_0_pe_0, 
    /* input*/ &from_linear_0_pe_0_to_linear_0_offset_7);
  Module5Func(
    /*output*/ &from_linear_0_offset_8_to_blur_x_0_pe_1, 
    /*output*/ &from_linear_0_offset_8_to_blur_x_0_pe_0, 
    /* input*/ &from_linear_0_offset_0_to_linear_0_offset_8);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_7_to_blur_x_0_offset_0, 
    /* input*/ &from_linear_0_offset_0_to_blur_x_0_pe_7, 
    /* input*/ &from_linear_0_offset_2_to_blur_x_0_pe_7, 
    /* input*/ &from_linear_0_offset_1_to_blur_x_0_pe_7);
  Module7Func(
    /*output*/ &from_linear_0_offset_9_to_blur_x_0_pe_0, 
    /* input*/ &from_linear_0_offset_1_to_linear_0_offset_9);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_6_to_blur_x_0_offset_1, 
    /* input*/ &from_linear_0_offset_1_to_blur_x_0_pe_6, 
    /* input*/ &from_linear_0_offset_3_to_blur_x_0_pe_6, 
    /* input*/ &from_linear_0_offset_2_to_blur_x_0_pe_6);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_5_to_blur_x_0_offset_2, 
    /* input*/ &from_linear_0_offset_2_to_blur_x_0_pe_5, 
    /* input*/ &from_linear_0_offset_4_to_blur_x_0_pe_5, 
    /* input*/ &from_linear_0_offset_3_to_blur_x_0_pe_5);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_4_to_blur_x_0_offset_3, 
    /* input*/ &from_linear_0_offset_3_to_blur_x_0_pe_4, 
    /* input*/ &from_linear_0_offset_5_to_blur_x_0_pe_4, 
    /* input*/ &from_linear_0_offset_4_to_blur_x_0_pe_4);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_3_to_blur_x_0_offset_4, 
    /* input*/ &from_linear_0_offset_4_to_blur_x_0_pe_3, 
    /* input*/ &from_linear_0_offset_6_to_blur_x_0_pe_3, 
    /* input*/ &from_linear_0_offset_5_to_blur_x_0_pe_3);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_2_to_blur_x_0_offset_5, 
    /* input*/ &from_linear_0_offset_5_to_blur_x_0_pe_2, 
    /* input*/ &from_linear_0_offset_7_to_blur_x_0_pe_2, 
    /* input*/ &from_linear_0_offset_6_to_blur_x_0_pe_2);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_1_to_blur_x_0_offset_6, 
    /* input*/ &from_linear_0_offset_6_to_blur_x_0_pe_1, 
    /* input*/ &from_linear_0_offset_8_to_blur_x_0_pe_1, 
    /* input*/ &from_linear_0_offset_7_to_blur_x_0_pe_1);
  Module6Func(
    /*output*/ &from_blur_x_0_pe_0_to_blur_x_0_offset_7, 
    /* input*/ &from_linear_0_offset_7_to_blur_x_0_pe_0, 
    /* input*/ &from_linear_0_offset_9_to_blur_x_0_pe_0, 
    /* input*/ &from_linear_0_offset_8_to_blur_x_0_pe_0);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_0_to_blur_x_0_offset_768, 
    /*output*/ &from_blur_x_0_offset_0_to_output_0_pe_7, 
    /* input*/ &from_blur_x_0_pe_7_to_blur_x_0_offset_0);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_1_to_blur_x_0_offset_769, 
    /*output*/ &from_blur_x_0_offset_1_to_output_0_pe_6, 
    /* input*/ &from_blur_x_0_pe_6_to_blur_x_0_offset_1);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_2_to_blur_x_0_offset_770, 
    /*output*/ &from_blur_x_0_offset_2_to_output_0_pe_5, 
    /* input*/ &from_blur_x_0_pe_5_to_blur_x_0_offset_2);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_3_to_blur_x_0_offset_771, 
    /*output*/ &from_blur_x_0_offset_3_to_output_0_pe_4, 
    /* input*/ &from_blur_x_0_pe_4_to_blur_x_0_offset_3);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_4_to_blur_x_0_offset_772, 
    /*output*/ &from_blur_x_0_offset_4_to_output_0_pe_3, 
    /* input*/ &from_blur_x_0_pe_3_to_blur_x_0_offset_4);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_5_to_blur_x_0_offset_773, 
    /*output*/ &from_blur_x_0_offset_5_to_output_0_pe_2, 
    /* input*/ &from_blur_x_0_pe_2_to_blur_x_0_offset_5);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_6_to_blur_x_0_offset_774, 
    /*output*/ &from_blur_x_0_offset_6_to_output_0_pe_1, 
    /* input*/ &from_blur_x_0_pe_1_to_blur_x_0_offset_6);
  Module3Func(
    /*output*/ &from_blur_x_0_offset_7_to_blur_x_0_offset_775, 
    /*output*/ &from_blur_x_0_offset_7_to_output_0_pe_0, 
    /* input*/ &from_blur_x_0_pe_0_to_blur_x_0_offset_7);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_768_to_blur_x_0_offset_1536, 
    /*output*/ &from_blur_x_0_offset_768_to_output_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_0_to_blur_x_0_offset_768);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_769_to_blur_x_0_offset_1537, 
    /*output*/ &from_blur_x_0_offset_769_to_output_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_1_to_blur_x_0_offset_769);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_770_to_blur_x_0_offset_1538, 
    /*output*/ &from_blur_x_0_offset_770_to_output_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_2_to_blur_x_0_offset_770);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_771_to_blur_x_0_offset_1539, 
    /*output*/ &from_blur_x_0_offset_771_to_output_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_3_to_blur_x_0_offset_771);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_772_to_blur_x_0_offset_1540, 
    /*output*/ &from_blur_x_0_offset_772_to_output_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_4_to_blur_x_0_offset_772);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_773_to_blur_x_0_offset_1541, 
    /*output*/ &from_blur_x_0_offset_773_to_output_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_5_to_blur_x_0_offset_773);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_774_to_blur_x_0_offset_1542, 
    /*output*/ &from_blur_x_0_offset_774_to_output_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_6_to_blur_x_0_offset_774);
  Module8Func(
    /*output*/ &from_blur_x_0_offset_775_to_blur_x_0_offset_1543, 
    /*output*/ &from_blur_x_0_offset_775_to_output_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_7_to_blur_x_0_offset_775);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1536_to_output_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_768_to_blur_x_0_offset_1536);
  Module10Func(
    /*output*/ &from_output_0_pe_7_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_0_to_output_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_1536_to_output_0_pe_7, 
    /* input*/ &from_blur_x_0_offset_768_to_output_0_pe_7);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1537_to_output_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_769_to_blur_x_0_offset_1537);
  Module10Func(
    /*output*/ &from_output_0_pe_6_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_1_to_output_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_1537_to_output_0_pe_6, 
    /* input*/ &from_blur_x_0_offset_769_to_output_0_pe_6);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1538_to_output_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_770_to_blur_x_0_offset_1538);
  Module10Func(
    /*output*/ &from_output_0_pe_5_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_2_to_output_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_1538_to_output_0_pe_5, 
    /* input*/ &from_blur_x_0_offset_770_to_output_0_pe_5);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1539_to_output_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_771_to_blur_x_0_offset_1539);
  Module10Func(
    /*output*/ &from_output_0_pe_4_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_3_to_output_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_1539_to_output_0_pe_4, 
    /* input*/ &from_blur_x_0_offset_771_to_output_0_pe_4);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1540_to_output_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_772_to_blur_x_0_offset_1540);
  Module10Func(
    /*output*/ &from_output_0_pe_3_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_4_to_output_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_1540_to_output_0_pe_3, 
    /* input*/ &from_blur_x_0_offset_772_to_output_0_pe_3);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1541_to_output_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_773_to_blur_x_0_offset_1541);
  Module10Func(
    /*output*/ &from_output_0_pe_2_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_5_to_output_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_1541_to_output_0_pe_2, 
    /* input*/ &from_blur_x_0_offset_773_to_output_0_pe_2);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1542_to_output_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_774_to_blur_x_0_offset_1542);
  Module10Func(
    /*output*/ &from_output_0_pe_1_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_6_to_output_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_1542_to_output_0_pe_1, 
    /* input*/ &from_blur_x_0_offset_774_to_output_0_pe_1);
  Module9Func(
    /*output*/ &from_blur_x_0_offset_1543_to_output_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_775_to_blur_x_0_offset_1543);
  Module10Func(
    /*output*/ &from_output_0_pe_0_to_super_sink, 
    /* input*/ &from_blur_x_0_offset_7_to_output_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_1543_to_output_0_pe_0, 
    /* input*/ &from_blur_x_0_offset_775_to_output_0_pe_0);
  Module11Func(
    /*output*/ &bank_0_output_0_buf, 
    /* input*/ &from_output_0_pe_7_to_super_sink, 
    /* input*/ &from_output_0_pe_6_to_super_sink, 
    /* input*/ &from_output_0_pe_5_to_super_sink, 
    /* input*/ &from_output_0_pe_4_to_super_sink, 
    /* input*/ &from_output_0_pe_3_to_super_sink, 
    /* input*/ &from_output_0_pe_2_to_super_sink, 
    /* input*/ &from_output_0_pe_1_to_super_sink, 
    /* input*/ &from_output_0_pe_0_to_super_sink);
  BurstWrite(bank_0_output_0, &bank_0_output_0_buf, coalesced_data_num);
}

}//extern "C"
