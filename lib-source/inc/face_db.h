#ifndef __FACE_DB_H__
#define __FACE_DB_H__

#include "rs_data.h"

#define PICTURE_NONE	-1
#define PICTURE_JPEG	0
#define PICTURE_NV12	1

#define ITEM_FLAG_MASK    0xf000
#define ITEM_FLAG_UNUSED  0xf000
#define ITEM_FLAG_INVALID 0x0000
#define ITEM_FLAG_VALID   0x1000

#define MAX_PERSON_ITEM 10000

// 4+512 5M
typedef struct {
	int size;
	char data[512];
} tInfo;

// 4+512*4 20M
typedef struct {
	int flag;
	float fr_feature[512];
} tFeature;

// 25M
typedef struct {
	tFeature feature;
	tInfo info;
} tPersonItem;

int person_db_init(char *path);
void person_db_uninit(void);

void person_db_reset(void);
void person_db_state(int *total, int *used, int *valid);

int add_person(int pic_format, int pic_size, char *pic_data, char *pic_path);
//int del_person(int id);
//int get_person(int id, tPersonItem **person);

void fill_person_info(int info_size, char *info);
void fill_person_feature(int feature_size, float *feature);

//int del_person_item(RS_PERSON_ITEM *p);
//int get_person_item(RS_PERSON_ITEM *p);
//int identify_person(int dimension, float *feature, float *similarity, tInfo **info);


#endif
