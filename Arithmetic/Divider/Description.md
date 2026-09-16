## Divider

* **Description**

    This design performs division by iteratively comparing segments of a 16-bit dividend with an 8-bit divisor. It calculates the quotient and remainder by shifting and subtracting bits until the process is complete.

* **Input Type**

    input_x: sc_uint<8> & sc_uint<16>

* **Output Type**

    o_result: sc_uint<16>

* **Lines of Code (Dut)**

    69 lines

* **Source**

    Y. Lu, S. Liu, Q. Zhang and Z. Xie, "RTLLM: An Open-Source Benchmark for Design RTL Generation with Large Language Model," 2024 29th Asia and South Pacific Design Automation Conference (ASP-DAC), Incheon, Korea, Republic of, 2024, pp. 722-727, doi: 10.1109/ASP-DAC58780.2024.10473904. keywords: {Design automation;Natural languages;Asia;Benchmark testing;Syntactics;Chatbots;Hardware},
