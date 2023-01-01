# ECE4820J Lab 10

> Author: :hamster:

## Clean Setup!

1. Where to copy the module?

   ```bash
   /lib/modules/$(uname -r)/kernel/drivers
   ```

2. Which command to generate `modules.dep` and its map?

   `depmod` under `sudo` mode.

3. How to ensure the module is loaded at boot time?

   Edit `/etc/modules-load.d/modules.conf` and add the module name.

4. Create a new `friends` group for grandpa!

   > https://linuxize.com/post/how-to-create-groups-in-linux/

   ```bash
   sudo groupadd -f friends
   ```
   > https://linuxize.com/post/how-to-add-user-to-group-in-linux
   
   ```bash
   sudo usermod -a -G friends grandpa
   sudo usermod -a -G friends grandpa_friend0
	```

5. What is `udev` ? How to define the rules of setting permissions automatically?

​		From the man page, `udev` is a "Dynamic device management", which supplies the system software with **device events**, **manages permissions** of device nodes and may create additional **sym-links** in the `/dev` directory, or renames network interfaces.

Nice reference :arrow_down:

>  https://zhuanlan.zhihu.com/p/373517974

​		Create a file `/etc/udev/rules.d/*.rules`, then set 

```
KERNEL=="dice0", ATTRS{idVendor}=="16c0", MODE="777"
```

### Hacking mum’s computer

1. Adjust the path!

   modify `~/.zshrc`, add 

   ```bash
   export PATH=WHERE_SU_IS:$PATH
   ```

   in the last line, and remember to delete it when the script ends!

2. `su` behavior!

    First, wait for a few seconds in a newline, then print `Authentication failure` in std-err, and lastly exit.

3. `read` command!

   Use `read -s <variable>` to hide input!

4. send email in terminal!

    + A few commands might work:

      + ssmtp (`sudo apt-get install ssmtp`)

      + mutt

      + sendmail (`sudo apt-get install mailutils`)

    + Edit config files (in `su` mode): `/etc/ssmtp/ssmtp.conf` and `/etc/ssmtp/revaliases`
    + send email by `sendmail -f <mail> -t <mail>`, end with `ctrl+D`

