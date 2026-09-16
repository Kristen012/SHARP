## SHARP description
Our SystemC HLS Benchmark Suite contains a total of 76 SystemC designs, all designed to support standard simulation and synthesis flows for SystemC HLS. This ensures reliability and usability within realistic ASIC design environments. The suite includes handwritten, LLM-generated, and adapted designs from sources such as S2CBench [1], RTLLM [2], HDLBits [3] and VerilogReader [4]. These designs collectively span a broad range of computation patterns, including basic arithmetic, cryptography, fundamental machine-learning operations, and a diverse set of control-dominant examples such as multiple FSMs, basic logic circuits, and algorithm-driven control structures.

Our benchmark suite provides a well-structured testbench environment with input readers, output monitors, and self-checking logic. Every benchmark includes predefined input patterns and reference outputs for reproducible coverage measurement, with inputs randomly generated under specific structural constraints. All designs have been successfully simulated and synthesized using a commercial HLS tool as part of our evaluation flow. These data provide baseline metrics for evaluation, optimization, and comparison.

## Citation
If you use the code, we would appreciate it if you cite the following paper:
```
Y.-W. Tang, S.-T. Sung, F.-Y. Hsu, W.-Y. Teng, H.-Y. Kao, J.-J. Liou and C.-T. Huang, "SHARP: A Comprehensive SystemC HLS Benchmark Suite and LLM-Assisted Testbench Optimization for RTL Coverage Closure," 2026 International VLSI Symposium on Technology, Systems and Applications (VLSI TSA), Hsinchu, Taiwan, 2026, pp. 1-4.
```

## License and Warranty​

Unless otherwise specified, the original content developed for this repository
is licensed under the MIT License. See [LICENSE](LICENSE) for details.

Some benchmark designs in this repository are adapted from third-party
sources, including S2CBench, RTLLM, HDLBits, and VerilogReader. These sources are
identified in the benchmark table and references below. Materials derived from
third-party sources remain subject to the applicable terms and conditions of
their respective original sources.

No warranty is provided regarding the correctness, completeness,
reliability, or fitness of this benchmark for any particular purpose.
This benchmark is provided "AS IS", without warranty of any kind,
express or implied.

## Benchmark
| Design Category | Design Name | Lines of Code (Dut) | Input Type | Output Type | Design Reference |
| :--- | :--- | :--- | :--- | :--- | :--- |
| AlgorithmicControl | FindGCD | 53 | sc_uint<8> | sc_uint<8> | Classical Algorithm |
| Arithmetic | Ave8 | 53 | sc_uint<8> | sc_uint<8> | S2CBench [1] |
|  | Divider | 69 | sc_uint<8> & sc_uint<16> | sc_uint<16> | RTLLM [2] |
|  | Shift | 41 | sc_uint<8> | sc_uint<8> | Classical Algorithm |
|  | Vadd | 51 | int | int | Classical Algorithm |
| Cryptography | Aes | 232 | sc_uint<8> | sc_uint<8> | S2CBench [1] |
| FiniteStateMachines | DesignAMooreFSM | 124 | sc_uint<8> | bool | HDLBits [3] |
| | FSMOneHotLogicEquations | 105 | bool | bool | HDLBits [3] |
| | FSMSequence1101Recognizer | 71 | bool | bool | HDLBits [3] |
| | FSMTheCompleteFSM | 109 | bool | bool | HDLBits [3] |
| | Fsm | 71 | bool | bool | RTLLM [2] |
| | Lemmings1 | 68 | bool | bool | HDLBits [3] |
| | Lemmings2 | 85 | bool | bool | HDLBits [3] |
| | Lemmings3 | 102 | bool | bool | HDLBits [3] |
| | Lemmings4 | 118 | bool | bool | HDLBits [3] |
| | OneHotFSM | 83 | bool & sc_uint<10> | bool & sc_uint<10> | HDLBits [3] |
| | PS2PacketParser | 67 | sc_uint<8> | bool | HDLBits [3] |
| | PS2PacketParserAndDatapath | 82 | sc_uint<8> | bool & sc_uint<24> | HDLBits [3] |
| | Q2A_2012 | 75 | bool | bool | HDLBits [3] |
| | Q2A_2013 | 104 | sc_uint<3> | sc_uint<3> | HDLBits [3] |
| | Q2B_2013 | 107 | bool | bool | HDLBits [3] |
| | Q3A | 89 | bool | bool | HDLBits [3] |
| | Q3B | 71 | bool | bool | HDLBits [3] |
| | Q3C | 81 | bool & sc_uint<3> | bool | HDLBits [3] |
| | Q5A | 63 | bool | bool | HDLBits [3] |
| | Q5B | 59 | bool | bool | HDLBits [3] |
| | Q6 | 75 | bool | bool | HDLBits [3] |
| | Q8 | 63 | bool | bool | HDLBits [3] |
| | Sequence_detector | 62 | bool | bool | RTLLM [2] |
| | SequenceRecognition | 102 | bool | bool | HDLBits [3] |
| | SerialReceiver | 99 | bool | bool | HDLBits [3] |
| | SerialReceiverAndDatapath | 114 | bool | bool & sc_uint<8> | HDLBits [3] |
| | SerialReceiverWithParityChecking | 128 | bool | bool & sc_uint<8> | HDLBits [3] |
| | SimpleFSM1 | 59 | bool | bool | HDLBits [3] |
| | SimpleFSM2 | 63 | bool | bool | HDLBits [3] |
| | SimpleStateTransition3 | 67 | bool | bool | HDLBits [3] |
| | b01 | 107 | bool | int | Llm_hardware_test_generation [4] |
| | b02 | 132 | bool | int | Llm_hardware_test_generation [4] |
| | b03 | 155 | bool | int | Llm_hardware_test_generation [4] |
| | b04 | 205 | bool | int | Llm_hardware_test_generation [4] |
| | b05 | 258 | bool | int | Llm_hardware_test_generation [4] |
| | b06 | 458 | bool | int | Llm_hardware_test_generation [4] |
| | m02 | 94 | bool | bool | Llm_hardware_test_generation [4] |
| | m03 | 131 | bool | bool | Llm_hardware_test_generation [4] |
| | m06 | 148 | bool | sc_uint<4> | Llm_hardware_test_generation [4] |
| ImageProcessing | LoGFilter | 264 | sc_uint<8> | sc_uint<8> | Classical Algorithm |
| LogicCircuits | 4BitShiftRegisterAndDownCounter | 61 | bool | sc_uint<4> | HDLBits [3] |
| | Q2B_2012 | 54 | sc_uint<6> & bool | bool | HDLBits [3] |
| | Q6B | 49 | sc_uint<3> & bool | bool | HDLBits [3] |
| | Q6C | 54 | sc_uint<6> & bool | bool | HDLBits [3] |
| | s01 | 69 | bool | int | Llm_hardware_test_generation [4] |
| | s03 | 63 | bool | int | Llm_hardware_test_generation [4] |
| Machine Learning | GELU | 46 | float(8, 23) | float(8, 23) | Classical Algorithm |
| | GEMM | 80 | int | int | Classical Algorithm |
| | Layernorm | 60 | float(8, 23) | float(8, 23) | Classical Algorithm |
| | Layernorm_fixed | 60 | fixed(8, 23) | fixed(8, 23) | Classical Algorithm |
| | ReLU | 47 | int | int | Classical Algorithm |
| | SiLU | 40 | float(8, 23) | float(8, 23) | Classical Algorithm |
| | Softmax_fixed | 87 | fixed(8, 23) | fixed(8, 23) | Classical Algorithm |
| Matrix&LinearAlgebra | Cholesky | 70 | fixed(8, 23) | fixed(8, 23) | S2CBench [1] |
| Sorting&DataProcessing | ArrayRotation | 64 | unsigned int | unsigned int | Classical Algorithm |
| | Clustering | 116 | sc_uint<16> | sc_uint<16> | Classical Algorithm |
| | MergeSort | 90 | sc_uint<8> | sc_uint<8> | Classical Algorithm |
| | Qsort | 83 | sc_uint<8> | sc_uint<8> | S2CBench [1] |

