# Simple_Linux_Shell
### Decription
This project was to write a simple shell in C using fork(), execvp(), pipe(), and dup2(). \
The shell runs on top of the Linux bash.

![image](https://github.com/user-attachments/assets/d6f2dda8-c186-4e3e-ac18-d75d1e9481ca)

In the future I would like to try to implemet a tab auto complete for 

### Issues
My biggest issue was trying to have both file input and manual user input to work with the same program. I thought of a couple different ways. But, when I started to code, I found it very easy. I only have to change one argument in the fgets function from stdin to the name of the file. In C, that was all I needed to do, which was very simple.
