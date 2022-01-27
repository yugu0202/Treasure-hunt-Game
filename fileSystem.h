
struct item
{
	char name[256];
	int type;
	int childCount;
	struct item* parent;
	struct item *child[100];
	char content[256];
};

typedef struct item Item;
