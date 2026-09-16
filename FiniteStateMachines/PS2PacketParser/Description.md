## PS2PacketParser

* **Description**

    This design parses three-byte PS/2 packets by verifying a synchronization bit in the first byte and sequentially processing the remaining bytes to detect and signal a completely received packet.

* **Input Type**

    input_x: sc_uint<8>

* **Output Type**

    o_result:  bool

* **Lines of Code (Dut)**

    67 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_ps2