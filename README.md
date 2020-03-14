# HeteroHalide
## Steps to run experiments (Blur as an example)
### Generate HeteroCL code
```
cd new/blur
bash run_blur.sh
```
Above commands are to compile HeteroHalide and then use Halide code `blur.cpp` to generate HeteroCL code `blur_gen.py`

### To verify the results
Generate input for both Halide code and HeteroCL code. Run each program and generate the output results. After that, we compare them to check if the output results are the same. 
```
python3 input_generator.py
```
Then run Halide code and HeteroCL code. After that, compare the output results. 
```
python3 compare.py
```

### Generate SODA code
The last sentence in HeteroCL code: `print(hcl.build(s, target = "soda"))` will print corresponding SODA code. Copy the code block and save it into a file `blur_gen.soda`
```
python3 blur_gen.py
```
### Obtain HLS report using Xilinx flow
Board: Xilinx VU9P
```
bash hls_report_soda_xilinx.sh
```

### Obtain HLS report using Intel flow
Board: Intel S10MX
```
bash hls_report_soda_intel.sh
```
Above commands first use SODA code to generate Intel OpenCL code `blur_intel_soda.cl`, and then compile the Intel FPGA OpenCL code and obtain the HLS report. The report is saved in the folder `blur_intel_soda`