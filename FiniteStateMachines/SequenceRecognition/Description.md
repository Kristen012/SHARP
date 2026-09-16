## SequenceRecognition

* **Description**

    This design processes a continuous bit stream by tracking consecutive ones to identify frame boundaries, detect stuffed bits for discarding, and signal transmission errors based on pattern length.

* **Input Type**

    input_x: bool

* **Output Type**

    o_result: bool

* **Lines of Code (Dut)**

    102 lines

* **Source**

    HDLbits: https://hdlbits.01xz.net/wiki/Fsm_hdlc