
typedef struct item
{
	char* name;
	int type;
	struct item* parent;
	struct item* child;
	struct item* next;
	char* content;
} Item;

void CreateFS(Item* );
Item* GetItem(Item* ,char* );
