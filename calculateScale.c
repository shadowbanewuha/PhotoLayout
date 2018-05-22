//
//  calculateScale.c
//  RACTest
//
//  Created by YHO on 2018/5/18.
//  Copyright © 2018年 YHO. All rights reserved.
//

#include "calculateScale.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_COUNT 3

PDouble_Link calculate_scale(float arr[], int size){
    PDouble_Link list = create_double_link(arr, size);
    list_group_node(list, 1);
    list_print(list);
    return list;
}

PDouble_Link create_double_link(float arr[], int size){
    PDouble_Link list = (PDouble_Link)malloc(sizeof(Double_Link));
    memset(list, 0, sizeof(Double_Link));
    list->root_list = list;
    if (size >= 1) {
        for (int i=0; i<size; i++) {
            PNode node = (PNode)malloc(sizeof(Node));
            node->value = arr[i];
            list_add_node(list, node);
        }
    }
    return list;
}

void list_add_node(PDouble_Link list, PNode node){
    if (list->count == 0) {
        list->head = node;
        list->tail = node;
        node->prev = NULL;
        node->next = NULL;
    }else{
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }
    list->count++;
}

void list_init_node_value(PDouble_Link list, float arr[0]){
    PNode temp = list->head;
    int i = 0;
    while (temp) {
        temp->value = arr[i++];
        temp = temp->next;
    }
}

PNode list_find_node(PDouble_Link list, int groupIndex, int order){
    if (list->max_group_index < groupIndex){
        return NULL;
    }
    
    PNode target_node = order ? list->head : list->tail;
    while (target_node) {
        bool stop = order ? (target_node->group_index == groupIndex) : (target_node->group_index == groupIndex - 1);
        if (stop) {
            target_node = order ? target_node : target_node->next;
            break ;
        }
        target_node = order ? target_node->next : target_node->prev;
    }
    return target_node;
}

PNode list_find_node_by_sign_node(PNode node, int groupIndex){
    PNode target_node = node;
    bool order = node->group_index < groupIndex;
    while (target_node) {
        bool stop = order ? (target_node->group_index == groupIndex) : (target_node->group_index == groupIndex - 1);
        if (stop) {
            target_node = order ? target_node : target_node->next;
            break ;
        }
        target_node = order ? target_node->next : target_node->prev;
    }
    return target_node;
}

void list_get_sub_double_link(PDouble_Link list, int groupIndex, PDouble_Link sub_list){
    if (list->max_group_index < groupIndex){
        sub_list = NULL;
    }
    
    PNode startNode = NULL;
    PNode temp = list->tail;
    int count = 0;
    while (temp) {
        if(temp->group_index == groupIndex - 1){
            startNode = temp->next;
            break ;
        }
        ++count;
        temp->group_index = groupIndex;
        temp = temp->prev;
    }
    sub_list->head = startNode;
    sub_list->tail = list->tail;
    sub_list->count = count;
    sub_list->min_group_index = groupIndex;
    sub_list->max_group_index = groupIndex;
    sub_list->root_list = list->root_list;
}

float list_group_total_value(PDouble_Link list, int groupIndex, int order){
    if (list->max_group_index < groupIndex)
        return 0;
    
    float result = 0;
    PNode temp = order ? list->head : list->tail;
    while (temp) {
        bool stop = order ? (temp->group_index > groupIndex) : (temp->group_index < groupIndex);
        if(temp->group_index == groupIndex){
            result += temp->value;
        }else if (stop){
            break;
        }
        temp = order ? temp->next : temp->prev;
    }
    return result;
}

float list_total_value(PDouble_Link list){
    return list_total_value_from_target_node(list->head);
}

float list_total_value_from_target_node(PNode node){
    float result = 0;
    while (node) {
        result += node->value;
        node = node->next;
    }
    return result;
}

int list_group_node_count(PDouble_Link list, int groupIndex, int order){
    if (list->max_group_index < groupIndex)
        return 0;
    
    int result = 0;
    PNode temp = order ? list->head : list->tail;
    while (temp) {
        bool stop = order ? (temp->group_index > groupIndex) : (temp->group_index < groupIndex);
        if(temp->group_index == groupIndex){
            ++result;
        }else if (stop){
            break;
        }
        temp = order ? temp->next : temp->prev;
    }
    return result;
}

