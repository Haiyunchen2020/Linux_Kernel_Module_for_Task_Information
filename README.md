# Linux_Kernel_Module_for_Task_Information

In this project, you will write a Linux kernel module that uses the /proc file system for displaying a task’s information based on its process identifier value pid. This project will involve writing a process identifier to the file /proc/pid. Once a pid has been written to the /proc file, subsequent reads from /proc/pid will report (1) the command the task is running, (2) the value of the task’s pid, and (3) the current state of the task. An example of how your kernel module will be accessed once loaded into the system is as follows:
   echo "1395" > /proc/pid
   cat /proc/pid
   command = [bash] pid = [1395] state = [1]
The echo command writes the characters "1395" to the /proc/pid file. Your kernel module will read this value and store its integer equivalent as it rep- resents a process identifier. The cat command reads from /proc/pid, where your kernel module will retrieve the three fields from the task struct associ- ated with the task whose pid value is 1395.
