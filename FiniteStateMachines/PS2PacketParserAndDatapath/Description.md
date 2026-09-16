## PS2PacketParserAndDatapath

* **Description**

    This design parses three-byte PS/2 packets by validating a synchronization bit, sequentially aggregating the incoming bytes into a 24-bit value, and outputting the completed packet upon successful detection.

* **Input Type**

    input_x: sc_uint<8>

* **Output Type**

    o_result:  bool & sc_uint<24>

* **Lines of Code (Dut)**

    82 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_ps2data