int list_group_node_count_from_target_node(PNode node){
    int count = 0;
    while (node) {
        ++count;
        node = node->next;
    }
    return count;
}

void list_group_node(PDouble_Link list, int order){
    
    list_group_node_by_target_node(list->head, order);
    
    return ;

    float wF = 3.2, wR = 2.2, wL = 1.6, wN = 1.1;
    int total_col = ceil((float)list->count / MAX_LINE_COUNT);
    float weight;
    if (order == 1) {
        if (total_col >= 3) {
            weight = wR;
        }else{
            weight = wL;
        }
    }else{
        float average = list_total_value(list) / (list->count);
        weight = wN > average ? wN : average;
    }
    
    float group_total_value = 0;
    int col_count = 0, group_index = list->min_group_index;
    PNode temp = list->head;
    while (temp) {
        group_total_value += temp->value;
        if (group_total_value > wF && col_count > 1) {
            temp->group_index = ++group_index;
            col_count = 1;
            group_total_value = temp->value;
        }else{
            temp->group_index = group_index;
            if(group_total_value > weight || ++col_count == MAX_LINE_COUNT){
                ++group_index;
                col_count = 0;
                group_total_value = 0;
            }
        }
        temp = temp->next;
    }
    list->max_group_index = group_index;
    
    if(col_count > 0){
        float last_group_value = list_group_total_value(list->root_list, group_index, 0);
        if (last_group_value < wN) {
            if (list->max_group_index >= 1) {
                Double_Link sub_list;
                memset(&sub_list, 0, sizeof(Double_Link));
                list_get_sub_double_link(list, --list->max_group_index, &sub_list);
                if (order) {
                    last_group_value = list_total_value(&sub_list);
                    if (last_group_value > wF || sub_list.count > MAX_LINE_COUNT) {
                        list_group_node(&sub_list, 0);
                        list->max_group_index = sub_list.max_group_index;
                    }
                }
            }
        }
    }
}

void list_group_node_by_target_node(PNode node, int order){
    float wF = 3.2, wR = 2.2, wL = 1.6, wN = 1.1;
    int total_count = list_group_node_count_from_target_node(node);
    int total_col = ceil((float)total_count / MAX_LINE_COUNT);
    float weight;
    if (order == 1) {
        if (total_col >= 3) {
            weight = wR;
        }else{
            weight = wL;
        }
    }else{
        float total_value = list_total_value_from_target_node(node);
        float average = total_value / total_count;
        weight = wN > average ? wN : average;
    }
    
    float group_total_value = 0;
    int col_count = 0, group_index = node->group_index;
    PNode temp = node;
    while (temp) {
        group_total_value += temp->value;
        if (group_total_value > wF && col_count > 1) {
            temp->group_index = ++group_index;
            col_count = 1;
            group_total_value = temp->value;
        }else{
            temp->group_index = group_index;
            if(group_total_value > weight || ++col_count == MAX_LINE_COUNT){
                ++group_index;
                col_count = 0;
                group_total_value = 0;
            }
        }
        temp = temp->next;
    }
    
    if(col_count > 0){
        if (group_total_value < wN) {
            int col = group_index - node->group_index;
            if (col >= 1) {
                PNode sub_head_node = list_find_node_by_sign_node(node, --group_index);
                if (order) {
                    group_total_value = list_total_value_from_target_node(sub_head_node);
                    col_count = list_total_value_from_target_node(sub_head_node);
                    if (group_total_value > wF || col_count > MAX_LINE_COUNT) {
                        list_group_node_by_target_node(sub_head_node, 0);
                    }
                }else{
                    while (sub_head_node) {
                        sub_head_node->group_index = group_index;
                        sub_head_node = sub_head_node->next;
                    }
                }
            }
        }
    }
}

void list_print(PDouble_Link list){
    PNode temp = list->head;
    while (temp) {
        printf("node[%d] = %f\n", temp->group_index, temp->value);
        temp = temp->next;
    }
    printf("*************============**************\n");
}

void list_clear(PDouble_Link list){
    list->root_list = NULL;
    PNode temp = list->head;
    while (temp) {
        temp->next->prev = NULL;
        temp->next = NULL;
        temp = temp->next;
        free(list->head);
        list->head = temp;
    }
    list = NULL;
}
