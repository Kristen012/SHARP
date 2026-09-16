## Aes

* **Description**

    This design performs AES encryption by iteratively applying byte substitution, row shifting, and column mixing. It processes input data with expanded round keys to transform plaintext into ciphertext.

* **Input Type**

    input_x: sc_uint<8>

* **Output Type**

    o_result: sc_uint<8>

* **Lines of Code (Dut)**

    232 lines

* **Source**

    B. C. Schafer and A. Mahapatra, "S2CBench: Synthesizable SystemC Benchmark Suite for High-Level Synthesis," in IEEE Embedded Systems Letters, vol. 6, no. 3, pp. 53-56, Sept. 2014, doi: 10.1109/LES.2014.2320556. keywords: {Benchmark testing;Arrays;Optimization;Algorithm design and analysis;Finite impulse response filters;Encryption;Benchmark testing;design automation;high level synthesis},
