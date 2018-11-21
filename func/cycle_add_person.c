#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "face_db.h"
#include "RSFaceSDK.h"
#include <opencv2/opencv.hpp>
using namespace cv; 

#define LENGTH 512
#define LEN 1000

int global_ret = 0;
char * db_path = "./database/tiny_face.db";
char * tmp_db_path = "./database/tmp_face.db";
int global_infolen = 0;

const char * APPID = "b59c5851eea4408f833f24b31b1fae9e";
const char * APPSECRET = "6ee59303feec5c73ca7f518bee7846ca410d9e53";
RSHandle lm = NULL;
RSHandle fd = NULL;
RSHandle fr = NULL;
Mat global_cvface;
rs_face  *rsface_array = NULL;
rs_face_feature global_facefeature;
int global_facecnt;
int global_cyccnt = 0;

float global_norm = 0;
int global_numcnt = 0;
#define NORMALIZE_EPS 0.00001

int feature_init(char * path)
{
        //init liscense manager
        global_ret = rsInitLicenseManager(&lm, APPID, APPSECRET);

        if (lm == NULL) {
                printf("Liscense Manager init error ! errno = %d\n", global_ret);
                return -1;
        }

        //init face detect
        rsInitFaceDetect(&fd, lm);

        if (fd == NULL) {
                printf("Face Detect init error\n");
                return -1;
        }

        //init face recognition
        rsInitFaceRecognition(&fr, lm, path);

        if (fr == NULL) {
                printf("Face Recognition init error\n");
                return -1;
        } 

        return 0;
}

void feature_free(void)
{
        rsUnInitFaceRecognition(&fr);
        rsUnInitFaceDetect(&fd);
        rsUnInitLicenseManager(&lm);
}

int run_face_add(char * inputFile, int infolen)
{
        //opencv load image
        //global_cvface = imread(inputFile, CV_LOAD_IMAGE_UNCHANGED);
        global_cvface = imread(inputFile);

        //run face detect
        global_ret = rsRunFaceDetect(fd, global_cvface.data,
                PIX_FORMAT_BGR888,
                global_cvface.cols,
                global_cvface.rows,
                global_cvface.step,
                RS_IMG_CLOCKWISE_ROTATE_0,
                &rsface_array,
                &global_facecnt);


        if (global_ret) {
                printf("run face detect func error ! errno = %d\n", global_ret);
                return global_ret;
        }

	if (global_ret > 1) {
		printf("find face %d\n", global_ret);
		return global_ret;
	}

        //run face get feature
        global_ret = rsRecognitionGetFaceFeature(fr, global_cvface.data,
                PIX_FORMAT_BGR888,
                global_cvface.cols,
                global_cvface.rows,
                global_cvface.step,
                rsface_array,
                &global_facefeature
        );

        if (global_ret) {
                printf("run face recognition error ! errno =%d \n", global_ret);
                return global_ret;
        } else {
                printf("run face recognition ok\n");
        }

        //free result face array
        releaseFaceDetectResult(rsface_array, global_facecnt);

	//normalized
        for (global_numcnt = 0; global_numcnt < LENGTH; global_numcnt++) {
                global_norm += global_facefeature.feature[global_numcnt] * global_facefeature.feature[global_numcnt];
        }
        for (global_numcnt = 0; global_numcnt < LENGTH; global_numcnt++) {
                global_facefeature.feature[global_numcnt] /= sqrt(global_norm + NORMALIZE_EPS);
        }


	//add feature to face_db
	//global_infolen = sizeof(inputFile);
	fill_person_info(infolen, inputFile);
	printf("name: %s, size: %d \n", inputFile, infolen);
	fill_person_feature(LENGTH, global_facefeature.feature);
	global_ret = add_person(PICTURE_NONE, 0, NULL, NULL);
	if(global_ret) {
		printf("add person to lib fail, %d\n", global_ret);
		return global_ret;
	}

        //add person to facelib
        //global_ret = rsRecognitionPersonCreate(fr, &global_facefeature);
        //if (global_ret) {
        //        printf("register person ok!,total person in face.db is %d\n", global_ret);
        //}

        return 0;
}


int ReadFileList (char * dirpath)
{
        DIR *dir;
        struct dirent *ptr;
        char base[LEN];
        char filepath[LEN];

        if ((dir = opendir(dirpath)) == NULL) {
                perror("Open dir error...");
                return -1;
        }

        while ((ptr = readdir(dir)) != NULL) {
                global_cyccnt++;
                if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
                        continue; //current or parrent dir
                } else if (ptr->d_type == 8) { //file
                        printf("file d_name: %s/%s \n", dirpath, ptr->d_name);
                        memset(filepath, '\0', sizeof(filepath));
                        strcpy(filepath, dirpath);
                        strcat(filepath, "/");
                        strcat(filepath, ptr->d_name);
                        global_ret = run_face_add(filepath, strlen(filepath));
                        if (global_ret) {
                                printf("get person feature error!\n");
                                return -1;
                        }
                } else if (ptr->d_type == 10) { //link file
                        printf("link d_name: %s/%s \n", dirpath, ptr->d_name);
                } else if (ptr->d_type == 4) { //dir
                        memset(base, '\0', sizeof(base));
                        strcpy(base, dirpath);
                        strcat(base, "/");
                        strcat(base, ptr->d_name);
                        ReadFileList(base);
                }
        }

        closedir(dir);

        return 0;
}


int database_init(char *path)
{
	int ret = 0;
	ret = person_db_init(path);
	return ret;
}


int main(int argc, char * argv[])
{
        int get_c;
        char * inputpath = NULL;

        if ((get_c = getopt(argc, argv, ":f:")) != -1) {
                switch (get_c) {
                        case 'f':
                                inputpath = optarg;
                                printf("dir path is %s\n", inputpath);
                                break;
                        default:
                                printf("use: -f for input dir path\n");
                                return -1;
                }
        } else {
                printf("use: -f for input dir path\n");
                return -1;
        }

        if (inputpath == NULL) {
                printf("input dir path is NULL\n");
                return -1;
        }

	if (access(db_path, F_OK)) {
		if (open(db_path, O_CREAT, S_IRUSR|S_IWUSR) == -1) {
			printf("create face_db fail\n");
			return -1;
		}
	}

	
	global_ret = database_init(db_path);
	if(global_ret) {
		printf("face_db init error! %d\n", global_ret);
		return global_ret;
	}

	global_ret = feature_init(tmp_db_path);
	if(global_ret) {
		printf("feature lib init error! %d\n", global_ret);
		return global_ret;
	}

	//global_ret = run_face_add(inputFile);
	global_ret = ReadFileList(inputpath);
        if (global_ret) {
                printf("read file fail, ret = %d\n", global_ret);
        } else {
                printf("read file ok!\n");
        }

	printf("total cycle is %d\n", global_cyccnt);

	feature_free();

	return 0;

}
