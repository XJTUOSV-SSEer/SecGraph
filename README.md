# SecGraph

## How to Build/Execute the Application

1. Install Intel(R) SGX SDK for Linux* OS

2. Build the project with the prepared Makefile:
   
   Using Hardware Mode and Debug build:
   
       `` $ cd SecGraph (or SecGraph-g) && make clean``
       
       `` $ make SGX_MODE=HW SGX_DEBUG=1``

3. Execute the binary directly:
   ```
    $ ./cryptoTestingApp
   ```

## Worklog after Fork:

Mon 4 Mar, 19

- This version is only tested on Ubuntu 16.04/18.04 with SGX >=2.0
