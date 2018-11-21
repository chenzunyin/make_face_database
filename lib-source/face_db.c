//#include "rs_common.h"
#include "face_db.h"
#include "rs_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static tPersonItem *m_items = NULL;
static tPersonItem *m_person = NULL;
static FILE *db_fp = NULL;
static int used_id_max = -1;
static int valid_id_cnt = 0;


int person_db_init(char *path)
{
	int i;
	int read_size;
	
	m_items = (tPersonItem *)malloc(MAX_PERSON_ITEM * sizeof(tPersonItem));
	if (m_items == NULL) {
		printf("%s: malloc m_items error\n", __func__);
		return -1;
	}
	
	m_person = (tPersonItem *)malloc(sizeof(tPersonItem));
	if (m_person == NULL) {
		printf("%s: malloc m_person error\n", __func__);
		free(m_items);
		return -1;
	}

	db_fp = fopen(path, "rb+");
	if (db_fp == NULL) {
		printf("%s: Cannot open %s\n", __func__, path);
		free(m_items);
		free(m_person);
		return -1;
	}
	
	printf("%s: open %s ok\n", __func__, path);
	
	for (i=0; i<MAX_PERSON_ITEM; i++) {
		m_items[i].feature.flag = ITEM_FLAG_UNUSED;
	}
	for (i=0; i<MAX_PERSON_ITEM; i++) {
		read_size = fread(m_items+i, 1, sizeof(tPersonItem), db_fp);
		if (read_size == sizeof(tPersonItem)) {
			if ((m_items[i].feature.flag & ITEM_FLAG_MASK) == ITEM_FLAG_VALID) {
				valid_id_cnt++;
			}
		} else {
			printf("%s: read_size=%d\n", __func__, read_size);
			break;
		}
	}
	
	used_id_max = i - 1;
	printf("%s: total=%d, used_id_max=%d, valid_id_cnt=%d\n", __func__, MAX_PERSON_ITEM, used_id_max, valid_id_cnt);

	return 0;
}

void person_db_uninit(void)
{
	if (db_fp == NULL) {
		return;
	}
	
	free(m_items);
	free(m_person);
	
	fclose(db_fp);
}

void person_db_reset(void)
{
	int i;
	
	for (i=0; i<=used_id_max; i++) {
		//del_person(i);
	}
	
	printf("%s: total=%d, used_id_max=%d, valid_id_cnt=%d\n", __func__, MAX_PERSON_ITEM, used_id_max, valid_id_cnt);
}

void person_db_state(int *total, int *used, int *valid)
{
	*total = MAX_PERSON_ITEM;
	*used = used_id_max;
	*valid = valid_id_cnt;
}

int add_person(int pic_format, int pic_size, char *pic_data, char *pic_path)
{
	int i;
	int ret;
	int write_size;
	char buf[32];
	
	if (valid_id_cnt >= MAX_PERSON_ITEM) {
		printf("%s: valid_id_cnt=%d, no space\n", __func__, valid_id_cnt);
		return -2;
	}
	
	for (i=0; i<MAX_PERSON_ITEM; i++) {
		if ((m_items[i].feature.flag & ITEM_FLAG_MASK) != ITEM_FLAG_VALID) {
			break;
		}
	}
	
	printf("%s: person id: %d\n", __func__, i);
	
	ret = fseek(db_fp, i * sizeof(tPersonItem), SEEK_SET);
	if (ret < 0) {
		printf("%s: fseek error\n", __func__);
		return -1;
	}
	
	m_person->feature.flag &= ~ITEM_FLAG_MASK;
	m_person->feature.flag |= ITEM_FLAG_VALID;
	write_size = fwrite(m_person, 1, sizeof(tPersonItem), db_fp);
	if (write_size != sizeof(tPersonItem)) {
		printf("%s: fwrite error\n", __func__);
		return -1;
	}
	fflush(db_fp);
	
	if ((m_items[i].feature.flag & ITEM_FLAG_MASK) == ITEM_FLAG_UNUSED) {
		used_id_max++;
	}
	
	memcpy(m_items+i, m_person, sizeof(tPersonItem));
	valid_id_cnt++;
/*	
	if (pic_format == PICTURE_JPEG) {
		sprintf(buf, "/data/picture/%06d.jpeg", i);
		file_copy(pic_path, buf);
	} else if (pic_format == PICTURE_NV12) {
		sprintf(buf, "/data/picture/%06d.nv12", i);
		save_to_file(buf, pic_data, pic_size);
	}
*/
	
	return 0;
}

