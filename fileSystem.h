
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
Item** GetItemList(Item* root,char* );
void SetContent(Item* ,char* ,char* );
void append(char* ,int ,Item* ,Item* );
