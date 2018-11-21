#ifndef __RS_DATA_H__
#define __RS_DATA_H__


#define TCP_SERVER_ADDR "192.168.32.3"
#define TCP_SERVER_PORT 10051


typedef enum {
	RS_CMD_TEST = 0x00,
	RS_CMD_SYSTEM_INFO = 0x01,
	RS_CMD_SAVE_FRAME = 0x02,
	RS_CMD_LIVENESS_SWITCH = 0x03,
	RS_CMD_CROP_SWITCH = 0x04,
	RS_CMD_RECO_AREA = 0x05,
	RS_CMD_SET_DEBUG_FLAG = 0x06,
	RS_CMD_GET_PERSON_ITEM = 0x07,
	RS_CMD_PING = 0x10,
	RS_CMD_VERSION = 0x11,
	RS_CMD_MODE = 0x12,
	RS_CMD_START = 0x13,
	RS_CMD_STOP = 0x14,
	RS_CMD_FW_PREPARE = 0x15,
	RS_CMD_FW_DATA = 0x16,
	RS_CMD_FW_UPDATE = 0x17,
	RS_CMD_REBOOT = 0x1b,
	RS_CMD_SET_SN = 0x1c,
	RS_CMD_GET_SN = 0x1d,
	RS_CMD_LIC_INFO = 0x1e,
	RS_CMD_FILE_OPEN = 0x1f,
	RS_CMD_FILE_DATA = 0x20,
	RS_CMD_FILE_DONE = 0x21,
	RS_CMD_FACE_PICTURE = 0x22,
	RS_CMD_TAKE_PHOTO = 0x24,
	RS_CMD_UVC_SOURCE = 0x25,
	RS_CMD_SLEEP = 0x26,
	RS_CMD_SET_ROTATE_ANGLE = 0x27,
	RS_CMD_SET_ALGO_MODEL = 0x28,
	RS_CMD_FACE_SWITCH = 0x29,
	RS_CMD_COMPARE_MODE = 0x2a,
	RS_CMD_DELETE_PERSON_ITEM = 0x2b,
	RS_CMD_SET_COMPARE_PARAM = 0x2c,
	RS_CMD_SET_TIME = 0x2d,
	RS_CMD_SET_RTSP_FORMAT = 0x2e,
	RS_CMD_RECT_SWITCH = 0x2f,
} RS_CMD_CODE;

typedef enum {
	RS_REPLY_TEST = 0x70,
	RS_REPLY_SYSTEM_INFO = 0x71,
	RS_REPLY_SAVE_FRAME = 0x72,
	RS_REPLY_LIVENESS_SWITCH = 0x73,
	RS_REPLY_CROP_SWITCH = 0x74,
	RS_REPLY_RECO_AREA = 0x75,
	RS_REPLY_SET_DEBUG_FLAG = 0x76,
	RS_REPLY_GET_PERSON_ITEM = 0x77,
	RS_REPLY_PING = 0x80,
	RS_REPLY_VERSION = 0x81,
	RS_REPLY_MODE = 0x82,
	RS_REPLY_START = 0x83,
	RS_REPLY_STOP = 0x84,
	RS_REPLY_FW_PREPARE = 0x85,
	RS_REPLY_FW_DATA = 0x86,
	RS_REPLY_FW_UPDATE = 0x87,
	RS_REPLY_REBOOT = 0x8b,
	RS_REPLY_SET_SN = 0x8c,
	RS_REPLY_GET_SN = 0x8d,
	RS_REPLY_LIC_INFO = 0x8e,
	RS_REPLY_FILE_OPEN = 0x8f,
	RS_REPLY_FILE_DATA = 0x90,
	RS_REPLY_FILE_DONE = 0x91,
	RS_REPLY_FACE_PICTURE = 0x92,
	RS_REPLY_TAKE_PHOTO = 0x94,
	RS_REPLY_UVC_SOURCE = 0x95,
	RS_REPLY_SLEEP = 0x96,
	RS_REPLY_SET_ROTATE_ANGLE = 0x97,
	RS_REPLY_SET_ALGO_MODEL = 0x98,
	RS_REPLY_FACE_SWITCH = 0x99,
	RS_REPLY_COMPARE_MODE = 0x9a,
	RS_REPLY_DELETE_PERSON_ITEM = 0x9b,
	RS_REPLY_SET_COMPARE_PARAM = 0x9c,
	RS_REPLY_SET_TIME = 0x9d,
	RS_REPLY_SET_RTSP_FORMAT = 0x9e,
	RS_REPLY_RECT_SWITCH = 0x9f,
	
	RS_REPLY_FACE = 0xa0,
	RS_REPLY_PHOTO = 0xa1,
	RS_REPLY_FACE_ATTR2 = 0xa3,
	RS_REPLY_PEOPLE_STATUS = 0xa4,
	RS_REPLY_PERSON_INFO = 0xa5,
	RS_REPLY_FACE_ATTR3 = 0xa6,
	RS_REPLY_PERSON_DB_INFO = 0xa7,
	RS_REPLY_PERSON_ITEM = 0xa8,
	
	RS_REPLY_ERROR_CODE = 0xb0,
} RS_REPLY_CODE;

