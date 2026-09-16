## SerialReceiverAndDatapath

* **Description**

    This design processes a serial bit stream, deserializing eight data bits received least significant bit first, and outputs the fully assembled data byte upon validating correct start and stop bits.

* **Input Type**

    input_x: bool

* **Output Type**

    o_result: bool & sc_uint<8>

* **Lines of Code (Dut)**

    114 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_serialdata