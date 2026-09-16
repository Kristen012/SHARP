## Softmax_fixed

* **Description**

    This design computes the Softmax function by identifying the maximum input to stabilize exponential calculations and normalizing the values into a probability distribution where all output elements sum to one.
    
* **Input Type**

    input_x: fixed(8, 23)

* **Output Type**

    o_result: fixed(8, 23)

* **Lines of Code (Dut)**

    87 lines

* **Source**

    Classical Algorithm