typedef enum {
	ERROR_OK = 0,
	ERROR_NO_MEMORY = -1,
	ERROR_FACE_DETECT = -2,
	ERROR_FACE_SCORE = -3,
	ERROR_EYE_DISTANCE = -4,
	ERROR_FACE_FEATURE = -5,
	ERROR_PICTURE_FORMAT = -6,
	ERROR_WRITE_FACE_DB = -7,
	ERROR_FACE_NOT_LIVENESS = -8,
	ERROR_FACE_DB_FULL = -9,
} RS_ERROR_CODE;

typedef enum {
	DEBUG_PRINT_TRACK_TIME				= 0x00000001,
	DEBUG_SAVE_NV12_TRACK_NO_PEOPLE		= 0x00000002,	// tmp
	DEBUG_PRINT_TRACK_RESULT			= 0x00000004,
	DEBUG_OUTPUT_TRACK_RESULT			= 0x00000008,
	DEBUG_PRINT_RECO_TIME				= 0x00000010,
	DEBUG_SAVE_NV12_NOT_LIVENESS		= 0x00000020,
	DEBUG_SAVE_NV12_RECO_SUCCESS		= 0x00000040,	// tmp
	DEBUG_SAVE_SIGNIN_PICTURE			= 0x00000080,
} RS_DEBUG_FLAG_CODE;

#define PING_REPLY "readsense server reply ping."
#define PICTURE_FORMAT_JPEG   "picture.jpeg"

#define HEAD_MAGIC "READSENSE"
#define MAGIC_SIZE (sizeof(HEAD_MAGIC) - 1)

#pragma pack (1)
typedef struct {
	unsigned char  magic[MAGIC_SIZE];
	unsigned char  version;
	unsigned char  crc;
	unsigned char  cmdtype;
	unsigned int   bodysize;
} packet_head;

// uart 115200 8n1
#define UART_DATA_SIZE        (5 * 1024)

#define HEADSIZE              sizeof(packet_head)
#define INDEX_SIZE            sizeof(unsigned int)
#define DATA_SIZE             (150 * 1024)
#define BUFSIZE               (INDEX_SIZE + DATA_SIZE)
#define PERSON_INFO_SIZE      512

typedef struct {
	packet_head head;
	unsigned char buffer[BUFSIZE];
} rspacket;


// cmd
typedef struct {
	unsigned char mode;
} RS_SYS_CONTROL;

typedef struct {
	unsigned char type;
} RS_UPDATE_TYPE;

typedef struct {
	unsigned int index;
	unsigned char data[DATA_SIZE];
} RS_INDEX_DATA;

typedef struct {
	unsigned char md5[16];
} RS_CHKSUM_MD5;

typedef struct {
	unsigned char sn[16];
} RS_MODULE_SN;

typedef struct {
	char path[64];
} RS_FILE_PARA;

typedef struct {
	unsigned char data[BUFSIZE];
} RS_ONLY_DATA;

typedef struct {
	char data[PERSON_INFO_SIZE];
} RS_FACE_PICTURE;

typedef struct {
	unsigned char recognition;
	char data[PERSON_INFO_SIZE];
} RS_TAKE_PHOTO;

typedef struct {
	unsigned char source;
} RS_UVC_SOURCE;

typedef struct {
	int angle;
} RS_ROTATE_ANGLE;

typedef struct {
	unsigned char mode;
} RS_ALGO_MODEL;