[1] B. C. Schafer and A. Mahapatra, "S2CBench: Synthesizable SystemC Benchmark Suite for High-Level Synthesis," in IEEE Embedded Systems Letters, vol. 6, no. 3, pp. 53-56, Sept. 2014, doi: 10.1109/LES.2014.2320556. keywords: {Benchmark testing;Arrays;Optimization;Algorithm design and analysis;Finite impulse response filters;Encryption;Benchmark testing;design automation;high level synthesis},

[2] Y. Lu, S. Liu, Q. Zhang and Z. Xie, "RTLLM: An Open-Source Benchmark for Design RTL Generation with Large Language Model," 2024 29th Asia and South Pacific Design Automation Conference (ASP-DAC), Incheon, Korea, Republic of, 2024, pp. 722-727, doi: 10.1109/ASP-DAC58780.2024.10473904. keywords: {Design automation;Natural languages;Asia;Benchmark testing;Syntactics;Chatbots;Hardware},

[3] HDLBits https://HDLBits.01xz.net/wiki/Main_Page

[4] R. Ma et al., "VerilogReader: LLM-Aided Hardware Test Generation," 2024 IEEE LLM Aided Design Workshop (LAD), San Jose, CA, USA, 2024, pp. 1-5, doi: 10.1109/LAD62341.2024.10691801. keywords: {Codes;Large language models;Conferences;Benchmark testing;Hardware;Test pattern generators;Prompt engineering;Logic;Hardware design languages;Optimization;Automatic Test Generation;LLM;Verilog},

## Each design contains the following files:

### Main

main.cpp: The entry point of the application. It instantiates the top-level system, manages the simulation/execution flow, and handles the initial setup and final cleanup.

### Dut

Dut.h: The header file for the core logic module. It defines the class structure, ports (inputs/outputs), internal registers, and functional interfaces of the design being verified.

Dut.cpp: The implementation file for the Dut class. It contains the actual hardware logic or algorithmic behavior that is being tested.

### System

System.h: Defines the system-level wrapper. It typically wraps the DUT along with other necessary components like interconnects.

System.cpp: Implements the structural wiring and integration of the system components, coordinating how the DUT interacts with its external interfaces.

### Testbench

Testbench.h: The header for the verification environment. It defines the infrastructure used to drive stimuli, monitor outputs, and manage reset signals.

Testbench.cpp: The implementation of the test sequence. It handles loading input data, feeding it into the System, and capturing results for validation.

### Define

define.h: A global configuration file containing macro definitions and constant values used across the entire project.

### data

input_x.dat: A raw data file containing the input stimulus (test vectors) to be processed by the DUT.

golden.dat: The "Golden Reference" file. It contains the expected correct output values used to compare against the DUT's actual results to determine if the test passed or failed.