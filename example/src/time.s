/**
 * FILE:		time.s
 * DESCRIPTION:	fast time functions
 * AUTHOR:		Sharan ***REMOVED***
 * COPYRIGHT:	Copyright (c) 2014 Sharan ***REMOVED***
 * LICENSE: 	MIT License
 */

    .syntax unified
    .cpu cortex-m4
    .fpu vfpv4-d16
    .thumb

    .extern msTicks
    .extern usTicks
    .extern usDelay
    .global Process_Time

Process_Time:
    ldr r0, =usDelay                /* load address of usDelay */
    ldr r1, [r0]                    /* load contents of address */
    cmp r1, #0                      /* compare R1 to 0 */
    itt ne                          /* check if R1 == 0 */
    subne r1, r1, #1                /* decrement usDelay by 1 */
    strne r1, [r0]                  /* store usDelay value */
    ldr r0, =usTicks                /* load address of usTicks */
    ldr r1, [r0]                    /* load contents of address */
    add r1, r1, #1                  /* increment usTicks by 1 */
    cmp r1, #1000                   /* compare to 1000 */
    blt EXIT_0                      /* exit if usTicks < 1000 */
    mov r1, #0                      /* set usTicks to 0 */
    str r1, [r0]                    /* store usTicks value */
    ldr r0, =msTicks                /* load address of usTicks */
    ldr r1, [r0]                    /* load contents of address */
    add r1, r1, #1                  /* increment msTicks by 1 */
EXIT_0:
    str r1, [r0]                    /* store usTicks or msTicks value */
    bx lr                           /* branch to caller */
