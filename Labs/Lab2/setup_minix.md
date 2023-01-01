# **VE482 Lab 2:** Setting up MINIX 3

## Setting up the virtual environment

We need to first download the MINIX 3 (version 3.2.1) image from [MINIX image](https://wiki.minix3.org/doku.php?id=www:download:start). After download, we can get the `iso` image by unzipping `minix_R3.2.1-972156d.iso.bz2`. We can choose from several popular virtual machines such as VMware and VirtualBox. Useful links:

1. [Tutorial on VMware with MINIX 3](https://wiki.minix3.org/doku.php?id=usersguide:runningonvmware) 
2. [Tutorial on VirtualBox with MINIX 3](https://wiki.minix3.org/doku.php?id=usersguide:runningonvirtualbox) 

## Establish SSH connection

Follow the following steps to setup the system and install openssh:

1. Select the default mode to login MINIX by pressing enter.

2. Use `root` as the login name.

3. Type `passwd` in the terminal to set the password.

4. Type `setup` in the terminal to start setting your virtual machine. You can keep everything as default by repeatedly pressing enter or type "yes".

5. After setup, disconnect your virtual machine with the image. Type `reboot` in the terminal to restart your VM. Now you can see that the IP address appears. Check your IP address by typing `ifconfig`. For example, my IP address is `192.168.130.138`.

6. Type in the terminal the following

   ```{shell}
   pkgin update
   pkgin install openssh
   ```

   to install openssh on MINIX 3.

7. Navigate to the openssh configuration file using

   ```shell
   vi /usr/pkg/etc/ssh/sshd_config
   ```

   On line 13, delete `#` and change the line to `Port 2222`. This step is to increase safety of connection. *This step is optional*.


   **Hints**: Type `i` to insert, type `x` to delete, press `esc` to stop inserting, type `:q` to quit without saving, type `:wq` to write and quit.

8. Use the terminal of your own computer (Mac or Linux). Make sure your terminal has `ssh` working, otherwise install it using

   ```shell
   sudo apt-get update
   sudo apt-get upgrade
   sudo apt-get install openssh-client
   ```

   Try to access the MINIX VM by typing

   ```shell
   ssh root@192.168.130.138 -p 2222
   ```

   After typing in the password, you should be able to login to MINIX. Try a few commands! Type `exit` to exit to your normal ternimal.
   
9. It is unsafe to let others login with root. So we add a user on minix:

   ```shell
   useradd gzh
   passwd gzh
   ```

10. Now let's make things easier! On your own computer (Mac or Linux) find the file `~/.ssh/config`.

    ```shell
    cd ~/.ssh/
    vim config
    ```

    Add the following to the file

    ```
    Host    minix
            HostName 192.168.130.138
            Port     2222
            User     gzh
    ```

    Save the file. Now try 

    ```shell
    ssh minix
    ```

11. However, you still need to type the password. Now we make it even more convenient. Type in the terminal of your own computer (Mac or Linux)

    ```shell
    chmod 700 ~/.ssh
    ssh-keygen -t rsa
    ```

    You are recommended not to set a passphrase. Now, a key pair named `id_rsa` and `id_rsa.pub` will be generated. Now type

    ```shell
    ssh-copy-id -i ~/.ssh/id_rsa gzh@192.168.130.138 -p 2222
    ```
    
    or
    
    ```shell
    ssh-copy-id minix
    ```

    and type in the MINIX password. Now try `ssh minix` again! You should be able to login without password.

    **Hint**: Do not share the private key `~/.ssh/id_rsa` with others! Share only the public key. Copy your public key in a file called `[Your Student ID].pub` and send it along with the lab report such that we can add you on the git server. This is critical for your project 2!!!

## Common Issues

1.  During `setup`, some students are stuck in step 3. This may be because they don't have the `iso` file connected to the VM.

2.  Some students have problem logging with `ssh`. In this case, check the `/usr/pkg/etc/ssh/sshd_config` file that the `#` character before `Port 2222` is deleted. 

3.  Some students using VirtualBox had problems to connect with `ssh` even if the the sshd configuration is correct. Please use VMware as a more stable VM system. SJTU offers students with a free license [here](https://vmap.sjtu.edu.cn/).
