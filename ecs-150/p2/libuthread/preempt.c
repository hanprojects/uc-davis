#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

// Global variables of signal attributes
// Use `static` to avoid exposure to user threads
static struct sigaction sa;
static struct itimerval new_timer, old_timer;
static sigset_t block_alarm;

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

/*
 * Conversion of second to microsecond
 * 1 second = 1000000 microseconds
 */
#define MICROSEC 1000000

// Function to create signal handler to force 
// the currently running thread to yield
// Source: https://www.gnu.org/software/libc/manual/html_mono/
// libc.html#Signal-Actions
void signal_handler(int signum)
{
        (void) signum; // unused
        uthread_yield();
}

// Start & stop preemption
// Source: https://www.gnu.org/software/libc/manual/html_mono/
// libc.html#Setting-an-Alarm
void preempt_start(void)
{
        // Set up handler to fire alarm
        // Source: 03.syscalls (page 41/43)
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGVTALRM, &sa, NULL);

        // Fire virtual alarm - cannot go over 1000000 microsecs
        new_timer.it_interval.tv_usec = MICROSEC/HZ; // microsec
        new_timer.it_interval.tv_sec = 0; // sec
        new_timer.it_value.tv_usec = MICROSEC/HZ; // microsec
        new_timer.it_value.tv_sec = 0; // sec

        setitimer(ITIMER_VIRTUAL, &new_timer, &old_timer);
}

void preempt_stop(void)
{
        // Reset signal to default
        // Source: https://man7.org/linux/man-pages/man2/sigaction.2.html
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = SA_RESETHAND;
        sigaction(SIGVTALRM, &sa, NULL);

        // Reset virtual alarm to default
        new_timer.it_interval.tv_usec = 0; // microsec
        new_timer.it_interval.tv_sec = 0; // sec
        new_timer.it_value.tv_usec = 0; // microsec
        new_timer.it_value.tv_sec = 0; // sec

        setitimer(ITIMER_VIRTUAL, &new_timer, &old_timer);
}

// Enable & disable preemption
// Source: https://www.gnu.org/software/libc/manual/html_mono/
// libc.html#Blocking-Signals (24.7.4)
void preempt_enable(void)
{
        // Initialize signal mask
        sigemptyset(&block_alarm);
        sigaddset(&block_alarm, SIGVTALRM);

        // Enable signal
        sigprocmask(SIG_UNBLOCK, &block_alarm, NULL);
}

void preempt_disable(void)
{
        // Initialize signal mask
        sigemptyset(&block_alarm);
        sigaddset(&block_alarm, SIGVTALRM);

        // Disable signal
        sigprocmask(SIG_BLOCK, &block_alarm, NULL);
}
