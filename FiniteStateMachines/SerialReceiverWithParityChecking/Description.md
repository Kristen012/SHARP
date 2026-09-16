## SerialReceiverWithParityChecking

* **Description**

    This design parses a serial bit stream, extracts eight data bits, and verifies start, stop, and odd parity bits to output the completely received data byte upon successful validation.

* **Input Type**

    input_x: bool

* **Output Type**

    o_result: bool & sc_uint<8>

* **Lines of Code (Dut)**

    128 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_serialdp