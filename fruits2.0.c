// It is required to analyse the fruit vendor shop using AVL tree and hashing and linked lists.
// The AVL tree is balanced based of the price of fruit and each fruit has a hashkey associated with it.
// Using hashkey we can find the fruit details in O(1).
// One more feature of this code is that it takes input the amount the customer has and returns the list
// maximum number of fruits the customer can buy.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// structure of the fruits
typedef struct FruitNode
{
    char fruit_name[20];
    int price;
    int key;
    int height;
    struct FruitNode *left;
    struct FruitNode *right;
}fruitnode;

// structure of the hashtable associated with a key and the address of the fruit node
typedef struct HashNode 
{
    int key;
    struct FruitNode *fruit_address;
}hashnode;

int total_cost=0;
const int capacity = 20;
int size = 0;
hashnode** arr;

// Structure to store all words from the fruits list.
typedef struct Node{
    char *word;
    struct Node *next;
}node;

node* word_head;

// converts the string to its respective number
int str_to_num(char str[])
{
    int num=0;
    for(int i=0;str[i]!='\0';i++)
    {
        num=(num*10)+(str[i]-'0');
    }
    return num;
}


// add words from the fruits list to the end of the words linked list
void add_word_at_end(node * temp)
{
    if(word_head==NULL)
    {
        word_head=temp;
    }
    else
    {
        node* temp1=word_head;
        while(temp1->next!=NULL)
        {
            temp1=temp1->next;
        }
        temp1->next=temp;
        temp->next=NULL;
    }
}

// Inserts the hashkey in the hashtable with it's respective fruit address.
void insert_hash(int key, fruitnode* fruit_address)
{
    hashnode* temp=(hashnode*)malloc(sizeof(hashnode));
    temp->key = key;
    temp->fruit_address = fruit_address;
    int hashIndex = key % capacity;
    while (arr[hashIndex] != NULL && arr[hashIndex]->key != key && arr[hashIndex]->key != -1) 
    {
        hashIndex++;
        hashIndex %= capacity;
    }
    if (arr[hashIndex] == NULL || arr[hashIndex]->key == -1)
    {
        size++;
    }
    arr[hashIndex] = temp;
}

// assigning hashkey with fruit address.
void assign_hash(fruitnode* root)
{
if(root!= NULL)
    {
        assign_hash(root->left);
        insert_hash(root->key,root);
        assign_hash(root->right);
    }
}


// To find the fruit details with the help of key in O(1).
fruitnode** find(int key)
{
    int hashIndex = (key % capacity);
    int counter = 0;
    while (arr[hashIndex] != NULL) 
    {
        int counter = 0;
        if (counter++ > capacity)
            break;
        if (arr[hashIndex]->key == key)
            return arr[hashIndex]->fruit_address;
        hashIndex++;
        hashIndex %= capacity;
    }
    return NULL;
}

// returns maximum of two numbers
int max(int a, int b)
{
    return (a > b)? a : b;
}

// returns height of the respective node
int height(fruitnode *N)
{
    if (N == NULL)
    {
        return 0;
    }
    return N->height;
}

// creates a new fruitnode 
fruitnode* newNode(char fruit_name[],int price,int hashkey)
{
    fruitnode* node = (fruitnode*)malloc(sizeof(fruitnode));
    strcpy(node->fruit_name,fruit_name);
    node->price=price;
    node->key = hashkey;
    node->height = 1; 
    node->left = NULL;
    node->right = NULL;
    return(node);
}

// right rotation of the avl tree
fruitnode *rightRotate(fruitnode *y)
{
    fruitnode *x = y->left;
    fruitnode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left),height(y->right)) + 1;
    x->height = max(height(x->left),height(x->right)) + 1;
    return x;
}

// left rotation of the avl tree
fruitnode *leftRotate(fruitnode *x)
{
    fruitnode *y = x->right;
    fruitnode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left),height(x->right)) + 1;
    y->height = max(height(y->left),height(y->right)) + 1;
    return y;
}

// returns the balance factor of the respective node
int getBalance(fruitnode *N)
{
    if (N == NULL)
    {
        return 0;
    }
    return height(N->left) - height(N->right);
}

