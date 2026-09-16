## m06

* **Description**

    This design arbitrates four request signals by managing them in an internal queue. It sequentially issues grants based on arrival order and outputs a specific identifier for each active request.

* **Input Type**

    input_x: bool

* **Output Type**

    o_result: sc_uint<4>

* **Lines of Code (Dut)**

    148 lines

* **Source**

    R. Ma et al., "VerilogReader: LLM-Aided Hardware Test Generation," 2024 IEEE LLM Aided Design Workshop (LAD), San Jose, CA, USA, 2024, pp. 1-5, doi: 10.1109/LAD62341.2024.10691801. keywords: {Codes;Large language models;Conferences;Benchmark testing;Hardware;Test pattern generators;Prompt engineering;Logic;Hardware design languages;Optimization;Automatic Test Generation;LLM;Verilog},