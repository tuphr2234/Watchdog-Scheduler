# Watchdog

In software systems, it's essential to ensure that critical processes can recover from failures and continue running. The Watchdog library allows you to achieve this by creating processes that are "immortal" and can be automatically restarted in the event of a failure.

The Watchdog library is designed to help you create processes that are resilient and can recover from failures. It provides two key functions: MakeMeImmortal and DoNotResuscitate, which enable you to control the behavior of a process and its watchdog mechanism.

## Watchdog Library Usage Instructions

The Watchdog library (watchdog_lib.so) provides a simple way to create processes that can recover from failures. To use the Watchdog library, follow these steps:

    1. Make sure you have the watchdog_lib.so shared object file and the watchdog
       executable in bin/release or bin/debug.       

    2. You should have the appropriate permissions to execute the watchdog executable.

    compilation example:
        gcc -o my_program my_program.c -L. -l:bin/release/watchdog_lib.so


- This Watchdog uses another module, Scheduler, that is also included in this repository.