// insert the fruitnode in avl tree and balance it.
fruitnode* insert(fruitnode* node,char fruit_name[], int price,int hashkey)
{
    if (node == NULL)
        return(newNode(fruit_name,price,hashkey));
    if (price < node->price)
        node->left = insert(node->left,fruit_name,price, hashkey);
    else if (price > node->price)
        node->right = insert(node->right, fruit_name,price, hashkey);
    else 
        return node;
    node->height = 1 + max(height(node->left),height(node->right));
    int balance = getBalance(node);
    // If this node becomes unbalanced, then there are 4 cases
    // Left Left Case
    if (balance > 1 && price < node->left->price)
        return rightRotate(node);
    // Right Right Case
    if (balance < -1 && price > node->right->price)
        return leftRotate(node);
    // Left Right Case
    if (balance > 1 && price > node->left->price)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case
    if (balance < -1 && price < node->right->price)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// function to display the fruitnode in sorted order
void display_InOrder(fruitnode* root)
{
if(root!= NULL)
    {
        display_InOrder(root->left);
        printf("name: %s\t\t price: %drs\t height: %d\t key: %d\n",root->fruit_name,root->price,root->height,root->key);
        display_InOrder(root->right);
    }
}

// displays the details of the fruit node
void display(fruitnode* root)
{
    printf("\tname: %s\t\t price: %drs\t height: %d\t key: %d\n\t",root->fruit_name,root->price,root->height,root->key);
}

// reads all the fruit and it's details from the fruit list and adds in the list of words
void add_to_word_list()
{
    FILE *file_ptr;
    char testarray[10000];
    file_ptr=fopen("fruits_list.txt","r");
    if(file_ptr==NULL) printf("file can't be opened \n");
    while(!feof(file_ptr))
    {
        node* temp=(node *)malloc(sizeof(node));
        temp->next=NULL;
        fscanf(file_ptr,"%s",testarray);
        temp->word=(char *)malloc(strlen(testarray)+1);
        strcpy(temp->word,testarray);
        add_word_at_end(temp);
    }
    fclose(file_ptr);
}

// returns the size of the word list
int size_of_word_list()
{
        int t=0;
        node* temp=word_head;
        while(temp!=NULL)
        {
            temp=temp->next;
            t++;
        }
        return t;
    }


// displays all the words in word list
void display_words()
{
    node* temp=word_head;
    if(word_head==NULL) printf("Word list is empty\n");
    while(temp!=NULL)
    {
        printf(" %s  \n",temp->word);
        temp=temp->next;
    }
}


// function to insert the fruit and its details from the word list to fruit avl tree
fruitnode* insert_fruits_from_list(fruitnode* root){
    char fruit_name[20];
    int fruit_price;
    int fruit_key;
    node* temp=word_head;
    if(word_head==NULL) printf("Word list is empty\n");
    int k=1;
    while(temp!=NULL)
    {   
        if(k==1){
        strcpy(fruit_name,temp->word);
        }
        else if(k==2){
            fruit_price=str_to_num(temp->word);
        }
        else if(k==3){
            fruit_key=str_to_num(temp->word);
            root=insert(root,fruit_name,fruit_price,fruit_key);
            k=0;
        }
        k++;
        temp=temp->next;
    }
    return root;
}

int check_sum=0;
int available_cost=0;
int customer_money=0;


// function to display the maximum number of fruits that the customer can buy
int sum_function(int money,int key)
{
    if(customer_money>=money)
    {
    display(find(key));
    available_cost=money;
    }
    return money;
}

// calculates the total sum of all fruits
void total_sum_of_fruits_costs(fruitnode* root)
{
if(root!= NULL)
    {
        total_sum_of_fruits_costs(root->left);
        total_cost+=root->price;
        check_sum=sum_function(total_cost,root->key);
        total_sum_of_fruits_costs(root->right);
    }
}

// displays the sum of price of all fruits and the maximum number of fruits that the customer can buy
void max_no_fruits(fruitnode* root)
{
    total_sum_of_fruits_costs(root);
    printf("\n\tsum of cost of all fruits is %d\n\t",total_cost);
    printf("\n\tsum of cost of fruits u can buy %d\n\t",available_cost);
    total_cost=0;
}


int main()
{
    fruitnode* root=NULL;
    add_to_word_list();
    root=insert_fruits_from_list(root);
    arr = (hashnode**)malloc(sizeof(hashnode*)*capacity);
    for (int i = 0; i < capacity; i++)
    {
        arr[i] = NULL;
    }
    assign_hash(root);
    while(1)
    {
        printf("\n\tEnter\t\n\t1.To display all fruits\n\t2.To find the fruit\n\t3.To get maximum no of fruits u can buy\n\t");
        int input;
        scanf("%d",&input);
        switch(input)
        {
            case 1:
                printf("\n");
                printf("\n");
                display_InOrder(root);
                printf("\n");
                printf("\n");
                break;
            case 2:
                printf("\tEnter the key of the fruit you want to search\n\t");
                int key;
                scanf("%d",&key);
                if (find(key) != -1)
                {
                    printf("\tThe key of the fruit you entered has been found in the fruit list\n\t\tThe details of the fruits are:-\n\t");
                    display(find(key));
                }
                else{
                    printf("\tKey %d does not exists\n\t",key);
                }
                break;
            case 3:
                printf("\tPlease enter the amount you have.\n\t");
                scanf("%d",&customer_money);
                max_no_fruits(root);
                break;
            default:
                break;
        }
    }
    return 0;
}