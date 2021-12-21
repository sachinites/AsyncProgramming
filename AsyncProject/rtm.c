/*
 * =====================================================================================
 *
 *       Filename:  rtm.c
 *
 *    Description:  This file implements the Routing Table Manager( Thread)
 *
 *        Version:  1.0
 *        Created:  02/14/2020 07:24:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Juniper Networks (https://csepracticals.com), sachinites@gmail.com
 *        Company:  Juniper Networks ( 2017 - 2020), Cisco Systems ( 2020 - Present )
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
 *        visit website : https://csepracticals.com for more courses and projects
 *                                  
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "rt.h"

rt_table_t *rt_table = NULL;

int
main(int argc, char **argv){

    if (!rt_table) {
        rt_table =
            rt_create_new_rt_table("Table1");
        printf(" New Routing Table Created\n");
    }

    for(;;){
        printf("Main Menu\n");
        printf("\t 1.  : Display Routing Table\n");
        printf("\t 2.  : Create New RT Entry\n");
        printf("\t 3.  : Update Existing RT Entry\n");
        printf("\t 4.  : Delete RT entry\n");
        printf("\t 5.  : exit\n");

        int choice;
        printf("Enter Choice : ");
        scanf("%d", &choice);


        switch(choice){
            case 1:
                break;
            case 5:
                exit(0);
            default:
                break;
        }
    }
    return 0;
}
