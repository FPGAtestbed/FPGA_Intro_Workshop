# Connecting To The FPGA Testbed

1. If you haven't already, download and install the X2GO client, which you can find here: https://wiki.x2go.org/doku.php/download:start

2. Open it up and, on the top menu, click on `Session` and select `New Session` from the drop down menu. 

3. A `Session preferences` box will open. You will need to alter the following fields:
    - `Host`: `nextgenio-login1` or `nextgenio-login2` depending on which group you are in. 
    - `Login`: `ngguestXX` (where `XX` will be a unique number for each of you)
    - `Password`: `FPGA_UCL_C0urse_2022`
    - `Use Proxy server for SSH connection` should be ticked 
    - In the `Proxy server` box: 
        - `Host` should be `hydra-vpn.epcc.ed.ac.uk
        - `Same login as on X2GO Server` should be ticked
        - `Same password as on X2GO Server` should be ticked
    - `Session type` should be `XFCE` in drop down menu

4. Click `OK` to confirm the session preferences. 

5. Click on the session, enter your password, and click `OK` to start the session. It may take a little while to load, but a new window should appear showing the remote desktop. 

6. Once connected, open a terminal and clone the git repository at https://github.com/UCL/FPGA_Intro_Workshop
    - You should use the `http` method to avoid having to set up ssh keys and link them to your github account. 

7. Go into the repository and type the following command to load the relevant modules.
    ```
    source setup_FPGA_modules.sh 
    ```

8. You can find out more information about the testbed and how to get started on it here: https://fpga.epcc.ed.ac.uk/