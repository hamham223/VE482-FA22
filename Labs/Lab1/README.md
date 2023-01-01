# ECE4820J Lab 1

> Author: Hamster :hamster:

1. 

```bash
touch test
mkdir dir && mv test dir
cp dir/test dir/test_copy.txt
ls dir
```

2. 

```bash
ls /etc | grep -E '.*127\.0\.0\.1\.*'
cat /etc/passwd | grep -e 'hamster' -e 'root'
```

3. 

```bash
sudo find /etc -atime -1
sudo find /etc -regex '.*netw.*'
```

4. + `>`: write to one file, cover the original content
   + `>>`: append to one file
   + `<<<`: read from the file as a single raw string
   + `>&1`: equal to `>1 2>&1`, redirect the std out and std err to `1`
   + `2>&1 >`: discard the std err, only redirect std out
   + `tee` : read from std in and write to std out and also files

5. + `xargs`: build and execute command lines from std in, providing even more options than the simple redirection `>` 
   + `|`: pipeline. `A | B` means the std in of `A` serves as the std out of `B`

6. + `head`: display first `x` lines of the file (default: 10)
   + `tail`: display last `x` lines of the file (default: 10)
   + We can use `tail -n L` , always display the last `L` rows of the text, where `L` is the length of the editor.

7. +  `top`: display all Linux processes
   + `ps`: current processes
   + `free -h`: display used and free memory (in suitable unit)
   + `vmstat`: display virtual memory usage

8. Their occurring time? `sh` is the prominent progenitor of modern shells, `bash` and `zsh` are improved version. `csh` is based on C language.

   > https://www.howtogeek.com/68563/HTG-EXPLAINS-WHAT-ARE-THE-DIFFERENCES-BETWEEN-LINUX-SHELLS/

9. + `$a` means the `a-th` argument to the script

   + `$?` means the exit code from the previous command

   + `$$` the Process ID of the shell itself

   + `$!` the Process ID that is lastly executed

     > https://www.cnblogs.com/chjbbs/p/6393935.html

10. + **PS3**: environment variable which contains the value of the prompt for the select operator inside the shell script.

      > https://linoxide.com/change-bash-prompt-variable-ps1/

    + ```bash
      #!/bin/bash
      echo "please select a value to display a month on the list below"
      select i in jan feb mar apr exit
      do
          case $i in
             jan) echo "January";;
             feb) echo "February";;
             mar) echo "March";;
             exit) exit;;
          esac
      done
      ```

    + ```bash
      $ chmod +x ps3-value.sh
      :~$./ps3-value.sh
      1) jan
      2) feb
      3) mar
      4) exit
      #? 2
      February
      #?
      ```

11. `iconv`: convert text from one character encoding to another

    It can convert from any of these encodings to any other, through Unicode conversion ?(

12. + `${#temp}`: length of `temp` 
    + `${temp%%word} `: Remove  matching suffix pattern (longest)
    + `${temp/pattern/string}`: Pattern substitution

13. + > https://www.geeksforgeeks.org/unix-file-system/

    + **/ :** root. Linux file system is like a tree.

    + **/bin :** Stands for “binaries” and contains certain fundamental utilities. Accessed by all users.

    + **/boot :** Contains all the files that are required for successful booting process.

    + **/dev :** Stands for “devices”. 

    + **/etc :** Contains system-wide configuration files and system databases.

    + **/home :** Contains the home directories for the users.

    + **/lib :** Contains system libraries.

    + **/media :** Default mount point for removable devices.

    + **/mnt :** Stands for “mount”. Contains file system mount points. 

    + **/usr/bin :** All binary programs not residing in /bin, /sbin or (rarely) /etc.

    + **/usr/lib :** Stores the required libraries and data files for programs stored within /usr or elsewhere.

    + **/var :** A short for “variable.” A place for files that  may change often – especially in size.

    + > https://www.educba.com/unix-file-system/

    + **/proc**: files that are related to system processes

    + > https://ipcisco.com/lesson/linux-file-system

    + **/srv**: contains server specific services data

    + **/opt**: 3-rd party applications, like Feishu

    + **/sbin**: includes system binary executable programs

    + **/sys**: virtual file system that stores the modifications on connected devices

    + **/usr/src**: source files for different parts of the system

    + **/usr/share**: all read-only architecture independent data files

    + ~~**/initrd.img**: Linux initial RAM disk image ??~~

    + ~~**/vmlinuz**: Linux kernel executable??~~

14.  See the file `guess.sh`

```bash
#!/bin/bash
echo This is a simple guess number game!
echo Author: Hamtser
echo 
echo Instructions: The computer will randomly generate a number in 0-32767, each round you should enter a number and guess.
echo The computer will tell you whether the number should be larger or smaller.
echo If you get the correct number, you win.
echo
echo Game Start NOW!

a=$RANDOM
read input
while [ $input -ne $a ]; do
	if [ $input -gt $a ] ; then
		echo "Smaller than" $input
	else 
		echo "Larger than" $input
	fi
	read input
done
echo You Win!!!
```

