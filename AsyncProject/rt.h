/*
 * =====================================================================================
 *
 *       Filename:  rt.h
 *
 *    Description:  This file defines the Data structure and public APIs to implement a simple Routing Table
 *
 *        Version:  1.0
 *        Created:  02/14/2020 07:33:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Juniper Networks (www.csepracticals.com), sachinites@gmail.com
 *        Company:  Juniper Networks
 *
 *        This file is part of the Inter-Thread Communication distribution (https://github.com/sachinites) 
 *        Copyright (c) 2019 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify it under the terms of the GNU General 
 *        Public License as published by the Free Software Foundation, version 3.
 *        
 *        This program is distributed in the hope that it will be useful, but
 *        WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *        General Public License for more details.
 *
 *        visit website : www.csepracticals.com for more courses and projects
 *                                  
 * =====================================================================================
 */

#ifndef __RT__
#define __RT__

/*Opaque Data structures*/
typedef struct rt_table_ rt_table_t;
typedef struct rt_table_entry_ rt_table_entry_t;

#define ROUTE_CREATE    1
#define ROUTE_UPDATE    2
#define ROUTE_DELETE    3

struct rt_table_entry_{

    char dest[16];
    char mask;
    char gw[16];
    char oif[32];
    time_t last_updated_time;
    struct rt_table_entry_ *next;
    struct rt_table_entry_ *prev;
     rt_table_t *rt_table; /* back ptr to owning rt table*/
};

struct rt_table_ {

    char rt_table_name[32];
    struct rt_table_entry_ *head;
    uint32_t count;
};

rt_table_t *
rt_create_new_rt_table(char *name);

int /*0 on success, -1 on failure*/
rt_insert_new_entry(rt_table_t *rt, 
                    char *dest, char mask,
                    char *gw, char *oif);

int /*0 on success, -1 on failure*/
rt_delete_rt_entry(rt_table_t *rt,
                   char *dest, char mask);

int /*0 on success, -1 on failure*/
rt_update_rt_entry(rt_table_t *rt,
                   char *dest, char mask,
                   char *new_gw, char *new_oif);

void
rt_display_rt_table(rt_table_t *rt);

rt_table_entry_t *
rt_look_up_rt_table_entry(rt_table_t *rt,
        char *dest, char mask);

#endif /* __RT__ */
