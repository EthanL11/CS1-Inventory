// Ethan Liang
// P6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19
#define TABLESIZE 300007

typedef struct item{
	char name[MAXLEN+1];
	int quantity;
	int saleprice;
}item;

typedef struct node{
item* iPtr;
struct node* next;
}node;

typedef struct hashtable{
	node** lists;
	int size;
}hashtable;

int totalcomplexity=0;

int hashfunc(char* word,int size){
	int len=strlen(word);
	int res=0;
	for(int i=0;i<len;i++){
		res=(1151*res+(word[i]-'a'))%size;
	}
	return(res);
}

// creates node for item
node* createnode(item* newitem){
	node* newnode=malloc(sizeof(node));
	newnode->iPtr=newitem;
	newnode->next=NULL;
	return(newnode);
}

void inserttable(hashtable* hash,item* newitem){
	int hashval=hashfunc(newitem->name,hash->size);

	// creates node and inserts node
	node* newnode=createnode(newitem);
	if(hash->lists[hashval]==NULL){
		hash->lists[hashval]=newnode;
	}
	else{
		newnode->next=hash->lists[hashval];
		hash->lists[hashval]=newnode;
	}
}

node* searchtable(hashtable* hash,char* itemname){
	int hashval=hashfunc(itemname,hash->size);

	// create a temp
	node* temp=hash->lists[hashval];
	// goes through the hashtable and returns node
	while(temp!=NULL){
		totalcomplexity++;
		if(strcmp(temp->iPtr->name,itemname)==0){
			return(temp);
		}
		temp=temp->next;
	}
	totalcomplexity++;
	// returns NULL if item doesnt exist
	return(NULL);
}

void buy(hashtable* hash,char* itemname,int quantity,int totalprice,int* totalcash){
	// search hashtable
	node* itemnode=searchtable(hash,itemname);

	// subtract cash
	*totalcash-=totalprice;

	// adds to the item that was already created
	if(itemnode!=NULL){
		itemnode->iPtr->quantity+=quantity;
		printf("%s %d %d\n",itemname,itemnode->iPtr->quantity,*totalcash);
	}

	// checks if it doesnt exists and creates a new item
	if(itemnode==NULL){
		item* newitem=malloc(sizeof(item));
		strcpy(newitem->name,itemname);
		newitem->quantity=quantity;
		inserttable(hash,newitem);
		printf("%s %d %d\n",itemname,quantity,*totalcash);
	}
}

void sell(hashtable* hash,char* itemname,int quantity,int* totalcash){
	// search hashtable
	node* itemnode=searchtable(hash,itemname);

	// checks if item exists
	if(itemnode!=NULL){

		// checks for amount previous quantity is more
		if(itemnode->iPtr->quantity>=quantity){
			itemnode->iPtr->quantity-=quantity;
			*totalcash+=(quantity*itemnode->iPtr->saleprice);
		}

		// not enough quantity turns it to 0
		else{
			*totalcash+=(itemnode->iPtr->quantity*itemnode->iPtr->saleprice);
			itemnode->iPtr->quantity=0;
		}
		printf("%s %d %d\n",itemname,itemnode->iPtr->quantity,*totalcash);
	}
}

void changeprice(hashtable* hash,char* itemname,int newprice){
	// search hashtable
	node* curnode=searchtable(hash,itemname);

	// changes only if item exist
	if(curnode!=NULL){
		curnode->iPtr->saleprice=newprice;
	}
}

int main(){
	int n;
	scanf("%d",&n);

	// allocate mem
	hashtable* hash=malloc(sizeof(hashtable));
	hash->size=TABLESIZE;
	hash->lists=malloc(TABLESIZE*sizeof(sizeof(node*)));
	for(int i=0;i<TABLESIZE;i++){
		hash->lists[i]=NULL;
	}

	// given
	int totalcash=100000;

	for(int i=0;i<n;i++){
		// get command
		char command[MAXLEN+1];
		scanf("%s",command);

		// buy command
		if(strcmp(command,"buy")==0){
			char itemname[MAXLEN+1];
			int quantity,totalprice;
			scanf("%s %d %d",itemname,&quantity,&totalprice);

			buy(hash,itemname,quantity,totalprice,&totalcash);
		}
		// sell command
		else if(strcmp(command,"sell")==0){
			char itemname[MAXLEN+1];
			int quantity;
			scanf("%s %d",itemname,&quantity);

			sell(hash,itemname,quantity,&totalcash);
		}
		// change price command
		else if(strcmp(command,"change_price")==0){
			char itemname[MAXLEN+1];
			int newprice;
			scanf("%s %d",itemname,&newprice);

			changeprice(hash,itemname,newprice);
		}
	}

	// print cash and complexity
	printf("%d\n",totalcash);
	printf("%d\n",totalcomplexity);

	// free mem
	for(int i=0;i<TABLESIZE;i++){
		while(hash->lists[i]!=NULL){
			node* temp=hash->lists[i];
			hash->lists[i]=hash->lists[i]->next;
			free(temp->iPtr);
			free(temp);
		}
	}
	free(hash->lists);
	free(hash);

	return(0);
}