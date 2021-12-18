/*
 * =====================================================================================
 *
 *       Filename:  event_loop.c
 *
 *    Description: This file implements the event loop functionality 
 *
 *        Version:  1.0
 *        Created:  12/18/2021 03:51:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Cisco Systems
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "event_loop.h"

static bool ev_loop_debug = true;