typedef struct {
	unsigned char open;
} RS_FACE_SWITCH, RS_RECT_SWITCH, RS_LIVENESS_SWITCH, RS_CROP_SWITCH;

typedef struct {
	unsigned char mode;
} RS_COMPARE_MODE;

typedef struct {
	float score;
	float front;
	float distance;
} RS_COMPARE_PARAM;

typedef struct {
	unsigned char format;
} RS_RTSP_FORMAT;

typedef struct {
	int flag;
} RS_DEBUG_FLAG;

typedef struct {
	int id;
	int size;
	char data[PERSON_INFO_SIZE];
} RS_PERSON_ITEM;


//reply
typedef struct {
	unsigned char status[30];
} RS_PING_STATUS;

typedef struct {
	char kernel[10];
	char rootfs[10];
	char algo[10];
} RS_VERSION;

typedef struct {
	unsigned char rep_ok;
} RS_REP_ONLY_STATUS;

typedef struct {
	unsigned int index;
	unsigned char rep_ok;
} RS_REP_INDEX_STATUS;

#define RS_FACE_RECOGNITION_FEATURE_DIMENSION 512
typedef struct tag_RS_FACE_RECOGNITION_RESULT
{
	int fr_available_flag;
	int fr_feature_dimension;
	float fr_feature[RS_FACE_RECOGNITION_FEATURE_DIMENSION];
} RS_FACE_RECOGNITION_RESULT;

typedef struct _RS_RECT {
	int left;
	int top;
	int width;
	int height;
} RS_RECT;

typedef struct tag_RS_FACE_ATTRIBUTES2_RESULT
{
	RS_RECT rect;
	float yaw;
	float pitch;
	float roll;
	int trackId;
	float confidence;
	float score;
} RS_FACE_ATTRIBUTES2_RESULT;

typedef struct tag_RS_FACE_ATTRIBUTES3_RESULT
{
	int trackId;
	RS_RECT rect;
	float blur_prob;
	float front_prob;
	float roll;
	float pitch;
	float yaw;
	float dist;
} RS_FACE_ATTRIBUTES3_RESULT;

typedef struct {
	unsigned char status;
} RS_REP_PEOPLE_STATUS;

typedef struct {
	float similarity;
	int size;
	char data[PERSON_INFO_SIZE];
} RS_REP_PERSON_INFO;

typedef struct {
	int code;
} RS_REP_ERROR_CODE;

struct my_tm {
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
};

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} RS_AREA;

typedef struct {
	int angle;
	float score;
	float front;
	float distance;
	unsigned char mode;
	unsigned char compare;
	unsigned char rect;
	unsigned char liveness;
	unsigned char sensor;
	unsigned char video;
	unsigned char model;
	struct my_tm now;
	unsigned char uvc;
	unsigned char photo_crop;
	RS_AREA reco_area;
	int debug_flag;
} RS_REP_SYSTEM_INFO;

typedef struct {
	int total;
	int used;
	int valid;
} RS_REP_PERSON_DB_INFO;

//
typedef enum {
	SYS_MODE_UVC = 0,
	SYS_MODE_RNDIS,
	SYS_MODE_MAX,
} eworkmode;

typedef enum {
	CMP_MODE_REMOTE = 0,
	CMP_MODE_LOCAL,
	CMP_MODE_MAX,
} ecomparemode;

typedef enum {
	TAKE_PHOTO_ONLY = 0,
	TAKE_PHOTO_RECO,
	TAKE_PHOTO_SIGNIN,
	TAKE_PHOTO_MAX,
} etakephoto;

typedef enum {
	UVC_SOURCE_NONE = 0,
	UVC_SOURCE_ISP = 3,
	UVC_SOURCE_CIF = 4,
	UVC_SOURCE_USB = 5,
	UVC_SOURCE_MAX,
} euvcsource;

typedef enum {
	ALGO_MODEL_NORMAL = 0,
	ALGO_MODEL_CARD,
	ALGO_MODEL_MAX,
} ealgomodel;

typedef enum {
	RTSP_FORMAT_H264_1280_720 = 0,
	RTSP_FORMAT_H264_640_480 = 1,
	RTSP_FORMAT_H264_640_360 = 2,
	RTSP_FORMAT_MAX,
} ertspformat;
#pragma pack ()


#endif