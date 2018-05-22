//
//  calculateScale.h
//  RACTest
//
//  Created by YHO on 2018/5/18.
//  Copyright © 2018年 YHO. All rights reserved.
//

#ifndef calculateScale_h
#define calculateScale_h

#include <stdio.h>

typedef struct _NODE {
    struct _NODE *prev;
    struct _NODE *next;
    int group_index;
    float value;
}Node, *PNode;

typedef struct _Double_Link{
    PNode head;
    PNode tail;
    int count;
    int min_group_index;
    int max_group_index;
    struct _Double_Link *root_list;
}Double_Link, *PDouble_Link;

PDouble_Link calculate_scale(float *, int);

PDouble_Link create_double_link(float *, int);
void list_add_node(PDouble_Link, PNode);
void list_init_node_value(PDouble_Link, float *);

void list_get_sub_double_link(PDouble_Link, int, PDouble_Link);

PNode list_find_node(PDouble_Link, int, int);
PNode list_find_node_by_sign_node(PNode, int);

void list_group_node(PDouble_Link, int);
void list_group_node_by_target_node(PNode, int);

float list_group_total_value(PDouble_Link, int, int);

float list_total_value(PDouble_Link);
float list_total_value_from_target_node(PNode);

int list_group_node_count(PDouble_Link, int, int);
int list_group_node_count_from_target_node(PNode);

void list_print(PDouble_Link);
void list_clear(PDouble_Link);

#endif /* calculateScale_h */
