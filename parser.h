struct json {
	char* name;
	char* attr;
	char* permission;
	struct json* children;
	char* inside;
	char* passwd;
};

typedef struct json Json;

Json* toStruct(char* ,Json*);
