## Layernorm_fixed

* **Description**

    This design performs layer normalization by calculating the mean and standard deviation of input data. It then normalizes each element to ensure a consistent distribution with zero mean and unit variance.
    
* **Input Type**

    input_x: fixed(8, 23)

* **Output Type**

    o_result: fixed(8, 23)

* **Lines of Code (Dut)**

    60 lines

* **Source**

    Classical Algorithm