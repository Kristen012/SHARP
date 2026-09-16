## SerialReceiver

* **Description**

    This design parses a serial bit stream by detecting a start bit, tracking eight data bits, and verifying a correct stop bit to signal the successful reception of a completed data byte.

* **Input Type**

    input_x: bool

* **Output Type**

    o_result: bool

* **Lines of Code (Dut)**

    99 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_serial