/*
int del_person(int id)
{
	int ret;
	int write_size;
	char buf[32];
	
	if ((id < 0) || (id > used_id_max)) {
		printf("%s: invalid id %d\n", __func__, id);
		return -1;
	}
	
	printf("%s: person id: %d\n", __func__, id);
	
	if ((m_items[id].feature.flag & ITEM_FLAG_MASK) != ITEM_FLAG_VALID) {
		printf("%s: already del\n", __func__);
		return 0;
	}
	
	ret = fseek(db_fp, id * sizeof(tPersonItem), SEEK_SET);
	if (ret < 0) {
		printf("%s: fseek error\n", __func__);
		return -1;
	}
	
	m_person->feature.flag &= ~ITEM_FLAG_MASK;
	m_person->feature.flag |= ITEM_FLAG_INVALID;
	write_size = fwrite(&m_person->feature.flag, 1, sizeof(m_person->feature.flag), db_fp);
	if (write_size != sizeof(m_person->feature.flag)) {
		printf("%s: fwrite error\n", __func__);
		return -1;
	}
	fflush(db_fp);
	
	m_items[id].feature.flag = m_person->feature.flag;
	valid_id_cnt--;
	
	sprintf(buf, "/data/picture/%06d.jpeg", id);
	file_delete(buf);
	sprintf(buf, "/data/picture/%06d.nv12", id);
	file_delete(buf);
	
	return 0;
}


int get_person(int id, tPersonItem **person)
{
	if ((id < 0) || (id > used_id_max)) {
		printf("%s: invalid id %d\n", __func__, id);
		return -1;
	}
	
	//printf("%s: person id: %d\n", __func__, id);
	
	if ((m_items[id].feature.flag & ITEM_FLAG_MASK) != ITEM_FLAG_VALID) {
		//printf("%s: invalid item\n", __func__);
		return -2;
	}
	
	*person = m_items + id;
	return 0;
}
*/

void fill_person_info(int info_size, char *info)
{
	if (info_size > sizeof(m_person->info.data)) {
		m_person->info.size = sizeof(m_person->info.data);
		
		printf("%s: info %d > %d\n", __func__, info_size, m_person->info.size);
	} else {
		m_person->info.size = info_size;
	}
	
	memcpy(m_person->info.data, info, m_person->info.size);
}

void fill_person_feature(int feature_size, float *feature)
{
	m_person->feature.flag &= ITEM_FLAG_MASK;
	m_person->feature.flag |= feature_size;
	memcpy(m_person->feature.fr_feature, feature, feature_size * sizeof(float));
}

/*
int del_person_item(RS_PERSON_ITEM *p)
{
	tPersonItem *person;
	int i;
	int ret;
	
	if ((p->id == -1) && (p->size == 0)) {
		person_db_reset();
		return 0;
	} else if (p->id != -1) {
		del_person(p->id);
		return 0;
	}
	
	for (i=0; i<=used_id_max; i++) {
		ret = get_person(i, &person);
		if (ret != 0) {
			continue;
		}
		
		if (memcmp(person->info.data, p->data, p->size) == 0) {
			del_person(i);
		}
	}
	
	return 0;
}


int get_person_item(RS_PERSON_ITEM *p)
{
	tPersonItem *person;
	int i;
	int ret;
	
	if ((p->id == -1) && (p->size == 0)) {
		send_person_db_info();
		return 0;
	} else if (p->id != -1) {
		ret = get_person(p->id, &person);
		if (ret != 0) {
			return ret;
		}
		send_person_item(p->id, &person->info);
		return 0;
	}
	
	for (i=0; i<=used_id_max; i++) {
		ret = get_person(i, &person);
		if (ret != 0) {
			continue;
		}
		
		if (memcmp(person->info.data, p->data, p->size) == 0) {
			send_person_item(i, &person->info);
		}
	}
	
	return 0;
}


int identify_person(int dimension, float *feature, float *similarity, tInfo **info)
{
	tPersonItem *person;
	int i;
	int ret;
	float value = 0.0, best_value = 0.0;
	int best_id = -1;
	
	for (i=0; i<=used_id_max; i++) {
		ret = get_person(i, &person);
		if (ret != 0) {
			continue;
		}
		
		if (dimension != (person->feature.flag & (~ITEM_FLAG_MASK))) {
			continue;
		}

		value = get_fr_feature_similarity_norm_neon(feature, person->feature.fr_feature, dimension);
		if (value > best_value) {
			best_value = value;
			best_id = i;
		}
	}
	
	printf("%s: best_id=%d, best_value=%f\n", __func__, best_id, best_value);
	
	if (best_id == -1) {
		return -1;
	}
	
	ret = get_person(best_id, &person);
	if (ret != 0) {
		return -1;
	}
	
	*similarity = best_value;
	*info = &person->info;
	
	printf("person info size is %d\n", person->info.size);

	return 0;
}
*/
